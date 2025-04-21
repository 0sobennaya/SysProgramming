#ifndef _WIN32_WINNT
#define	_WIN32_WINNT	0x0A00
#endif	

#include <boost/asio.hpp>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <fstream>

#include "Session.h"

using boost::asio::ip::tcp;

struct header {
	int type;
	int num;
	int addr;
	int size;
};

enum MessageType
{
	INIT,
	EXIT,
	START,
	SEND,
	STOP,
	CONFIRM
};

std::vector<Session*> sessions;
int i = 0;

extern "C" {
	__declspec(dllimport) void sendServer(tcp::socket& s, int type, int num = 0, int addr = 0, const wchar_t* str = nullptr);
	__declspec(dllimport) std::wstring receiveServer(tcp::socket& s, header& h);
}

void WriteFile(int sessionID, const std::wstring& message) {
	std::wofstream out;
	out.imbue(std::locale("Russian_Russia"));
	out.open(std::to_string(sessionID) + ".txt", std::ios::app);
	if (out.is_open()) {
		out << message << std::endl;
	}
	out.close();
}

void MyThread(Session* session)
{
	SafeWrite("session", session->sessionID, "created");
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				SafeWrite("session", session->sessionID, "closed");
				delete session;
				return;
			}
			case MT_DATA:
			{
				WriteFile(session->sessionID, m.data);
			}
			}
		}
	}
	return;
}

void processClient(tcp::socket s)
{
	try
	{
		while (true)
		{
			header h = { 0 };
			std::wstring str = receiveServer(s, h);
			switch (h.type)
			{
			case INIT: {

				break;
			}
			case START:
			{
				for (int m = 0; m < h.num; ++m) {
					sessions.push_back(new Session(i++));
					std::thread t(MyThread, sessions.back());
					t.detach();
				}
				break;
			}
			case STOP:
			{
				if (i > 0) {

					sessions.back()->addMessage(MT_CLOSE);
					sessions.pop_back();
					i--;
				}
				break;
			}
			case EXIT:
			{
				sessions.clear();
				sendServer(s, CONFIRM, i);
				return;
			}
			case SEND:
			{
				switch (h.addr)
				{
				case 0: {
					for (auto& session : sessions) {
						session->addMessage(MT_DATA, str);
					}
					break;
				}
				case 1: {
					SafeWrite("Main Thread ", str);
					break;
				}
				default: {
					sessions[h.addr - 2]->addMessage(MT_DATA, str);
					break;
				}

				}
			}
			}
			sendServer(s, CONFIRM, i);
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Client exception: " << e.what() << std::endl;
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


