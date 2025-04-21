#ifndef _WIN32_WINNT
#define	_WIN32_WINNT	0x0A00
#endif						

#include <boost/asio.hpp>

#include <iostream>
#include "Session.h"
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <fstream>

using boost::asio::ip::tcp;

struct header {
	int type;
	int num;
	int adr;
	int size;
};

enum MessageType {
	INIT,
	EXIT,
	START,
	SEND,
	STOP,
	CONFIRM
};
vector<Session*> sessions;
int i = 0;

extern "C" {
	__declspec(dllimport) void SendServer(tcp::socket& s, int type, int num = 0, int adr = 0, const wchar_t* str = nullptr);
	__declspec(dllimport) std::wstring ReceiveServer(tcp::socket& s, header& h);
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

//void start()
//{
//	std::wcin.imbue(std::locale("rus_rus.866"));
//	std::wcout.imbue(std::locale("rus_rus.866"));
//
//	
//	
//	
//
//	while (i >=0)
//	{
//
//		int n = WaitForMultipleObjects(4, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
//		switch (n)
//		{
//		case 0:
//		{
//			sessions.push_back(new Session(i++));
//			std::thread t(MyThread, sessions.back());
//			t.detach();
//			SetEvent(hConfirmEvent);
//			break;
//		}
//		case 1:
//
//			if (i == 0) {
//				SetEvent(hCloseEvent);
//				SetEvent(hConfirmEvent);
//				break;
//			}
//			sessions.back()->addMessage(MT_CLOSE);
//			sessions.pop_back();
//			SetEvent(hConfirmEvent);
//			i--;
//			break;
//
//		case 2:
//		{
//			sessions.clear();
//			i = -1;
//			SetEvent(hConfirmEvent);
//			return;
//		}
//		case 3:
//		{
//			header h;
//			std::wstring message = mapreceive(h);
//
//			switch (h.addr)
//			{
//			case 0: {
//				for (auto& session : sessions) {
//					session->addMessage(MT_DATA, message);
//				}
//				break;
//			}
//			case 1: {
//				SafeWrite("Main Thread ", message);
//				break;
//			}
//			default: {
//				sessions[h.addr - 2]->addMessage(MT_DATA, message);
//				break;
//			}
//
//			}
//			SetEvent(hConfirmEvent);
//		}
//		}
//	} 
//	
//	SetEvent(hConfirmEvent);
//	
//}

void processClient(tcp::socket s)
{
	try
	{
		while (true)
		{
			header h = { 0 };
			std::wstring str = ReceiveServer(s, h);
			switch (h.type)
			{
			case INIT:
			{
				break;
			}
			case START:
			{
				for (int j = 0; j < h.num; j++) {
					sessions.push_back(new Session(i++));
					std::thread t(MyThread, sessions.back());
					t.detach();
				}
				
				break;
			}
			case STOP:

				if (i > 0) {
					
					sessions.back()->addMessage(MT_CLOSE);
					sessions.pop_back();
				
					i--;
					
				}
				break;

			case EXIT:
			{
				sessions.clear();
				SendServer(s, CONFIRM, i);
				
				
				return;
			}
			case SEND:
			{
				
				switch (h.adr)
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
					sessions[h.adr - 2]->addMessage(MT_DATA,str);
					break;
				}

				}
				
			}
			}
			SendServer(s, CONFIRM, i);
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Client exception: " << e.what() << endl;
	}
}



int main()
{
	std::wcin.imbue(std::locale("rus_rus.866"));
	std::wcout.imbue(std::locale("rus_rus.866"));
	try
	{
		int port = 12346;
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

