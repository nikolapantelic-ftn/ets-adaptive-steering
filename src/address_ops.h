#ifndef ADDRESS_OPS_H_INCLUDED
#define ADDRESS_OPS_H_INCLUDED

#include <windows.h>

#define STEER_SENS_BASE_ADDR 0x7FF63F5A30C0 //The base pointer address for steering sensitivity value
#define TRUCK_SPEED_BASE_ADDR 0x7FF63F5A30C0 //The base pointer address for truck speed value

LPVOID findAddr(HANDLE hProcess, LPVOID basePtr, int *offsets, int offsetsCnt);
LPVOID findSteerSensAddr(HANDLE hProcess);
LPVOID findTruckSpeedAddr(HANDLE hProcess);

#endif // ADDRESS_OPS_H_INCLUDED
