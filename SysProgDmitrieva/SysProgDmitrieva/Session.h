#pragma once
#include "message.h"

#include <queue>
#include <mutex>
#include <chrono>

class Session
{
public:
	int id;
	std::wstring name;
	std::queue<Message> messages;
	chrono::time_point<std::chrono::steady_clock> lastAccess;

	std::mutex mx;
	Session(int id, std::wstring name)
		:id(id), name(name), lastAccess(std::chrono::steady_clock::now())
	{
	}

	void touch() {
		lastAccess = std::chrono::steady_clock::now();
	}

	void add(Message& m)
	{
		lock_guard<std::mutex> lg(mx);
		messages.push(m);
	}

	void send(tcp::socket& s)
	{
		touch();
		lock_guard<mutex> lg(mx);
		if (messages.empty())
		{
			Message::send(s, id, MR_BROKER, MT_NODATA);
		}
		else
		{
			messages.front().send(s);
			messages.pop();
		}
	}
};

