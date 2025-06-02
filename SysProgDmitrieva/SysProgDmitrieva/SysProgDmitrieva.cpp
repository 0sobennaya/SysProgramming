#include "Session.h"
#include "SysProg.h"

#include <map>

int maxID = MR_USER;
std::map<int, std::shared_ptr<Session>> sessions;

void cleanupInactiveSessions()
{
	const auto timeout = chrono::seconds(15);
	while (true)
	{
		this_thread::sleep_for(chrono::seconds(2));

		auto now = std::chrono::steady_clock::now();
		vector<int> expired;

		for (auto& [id, session] : sessions)
		{
			if (now - session->lastAccess > timeout)
				expired.push_back(id);
		}

		for (int id : expired)
		{
			sessions.erase(id);
			SafeWrite(L"Клиент#", id, L"отключён");

			for (auto& [sess_id, sess] : sessions)
			{
				auto m = Message(sess_id, id, MT_EXIT);
				sess->add(m);
			}
		}
	}
}


void processClient(tcp::socket s)
{
	try
	{
		Message m;
		int code = m.receive(s);
		SafeWrite(m.header.to, ": ", m.header.from, ": ", m.header.type);
		switch (code)
		{
		case MT_INIT:
		{
			auto session = std::make_shared<Session>(++maxID, m.data);
			sessions[session->id] = session;
			Message::send(s, session->id, MR_BROKER, MT_INIT);

			for (auto& [id, ses] : sessions)
			{
				if (id != session->id)
				{
					Message mes = Message(id, session->id, MT_INIT);
					ses->add(mes);
					mes = Message(session->id, id, MT_INIT);
					sessions[session->id]->add(mes);
				}
			}
			break;
		}
		
		case MT_GETDATA:
		{
			auto iSession = sessions.find(m.header.from);
			if (iSession != sessions.end())
			{
				iSession->second->send(s);
			}
			break;
		}
		default:
		{
			auto iSessionFrom = sessions.find(m.header.from);
			if (iSessionFrom != sessions.end())
			{
				auto iSessionTo = sessions.find(m.header.to);
				if (iSessionTo != sessions.end())
				{
					iSessionTo->second->add(m);
				}
				else if (m.header.to == MR_ALL)
				{
					for (auto& [id, session] : sessions)
					{
						if (id != m.header.from)
							session->add(m);
					}
				}
				else if (m.header.to == MR_BROKER)
				{
					SafeWrite("Главный поток, сообщение \"", m.data, "\"");
				}
				Message::send(s, m.header.from, MR_BROKER, MT_CONFIRM);
			}
			break;
		}
		}

	}
	catch (std::exception& e)
	{
		std::wcerr << "Exception: " << e.what() << endl;
	}
}



int main()
{
	std::wcin.imbue(std::locale("rus_rus.866"));
	std::wcout.imbue(std::locale("rus_rus.866"));

	try
	{
		int port = 12345;
		boost::asio::io_context io;
		tcp::acceptor a(io, tcp::endpoint(tcp::v4(), port));

		std::thread(cleanupInactiveSessions).detach();
		while (true)
		{
			std::thread(processClient, a.accept()).detach();
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}


