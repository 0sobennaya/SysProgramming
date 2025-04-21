#pragma once

#ifndef _WIN32_WINNT
#define	_WIN32_WINNT	0x0A00
#endif	

#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;
using namespace std;

template <class T>
inline void sendData(tcp::socket& s, T* data, size_t n = 0)
{
	if (!n)
		n = sizeof(T);
	boost::system::error_code error;
	boost::asio::write(s, boost::asio::buffer(data, n), error);
	if (error)
		throw boost::system::system_error(error);
}

template <class T>
inline void receiveData(tcp::socket& s, T* data, size_t n = 0)
{
	if (!n)
		n = sizeof(T);
	boost::system::error_code error;
	boost::asio::read(s, boost::asio::buffer(data, n), error);
	if (error)
		throw boost::system::system_error(error);
}