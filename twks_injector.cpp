#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
using namespace std;
DWORD GetPID(const wchar_t* n){
HANDLE s=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
PROCESSENTRY32W e;e.dwSize=sizeof(e);
if(Process32FirstW(s,&e)){do{
if(!_wcsicmp(e.szExeFile,n)){CloseHandle(s);return e.th32ProcessID;}
}while(Process32NextW(s,&e));}
CloseHandle(s);return 0;}
bool Inject(DWORD pid,const wchar_t* dll){
HANDLE h=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
if(!h)return false;
size_t sz=(wcslen(dll)+1)*2;
void* m=VirtualAllocEx(h,0,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
if(!m){CloseHandle(h);return false;}
WriteProcessMemory(h,m,dll,sz,0);
FARPROC f=GetProcAddress(GetModuleHandleW(L"kernel32.dll"),"LoadLibraryW");
HANDLE t=CreateRemoteThread(h,0,0,(LPTHREAD_START_ROUTINE)f,m,0,0);
if(!t){VirtualFreeEx(h,m,0,MEM_RELEASE);CloseHandle(h);return false;}
WaitForSingleObject(t,10000);
DWORD r=0;GetExitCodeThread(t,&r);
VirtualFreeEx(h,m,0,MEM_RELEASE);
CloseHandle(t);CloseHandle(h);return r!=0;}
int main(){
SetConsoleTitleW(L"TWKS v3.2.1");
wcout<<L"\n TWKS INJECTOR v3.2.1\n @xlsqrtilovvo\n\n";
wcout<<L"[*] Aspettando Roblox...\n";
DWORD pid=0;
while(!(pid=GetPID(L"RobloxPlayerBeta.exe")))Sleep(500);
wcout<<L"[+] PID: "<<pid<<L"\n";Sleep(2000);
wchar_t p[MAX_PATH];GetCurrentDirectoryW(MAX_PATH,p);
wcscat(p,L"\\twks_executor.dll");
wcout<<L"[*] Injecting...\n";
if(Inject(pid,p))wcout<<L"\n[+] INJECTION OK!\n";
else wcout<<L"\n[-] ERRORE - Esegui come Admin\n";
wcout<<L"\nInvio...";cin.get();return 0;}