#include "pch.h"
#include "asio.h"
#include "message.h"

int Message::clientID = 0;

void Message::send(tcp::socket& s, int to, int from, int type, const wstring& data)
{
	Message m(to, from, type, data);
	m.send(s);
}
