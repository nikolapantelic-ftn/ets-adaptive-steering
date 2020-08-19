#include <iostream>
#include <windows.h>
#include <math.h>

using namespace std;

#define STEER_SENS_BASE_ADDR 0x7FF63F5A30C0 //The base pointer address for steering sensitivity value
#define TRUCK_SPEED_BASE_ADDR 0x7FF63F5A30C0 //The base pointer address for truck speed value
#define MAX_SENS_VAL 3

LPVOID findSteerSensAddr(HANDLE hProcess, LPVOID basePtr);
LPVOID findTruckSpeedAddr(HANDLE hProcess, LPVOID basePtr);
float expDecl(float x);

int main()
{
    DWORD pid;
    HANDLE hProcess = NULL;
    LPVOID steerSensBasePtr = (void *)STEER_SENS_BASE_ADDR;
    LPVOID truckSpeedBasePtr = (void *)TRUCK_SPEED_BASE_ADDR;
    LPVOID rSteerSensAddr = NULL;
    LPVOID truckSpeedAddr = NULL;
    float rSteerSens;
    float truckSpeed;
    float sensVal = 0;

    HWND hWnd = FindWindow(NULL, "Euro Truck Simulator 2");
    if(!hWnd)
    {
        cout << "Window not found" << endl;
        return 1;
    }

    if(!GetWindowThreadProcessId(hWnd, &pid))
    {
        cerr << "Get window process ID error: " << GetLastError() << endl;
        return 1;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if(!hProcess)
    {
        cerr << "Can not open process: " << GetLastError() << endl;
        return 1;
    }

    rSteerSensAddr = findSteerSensAddr(hProcess, steerSensBasePtr);
    truckSpeedAddr = findTruckSpeedAddr(hProcess, truckSpeedBasePtr);

    if(!rSteerSensAddr || !truckSpeedAddr) return 2;
    if(!ReadProcessMemory(hProcess, rSteerSensAddr, &rSteerSens, sizeof(float), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return 2;
    }
    cout << rSteerSens << endl;

    if(!ReadProcessMemory(hProcess, truckSpeedAddr, &truckSpeed, sizeof(float), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return 2;
    }
    cout << truckSpeed << endl;

    while(TRUE)
    {
        ReadProcessMemory(hProcess, truckSpeedAddr, &truckSpeed, sizeof(float), NULL);
        sensVal = MAX_SENS_VAL * expDecl(truckSpeed);
        WriteProcessMemory(hProcess, rSteerSensAddr, &sensVal, sizeof(float), NULL);
    }
    return 0;
}

LPVOID findSteerSensAddr(HANDLE hProcess, LPVOID basePtr)
{
    int OFFSET1 = 0x1310;
    int OFFSET2 = 0x888;
    DWORD_PTR buffPtr;

    if(!ReadProcessMemory(hProcess, basePtr, &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    if(!ReadProcessMemory(hProcess, (void *)(buffPtr + OFFSET1), &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    return (void *)(buffPtr + OFFSET2);
}

LPVOID findTruckSpeedAddr(HANDLE hProcess, LPVOID basePtr)
{
    int OFFSET1 = 0x1310;
    int OFFSET2 = 0x840;
    int OFFSET3 = 0x2C0;
    int OFFSET4 = 0x8;
    int OFFSET5 = 0x28;
    int OFFSET6 = 0x18;
    DWORD_PTR buffPtr;

    if(!ReadProcessMemory(hProcess, basePtr, &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    if(!ReadProcessMemory(hProcess, (void *)(buffPtr + OFFSET1), &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    if(!ReadProcessMemory(hProcess, (void *)(buffPtr + OFFSET2), &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    if(!ReadProcessMemory(hProcess, (void *)(buffPtr + OFFSET3), &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    if(!ReadProcessMemory(hProcess, (void *)(buffPtr + OFFSET4), &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    if(!ReadProcessMemory(hProcess, (void *)(buffPtr + OFFSET5), &buffPtr, sizeof(DWORD_PTR), NULL))
    {
        cerr << "Memory read error: " << GetLastError();
        return NULL;
    }

    return (void *)(buffPtr + OFFSET6);
}

float expDecl(float x)
{
    return 0.95 * exp(-0.1*x) + 0.05;
}

