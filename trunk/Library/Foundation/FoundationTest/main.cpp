
#include "Foundation.h"

#pragma comment(lib, "Foundation.lib")


#include <assert.h>
#include <stack>
#include <iostream>


using namespace std;



#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

int main()
{
		ProcessSpace::ProcessStartInfo info;
		SysSpace::Environment env;
		
		info.env = env.GetEnvVar();
		FileSysSpace::Path::GetCurrentPath(info.cur_dir);
		ProcessSpace::ProcessHandle hdl = ProcessSpace::Process::Launch(L"c:\\Windows\\System32\\taskmgr.exe", L"", info);

		hdl.Wait();


		cout << "done" << endl;
		cin.get();
		return 0;

}