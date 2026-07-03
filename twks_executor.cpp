#include <windows.h>
#include <cstdint>
#include <cstdio>
namespace off{
const uintptr_t TaskScheduler=0x81CC868;
const uintptr_t ScriptContext=0x440;
const uintptr_t LocalPlayer=0x130;
const uintptr_t WalkSpeed=0x1D0;
const uintptr_t JumpPower=0x1A4;
const uintptr_t Health=0x188;
const uintptr_t Gravity=0x210;
const uintptr_t Children=0x70;
const uintptr_t Name=0x98;
const uintptr_t CFrame=0xC8;
const uintptr_t FOV=0x140;
const uintptr_t FakeDataModel=0x7C3D2E8;}
uintptr_t base=0;
DWORD WINAPI PipeThread(LPVOID){
HANDLE p=CreateNamedPipeW(L"\\\\.\\pipe\\TWKSExecutor",
PIPE_ACCESS_DUPLEX,PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
1,65536,65536,0,NULL);
if(p==INVALID_HANDLE_VALUE)return 1;
char buf[65536];
while(1){if(ConnectNamedPipe(p,NULL)){
DWORD r;if(ReadFile(p,buf,65535,&r,NULL)){
buf[r]=0;OutputDebugStringA(buf);
DWORD w;WriteFile(p,"OK",2,&w,NULL);}
DisconnectNamedPipe(p);}}return 0;}
DWORD WINAPI InitThread(LPVOID){
Sleep(1000);
base=(uintptr_t)GetModuleHandleW(L"RobloxPlayerBeta.exe");
if(!base)return 1;
char m[128];
sprintf(m,"[TWKS] Base: 0x%llX",(unsigned long long)base);
OutputDebugStringA(m);
CreateThread(NULL,0,PipeThread,NULL,0,NULL);
OutputDebugStringA("[TWKS] Pronto!");return 0;}
extern "C" __declspec(dllexport)
BOOL APIENTRY DllMain(HMODULE h,DWORD reason,LPVOID){
if(reason==DLL_PROCESS_ATTACH){
DisableThreadLibraryCalls(h);
CreateThread(NULL,0,InitThread,NULL,0,NULL);}
return TRUE;}