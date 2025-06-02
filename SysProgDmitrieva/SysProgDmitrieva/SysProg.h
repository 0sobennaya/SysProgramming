#pragma once

#include <iostream>
#include <string>
#include <thread>


inline void DoWrite()
{
	std::wcout << std::endl;
}

template <class T, typename... Args> inline void DoWrite(T& value, Args... args)
{
	std::wcout << value;
	DoWrite(args...);
}

std::mutex m;
template <typename... Args> inline void SafeWrite(Args... args)
{
	std::lock_guard<std::mutex> lg(m);
	DoWrite(args...);
}