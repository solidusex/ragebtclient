#pragma once

#if(0)


static void base64_encoder(unsigned char *out, const unsigned char *in, int inlen)
{
		const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

				
				printf("listen start\n");
				while(true)
				{
						SockStream *ss = new SockStream;
						res = lis_sock.Accept(*ss);

						assert(res);

						printf("listen success\n");

						res = ss->GetLocalAddr(addr);
						assert(res);
						printf("remote address == %s : %d\n", addr.Str().c_str(), addr.Port());

						::Sleep(35);
						t_byte buf[41];
						memset(buf,40,0);
						
						strcpy((char*)buf, "Hello client\n");
						int n = ss->Send(buf, 40, NULL,NULL,0);
						
						
						
				}

		}
};



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




#define PROXY_IP "192.168.0.2"

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
		addr.SetPort(8512);
		
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

		printf("rep command == %d\n", (int)rep_pkg.command);

		if(rep_pkg.command != 90)
		{
				delete sock_ss;
				return NULL;
		}

		return sock_ss;
}



SockStream* test_connect4a()
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

		addr.SetAddr("x.0.0.0");
		addr.SetPort(8512);
		t_byte buf[600];

		buf[0] = 4;
		buf[1] = 1;
		
		t_uint16 port = htons(addr.Port());
		t_uint32 ip = addr.IP();
		
		((t_byte*)&ip)[0] = 0;
		((t_byte*)&ip)[1] = 0;
		((t_byte*)&ip)[2] = 0;
		((t_byte*)&ip)[3] = 'x';

		memcpy(&buf[2], &port, 2);
		memcpy(&buf[4], &ip, 4);
		buf[8] = 0;
		
		size_t len = strlen("61.149.113.217");
		memcpy(&buf[9],"61.149.113.217", len );
		buf[9 + len] = 0;
		
		int n = sock_ss->Send(buf, 9 + len + 1);

		if(n != 9 + len + 1)abort();
		


		ReplyPacket rep_pkg;
		


		n = sock_ss->Recv((t_byte*)&rep_pkg, sizeof(ReplyPacket));

		if(n != sizeof(ReplyPacket))
		{
				printf("n == %d\n", n);
				abort();
		}

		printf("command == %d\n", (int)rep_pkg.command);
		
		if(rep_pkg.command != 90)
		{
				delete sock_ss;
				return NULL;
		}


		return sock_ss;
}





SockStream* test_connect5(const std::string &name, const std::string &password)
{
		if(name.size() > 255 || password.size() > 255)return NULL;

		SockStream *sock_ss = new SockStream();
		t_byte buf[1024 + 1] = {""};

		bool res = sock_ss->Open();

		assert(res);

		InetAddress addr(PROXY_IP, 1080);

		res = Connector::Connect(*sock_ss, addr);

		if(!res)
		{
				printf("connect to proxy server failed\n");
				abort();
		}

		size_t len = 0;

		if(name.empty() || password.empty())
		{
				buf[0] = 5;
				buf[1] = 1;
				buf[2] = 0;
				len = 3;
		}else
		{
				buf[0] = 5;
				buf[1] = 2;
				buf[2] = 0;
				buf[3] = 2;
				len = 4;
		}
	

		int n = sock_ss->Send(buf,len);

		if(n != len)assert(0);

		

		n = sock_ss->Recv(buf, 2);
		
		if(n != 2)
		{
				printf("recv failed\n");
				abort();
		}

		if(buf[0] !=5 || (buf[1] != 0 && buf[1] != 2))
		{
				printf("reply ans failed ver == %d   method == %d\n", (int)buf[0], (int)buf[1]);
				assert(0);
				abort();
		}

		
		if(buf[1] == 2)
		{
				printf("need auth\n");
				size_t idx = 0;
				buf[idx] = 1;
				idx++;
				buf[idx] = name.size();
				idx++;
				memcpy(&buf[idx], (t_byte*)name.c_str(), name.size());
				idx += name.size();

				buf[idx] = password.size();
				idx++;
				memcpy(&buf[idx], (t_byte*)password.c_str(), password.size());

				n = sock_ss->Send(buf, idx + 1);

				if(n != idx + 1) assert(0);

				n = sock_ss->Recv(buf,2);

				if(n != 2)assert(0);

				if(buf[0] != 1){printf("ver == %d\n", buf[0]); assert(0);}

				if(buf[1] != 0)assert(0);


		}
		
		addr.SetAddr("61.149.113.217");
		addr.SetPort(8512);

		
		
		t_uint32 ip = addr.IP();
		t_uint16 port = htons(addr.Port());

		buf[0] = 5;
		buf[1] = 1;
		buf[2] = 0;
		buf[3] = 3;
		
		buf[4] = addr.Str().size();

		memcpy(&buf[5],addr.Str().c_str(), buf[4]);
		
		size_t nlen = 5 + buf[4];

		memcpy(&buf[nlen], &port, 2);
		nlen +=2;

		n = sock_ss->Send(buf, nlen);

		if(n != nlen)
		{
				printf("send failed n == %d\n", n);
				abort();
		}
		printf("nlen == %d\n", nlen);

		WSASetLastError(0);
		n = sock_ss->Recv(buf, 4);
		if(n != 4)
		{
				printf("LastError == %d\n", WSAGetLastError());
				assert(0);
		}

		if(buf[0] != 5 && buf[1] != 0 && buf[2] != 0)
		{
				printf("buf[0] == %d\n", (int)buf[0]);
		}
		
		
		switch(buf[3])
		{
		case 1:
		{
				printf("xxxxxxxxxxxxxxxxxxxx\n");
				n = sock_ss->Recv(buf, 6);
				if(n != 6)
				{
						printf("recv failed n == %d\n",n);
						assert(0);
						abort();
				}

				addr.SetAddr(*(t_uint32*)buf);
				addr.SetPort(ntohs((*(t_uint16*)(buf + 4))));

				printf("connect success == %s:%d\n", addr.Str().c_str(), addr.Port());
		}		
				break;
		case 3:
		{
				if(sock_ss->Recv(buf, 1) != 1)
				{
						assert(0);
						abort();
				}
				size_t domain_len = buf[0];
				if(buf[0] == 0)
				{
						assert(0);
						abort();
				}
				
				if(sock_ss->Recv(buf, domain_len) != domain_len)
				{
						assert(0);
						abort();
				}
		}
		case 4:
		{
				if(sock_ss->Recv(buf, 18) != 18)
				{
						assert(0);
						abort();
				}
				break;

				
		}
				break;
		default:
				printf("buf[3] == %d, protocol invalid\n", buf[3]);
				return NULL;
		
		}

		printf("success\n");

		abort();

		
		
		return NULL;
}


SockStream* test_connect_http(const std::string &name, const std::string &password)
{
		if(name.size() > 255 || password.size() > 255)return NULL;

		SockStream *sock_ss = new SockStream();
		

		bool res = sock_ss->Open();

		assert(res);

		InetAddress addr(PROXY_IP, 808);

		res = Connector::Connect(*sock_ss, addr);

		if(!res)
		{
				printf("connect to proxy server failed\n");
				abort();
		}

		char buf[8192];

		sprintf(buf, "CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n", "192.168.0.2", 8512, "192.168.0.2",808);

		char pass_buf[4096];
		pass_buf[0] = 0;
		char out_buf[8192];
		strcat(pass_buf, name.c_str());
		strcat(pass_buf, ":");
		strcat(pass_buf, password.c_str());

		printf("pass_buf == %s\n", pass_buf);
		base64_encoder((unsigned char*)out_buf, (unsigned char*)pass_buf,strlen(pass_buf));

		printf("out_buf == %s\n", out_buf);

		strcat((char*)buf, "Authorization: Basic ");
		strcat((char*)buf, out_buf);
		strcat((char*)buf, "\r\n");

		strcat((char*)buf, "Proxy-Authorization: Basic ");
		strcat((char*)buf, out_buf);
		strcat((char*)buf, "\r\n\r\n");
		
		printf("%s\n",buf);

		
		
		int n = sock_ss->Send((t_byte*)buf, strlen(buf) + 1, NULL, NULL,0);

		if(n != strlen(buf) + 1)
		{
				printf("n == %d\n", n);
				assert(0);
		}

		n = sock_ss->Recv((t_byte*)buf,40, NULL,NULL,0);
		printf("buf == %s\n",buf);
		printf("getnetlasterror == %d\n", ::WSAGetLastError());
		if(n != 40)
		{
				printf("n == %d\n", n);
				assert(0);
		}

		memset(buf,0,8192);
		strcpy(buf,"Hello Proxy");
		
		n = sock_ss->Send((const t_byte*)buf, strlen(buf) + 1, NULL,NULL,0);
		
		printf("n == %d\n", n);
		if(n == 0 || n == -1)
		{
				assert(0);
		}

		cin.get();
		

		
		
		return sock_ss;




}


int main()
{
		NetInit();

		cin.get();
		Thread *pthd = new Thread(new TestProxy, &TestProxy::test);
		
		cin.get();
		SockStream *pconn = test_connect_http("solidus","1");

		if(pconn == NULL) { printf("failed\n");abort();}

		char buf[50];

		strcpy(buf, "xxxxxxxxxxxxxxxxxxxxxxx\n");
		
		pconn->Send((t_byte*)buf, 20);

		cin.get();
		delete pthd;
		delete pconn;

		printf("done\n");
		cin.get();

		return 0;

}






#endif


// BitSet.h

#ifndef BITSET_H
#define BITSET_H

#include <iostream>
#include <stdexcept>
#include <limits>

// Class BitSet:
//   Class BitSet is a sequence of bits, and some operations are defined on it.
// Notes:
// (1) When an operation will be done on the bit at position 'pos', 
//       you have to be sure that 'pos' satisfies the inequation pos < size(),
//       where size() returns the number of bits in this BitSet object.
//     If pos >= size(), an exception with type out_of_range will be thrown.
//     Note that the position 'pos' starts at zero. 
// (2) In these operators: &=, |=, ^=, &, | and ^, 
//       if the two BitSet objects have unequal values returned by size() , 
//       then an exception with type lenth_error will be thrown.
// (3) When the memory allocation fails, an exception with type bad_alloc will be thrown.
class BitSet
{
public:
    typedef unsigned char block_type;
    typedef unsigned char size_type;
    
public:
    enum {BLOCK_BYTES = sizeof(block_type)};
    enum {BLOCK_BITS = std::numeric_limits<block_type>::digits};
    
public:
    //--- copy control functions -------------------------
    BitSet(size_type bitNum);
    BitSet(const BitSet& bits);
    BitSet& operator= (const BitSet&);
    ~BitSet();
    
    //--- bit opearations --------------------------------
    // left-shift operator
    BitSet& operator<<= (size_type num);    
    // right-shift operator
    BitSet& operator>>= (size_type num);
    
    // set the bit at position pos when tagSet is true(default); otherwise, clear the bit 
    BitSet& set(size_type pos, bool tagSet = true);
    // clear all the bits when tagClear is true(default); otherwise, set all the bits
    BitSet& clear(bool tagClear = true);
    
    // flip all the bits
    BitSet& flip();
    BitSet operator~ ();
    // flip the bit at position pos
    BitSet& flip(size_type pos);
    
    // test the bit at position pos
    bool test(size_type pos) const;    

    //--- operation between two BitSet objects-----------------
    BitSet& operator&= (const BitSet& bit);     // bitwise AND
    BitSet& operator|= (const BitSet& bit);     // bitwise inclusive OR
    BitSet& operator^= (const BitSet& bit);     // bitwise exclusive OR
    bool operator== (const BitSet&) const;      // equal: bitwise equal
    
    // get the number of bits in this BitSet object
    size_type size() const;
    
private:
    void leftShift(size_type num);    // left shift
    void rightShift(size_type num);   // right shift    

private:
    size_type m_bitNum;    // the number of bits in this BitSet object
    size_type m_size;      // the number of elements with type BitSet::block_type used to store the bits
    block_type *m_pBits;   // a pointer to an array with element type BitSet::block_type,
                           // used to store the bits information
    block_type m_mask;     // used to filter the redundant bits 
                           // in the array element with the highest address
};

// declarations of nonmember functions
BitSet operator<< (const BitSet&, BitSet::size_type);   // left-shift operator
BitSet operator>> (const BitSet&, BitSet::size_type);   // right-shift operator
BitSet operator& (const BitSet&, const BitSet&);        // bitwise AND
BitSet operator| (const BitSet&, const BitSet&);        // bitwise inclusive OR
BitSet operator^ (const BitSet&, const BitSet&);        // bitwise exclusive OR
bool operator!= (const BitSet&, const BitSet&);         // not equal

//--- inline member functions ----------------------------------
//--- copy control functions --------------------
inline BitSet::BitSet(size_type bitNum)
{
    m_bitNum = bitNum;
    size_type freeBits = (BLOCK_BITS - m_bitNum % BLOCK_BITS) % BLOCK_BITS;
    m_size = m_bitNum / BLOCK_BITS + (freeBits == 0 ? 0 : 1);
    m_pBits = new block_type[m_size]; 
    if (m_pBits == NULL)
        throw std::bad_alloc();
    clear();  // clear all bits
    // calculate the mask value
    m_mask = ~block_type(0);
    m_mask >>= freeBits;
}

inline BitSet::BitSet(const BitSet& bit)
{
    m_size = bit.m_size;
    m_pBits = new block_type[m_size];
    if (m_pBits == NULL)
        throw std::bad_alloc();
    memcpy(m_pBits, bit.m_pBits, m_size * BLOCK_BYTES);
    m_bitNum = bit.m_bitNum;
    m_mask = bit.m_mask;
}

inline BitSet& BitSet::operator= (const BitSet& bit)
{
    if (this == &bit)    // self assignment
        return (*this);
    if (m_size != bit.m_size) {
        delete [] m_pBits;
        m_size = bit.m_size;
        m_pBits = new block_type[m_size];
        if (m_pBits == NULL)
            throw std::bad_alloc();
    }
    memcpy(m_pBits, bit.m_pBits, m_size * BLOCK_BYTES);
    m_bitNum = bit.m_bitNum;
    m_mask = bit.m_mask;
    return (*this);
}

inline BitSet::~BitSet() 
{
    delete [] m_pBits;
}

//--- bit opearations -----------------------------
// left shift operator
inline BitSet& BitSet::operator<<= (size_type num) 
{
    leftShift(num);
    return (*this);
}

// right shift operator
inline BitSet& BitSet::operator>>= (size_type num) 
{
    rightShift(num);
    return (*this);
}

// clear all the bits when tagClear is true(default); otherwise, set all the bits


// flip all the bits
inline BitSet& BitSet::flip() 
{
    for (size_type i = 0; i < m_size; ++i)
        m_pBits[i] = ~m_pBits[i];
    m_pBits[m_size-1] &= m_mask;  // filter the redundant bits
    return (*this);
}

// flip all the bits
inline BitSet BitSet::operator~ () 
{
    return BitSet(*this).flip();
}  

//--- operation between two BitSet objects-----------------
// bitwise AND
inline BitSet& BitSet::operator&= (const BitSet& bit) 
{
    if (m_bitNum != bit.m_bitNum) 
        throw std::length_error("Error: two BitSet objects have not equal lengths");
    for (size_type i = 0; i < m_size; ++i) 
        m_pBits[i] &= bit.m_pBits[i];
    return (*this);
}

// bitwise inclusive OR
inline BitSet& BitSet::operator|= (const BitSet& bit) 
{
    if (m_bitNum != bit.m_bitNum) 
        throw std::length_error("Error: two BitSet objects have not equal lengths");
    for (size_type i = 0; i < m_size; ++i) 
        m_pBits[i] |= bit.m_pBits[i];
    return (*this);
}

// bitwise exclusive OR
inline BitSet& BitSet::operator^= (const BitSet& bit) 
{
    if (m_bitNum != bit.m_bitNum) 
        throw std::length_error("Error: two BitSet objects have not equal lengths");
    for (size_type i = 0; i < m_size; ++i) 
        m_pBits[i] ^= bit.m_pBits[i];
    return (*this);
}

// equal: bitwise equal
inline bool BitSet::operator== (const BitSet &bit) const
{
    if (m_bitNum != bit.m_bitNum) 
        return false;
    for (size_type i = 0; i < m_size; ++i) 
        if (m_pBits[i] != bit.m_pBits[i])
            return false;
    return true;
}

// get the number of bits in this BitSet object
inline BitSet::size_type BitSet::size() const
{
    return m_bitNum;
}

//--- nonmember functions -----------------------------------
// left shift operator
inline BitSet operator<< (const BitSet &bit, BitSet::size_type num)
{
    return BitSet(bit) <<= num;
}

// right shift operator
inline BitSet operator>> (const BitSet &bit, BitSet::size_type num)
{
    return BitSet(bit) >>= num;
}

// bitwise AND
inline BitSet operator& (const BitSet &lhs, const BitSet &rhs) 
{
    return BitSet(lhs) &= rhs;
}

// bitwise inclusive OR
inline BitSet operator| (const BitSet &lhs, const BitSet &rhs) 
{
    return BitSet(lhs) |= rhs;
}

// bitwise exclusive OR
inline BitSet operator^ (const BitSet &lhs, const BitSet &rhs) 
{
    return BitSet(lhs) ^= rhs;
}

// not equal
inline bool operator!= (const BitSet &lhs, const BitSet &rhs)
{
    return !(lhs == rhs);
}

// endif: BITSET_H
#endif 