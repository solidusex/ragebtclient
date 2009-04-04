/*
class TimerEx
{
private:
#pragma pack(push, 1)
		struct Thunk
		{
				t_byte			push;
				t_uint32		pthis;
				t_byte			call;
				t_uint32		offset;
				t_byte			ret;
				t_uint16		retn;
		};
#pragma pack(pop)
private:
		Thunk			m_thunk;
		t_uint32		m_timer_id;
public:
		static void TimerFunc(TimerEx *ptimer)
		{
				ptimer->OnTimer();
		}
public:
		virtual void OnTimer()
		{
				printf("OnTimer\n");
		}

public:
		TimerEx() : m_timer_id(0)
		{
				memset(&m_thunk, 0, sizeof(Thunk));

				m_thunk.push = 0x68;
				m_thunk.pthis = (t_uint32)this;
				m_thunk.call = 0xE8;
				t_uint32 proc_addr = (t_uint32)(&TimerEx::TimerFunc);
				t_uint32 op_offset = (t_uint32)&(m_thunk.call);
				op_offset += 5;
				m_thunk.offset = proc_addr - op_offset;
				m_thunk.ret = 0xC2;
				m_thunk.retn = 20;
		}
		
		virtual ~TimerEx()
		{
				if(m_timer_id != 0)
				{
						StopTimer();
				}
		}

private:
		TimerEx(const TimerEx &other);
		TimerEx& operator=(const TimerEx &other);
public:

		bool StartTimer(t_uint32 interval)
		{
				m_timer_id = ::SetTimer(NULL, 0, interval, (::TIMERPROC)&m_thunk);

				return m_timer_id != 0;
		}


		void StopTimer()
		{
				if(m_timer_id != 0)
				{
						::KillTimer(NULL,m_timer_id);
						m_timer_id = 0;
				}
		}

};

class TimerExTest : public TimerEx
{
public:
		virtual void OnTimer()
		{
				printf("TimerExTest\n");
		}
};

void Run();

int main()
{
		TimerEx			tex;
		TimerExTest		tex2;
		tex.StartTimer(1000);
		tex2.StartTimer(2000);

		Run();


		printf("done\n");

		cin.get();
		
		return 0;
}

void Run()
{

		::MSG msg;

		BOOL bRet;
		HWND hWnd;

		while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
		{ 
				if (bRet == -1)
				{
						// handle the error and possibly exit
						cout << ::GetLastError() << endl;
						exit(-1);
				}
				else
				{
						TranslateMessage(&msg); 
						DispatchMessage(&msg);
				}
		}

}


*/