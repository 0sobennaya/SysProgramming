#include "pch.h"
#include "mydll.h"

wchar_t* copy_wstring(const std::wstring& str)
{
    if (str.empty())
    {
        return nullptr;
    }
    std::size_t size = (str.length() + 1) * sizeof(wchar_t);
    wchar_t* buffer = (wchar_t*)malloc(size);
    if (buffer) {
        memcpy(buffer, str.c_str(), size);
    }
    return buffer;
}

MessageTransfer SendMsg(int to, int type, const wchar_t* data)
{

    boost::asio::io_context io;
    tcp::socket s(io);
    tcp::resolver r(io);
    boost::asio::connect(s, r.resolve("127.0.0.1", "12345"));


    Message m(to, Message::clientID, type, data);
    m.send(s);
    if (m.receive(s) == MT_INIT)
    {
        Message::clientID = m.header.to;
    }

    MessageTransfer result = {};
    result.header = m.header;
    result.data = copy_wstring(m.data);
    result.clientID = m.clientID;

    return result;
}

void FreeMessageTransfer(MessageTransfer msg)
{
    if (msg.data)
    {
        free((void*)msg.data);
    }
}