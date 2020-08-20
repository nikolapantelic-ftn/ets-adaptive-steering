#ifndef ADDRESS_OPS_H_INCLUDED
#define ADDRESS_OPS_H_INCLUDED

#include <windows.h>
#include <TlHelp32.h>

#define BASE_ADDR_OFFSET 0x15130C0 //The base pointer address for steering sensitivity value


LPVOID findAddr(HANDLE hProcess, LPVOID basePtr, int *offsets, int offsetsCnt);
LPVOID findSteerSensAddr(HANDLE hProcess, DWORD_PTR modBaseAddr);
LPVOID findTruckSpeedAddr(HANDLE hProcess, DWORD_PTR modBaseAddr);
DWORD_PTR GetModuleBaseAddress(DWORD pid, char* modName);

#endif // ADDRESS_OPS_H_INCLUDED
