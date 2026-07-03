#include <windows.h>
#include <cstdint>
#include <cstdio>
uintptr_t base=0;
DWORD WINAPI Pipe(LPVOID){HANDLE p=CreateNamedPipeW(L"\\\\.\\pipe\\TWKSExecutor",PIPE_ACCESS_DUPLEX,PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,1,65536,65536,0,NULL);if(p==INVALID_HANDLE_VALUE)return 1;char buf[65536];while(1){if(ConnectNamedPipe(p,NULL)){DWORD r;if(ReadFile(p,buf,65535,&r,NULL)){buf[r]=0;OutputDebugStringA(buf);DWORD w;WriteFile(p,"OK",2,&w,NULL);}DisconnectNamedPipe(p);}}return 0;}
DWORD WINAPI Init(LPVOID){Sleep(1000);base=(uintptr_t)GetModuleHandleW(L"RobloxPlayerBeta.exe");char m[128];sprintf(m,"[TWKS] 0x%llX",(unsigned long long)base);OutputDebugStringA(m);CreateThread(0,0,Pipe,0,0,0);return 0;}
extern "C" __declspec(dllexport) BOOL APIENTRY DllMain(HMODULE h,DWORD r,LPVOID){if(r==DLL_PROCESS_ATTACH){DisableThreadLibraryCalls(h);CreateThread(0,0,Init,0,0,0);}return TRUE;}
