#include "address_ops.h"

LPVOID findSteerSensAddr(HANDLE hProcess)
{
    int steerSensAddrOffsets[2] = {0x1310, 0x888};
    LPVOID basePtr = (void *)STEER_SENS_BASE_ADDR;
    return findAddr(hProcess, basePtr, steerSensAddrOffsets, 2);
}

LPVOID findTruckSpeedAddr(HANDLE hProcess)
{
    int truckSpeedAddrOffsets[6] = {0x1310, 0x840, 0x2C0, 0x8, 0x28, 0x18};
    LPVOID basePtr = (void *)TRUCK_SPEED_BASE_ADDR;
    return findAddr(hProcess, basePtr, truckSpeedAddrOffsets, 6);
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
