#if(0)
t_uint32 str_to_num(const std::string &str, size_t base)
{
		t_uint32 res = 0;

		for(size_t i = 0; i < str.size(); ++i)
		{
				res *= base;
				switch(str[i])
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
						res += (str[i] - '0');
						break;
				case 'A':
				case 'a':
						res += 10;
						break;
				case 'B':
				case 'b':
						res += 11;
						break;
				case 'C':
				case 'c':
						res += 12;
						break;
				case 'D':
				case 'd':
						res += 13;
						break;
				case 'E':
				case 'e':
						res += 14;
						break;
				case 'F':
				case 'f':
						res += 15;
						break;
				default:
						assert(0);
				}
		}


		return res;

}

std::string uint8_to_str(t_uint8 num, size_t base)
{
		std::string res;

		while(num > 0)
		{
				t_uint8 n = num % base;
				switch(n)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
						res.insert(res.begin(), n + '0');
						break;
				case 10:	
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
						res.insert(res.begin(), char(n + 55));
						break;
				default:
						assert(0);
				}
				num /= base;
		}
		if(res.empty()) res += "0";
		return res;
}
#endif

#if(0)

typedef FastMutex				MutexT;
//typedef SpinLock				MutexT;
//typedef RecursiveSpinLock		MutexT;
//typedef Mutex					MutexT;
typedef MutexT::ScopeLock		LockT;

Event	event(false);

Barrier	barrier(20);

MutexT mtx;
int x = 0;
class Test
{
public:
		void test()
		{
				TimeSpace::CycleTimer cycle;
			//	printf("overhead == %I64d\n", cycle.GetOverHead());
				while(true)
				{
						event.Wait();

						barrier.Acquire();
						
						cycle.Start();
						
						for(size_t i = 0; i < 3; ++i)
						{
								LockT lock(mtx);
								++x;
						}
						printf("%I64d\n", cycle.Stop());
				}
		}
};

int main()
{
		for(size_t i = 0; i < 20; ++i)
		{
		
				new Thread(new Test, &Test::test);
		}

		while(true)
		{
				cin.get();
				event.Set();
				event.Reset();
		}
		
		printf("done\n");

		cin.get();
		
		return 0;
}




class TaskBMsg : public ThreadSpace::TaskMsg
{
public:
		TaskBMsg()
		{

		}

		virtual ~TaskBMsg()
		{
				
				
		}
};



class TaskImplB : public ThreadSpace::Task
{
public:
		typedef ThreadSpace::MsgList MsgList;
private:
		int m_id;
		t_ulong priority;
public:
		TaskImplB(int id, t_ulong prio) : m_id(id), priority(prio)
		{

		}
public:
		virtual void Process(TaskMsg *pmsg)
		{
				printf("Task id == %d in thread == %d, priority == %d\n", m_id, ::GetCurrentThreadId(), priority);
				
				delete *pmsg;
		}
};

std::vector<Task*> v_task;
ThreadSpace::FastMutex fast_mutex;
ThreadSpace::TaskPool tp;
class InputThread
{
public:
		t_ulong num;
public:
		void input()
		{
				for(size_t i =  0; i < num; ++i)
				{
						FastMutex::ScopeLock lock(fast_mutex);
						Task *ptask = v_task[i % v_task.size()];
						if(ptask != 0)
						{
								tp.PostMessageEx(ptask, new TaskBMsg);
						}
				}

		}

};
int main()
{		
		

		bool res = tp.Start(100);
		
		srand(time(0));
		for(size_t idx = 0; idx < 1000; ++idx)
		{
				Task *ptask = new TaskImplB(idx, idx + 5);
				v_task.push_back(ptask);
				tp.RegisterTask(ptask, idx + 5);
		}
		

		std::string str;
		while(std::getline(cin, str))
		{
				if(str == "quit")
				{
						break;
				}

				if(str == "remove")
				{
						FastMutex::ScopeLock lock(fast_mutex);
						int n = rand()%v_task.size();
						Task *ptask = v_task[n];
						if(ptask != 0)
						{
								res = tp.RemoveTask(ptask, true);
								v_task[n] = 0;
								assert(res);
								printf("task %d was removed\n", n);
						}
						continue;
				}
		
				try{
						t_uint32 x = FormatSpace::NumberParser::ParseInt32(str);
						InputThread *pit = new InputThread;
						pit->num = x;

						new Thread(pit, &InputThread::input);
				}catch(...)
				{
						printf("bud input\n");
						continue;
				}
				
				
				
				
				printf("input\n");
		}



		printf("Closing \n");
		tp.Close();
		printf("Clossed\n");

		printf("done\n");

		return 0;
}

#endif













#if(0)
#define W_SIZE				2	//word size;
#define D_SIZE				4	// dword size;

#define OVER_HEAD			8	//HEAD && FOOTER的负载

#define ALIGN_SIZE			8


#define ALIGN_TO(alloc_size)	(((alloc_size + ALIGN_SIZE)	& ~(ALIGN_SIZE-1)))

#define MIN_MEM_SIZE		(1024*KB)

#define MIN_ALLOC_SIZE		(16)

#define PACK(val, pad)  ((val) | (pad))

/////////////////////////////////////////都是针对纯指针进行操作）//////////////////////

#define GET(ptr) (*(t_uint32*)(ptr))

#define PUT(ptr, val) ( GET(ptr) = (val))

#define GET_SIZE(ptr) (GET(ptr) & ~0x07)			//后三位与0//因为分配的颗粒度不可能低于8

#define GET_THIS_IS_USED(ptr) ((bool)(GET(ptr) & BIT_MARK(0)))		//这块是否已分配//第0位代表这块是否已分配

#define GET_PREV_IS_USED(ptr) ((bool)(GET(ptr) & BIT_MARK(1)))		//上一块是否已分配//第1位代表这块是否已分配

/////////////////////////////////根据给定的内存块寻找相对资源//////////////////////////////

#define HDR_P(ptr)  ((t_byte*)ptr - D_SIZE)

#define FTR_P(ptr)  ((t_byte*)ptr + GET_SIZE(HDR_P(ptr)) - (2 * D_SIZE))

#define NEXT_BLOCK_PTR(ptr) ( (t_byte*)ptr + GET_SIZE(HDR_P(ptr)))

#define PREV_BLOCK_PTR(ptr) ( (t_byte*)ptr - (GET_SIZE( (t_byte*)ptr - D_SIZE)))


class FixedMemPool : NonCopyable
{
public:
		t_byte			*m_p_mem;
		t_byte			*m_end_barrier;
		t_uint32		m_bytes;

private:
		t_byte*	fit_find(t_uint32 size)
		{
				for(t_byte *res_ptr = m_p_mem; res_ptr < m_end_barrier; res_ptr = HDR_P(NEXT_BLOCK_PTR(res_ptr + D_SIZE)))
				{
						if(GET_SIZE(res_ptr) >= size)
						{
								return res_ptr;
						}
				}

				return 0;
		}

		void place(t_byte *chunk_ptr, t_uint32 size)
		{
				t_uint32 chunk_size = GET_SIZE(chunk_ptr);
				if(chunk_size
				
		}
public:
		FixedMemPool(t_uint32 mem_size)
		{
				t_uint32 alloc_size = (mem_size > MIN_MEM_SIZE  ? mem_size : MIN_MEM_SIZE);
				alloc_size = ALIGN_TO(alloc_size);
				m_bytes = alloc_size;
				m_p_mem = (t_byte*)::malloc(alloc_size + D_SIZE);
				
				assert(m_p_mem != 0);

				PUT(m_p_mem, PACK(alloc_size, 0x02));	//设置前一个为已占用；
				
				t_byte *this_ptr = m_p_mem + D_SIZE;
				PUT(FTR_P(this_ptr), PACK(alloc_size, 0x02)); //设置脚标记,前一位已占用
				m_end_barrier = HDR_P(NEXT_BLOCK_PTR(this_ptr));
				//设置终结块为已占用,终结块的前一块为为未被占用
				PUT(m_end_barrier, PACK(0, (0x01|0x00)));
		}

		~FixedMemPool()
		{
				::free(m_p_mem);		
		}

public:
		void* Malloc(t_uint32 size)
		{
				t_uint32 alloc_size = (MIN_MEM_SIZE < size ? MIN_MEM_SIZE : ALIGN_TO(size));

				if(alloc_size >= m_bytes)
				{
						return 0;
				}else
				{
						t_byte *res = fit_find(alloc_size + 4);
						if(res != 0)
						{
								place(res, alloc_size + 4);
								return res + 4;
						}else
						{
								return 0;
						}

				}
		}

		void Free(void *ptr);

		bool HasThisBlock(const void *ptr) const
		{
				bool is_ok = ((ptr > m_p_mem )&& (ptr < m_p_mem + m_bytes));
				return is_ok;
		}
};



int main()
{
		FixedMemPool fmp(500 *MB);

		assert(fmp.m_end_barrier == HDR_P(NEXT_BLOCK_PTR(fmp.m_p_mem + 4)));
		

		cin.get();
		
		return 0;
}
#endif

#if(0)
class CMemoryPool
{
public:
		// Ways the memory pool can grow when it needs to make a new blob.
		enum
		{
				GROW_NONE=0,		// Don't allow new blobs.
				GROW_FAST=1,		// New blob size is numElements * (i+1)  (ie: the blocks it allocates
				// get larger and larger each time it allocates one).
				GROW_SLOW=2			// New blob size is numElements.
		};

		CMemoryPool(int blockSize, int numElements, int growMode = GROW_FAST);
		
		~CMemoryPool();

		void*		Alloc();	// Allocate the element size you specified in the constructor.
		
		void*		Alloc( unsigned int amount );
		
		void		Free(void *pMem);

		// Frees everything
		void		Clear();





private:
		class CBlob
		{
		public:
				CBlob	*m_pPrev, *m_pNext;
				int		m_NumBytes;		// Number of bytes in this blob.
				char	m_Data[1];
		};


		// Resets the pool
		void		Init();
		void		AddNewBlob();
private:

		int			m_BlockSize;
		int			m_BlocksPerBlob;

		int			m_GrowMode;	// GROW_ enum.

		// FIXME: Change m_ppMemBlob into a growable array?
		CBlob			m_BlobHead;
		void			*m_pHeadOfFreeList;
		int				m_BlocksAllocated;
		int				m_PeakAlloc;
		unsigned short	m_NumBlobs;

};


#undef max
#define max(x,y) (((x)>(y)) ? (x) : (y))

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------

CMemoryPool::CMemoryPool(int blockSize, int numElements, int growMode)
{
		m_BlockSize = blockSize < sizeof(void*) ? sizeof(void*) : blockSize;
		m_BlocksPerBlob = numElements;
		m_PeakAlloc = 0;
		m_GrowMode = growMode;
		Init();
		AddNewBlob();
}

//-----------------------------------------------------------------------------
// Purpose: Frees the memory contained in the mempool, and invalidates it for
//			any further use.
// Input  : *memPool - the mempool to shutdown
//-----------------------------------------------------------------------------
CMemoryPool::~CMemoryPool()
{
	if (m_BlocksAllocated > 0)
	{
		printf("Leak\n");
	}
	Clear();
}


//-----------------------------------------------------------------------------
// Resets the pool
//-----------------------------------------------------------------------------
void CMemoryPool::Init()
{
	m_NumBlobs = 0;
	m_BlocksAllocated = 0;
	m_pHeadOfFreeList = 0;
	m_BlobHead.m_pNext = m_BlobHead.m_pPrev = &m_BlobHead;
}


//-----------------------------------------------------------------------------
// Frees everything
//-----------------------------------------------------------------------------
void CMemoryPool::Clear()
{
	// Free everything..
	CBlob *pNext;
	for( CBlob *pCur = m_BlobHead.m_pNext; pCur != &m_BlobHead; pCur = pNext )
	{
		pNext = pCur->m_pNext;
		free( pCur );
	}
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Reports memory leaks 
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMemoryPool::AddNewBlob()
{
	int sizeMultiplier;

	if( m_GrowMode == GROW_SLOW )
	{
		sizeMultiplier = 1;
	}
	else if( m_GrowMode == GROW_NONE )
	{
		// Can only have one allocation when we're in this mode
		assert(m_NumBlobs == 0 );
	}
	
	// GROW_FAST and GROW_NONE use this.
	sizeMultiplier = m_NumBlobs + 1;

	// maybe use something other than malloc?
	int nElements = m_BlocksPerBlob * sizeMultiplier;
	int blobSize = m_BlockSize * nElements;
	//CBlob *pBlob = (CBlob*)malloc( sizeof(CBlob) + blobSize - 1 );
	CBlob *pBlob = (CBlob*)malloc( sizeof(CBlob) + blobSize );
	assert(pBlob);
	
	// Link it in at the end of the blob list.
	pBlob->m_NumBytes = blobSize;
	pBlob->m_pNext = &m_BlobHead;
	pBlob->m_pPrev = pBlob->m_pNext->m_pPrev;
	pBlob->m_pNext->m_pPrev = pBlob->m_pPrev->m_pNext = pBlob;

	// setup the free list
	m_pHeadOfFreeList = pBlob->m_Data;
	assert(m_pHeadOfFreeList);
	

	void **newBlob = (void**)m_pHeadOfFreeList;
	for (int j = 0; j < nElements-1; j++)
	{
		newBlob[0] = (char*)newBlob + m_BlockSize;
		newBlob = (void**)newBlob[0];
	}

	// null terminate list
	newBlob[0] = NULL;
	m_NumBlobs++;
}



void* CMemoryPool::Alloc()
{
	return Alloc( m_BlockSize );
}

//-----------------------------------------------------------------------------
// Purpose: Allocs a single block of memory from the pool.  
// Input  : amount - 
//-----------------------------------------------------------------------------
void *CMemoryPool::Alloc( unsigned int amount )
{
	void *returnBlock;

	if ( amount > (unsigned int)m_BlockSize )
		return NULL;

	if( !m_pHeadOfFreeList )
	{
			// returning NULL is fine in GROW_NONE
			if( m_GrowMode == GROW_NONE )
			{
					//Assert( !"CMemoryPool::Alloc: tried to make new blob with GROW_NONE" );
					return NULL;
			}else
			{
					// overflow
				AddNewBlob();
			}

			// still failure, error out

			if( !m_pHeadOfFreeList )
			{
					assert(0);
					return NULL;
			}
	}
	m_BlocksAllocated++;
	m_PeakAlloc = max(m_PeakAlloc, m_BlocksAllocated);

	returnBlock = m_pHeadOfFreeList;

	// move the pointer the next block
	m_pHeadOfFreeList = *((void**)m_pHeadOfFreeList);

	return returnBlock;
}

//-----------------------------------------------------------------------------
// Purpose: Frees a block of memory
// Input  : *memBlock - the memory to free
//-----------------------------------------------------------------------------
void CMemoryPool::Free( void *memBlock )
{
		if ( memBlock != 0 )
		{
#ifdef _DEBUG
				bool bOK = false;
				for( CBlob *pCur=m_BlobHead.m_pNext; pCur != &m_BlobHead; pCur=pCur->m_pNext )
				{
						if (memBlock >= pCur->m_Data && (char*)memBlock < (pCur->m_Data + pCur->m_NumBytes))
						{
								bOK = true;
						}
				}
				assert(bOK);
#endif


				
				m_BlocksAllocated--;

				// make the block point to the first item in the list
				*((void**)memBlock) = m_pHeadOfFreeList;

				// the list head is now the new block
				m_pHeadOfFreeList = memBlock;

		}
}



double randx[10] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.5 };

int main ()
{
		srand(time(0));
		CMemoryPool mp(2*KB, 100000, CMemoryPool::GROW_NONE);
		
		cin.get();
		DWORD beg = ::GetTickCount();

		std::vector<t_byte*> vptr(100000, 0);
		for(size_t i = 0; i < vptr.size(); ++i)
		{
				size_t as = (rand() % (2*KB) * randx[rand() % 10]);
				as = (as > (2*KB) ? (2*KB) : as);
				vptr[i] = (t_byte*)mp.Alloc(as);
				assert(vptr[i]);
				
				memset(vptr[i], 0, as);
				
		}
		
		
		for(size_t i = 0; i < vptr.size(); ++i)
		{
				mp.Free(vptr[i]);
		}
		
		DWORD end = ::GetTickCount();

		printf("%d\n", end - beg);

		printf("%s\n", "done");
		cin.get();
		return 0;
}

#endif






#if(0)


namespace X {



t_ulong main_id = 0;

class MsgEx : public MsgSpace::Message
{
public:
		MsgEx(t_ulong t) : x(t)
		{
		}
		
		virtual ~MsgEx()
		{
				static int c = 0;
				printf("release == %d\n", c++);
		}

		t_ulong x;
};

class Test
{
public:
		void print()
		{
				Sleep(1000);
				t_ulong res = (t_ulong)SendMsgTimeoutEx(main_id, new MsgEx(::GetCurrentThreadId()), 5000);
				printf("res == %d\n", res);
		}

};

}

int main()
{
		

		X::main_id = ::GetCurrentThreadId();
		
		{
		MsgSpace::MsgContext ctx;
		
		for(size_t i = 0; i < 50; ++i)
		{
				new Thread(new X::Test, &X::Test::print);
		}
		
		X::MsgEx *pmsg = 0;

		
		while(pmsg = (X::MsgEx*)ctx.GetMessageEx())
		{
				printf("pmsg == %d\n", pmsg->x);
				ctx.ReplyMessageEx((void*)33);
				::Sleep(50);

				do{
				if(ctx.PeekMessageEx())
				{
						cout << ((X::MsgEx*)(ctx.PeekMessageEx()))->x << endl;
				}

				}while(0);

		}
		
		cin.get();

		
		
		}

		printf("done\n");

		cin.get();

		return 0;

}

#endif












#if(0)
namespace ServerSpace {

#pragma pack(push, 1)
struct Packet
{
		t_uint64	start_pos;
		t_uint32	len;
};

#pragma pack(pop, 1)

class PeerConnection;

void notify_connection_comein(PeerConnection *pc);

void notify_connection_abort(PeerConnection *pc);

void notify_packet_comein(PeerConnection *pc, const Packet *packet);


#define VALIDATE_STR "TEST PROTOCOL"

static const size_t vstr_len = strlen(VALIDATE_STR);

#define RECV_SIZE (4*KB)
#define SEND_SIZE (4*KB)

typedef StreamSpace::MemStreamBuffer<>	MemSSBuffer;

class PeerConnection : public NetEventHandler
{
private:
		enum ConnState
		{
				VALIDATE	= 0x000A,
				RECVING		= 0x000B
		};

		struct ConnectionInvalid : public std::runtime_error
		{
				ConnectionInvalid(const std::string &msg) : std::runtime_error(msg)
				{

				}

		};

		struct ConnectionAbort : public std::runtime_error
		{
				ConnectionAbort(const std::string &msg) : std::runtime_error(msg)
				{

				}
		};
		typedef SpinLock		MutexType;
private:
		SockStream		m_sock;
		Buffer			m_recv_buf;
		
		ConnState		m_state;
		
		//FastMutex		m_mutex;
		
		MutexType		m_mutex;
		Buffer			m_send_buf;
public:
		PeerConnection(NetHandle n_hdl)
				: m_state(VALIDATE)
		{
				m_sock.Attach(n_hdl);
				assert(m_sock.IsValid());
				Mask(INPUT_MASK|TIMER_MASK);
				m_sock.Enable(NET_NONBLOCK);
		}

		virtual NetHandle GetHandle()const
		{
				return m_sock.GetHandle();
		}


		bool recv_validate()
		{
				std::vector<t_byte> buf(vstr_len);

				int rlen = m_sock.Recv(&buf[0], buf.size());

				if(rlen > 0)
				{
						m_recv_buf.Insert(&buf[0], rlen);
						if(m_recv_buf.Size() < vstr_len)
						{
								return false;
						}else
						{
								Buffer::IntervalType iter = m_recv_buf.Data();
								std::vector<char> v_str(vstr_len + 1);
								char *pstr = &v_str[0];
								if(iter.first.Left() > 0)
								{
										int left = iter.first.Left();
										memcpy(pstr, iter.first.m_begin, left);
										pstr += left;
								}

								if(iter.second.Left() > 0)
								{
										int left = iter.second.Left();
										memcpy(pstr, iter.second.m_begin, left);
								}
								m_recv_buf.Erase(vstr_len);
								v_str[vstr_len] = '\0';
								if(strcmp(VALIDATE_STR, &v_str[0]) == 0)
								{
										return true;
								}else
								{
										throw ConnectionInvalid("peer connection is invalid, disconnected");
										return false;
								}
						}

				}else
				{
						if(NetGetLastError() == EWOULDBLOCK)
						{
								return false;
						}else
						{
								throw ConnectionInvalid("peer connection is invalid, disconnected");
								return false;
						}
				}
		}
		
		void on_recv_packet()
		{
				NetSetLastError(0);

				int avail = m_sock.Available();
				if(avail > 0)
				{
						Buffer::Interval iter = m_recv_buf.Allocate(avail);

						int rn = m_sock.Recv(iter.m_begin, iter.Left());
						if(rn > 0)
						{
								assert(rn == iter.Left());
						}else
						{
								if(NetGetLastError() != EWOULDBLOCK)
								{
										throw ConnectionAbort("connection abort");
								}
						}
				}else
				{
						if(NetGetLastError() != EWOULDBLOCK)
						{
								throw ConnectionAbort("connection abort");
						}
				}

				while(m_recv_buf.Size() >= sizeof(Packet))
				{
						Buffer::IntervalType iter = m_recv_buf.Data();
						Packet packet;
						t_byte *p_data = (t_byte*)&packet;

						int left = sizeof(packet);
						if(iter.first.Left() > 0)
						{
								int need_len = (iter.first.Left() > sizeof(packet) 
										? sizeof(packet) : iter.first.Left());

								memcpy(p_data, iter.first.m_begin, need_len);
								left -= need_len;
								p_data += need_len;
						}

						if(left > 0)
						{
								assert(iter.second.Left() >= left);

								memcpy(p_data, iter.second.m_begin, left);
						}
						m_recv_buf.Erase(sizeof(packet));
						notify_packet_comein(this, &packet);
						
				}
		}
		
		virtual void OnInput()
		{
				try{
						switch(m_state)
						{
						case VALIDATE:
								if(recv_validate())
								{
										Duplicate();
										notify_connection_comein(this);
										m_state = RECVING;
										return;
								}else
								{
										return;
								}
								break;
						case RECVING:
								{
										on_recv_packet();
										OnTimer();
										break;
								}
						default:
								assert(0);
								break;
						}
				}
				catch(ConnectionInvalid &expt)
				{
						printf("%s\n", expt.what());
						GetSelector()->RemoveHandler(this);
						return;

				}
				catch(ConnectionAbort &expt)
				{
						printf("%s\n", expt.what());
						notify_connection_abort(this);
						GetSelector()->RemoveHandler(this);
						return;
				}
		}
	
		bool send_cache_buf()
		{
				while(m_send_buf.Size() > 0)
				{
						
						Buffer::IntervalType iter = m_send_buf.Data();
				
						if(iter.first.Left() > 0)
						{
								int left = iter.first.Left();
								size_t send_n = (left > SEND_SIZE ? SEND_SIZE : left);

								int slen = m_sock.Send(iter.first.m_begin, send_n);

								if(slen > 0)
								{
										m_send_buf.Erase(slen);
								}else
								{
										return false;
								}
						}

						if(iter.second.Left() > 0)
						{
								int left = iter.second.Left();
								size_t send_n = (left > SEND_SIZE ? SEND_SIZE : left);

								int slen = m_sock.Send(iter.second.m_begin, send_n);

								if(slen > 0)
								{
										m_send_buf.Erase(slen);
								}else
								{
										return false;
								}
						}
				}
				
				
				return true;
		}

		virtual void OnOutput()
		{
				NetSetLastError(0);
				
				MutexType::ScopeLock lock(m_mutex);
				if(!send_cache_buf())
				{
						if(NetGetLastError() != EWOULDBLOCK)
						{
								notify_connection_abort(this);
								GetSelector()->RemoveHandler(this);
								return;
						}
				}else
				{
						ClrMask(OUTPUT_MASK);
				}
		}

		
		void WriteBuf(const t_byte *pbuf, size_t len)
		{
				assert(pbuf != 0 && len != 0);
				MutexType::ScopeLock lock(m_mutex);
				m_send_buf.Insert(pbuf, len);
		}

		virtual void OnTimer()
		{
				MutexType::ScopeLock lock(m_mutex);
				if(!m_send_buf.Empty())
				{
						AddMask(OUTPUT_MASK);
				}
		}

		virtual ~PeerConnection()
		{
				printf("peer connection release\n");
		}
};

#define CONNECTION_COMIN  0x0001
#define CONNECTION_ABORT  0x0002
#define CONNECTION_PACKET 0x0003



class PacketMsg //: public MsgSpace::Message
{
public:
		t_uint32  msg_type;
		void	  *param1;
		void	  *param2;
public:
		PacketMsg(t_uint32 type, void *p1, void *p2) 
				: msg_type(type), param1(p1), param2(p2)
		{
				
		}

		/*virtual ~PacketMsg()
		{
				//printf("PacketMsg release\n");
		}*/
};

ThreadSpace::TSDataQueue  msg_queue;

void notify_connection_comein(PeerConnection *pc)
{
		//PostMsgEx(main_id, new PacketMsg(CONNECTION_COMIN, (void*)pc, 0), 3);
		msg_queue.Enqueue((void*)new PacketMsg(CONNECTION_COMIN, (void*)pc, 0));
		
}

void notify_connection_abort(PeerConnection *pc)
{
		//PostMsgEx(main_id, new PacketMsg(CONNECTION_ABORT, (void*)pc, 0), 2);

		msg_queue.Enqueue((void*)new PacketMsg(CONNECTION_ABORT, (void*)pc, 0));

}

void notify_packet_comein(PeerConnection *pc, const Packet *packet)
{
		Packet *pkg = new Packet;
		*pkg = *packet;
		//PostMsgEx(main_id, new PacketMsg(CONNECTION_PACKET, (void*)pc, (void*)pkg), 0);
		msg_queue.Enqueue((void*)new PacketMsg(CONNECTION_PACKET, (void*)pc, (void*)pkg));
		
}

class PeerAcceptor : public NetSpace::AcceptorEx
{
public:
		virtual void on_accepted(NetHandle hdl)
		{
				PeerConnection *pc = new PeerConnection(hdl);
				GetSelector()->RegisterHandler(pc);
		}

		virtual void on_error(t_ulong err_code)
		{
				printf("acceptor encounter a fatal error == %d\n", err_code);
				exit(-1);
		}

		PeerAcceptor(const InetAddress &addr) : AcceptorEx(addr)
		{


		}
};

class ServerTest
{
public:
		InetAddress		addr;
		Selector		*pselector;
public:
		void test()
		{
				try{
						PeerAcceptor *pa = new PeerAcceptor(addr);
						bool res = pselector->RegisterHandler(pa);
						assert(res);
						assert(pa->GetSelector() != 0);
				}catch(...)
				{
						printf("acceptor failed\n");
						assert(0);
						exit(-1);
				}
				
				
				pselector->Run(50);

				printf("selector done\n");
		}

};

void run()
{
		std::set<NetHandle>  pc_set;

		File file;
		bool res = file.Open(L"D:\\1.rar", F_READ_ONLY|F_OPEN_EXISTED|F_SHARED_READ);
		
		printf("file size == %I64d\n", file.GetLength());

		assert(res);

		//MsgSpace::MsgContext context;

		while(true)
		{
		//		PacketMsg *pmsg = (PacketMsg*)context.GetMessageEx();
				//printf("data queue size == %d\n", msg_queue.Size());
				PacketMsg *pmsg = (PacketMsg*)msg_queue.WaitDequeue();
				assert(pmsg);

				switch(pmsg->msg_type)
				{
				case CONNECTION_COMIN:
				{
						printf("connection established\n");
						PeerConnection* pc = (PeerConnection*)pmsg->param1;
						pc_set.insert(pc->GetHandle());
						continue;
				}
				case CONNECTION_ABORT:
				{
						printf("CONNECTION_ABORT\n");
						PeerConnection* pc = (PeerConnection*)pmsg->param1;
						assert(pc_set.count(pc->GetHandle()) == 1);
						pc_set.erase(pc->GetHandle());
						pc->Release();
						continue;	
				}
				case CONNECTION_PACKET:
				{
						PeerConnection* pc = (PeerConnection*)pmsg->param1;
						Packet *pkg = (Packet*)pmsg->param2;
						printf("recv packet start_pos == %I64d : len == %d\n", pkg->start_pos, pkg->len);
						if(pc_set.count(pc->GetHandle()) == 0)
						{
								printf("nethandle val == %d\n", pc->GetHandle());
								assert(0);
						}

						
						
						//std::vector<t_byte> buf(pkg->len);
						std::vector<t_byte> buf(pkg->len);
						
						//file.Seek(pkg->start_pos, FileSysSpace::BEG, true);
						
						//file.Read(&buf[0], buf.size());
						
						pc->WriteBuf(&buf[0], buf.size());
					
						break;
				}
				}
				delete pmsg;
		}
}

}



namespace StreamSpace {

class  DefaultMemBufferImplEx
{
private:
		t_byte*					m_first;
		t_byte*					m_last;
		t_byte*					m_read_cursor;
		t_byte*					m_write_cursor;
		t_uint32				m_buf_size;
public:
		DefaultMemBufferImplEx();
		
		~DefaultMemBufferImplEx();

private:
		void increase_capability(size_t len);

		bool move_internal(size_t len);
public:
		const t_byte* GetContent()const;
		
		void Put(const t_byte *arr, size_t len);

		size_t Get(t_byte *arr, size_t n);

		size_t Skip(size_t n);
		
		t_uint64  Available()const ;

		void Clear();

		bool IsEmpty()const;

		size_t GetBufSize()const;
};


const t_byte* DefaultMemBufferImplEx::GetContent()const
{
		if(!IsEmpty())
		{
				return m_read_cursor;
		}else
		{
				return 0;
		}
		
}

DefaultMemBufferImplEx::DefaultMemBufferImplEx() 
								: m_first(0)
								, m_last(0)
								, m_read_cursor(0)
								, m_write_cursor(0)
								, m_buf_size(0)
{
		
}

DefaultMemBufferImplEx::~DefaultMemBufferImplEx()
{
		Clear();

}


t_uint64  DefaultMemBufferImplEx::Available()const
{
		return (m_write_cursor - m_read_cursor);
}

void DefaultMemBufferImplEx::Clear()
{
		::free(m_first);
		m_first = 0;
		m_last = 0;
		m_read_cursor = 0;
		m_write_cursor = 0;
		m_buf_size = 0;
}

bool DefaultMemBufferImplEx::IsEmpty()const
{
		return (Available() == 0);
}

bool DefaultMemBufferImplEx::move_internal(size_t len)
{
		if(m_buf_size == 0) return false;

		assert(m_read_cursor >= m_first);
		
		size_t vacancy_len = m_read_cursor - m_first;
		size_t cur_size = m_write_cursor - m_read_cursor;
		size_t cap_size = m_last - m_write_cursor;
		
		printf("vacancy_len == %d\n", vacancy_len);

		if(vacancy_len < cur_size || (cap_size + vacancy_len) < len)
		{
				return false;
		}else
		{
				printf("xxxxxxxxxxxxxx\n");
				memcpy(m_first, m_read_cursor, cur_size);
				m_read_cursor = m_first;
				m_write_cursor = m_read_cursor + cur_size;
				return true;
		}
}

void DefaultMemBufferImplEx::Put(const t_byte *arr, size_t len)
{
		assert(m_write_cursor <= m_last);

		if(m_write_cursor == m_last || (m_last - m_write_cursor) < len )
		{
				if(!move_internal(len))
				{
						float inc_size = m_buf_size * 1.5f + len;
						increase_capability(inc_size);
						//increase_capability(len);
				}
		}
		
		assert(m_first != 0);
		assert(m_write_cursor >= m_read_cursor);
		assert((m_last - m_write_cursor) >= len);
		memcpy(m_write_cursor, arr, len);
		m_write_cursor += len;
}

size_t DefaultMemBufferImplEx::Skip(size_t n)
{
		size_t remain_len = m_write_cursor - m_read_cursor;
		
		size_t read_n = (remain_len < n ? remain_len : n);
		
		if(read_n != 0)
		{
				m_read_cursor += read_n;
		}
		return read_n;
}

size_t DefaultMemBufferImplEx::Get(t_byte *arr, size_t n)
{
		size_t remain_len = m_write_cursor - m_read_cursor;
		
		size_t read_n = (remain_len < n ? remain_len : n);
		
		if(read_n != 0)
		{
				memcpy(arr, m_read_cursor, read_n);
				m_read_cursor += read_n;
		}

		return read_n;
}


void DefaultMemBufferImplEx::increase_capability(size_t len)
{
		assert(m_read_cursor <= m_write_cursor);
		assert(m_buf_size >= 0);

		t_byte *pbuf = (t_byte*)::malloc(m_buf_size + len);
		
		assert(m_write_cursor >= m_read_cursor);
		size_t cont_len = m_write_cursor - m_read_cursor;
		
		if(cont_len > 0)
		{
				memcpy(pbuf, m_read_cursor, cont_len);
		}

		m_buf_size += len;
		
		::free(m_first);
		m_first = pbuf;
		m_last = m_first + m_buf_size;

		m_read_cursor = m_first;
		m_write_cursor = m_read_cursor + cont_len;

}

size_t DefaultMemBufferImplEx::GetBufSize()const
{
		return m_buf_size;

}

}











		File file;

		bool res = file.Open("D:\\1.rar", STD_READ);
		assert(res);

		File file2;

		res = file2.Open("D:\\2.rar", F_READ_WRITE|F_CREATE_ALWAYS|F_SHARED_READ);
		assert(res);

		SimpleBuffer	simple_buf;

		t_uint64 remain_len = file.GetLength();

		srand(time(0));

		while(remain_len > 0)
		{
				//t_byte buf[4096];

				t_uint64 len = rand() % 4096;
				size_t rn = min(len, remain_len);
				if(rn > 0)
				{
						t_byte *pbuf = simple_buf.Allocate(rn);
						size_t real_rn = file.Read(pbuf, rn);
						assert(rn == real_rn);
						//simple_buf.Put(buf, rn);
				}

				size_t rand_rn = (rand()%4096);
				size_t write_len = min(simple_buf.Size(), rand_rn);
				//				printf("write_len == %d\n", write_len);
				//				printf("simple buf.available() == %d\n", simple_buf.Available());
				if(write_len > 0)
				{
						assert(simple_buf.Size() >= write_len);

						file2.Write(simple_buf.Data(), write_len);

						simple_buf.Erase(write_len);
				}
				remain_len -= rn;

				//				printf("remain len == %I64d\n", remain_len);
		}


		cout << simple_buf.Size() << endl;

		file2.Write(simple_buf.Data(), simple_buf.Size());


		cout << "buffer size == " << simple_buf.RawBufSize() << endl;


		
#endif












