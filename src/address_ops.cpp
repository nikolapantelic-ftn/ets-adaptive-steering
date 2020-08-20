#include "address_ops.h"

LPVOID findSteerSensAddr(HANDLE hProcess, DWORD_PTR modBaseAddr)
{
    int steerSensAddrOffsets[2] = {0x1310, 0x888};
    LPVOID basePtr = (void *)(modBaseAddr + BASE_ADDR_OFFSET);
    return findAddr(hProcess, basePtr, steerSensAddrOffsets, 2);
}

LPVOID findTruckSpeedAddr(HANDLE hProcess, DWORD_PTR modBaseAddr)
{
    int truckSpeedAddrOffsets[6] = {0x1310, 0x840, 0x2C0, 0x8, 0x28, 0x18};
    LPVOID basePtr = (void *)(modBaseAddr + BASE_ADDR_OFFSET);
    return findAddr(hProcess, basePtr, truckSpeedAddrOffsets, 6);
}

DWORD_PTR GetModuleBaseAddress(DWORD pid, char* modName)
{
    DWORD_PTR modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if(hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry = { 0 };
        modEntry.dwSize = sizeof(MODULEENTRY32);
        if(Module32First(hSnap, &modEntry))
        {
            do
            {
                if(_stricmp(modEntry.szModule, modName) == 0)
                {
                    modBaseAddr = (DWORD_PTR)modEntry.modBaseAddr;
                    break;
                }
            }while(Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

LPVOID findAddr(HANDLE hProcess, LPVOID basePtr, int *offsets, int offsetsCnt)
{
    DWORD_PTR buffPtr;
    if(!ReadProcessMemory(hProcess, basePtr, &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        return NULL;
    }

    for(int i = 0; i < offsetsCnt - 1; i++)
    {
        if(!ReadProcessMemory(hProcess, (void *)(buffPtr + offsets[i]), &buffPtr, sizeof(DWORD_PTR), NULL))
        {
            return NULL;
        }
    }

    return (void *)(buffPtr + offsets[offsetsCnt - 1]);
}
