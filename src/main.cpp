#include <iostream>
#include <windows.h>
#include <math.h>

#include "address_ops.h"

using namespace std;

#define MAX_SENS_VAL 3

float expDecl(float x, float k, float minVal);

int main()
{
    DWORD pid;
    HANDLE hProcess = NULL;
    LPVOID rSteerSensAddr = NULL;
    LPVOID truckSpeedAddr = NULL;
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

    cout << "Game process successfuly opened." << endl << "Program active." << endl;

    string modName = "eurotrucks2.exe";
    char *cstr = new char[modName.length() + 1];
    strcpy(cstr, modName.c_str());
    DWORD_PTR modBaseAddr = GetModuleBaseAddress(pid, cstr);

    rSteerSensAddr = findSteerSensAddr(hProcess, modBaseAddr);
    truckSpeedAddr = findTruckSpeedAddr(hProcess, modBaseAddr);

    if(!rSteerSensAddr || !truckSpeedAddr)
    {
        cerr << "Address not found: " << GetLastError() << endl;
        return 2;
    }

    while(TRUE)
    {
        ReadProcessMemory(hProcess, truckSpeedAddr, &truckSpeed, sizeof(float), NULL);
        sensVal = MAX_SENS_VAL * expDecl(truckSpeed, 0.05, 0.05);
        WriteProcessMemory(hProcess, rSteerSensAddr, &sensVal, sizeof(float), NULL);
    }
    return 0;
}

float expDecl(float x,
              float k, // decay rate, higher value results in faster decay
              float minVal)  // minimum resulting value (should be minVal > 0 && minVal < 1)
{
    return (1 - minVal) * exp(-k*x) + minVal;
}

