// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "asio.h"


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

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

boost::asio::io_context io;
tcp::socket* MySocket = nullptr;

extern "C" {

    __declspec(dllexport) void sendServer(tcp::socket& s, int type, int num = 0, int addr = 0, const wchar_t* str = nullptr) {
        header h;
        if (str != nullptr)
            h = { type, num, addr, (int)(wcslen(str) * sizeof(wchar_t)) };
        else
            h = { type, num, addr, 0 };

        sendData(s, &h);
        if (h.size)
            sendData(s, str, h.size);
    }

    __declspec(dllexport) std::wstring receiveServer(tcp::socket& s, header& h) {
        receiveData(s, &h);
        std::wstring str;
        if (h.size)
        {
            str.resize(h.size / sizeof(wchar_t));
            receiveData(s, str.data(), h.size);
        }
        return str;
    }

    __declspec(dllexport) header sendClient(int type, int num = 0, int addr = 0, const wchar_t* str = nullptr) {
        if ((MessageType)type == INIT)
        {
            MySocket = new tcp::socket(io);
            tcp::resolver r(io);
            boost::asio::connect(*MySocket, r.resolve("127.0.0.1", "12345"));
        }

        sendServer(*MySocket, type, num, addr, str);

        header hConfirm = { 0 };
        receiveData(*MySocket, &hConfirm);

        if ((MessageType)type == EXIT)
        {
            delete MySocket;
        }

        return hConfirm;
    }

}




