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
vector<int> arr { 3,5,1,8,6,2,4,2,9,7 };
int nStepListLen = 0;

struct Output {
    bool sorted = false;
    int current_idx = 0;
};

struct Output bubble_sort_1_step(void) {
    
    for (int i = 0; i < S - 1; i++)
        for (int j = 0; j < S - i - 1; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                swap(arr[j], arr[j + 1]);
                return Output { false, j };
            }
        }
    return Output { true, S - 1 };
}

struct Output selection_sort_1_step(void) {
    for (int i = 0; i < S - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < S; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
            return Output{ false, i };
        }
    }
    return Output{ true, S - 1 };
}

struct Step {
    vector<int> array_state;
    int current_idx = 0;
    struct Step* next = nullptr;
    struct Step* prev = nullptr;
} *START = nullptr, *p = nullptr;

struct Step* createStep(vector<int> arr, int current_idx) {
    struct Step* t = new Step;
    t->array_state = arr;
    t->current_idx = current_idx;
    return t;
}

struct Step* createStepList(void) {
    struct Output sorted { false, 0 };
    START = createStep(arr, 0);
    struct Step* q = START;
    while (!sorted.sorted) {
        sorted = selection_sort_1_step();
        q->next = createStep(arr, sorted.current_idx); nStepListLen++;
        START->prev = q->next;
        q->next->next = START;
        q->next->prev = q;
        q = q->next;

    }
    return START;
}

void renderBars(vector<wchar_t>& screen) 
{
    int start = (nScreenHeight  - S * 2) / 2;
    short nShade = 0x2593; 
    short currShade = 0x2591;
    for (int i = 0; i < S; i++) 
    {
        int t = p->array_state[i];

        // Formula to write on the screen at (x, y) => screen[y * nScreenWidth + x]
        for (int j = 0; j < t * 2; j++)
        {
            screen[((2 * i) + start) * nScreenWidth     + 5] = (wchar_t)(t + L'0');
            screen[((2 * i) + start) * nScreenWidth     + 6] = L'|';
            screen[((2 * i) + start) * nScreenWidth + j + 7] = (i == p->current_idx) ? currShade: nShade;
            screen[((2 * p->current_idx) + start) * nScreenWidth + 3] = 0x2192;
        }
    }
}

int main()
{
    vector<wchar_t> screen(nScreenWidth * nScreenHeight);
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    START = createStepList();
    p = START;

    while (1)
    {
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) 
        {
            while (GetAsyncKeyState((unsigned short)'A') & 0x8000) {/* wait */}
            p = p->prev;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            while (GetAsyncKeyState((unsigned short)'D') & 0x8000) {/* wait */}
            p = p->next;
        }
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) return EXIT_SUCCESS;
        
        renderBars(screen);
        auto title = L"Array Sorting Visualisation (Steps needed: %d)";
        swprintf_s(screen.data(), (size_t)50, title, nStepListLen - 1);

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen.data(), nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}