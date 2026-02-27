#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <string>
#include <cstdlib>
using namespace std;
constexpr auto S = 10;

int nScreenWidth = 70;
int nScreenHeight = 35;
vector<int> arr { 3,5,1,8,6,2,2,4,7,9 };

bool bubble_sort_1_step(void) {
    for (int i = 0; i < S - 1; i++)
        for (int j = 0; j < S - i - 1; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                swap(arr[j], arr[j + 1]);
                return false;
            }
        }
    return true;
}

struct Step {
    vector<int> current;
    struct Step* next = nullptr;
    struct Step* prev = nullptr;
} *START = nullptr;

struct Step* createStep(vector<int> arr) {
    struct Step* t = new Step;
    t->current = arr;
    return t;
}

struct Step* createStepList(void) {
    bool sorted = false;
    START = createStep(arr);
    struct Step* q = START;
    while (!sorted) {
        sorted = bubble_sort_1_step();
        q->next = createStep(arr);
        q->next->next = START;
        q->next->prev = q;
        q = q->next;

    }
    return START;
}

void renderBars(vector<wchar_t>& screen) 
{
    int start = (nScreenHeight  - S * 2) / 2;
    short nShade = 0x2593; // 0x2591;
    for (int i = 0; i < S; i++) 
    {
        int t = arr[i];

        for (int j = 0; j < t * 2; j++)
        {
            screen[((2 * i) + start) * nScreenWidth     + 3] = (wchar_t)(t + L'0');
            screen[((2 * i) + start) * nScreenWidth     + 4] = L'|';
            screen[((2 * i) + start) * nScreenWidth + j + 5] = nShade;
        }

    }
}

int main()
{
    vector<wchar_t> screen(nScreenWidth * nScreenHeight);
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    while (1)
    {
        Sleep(3000);

        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

        bubble_sort_1_step();

        // Formula to write on the screen at (x, y) => screen[y * nScreenWidth + x]
        renderBars(screen);

        auto title = L"Array Bubble Sorting Visualisation";

        swprintf_s(screen.data(), (size_t)35, title);

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen.data(), nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    return 0;
}

//        if (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
//            while (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
//                // Do nothing, just wait
//            }
//            break;
//        }