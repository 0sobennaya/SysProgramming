#include <iostream>
#include "Session.h"
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <fstream>


struct header
{
	int addr;
	int size;
};

extern "C" {
	__declspec(dllimport) std::wstring mapreceive(header& h);

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

			}
		}
	}
	return;
}

void start()
{
	std::wcin.imbue(std::locale("rus_rus.866"));
	std::wcout.imbue(std::locale("rus_rus.866"));

	
	vector<Session*> sessions;

	HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
	HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
	HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");
	HANDLE hCloseEvent = CreateEvent(NULL, FALSE, FALSE, L"CloseEvent");
	HANDLE hSendEvent = CreateEvent(NULL, FALSE, FALSE, L"SendEvent");
	HANDLE hControlEvents[4] = { hStartEvent, hStopEvent , hCloseEvent, hSendEvent};
	int i = 0;
	

	while (i >=0)
	{

		int n = WaitForMultipleObjects(4, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (n)
		{
		case 0:
		{
			sessions.push_back(new Session(i++));
			std::thread t(MyThread, sessions.back());
			t.detach();
			SetEvent(hConfirmEvent);
			break;
		}
		case 1:

			if (i == 0) {
				SetEvent(hCloseEvent);
				SetEvent(hConfirmEvent);
				break;
			}
			sessions.back()->addMessage(MT_CLOSE);
			sessions.pop_back();
			SetEvent(hConfirmEvent);
			i--;
			break;

		case 2:
		{
			sessions.clear();
			i = -1;
			SetEvent(hConfirmEvent);
			return;
		}
		case 3:
		{
			header h;
			std::wstring message = mapreceive(h);

			switch (h.addr)
			{
			case 0: {
				for (auto& session : sessions) {
					session->addMessage(MT_DATA, message);
				}
				break;
			}
			case 1: {
				SafeWrite("Main Thread ", message);
				break;
			}
			default: {
				sessions[h.addr - 2]->addMessage(MT_DATA, message);
				break;
			}

			}
			SetEvent(hConfirmEvent);
		}
		}
	} 
	
	SetEvent(hConfirmEvent);
	
}



int main()
{
	std::vector<Session*> sessions;
	int i = 0;

	HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
	HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
	HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");
	HANDLE hCloseEvent = CreateEvent(NULL, FALSE, FALSE, L"CloseEvent");
	HANDLE hControlEvents[3] = { hStartEvent, hStopEvent, hCloseEvent };

	while (i >= 0) {
		int n = WaitForMultipleObjects(3, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (n)
		{
		case 0:
		{
			sessions.push_back(new Session(i++));
			CloseHandle(CreateThread(NULL, 0, MyThread, (LPVOID)sessions.back(), 0, NULL));
			SetEvent(hConfirmEvent);
			break;
		}
		case 1:
		{
			if (i == 0) {
				SetEvent(hCloseEvent);
				break;
			}
			sessions.back()->addMessage(MT_CLOSE);
			sessions.pop_back();
			SetEvent(hConfirmEvent);
			i--;
			break;
		}
		case 2:
		{
			sessions.clear();
			SetEvent(hConfirmEvent);
			break;
		}

		}
	}
	SetEvent(hConfirmEvent);


	/*return 0;
    start();*/
	return 0;
}

