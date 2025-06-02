#pragma once
#include "message.h"

struct MessageTransfer
{
	MessageHeader header = {};
	const wchar_t* data = nullptr;
	int clientID = 0;
};

extern "C"
{
	__declspec(dllexport) MessageTransfer SendMsg(int to, int type = MT_DATA, const wchar_t* data = nullptr);
	__declspec(dllexport) void FreeMessageTransfer(MessageTransfer msg);
}