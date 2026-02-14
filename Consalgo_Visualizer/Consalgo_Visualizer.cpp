// Consalgo_Visualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
using namespace std;
constexpr auto S = 10;


int arr[10] = { 9,8,7,6,5,4,3,2,1,0 };
int flag = 0;

void bubble_sort(void);
void print_array(void);

int main()
{
    bubble_sort();
    print_array();

    
    while (1) 
    {
        print_array();
        while (1) {
            if (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
                while (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
                    // Do nothing, just wait
                }
                break;
            }
        }
        bubble_sort();
        if (flag == 0) break;
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