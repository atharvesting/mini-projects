// Consalgo_Visualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <cstdlib>
using namespace std;
constexpr auto S = 10;

int nScreenWidth = 60;
int nScreenHeight = 20;

int arr[S] = { 3,5,1,8,6,2,0,4,7,9 };
int flag = 0;

void bubble_sort(void);
void print_array(void);

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

    short nShade = L'>';// 0x2591;
    for (int i = 0; i < S; i++) {
        int t = arr[i];
        for (int j = 0; j < t; j++) {
            screen[2 * i * nScreenWidth + j] = nShade;
        }
        
    }

    /**
    * Formula to write on the screen at (x, y):
    * screen[y * nScreenWidth + x]
    */

    //while (1) 
    //{
    //    system("cls");
    //    print_array();
    //    while (1) {
    //        if (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
    //            while (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
    //                // Do nothing, just wait
    //            }
    //            break;
    //        }
    //    }
    //    bubble_sort();
    //    if (flag == 0) break;
    //}
    while (1) {
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}

void bubble_sort(void) {
    flag = 0;
    for (int i = 0; i < S - 1; i++)
        for (int j = 0; j < S - i - 1; j++) {
            if (arr[j + 1] < arr[j]) {
                swap(arr[j], arr[j + 1]);
                flag = 1;
                return;
            }
        }
}

void print_array(void) {
    for (int i = 0; i < S; i++) {
        cout << arr[i] << "  ";
    }
    cout << endl;
}