
/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */
#pragma once



#if defined(FD_SETSIZE)

#undef FD_SETSIZE

#endif

#define FD_SETSIZE 4096

#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#if defined(SetPort)
#undef SetPort
#endif

namespace NetSpace {






#define VERLOW   2

#define VERHIGH  2




#define NetInit()												\
		do{														\
				WSADATA			wsda;							\
				::memset(&wsda, 0, sizeof(wsda));				\
				WORD ver_req = MAKEWORD(VERLOW, VERHIGH);		\
				DWORD code = WSAStartup(ver_req, &wsda );		\
				if(code != 0) throw ExceptionSpace::NetException("WSAStartup Failed\n", code);\
		}while(0)


#define NetGetLastError ::WSAGetLastError

#define NetSetLastError(code) (::WSASetLastError(code))






#define NET_INVALID_SOCKET		INVALID_SOCKET
#define NET_SOCK_ERROR			SOCKET_ERROR

typedef SOCKET					NetHandle;

typedef sockaddr				SA;

typedef int						SockLen;


enum SockCtrlCmd
{
		NET_NONBLOCK	= 0x00

};




struct iovec
{
		t_ulong iov_len;
		char	*iov_base;
		
		operator WSABUF&(void) { return *((WSABUF *)this); }
		
};







///////////////error code define//////////////////////////////////
#  define ETIME                   ERROR_SEM_TIMEOUT
#  define EWOULDBLOCK             WSAEWOULDBLOCK
#  define EINPROGRESS             WSAEINPROGRESS
#  define EALREADY                WSAEALREADY
#  define ENOTSOCK                WSAENOTSOCK
#  define EDESTADDRREQ            WSAEDESTADDRREQ
#  define EMSGSIZE                WSAEMSGSIZE
#  define EPROTOTYPE              WSAEPROTOTYPE
#  define ENOPROTOOPT             WSAENOPROTOOPT
#  define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#  define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#  define EOPNOTSUPP              WSAEOPNOTSUPP
#  define EPFNOSUPPORT            WSAEPFNOSUPPORT
#  define EAFNOSUPPORT            WSAEAFNOSUPPORT
#  define EADDRINUSE              WSAEADDRINUSE
#  define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#  define ENETDOWN                WSAENETDOWN
#  define ENETUNREACH             WSAENETUNREACH
#  define ENETRESET               WSAENETRESET
#  define ECONNABORTED            WSAECONNABORTED
#  define ECONNRESET              WSAECONNRESET
#  define ENOBUFS                 WSAENOBUFS
#  define EISCONN                 WSAEISCONN
#  define ENOTCONN                WSAENOTCONN
#  define ESHUTDOWN               WSAESHUTDOWN
#  define ETOOMANYREFS            WSAETOOMANYREFS
#  define ETIMEDOUT               WSAETIMEDOUT
#  define ECONNREFUSED            WSAECONNREFUSED
#  define ELOOP                   WSAELOOP
#  define EHOSTDOWN               WSAEHOSTDOWN
#  define EHOSTUNREACH            WSAEHOSTUNREACH
#  define EPROCLIM                WSAEPROCLIM
#  define EUSERS                  WSAEUSERS
#  define EDQUOT                  WSAEDQUOT
#  define ESTALE                  WSAESTALE
#  define EREMOTE                 WSAEREMOTE



























}