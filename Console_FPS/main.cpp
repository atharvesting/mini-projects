// ConsoleFPS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
// Crucial header. Contains all functions needed to talk to Windows OS to create windows and manipulate the console.
#include <Windows.h>
#include <string>
using namespace std;

// Setting screen dimensions for play
int nScreenWidth = 120;
int nScreenHeight = 40;

// float used for the position tracking so that movement is smooth.
// int would result in choppy exact movements
float fPlayerX = 0.0f;  // X-coordinate position
float fPlayerY = 0.0f;  // Y-coordinate position
float fPlayerA = 0.0f;  // Facing Angle. Likely pointed east (Theta = 0) from top down.

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0;  // 45 degree cone of vision
float fDepth = 16.0f;

int main()
{
    // Creating the Screen Buffer
    /*
    * wchar_t datatype (2 bytes) required if Unicode characters (special characters for drawing, foreign languages etc.)
    * screen holds the memory address of the array
    * new modifies is used to reserve an entire chunk necessary memory
    * We are not creating a 2-d array. Instead, a 1-d array that will hold all characters that will fill the screen
    */
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    /*
    * HANDLE is a ticket provided by Windows that gives access to grapb a console buffer and use it
    * CreateConsoleScreenBuffer creates a new screen buffer which is secondary and hidden as opposed to the default used by CP
    * GENERIC_READ | GENERIC_WRITE: These are permissions given to the program by Windows. | (OR) combines both permissions.
    * CONSOLE_TEXTMODE_BUFFER for text grid as opposed to a graphics window.
    */
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    // Set the newly created screen buffer as the active screen that the user sees.
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;  // Archaic Microsoft equivalent of the int datatype

    wstring map;  // Just like with the wchar_t screen, wstring allows for unicode characters
    map += L"################";  // L prefix necessary to indicate string containing wide characters (wchar_t)
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    // Game Loop
    while (1) {

        for (int x = 0; x < nScreenWidth; x++)  // Loop for every vertical column in the console to draw
        {
            // Since 3-d graphics are out of scope for a console screen, we simulate them using ray casting.
            // For every column, we shoot a ray to see what it hits. If close, draw a big wall. If not, a smaller one.
            // For each column, calculate the projected ray angle into world space
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            // Loop until the ray hits a wall or travels too far
            while (!bHitWall && fDistanceToWall < fDepth)
            {
                // Baby steps for the ray travel through the world if loop conditions are still satisfied
                // Choosing a step of 0.1 is also strategic:
                // Higher (1.0f): Faster gameplay. But ray might skip a thin wall or a corner, i.e. anything with an thinner width
                // Lower (0.001f): Super precise, but calculation-intensive. This will slow the game down
                fDistanceToWall += 0.1f;  

                // Tracking current ray position 
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                // Testing if ray is out of bounds
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) 
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else 
                {
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }

            // Calculate distance to ceiling and floor
            // Doing this is the first part of building the illusion of perspective
            // Because the further a wall is, the more of the floor and ceiling we will be able to see in that area
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloat = nScreenHeight - nCeiling;
        }


        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

    }

    return 0;
}