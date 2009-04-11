
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
#include "SockStream.h"


namespace NetSpace {



int SockStream::Recv(t_byte *buf, size_t len, int flags)const
{
		assert(IsValid());
		return ::recv(m_sock, (char*)buf, (int)len, flags);
}

int SockStream::Recv(t_byte *buf, size_t len, size_t *bt, const t_uint64 *timeout, int flags)const
{
		assert(IsValid());

		size_t temp;
		size_t &bytes_transferred = (bt == 0 ? temp : *bt);
		bytes_transferred = 0;
		int n = 0;
		
		t_uint64 time_temp = (timeout  != 0 ? *timeout : 0);

		t_uint64 *ptimeout = (timeout  != 0 ? &time_temp : 0);

		TimeSpace::StopWatch watch;
		watch.Start();

		for (bytes_transferred = 0;	bytes_transferred < len; )
		{
				n = Recv(buf + bytes_transferred, len - bytes_transferred, flags);
				if(n == -1 || n == 0)
				{
						if(n == -1 && NetGetLastError() == EWOULDBLOCK)
						{
								int res = Utility::HandleReadReady(m_sock, ptimeout);
								switch(res)
								{
								case 0:
										NetSetLastError(ETIME);
										return -1;
								case -1:
										return -1;
								default:
										n = 0;
								}
						}else
						{
								return n;
						}
				}

				bytes_transferred += n;
				
				if(timeout != 0)
				{
						t_uint64 elapsed = watch.Elapsed() / 1000;
						if(elapsed < time_temp)
						{
								time_temp -= elapsed;
						}else
						{
								NetSetLastError(ETIME);
								return -1;
						}
				}
		}
		
		return static_cast<int>(bytes_transferred);
}


int SockStream::Send(const t_byte *buf, size_t len, int flags)const
{
		assert(IsValid());
		return ::send(m_sock, (char*)buf, (int)len, flags);
}

int SockStream::Send(const t_byte *buf, size_t len, size_t *bt, const t_uint64 *timeout, int flags)const
{
		assert(IsValid());

		size_t temp;
		size_t &bytes_transferred = (bt == 0 ? temp : *bt);
		bytes_transferred = 0;
		int n;
		int err_code = 0;

		t_uint64 time_temp = (timeout  != 0 ? *timeout : 0);

		//t_uint64 *ptimeout = (timeout  != 0 ? &time_temp : 0);
		
		TimeSpace::StopWatch watch;
		watch.Start();

		for (bytes_transferred = 0;	bytes_transferred < len; )
		{
				n = Send(buf + bytes_transferred, len - bytes_transferred, flags);
				
				if (n == 0 || n == -1)
				{
						err_code = NetGetLastError();
						if (n == -1 &&	(err_code == EWOULDBLOCK ))//|| err_code == ENOBUFS))
						{
								int res = Utility::HandleWriteReady(m_sock, timeout);
								switch(res)
								{
								case 0:
										NetSetLastError(ETIME);
										return -1;
								case -1:
										return -1;
								default:
										n = 0;
								}
						}else
						{
								return n;
						}
				}
				
				bytes_transferred += n;
				if(timeout != 0)
				{
						t_uint64 elapsed = watch.Elapsed() / 1000;
						if(elapsed < time_temp)
						{
								time_temp -= elapsed;
						}else
						{
								NetSetLastError(ETIME);
								return -1;
						}
				}
		}
		
		return static_cast<int>(bytes_transferred);
}


bool SockStream::Open()
{
		assert(!IsValid());
		return NetSock::Open(AF_INET, SOCK_STREAM, 0);

}

bool SockStream::Open(const InetAddress &local_addr)
{
		bool res = SockStream::Open();
		if(!res) return false;
		return (::bind(m_sock, local_addr.Saddr(), local_addr.Length()) == 0);
}

/*
void SockStream::Close()
{
		NetSock::Close();
}*/

bool SockStream::CloseReader()
{
		if(m_sock != NET_INVALID_SOCKET)
		{
				return (::shutdown(m_sock, SD_RECEIVE) == 0);
		}else
		{
				return false;
		}
}


bool SockStream::CloseWriter()
{
		if(m_sock != NET_INVALID_SOCKET)
		{
				return (::shutdown(m_sock, SD_SEND) == 0);
		}else
		{
				return false;
		}

}


int SockStream::Available()const
{
		int buf_n = 0;
		int res = (Control(FIONREAD,  (void*)&buf_n) ? buf_n : -1);
		return res;

}

SockStream::SockStream(NetHandle sock_handle)
{
		assert(sock_handle != NET_INVALID_SOCKET);
		m_sock = sock_handle;
}

SockStream::SockStream(void)
{

		

}

SockStream::~SockStream()
{


}

NetHandle SockStream::Attach(NetHandle sock)
{
		NetHandle tmp = m_sock;
		m_sock = sock;
		return tmp;

}

}





















#if(0)

int SockStream::Recv(t_byte *buf, size_t len, size_t *bt, int flags)const
{
		assert(IsValid());

		size_t temp;
		size_t &bytes_transferred = bt == 0 ? temp : *bt;
		bytes_transferred = 0;
		int n = 0;
		
		for(bytes_transferred = 0;	bytes_transferred < len; bytes_transferred += n)
		{
				n = Recv(buf + bytes_transferred, len - bytes_transferred, flags);
				
				if (n == 0)
				{
						return 0;
				}else if (n == -1)
				{
						if (NetGetLastError() == EWOULDBLOCK && 
								Utility::HandleReadReady(m_sock) == 1)
						{
								n = 0;
								continue;
						}else
						{
								return -1;
						}
				}
		}

		return static_cast<int>(bytes_transferred);
}



int SockStream::Send(const t_byte *buf, size_t len, size_t *bt, int flags)const
{

		assert(IsValid());

		size_t temp;
		size_t &bytes_transferred = bt == 0 ? temp : *bt;
		bytes_transferred = 0;
		int n = 0;
		
		for(bytes_transferred = 0;	bytes_transferred < len; bytes_transferred += n)
		{
				n = Send(buf + bytes_transferred, len - bytes_transferred, flags);
				
				if (n == 0)
				{
						return 0;
				}else if (n == -1)
				{
						if (NetGetLastError() == EWOULDBLOCK && 
								Utility::HandleReadReady(m_sock) == 1)
						{
								n = 0;
								continue;
						}else
						{
								return -1;
						}
				}
		}

		return static_cast<int>(bytes_transferred);

}
#endif