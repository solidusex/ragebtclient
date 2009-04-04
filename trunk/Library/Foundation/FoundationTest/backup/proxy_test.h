// FoundationTest.cpp : Defines the entry point for the console application.
//

#if(0)

#include "Foundation.h"
#pragma comment(lib, "Foundation.lib")


#include <windows.h>
#include <ctime>
#include <bitset>
#include <iostream>
#include <queue>
#include <cctype>
#include <stack>
#include <fstream>

using namespace std;

 
using namespace FileSysSpace;



using namespace StreamSpace;
using namespace ThreadSpace;

using namespace MsgSpace;


using namespace NetSpace;

using namespace DSSpace;

#define PATH_D L"i:\\BT Cache\\"

static t_ulong main_id = 0;

#include <setjmp.h>

#include <locale.h>

#include "test.h"

#include <cmath>

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <algorithm>
#include <string>
#include <conio.h>


using namespace std;
#include<windows.h>


class TestProxy
{
public:
		void test()
		{
				printf("beg\n");

				Acceptor lis_sock;
				InetAddress addr;
				addr.SetPort(8512);
				
				bool res = lis_sock.Open(addr);
				assert(res);

				SockStream ss;
				
				printf("listen start\n");
				res = lis_sock.Accept(ss);
				
				assert(res);

				printf("listen success\n");

				res = ss.GetRemoteAddr(addr);
				assert(res);
				printf("remote address == %s\n", addr.Str().c_str());


				t_byte buf[20];

				int n = ss.Recv(buf, 20);

				

				printf((const char*)buf);

		}
};


class TestBindProxy
{
public:
		void test()
		{
				printf("beg bind\n");

				Acceptor lis_sock;
				InetAddress addr;
				addr.SetPort(9512);
				
				bool res = lis_sock.Open(addr);
				assert(res);

				SockStream ss;
				
				printf("bind listen start\n");
				res = lis_sock.Accept(ss);
				
				assert(res);

				printf("listen success\n");

				res = ss.GetRemoteAddr(addr);
				assert(res);
				printf("remote address == %s\n", addr.Str().c_str());


				t_byte buf[20];

				int n = ss.Recv(buf, 20);

				

				printf((const char*)buf);

		}
};



class ConnTestProxy
{
public:
		InetAddress proxy_addr;
public:
		void test()
		{
				SockStream sock_ss;
				bool res = sock_ss.Open();

				assert(res);


				printf("connecting to proxy_server\n");
				res = Connector::Connect(sock_ss, proxy_addr);

				if(res)
				{
						printf("successxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
						char buf[20];
						strcpy(buf, "hello proxy\n");
						sock_ss.Send((t_byte*)buf, 20);
						
				}


		}


};

/*
# of bytes:
		+----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
    	   1    1      2              4           variable       1

# of bytes:		   
		+----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
 		  1    1      2              4

*/

#pragma pack(push, 1)

typedef struct ProxyPacket
{
		t_byte			version;
		t_byte			command;
		t_uint16		dest_port;
		t_uint32		dest_ip;
		t_byte			packet_null;

}ProxyPacket;

typedef struct ReplyPacket
{
		t_byte			version;
		t_byte			command;
		t_uint16		dest_port;
		t_uint32		dest_ip;
}ReplyPacket;
#pragma pack(pop)

#define PROXY_IP "222.221.6.144"

SockStream* test_connect()
{
		SockStream *sock_ss = new SockStream();

		bool res = sock_ss->Open();

		assert(res);

		InetAddress addr(PROXY_IP, 1080);

		res = Connector::Connect(*sock_ss, addr);

		if(!res)
		{
				printf("connect to proxy server failed\n");
				abort();
		}


		addr.SetAddr("61.149.113.217");
		addr.SetPort( 8512);
		ProxyPacket pkg;

		pkg.version = 4;
		pkg.command = 1;
		pkg.dest_port = htons(addr.Port());
		pkg.dest_ip =  addr.IP();

		pkg.packet_null = 0;

		int n = sock_ss->Send((const t_byte*)&pkg, sizeof(pkg));

		if(n != sizeof(pkg))abort();



		ReplyPacket rep_pkg;



		n = sock_ss->Recv((t_byte*)&rep_pkg, sizeof(ReplyPacket));

		if(n != sizeof(ReplyPacket))
		{
				printf("n == %d\n", n);
				abort();
		}



		printf("%d\n", (int)rep_pkg.command);

		return sock_ss;
}



SockStream* test_bind(InetAddress &addr_out)
{
		SockStream *sock_ss = new SockStream();

		bool res = sock_ss->Open();

		assert(res);

		InetAddress addr(PROXY_IP, 1080);

		res = Connector::Connect(*sock_ss, addr);

		if(!res)
		{
				printf("connect to proxy server failed\n");
				abort();
		}


		addr.SetAddr("61.149.113.217");
		addr.SetPort(9512);
		ProxyPacket pkg;

		pkg.version = 4;
		pkg.command = 2;
		pkg.dest_port = htons(addr.Port());
		pkg.dest_ip =  addr.IP();

		pkg.packet_null = 0;

		int n = sock_ss->Send((const t_byte*)&pkg, sizeof(pkg));

		if(n != sizeof(pkg))abort();



		ReplyPacket rep_pkg;



		n = sock_ss->Recv((t_byte*)&rep_pkg, sizeof(ReplyPacket));

		if(n != sizeof(ReplyPacket))
		{
				printf("n == %d\n", n);
				abort();
		}

		printf("%d\n", (int)rep_pkg.command);

		addr_out.SetAddr(rep_pkg.dest_ip);
		addr_out.SetPort(ntohs(rep_pkg.dest_port));

		return sock_ss;
}


int main()
{		
		NetInit();

		Thread *thd = new Thread(new TestProxy, &TestProxy::test);
		new Thread(new TestBindProxy, &TestBindProxy::test);
		
		cin.get();
		
		SockStream *pconn = test_connect();
		
		pconn->Send((t_byte*)"123", 4);

		t_byte buf[20];

		int n = pconn->Recv(buf, 20);

		printf("n == %d\n", n);
		

		InetAddress addr;
		SockStream *pbind = test_bind(addr);
		



		
		ConnTestProxy * pct = new ConnTestProxy();

		pct->proxy_addr = addr;

		Thread *pthd = new Thread(pct, &ConnTestProxy::test);


		ReplyPacket rep_pkg;
		
		printf("recving second reply\n");

		n = pbind->Recv((t_byte*)&rep_pkg, sizeof(ReplyPacket));

		if(n != sizeof(ReplyPacket))
		{
				printf("n == %d\n", n);
				abort();
		}

		addr.SetAddr(rep_pkg.dest_ip);
		addr.SetPort(ntohs(rep_pkg.dest_port));

		printf("in sock == %s:%d\n", addr.Str().c_str(), addr.Port());

		/*



		addr.SetPort( 9512);
		pkg.version = 4;
		pkg.command = 2;
		pkg.dest_port = htons(addr.Port());
		pkg.dest_ip =  addr.IP();

		pkg.packet_null = 0;

		n = sock_ss.Send((const t_byte*)&pkg, sizeof(pkg));

		if(n != sizeof(pkg))abort();
		
		

		

		

		n = sock_ss.Recv((t_byte*)&rep_pkg, sizeof(ReplyPacket));
		
		if(n != sizeof(ReplyPacket))
		{
				printf("n == %d\n", n);
				abort();
		}
		


		printf("%d\n", (int)rep_pkg.command);


		InetAddress proxy_addr;
		proxy_addr.SetAddr(rep_pkg.dest_ip);
		proxy_addr.SetPort(::ntohs(rep_pkg.dest_port));

		printf("%s : %d\n", proxy_addr.Str().c_str(), proxy_addr.Port());


				
		ConnTestProxy * pct = new ConnTestProxy();

		pct->proxy_addr = proxy_addr;

		Thread *pthd = new Thread(pct, &ConnTestProxy::test);

		printf("recving second reply\n");
		n = sock_ss.Recv((t_byte*)&rep_pkg, sizeof(ReplyPacket));

		assert(n == sizeof(ReplyPacket));*/
		

		printf("done\n");
		cin.get();

		return 0;

}
















class connect1{
public:
		connect1();
		int ConnectSocks5(SOCKET ProxySocket,char *usr,char *psw,char *DstHost,int DstPort);
		int ConnectSocks4(SOCKET ProxySocket,char *usr,char *psw,char *DstHost,int DstPort);
		int ConnectHttp(SOCKET ProxySocket,char *usr,char *psw,char *DstHost,int DstPort);
		void to64frombits(unsigned char *out, const unsigned char *in, int inlen);
		void test_sending(SOCKET s);
};

connect1::connect1(){
}

//return value:0=success; -1=connect failed; -2=wrong usr and psw
int connect1::ConnectSocks5(SOCKET ProxySocket,char *usr,char *psw,char *DstHost,int DstPort){
		/*BOOLEAN bNeedAuth = false;
		if(usr!=NULL)
		bNeedAuth = true;
		bNeedAuth = true;
		*/

		int nLen = 0;
		char szBuffer[1024 + 1] = {""};
		/*if (!bNeedAuth) {
		szBuffer[0] = 5;
		szBuffer[1] = 1;
		szBuffer[2] = 0;
		nLen = 3;
		send(ProxySocket, szBuffer, nLen, 0);
		nLen = 2;
		recv(ProxySocket, szBuffer, nLen, 0);
		if(szBuffer[0] != 5 || szBuffer[1] != 0)
		{
		return -1;
		}
		} 
		else */
		{
				szBuffer[0] = 5;
				szBuffer[1] = 2;
				szBuffer[2] = 0;
				szBuffer[3] = 2;
				nLen = 4;
				send(ProxySocket, szBuffer, nLen, 0);
				nLen = 2;
				recv(ProxySocket, szBuffer, nLen, 0);
				char *lpszUserName = usr;
				char *lpszPassword = psw;
				if(szBuffer[0] == 5)//need auth
				{
						if(szBuffer[1] == 2)
						{
								szBuffer[0] = 1;
								nLen = strlen(lpszUserName);
								szBuffer[1] = nLen;
								strncpy(szBuffer+2, lpszUserName,strlen(lpszUserName));
								nLen += 2;
								szBuffer[nLen] = strlen(lpszPassword);
								strcpy(szBuffer+nLen+1, lpszPassword);
								nLen = nLen + 1 + strlen(lpszPassword);
								send(ProxySocket, szBuffer, nLen, 0);
								nLen = 2;
								recv(ProxySocket, szBuffer, nLen, 0);
								if(szBuffer[1] != 0)
								{
										return -2;
								}
						}

				}

		}

		//translate DestAddr
		long v = inet_addr(DstHost);
		szBuffer[0] = 5;
		szBuffer[1] = 1;
		szBuffer[2] = 0;
		szBuffer[3] = 1;//ipv4
		szBuffer[4] = ((unsigned char *)&(v))[0];//10;
		szBuffer[5] = ((unsigned char *)&(v))[1];//23;
		szBuffer[6] = ((unsigned char *)&(v))[2];//7;
		szBuffer[7] = ((unsigned char *)&(v))[3];//197;
		unsigned short uPort = htons(DstPort);
		memcpy(szBuffer+8, &uPort, 2);
		nLen = 8 + 2;
		send(ProxySocket, szBuffer, nLen, 0);
		nLen = 10;
		recv(ProxySocket, szBuffer, nLen, 0);

		if(szBuffer[0] != 5 || szBuffer[1] != 0)
		{
				return -1;
		} else {
				//printf("Connect socks proxy ok!\n");
				return 0;
		}

		return 0;
}

//return value:0=success; -1=connect failed;
int connect1::ConnectSocks4(SOCKET ProxySocket,char *usr,char *psw,char *DstHost,int DstPort){
		BOOLEAN bNeedAuth = false;
		if(usr!=NULL)
				bNeedAuth = true;
		int nLen = 0;
		char szBuffer[1024 + 1] = {""};
		szBuffer[0] = 4;
		szBuffer[1] = 1;
		//szBuffer[2] = 0;
		unsigned short uPort = htons(DstPort);
		memcpy(szBuffer+2, &uPort, 2);
		long v = inet_addr(DstHost);
		szBuffer[4] = ((unsigned char *)&(v))[0];//10;
		szBuffer[5] = ((unsigned char *)&(v))[1];//23;
		szBuffer[6] = ((unsigned char *)&(v))[2];//7;
		szBuffer[7] = ((unsigned char *)&(v))[3];//197;

		nLen = 8;
		send(ProxySocket, szBuffer, nLen, 0);
		nLen = 8;
		recv(ProxySocket, szBuffer, nLen, 0);
		if(szBuffer[0] != 0 || szBuffer[1] != 90)
		{
				return -1;
		}
		else
		{
				//printf("Connect socks proxy ok!\n");
				return 0;
		}

		return 0;
}

//return value:0=success; -1=connect failed; -2=wrong usr and psw
int connect1::ConnectHttp(SOCKET ProxySocket,char *usr,char *psw,char *DstHost,int DstPort){
		BOOLEAN bNeedAuth = false;
		if(usr!=NULL)
				bNeedAuth = true;
		int nLen = 0;
		char szBuffer[1024 + 1] = {""};
		if (!bNeedAuth) {
				sprintf(szBuffer,"CONNECT %s:%d HTTP/1.0\r\n\r\n",DstHost,DstPort);  
		} 
		else 
		{
				//Proxy-Authorization:   Basic     
				char   szAuth[1024+1] = {""};   
				char   szAuthT[1024+1] = {""};   
				sprintf(szAuthT, "%s:%s", usr, psw);   
				to64frombits((unsigned char*)szAuth,(unsigned char*)szAuthT,strlen(szAuthT));   
				sprintf(szBuffer, "CONNECT %s:%d HTTP/1.0\r\nProxy-Authorization:Basic %s\r\n\r\n",DstHost,DstPort,szAuth);   
		}

		nLen = strlen(szBuffer);
		send(ProxySocket, szBuffer, nLen, 0);
		nLen   =   1024;   
		recv(ProxySocket,szBuffer,nLen,0);   
		if(strnicmp(szBuffer,"HTTP/1.0 200",strlen("HTTP/1.0 200"))==0 ||     
				strnicmp(szBuffer,"HTTP/1.1 200",strlen("HTTP/1.1 200")) == 0)   
				return 0;   
		else
				return -1;

		return 0;
}

//base64 encode
void connect1::to64frombits(unsigned char *out, const unsigned char *in, int inlen){
		const char base64digits[] =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		for (; inlen >= 3; inlen -= 3)
		{
				*out++ = base64digits[in[0] >> 2];
				*out++ = base64digits[((in[0] << 4) & 0x30)|(in[1] >> 4)];
				*out++ = base64digits[((in[1] << 2) & 0x3c)|(in[2] >> 6)];
				*out++ = base64digits[in[2] & 0x3f];
				in += 3;
		}
		if (inlen > 0)
		{
				unsigned char fragment;
				*out++ = base64digits[in[0] >> 2];
				fragment = (in[0] << 4) & 0x30;
				if (inlen > 1)
						fragment |= in[1] >> 4;
				*out++ = base64digits[fragment];
				*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
				*out++ = '=';
		}
		*out = NULL;
}

void connect1::test_sending(SOCKET MySocket){
		//tests ending
		//+=+=+=+=+=
		char send_string [ ] = 
				"CONNECT 10.23.7.194:443 HTTP/1.0\r\nUser-Agent: sslvpn\r\n\r\n";

		int nret = 0;
		nret = send( MySocket, send_string, sizeof( send_string ) -1, 0 );
		if( nret <= 0 )
		{
				printf("Could not send any data.");
				exit(1);
		}

		int MAX = 1024;
		char *recv_string = new char [MAX];
		memset(recv_string,0,MAX-1);
		nret = 0;
		nret = recv( MySocket, recv_string, MAX -1, 0 );
		if( nret <= 0 )
		{
				printf("Did not receive any data.");
				exit(1);
		}
		printf("%s\n",recv_string);
}

int main(){
		WSADATA wsaData; 
		WORD wVersionRequested = MAKEWORD(2, 0);   
		WSAStartup(wVersionRequested, &wsaData);

		if (WSAStartup(wVersionRequested, &wsaData) < 0 )  
		{
				printf("Wrong version");
				exit(1);
		}

		SOCKET MySocket = socket(AF_INET, SOCK_STREAM, 0);
		char *targetip = "192.168.0.2";
		
		struct hostent *target_ptr = gethostbyname( targetip );

		if( target_ptr == NULL ) 
		{
				printf("Can not resolve name.");
				exit(1);
		}

		sockaddr_in sock;
		memcpy( &sock.sin_addr.s_addr, target_ptr->h_addr, target_ptr->h_length );
		sock.sin_family = AF_INET;
		sock.sin_port = htons( 1080 );

		if ( connect (MySocket, (struct sockaddr *)&sock, sizeof (sock) ) )
		{
				printf("Failed to connect proxy.\n");
				exit(1); 
		}

		connect1 toms;
		connect1 *p;
		p = &toms;
		if(p->ConnectHttp(MySocket,"tomqq","888888","192.168.0.2",808)!=0)
				printf("connect failed!\n");
		else
				//test connect  
				p->test_sending(MySocket);

		return 0;
}



#endif
