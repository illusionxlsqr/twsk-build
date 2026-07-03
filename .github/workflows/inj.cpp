#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
using namespace std;
DWORD GetPID(const wchar_t* n){HANDLE s=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);PROCESSENTRY32W e;e.dwSize=sizeof(e);if(Process32FirstW(s,&e)){do{if(!_wcsicmp(e.szExeFile,n)){CloseHandle(s);return e.th32ProcessID;}}while(Process32NextW(s,&e));}CloseHandle(s);return 0;}
int main(){cout<<"[*] Waiting..."<<endl;DWORD pid=0;while(!(pid=GetPID(L"RobloxPlayerBeta.exe")))Sleep(500);cout<<"[+] PID: "<<pid<<endl;HANDLE h=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);if(!h){cout<<"[-] OpenProcess fail"<<endl;cin.get();return 1;}wchar_t p[MAX_PATH];GetCurrentDirectoryW(MAX_PATH,p);wcscat(p,L"\\twks_executor.dll");size_t sz=(wcslen(p)+1)*2;void* m=VirtualAllocEx(h,0,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);WriteProcessMemory(h,m,p,sz,0);HANDLE t=CreateRemoteThread(h,0,0,(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),"LoadLibraryW"),m,0,0);WaitForSingleObject(t,10000);DWORD r=0;GetExitCodeThread(t,&r);VirtualFreeEx(h,m,0,MEM_RELEASE);CloseHandle(t);CloseHandle(h);if(r)cout<<"[+] INJECTED!"<<endl;else cout<<"[-] FAIL"<<endl;cin.get();return 0;}
