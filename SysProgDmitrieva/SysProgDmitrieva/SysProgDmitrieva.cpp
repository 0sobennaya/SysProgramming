#include <iostream>
#include "Session.h"
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <fstream>

struct header
{
	int adr;
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
			case MT_DATA:
			{
				WriteFile(session->sessionID, m.data);
				/*SafeWrite("session", session->sessionID, "data", m.data);
				Sleep(500 * session->sessionID);
				break;*/
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

	InitializeCriticalSection(&cs);
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
			thread t(MyThread, sessions.back());
			t.detach();
			SetEvent(hConfirmEvent);
			break;
		}
		case 1:
		{
			if (!sessions.empty())
			{
				sessions.back()->addMessage(MT_CLOSE);
				sessions.pop_back();
				--i;
				SetEvent(hConfirmEvent);
				break;
			}
			else
			{
				SetEvent(hCloseEvent);
				SetEvent(hConfirmEvent);
				return;
			}
		}
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

			switch (h.adr)
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
				sessions[h.adr - 2]->addMessage(MT_DATA, message);
				break;
			}

			}
			SetEvent(hConfirmEvent);
		}
		}
	} 
	
	SetEvent(hConfirmEvent);
	DeleteCriticalSection(&cs);
}



int main()
{
    start();
	return 0;
}

