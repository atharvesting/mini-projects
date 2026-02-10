#include <iostream>
#include <chrono>

// Crucial header. Contains all functions needed to talk to Windows OS to create windows and manipulate the console.
#include <Windows.h>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

// Setting screen dimensions for play. Notice use of a global variable.
int nScreenWidth = 120;
int nScreenHeight = 40;

// float used for the position tracking so that movement is smooth.
// int would result in choppy exact movements
float fPlayerX = 8.0f;  // X-coordinate position
float fPlayerY = 8.0f;  // Y-coordinate position
float fPlayerA = 0.0f;  // Facing Angle. Likely pointed east (Theta = 0 from top down) to begin with.

// Minimap dimensions
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
    * Direct assignment to pointer works because the array identifier is a address itself.
    */
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    /*
    * HANDLE is a ticket provided by Windows OS that gives access to grab a console buffer and use it.
    * CreateConsoleScreenBuffer creates a new screen buffer which is secondary and hidden as opposed to the default used by CP.
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
    map += L"#........#######";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#......#.......#";
    map += L"#......#.......#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"##########.....#";
    map += L"#........#.....#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    // Game Loop
    while (1) {
        /*
        * Since every computer configuration will result in different speeds, it is necessary to adapt to this inconsistency
        * by adapting the movement speed to the number of loops executed by the program per second.
        * For example, without adaption, fPlayerA -= 2.0f will result in:
        *   Unplayably fast rotation in a fast system (0.001s per loop), Speed ~ 2000 units of rotation per second
        *   Very slow rotation in a slow system (0.1 per loop), Speed ~ 20 units of rotation per second
        * 
        * fElapsedTime calculates time between loops and uses S=D/T to make movement speeds consistent across devices.
        */
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        /*
        * Function part of Windows API that detects key presses.
        * Returns a short int (16 bit) where the most significant bit indicates whether the key is being pressed right now.
        * The hexadecimal 0x8000 accesses this bit and an AND operation is performed.
        */
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) 
            fPlayerA -= (2.0f) * fElapsedTime;  // The degre of movement becomes inversely proportional to the frame rate.
        if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
            fPlayerA += (2.0f) * fElapsedTime;
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }

        for (int x = 0; x < nScreenWidth; x++)  // Loop for every vertical column in the console to draw
        {
            // Since 3-d graphics are out of scope for a console screen, we simulate them using ray casting.
            // For every column, we shoot a ray to see what it hits. If close, draw a big wall. If not, a smaller one.
            // For each column, calculate the projected ray angle into world space
            float fHalfFOV = fFOV / 2.0f;
            // (fPlayerA - fHalfFOV) represents the left edge in the field of view. This is our starting point to "shoot" rays.
            // then, we add the required angle proportional to the column that we are targeting.
            float fRayAngle = (fPlayerA - fHalfFOV) + ((float)x / (float)nScreenWidth) * fFOV;

            float fDistanceToWall = 0;  // Initialize the distance from player to wall for every ray that is cast.
            bool bHitWall = false;
            bool bBoundary = false;

            // Unit vectors for the player. Purely to indicate direction separately in horizontal and vertical directions.
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            // Loop until the ray hits a wall or travels too far
            while (!bHitWall && fDistanceToWall < fDepth)
            {
                /* 
                * Baby steps for the ray travel through the world if loop conditions are still satisfied.
                * Choosing a step of 0.1 is also strategic:
                *    Higher (1.0f): Faster gameplay. But ray might skip a thin wall or a corner, i.e. anything with an thinner width than expected.
                *    Lower (0.001f): Super precise, but calculation-intensive. This will slow the game down.
                */
                fDistanceToWall += 0.1f;

                // Tracking current ray position. Remember that DistanceToWall is incrementing. This is like y = mx + b.
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
                    // Ray is inbounds so test to see if ray cell is a wall block
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;

                        vector<pair<float, float>> p;  // distance to perfect corner, dot product

                        for (int tx = 0; tx < 2; tx++)
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = (float)nTestY + ty - fPlayerY;
                                float vx = (float)nTestX + tx - fPlayerX;

                                float d = sqrt(vx * vx + vy * vy);
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d);

                                p.push_back(make_pair(d, dot));
                            }
                        sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

                        float fBound = 0.008;
                        if (acos(p.at(0).second) < fBound) bBoundary = true;
                        if (acos(p.at(1).second) < fBound) bBoundary = true;
                        //if (acos(p.at(2).second) < fBound) bBoundary = true;

                    }
                }
            }
            // Calculate distance to ceiling and floor
            // Doing this is the first part of building the illusion of perspective
            // Because the further a wall is, the more of the floor and ceiling we will be able to see in that area
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            short nShade = ' ';
            short gShade = ' ';

            if (fDistanceToWall <= fDepth / 4.0f)           nShade = 0x2588;
            else if (fDistanceToWall < fDepth / 3.0f)       nShade = 0x2593;
            else if (fDistanceToWall < fDepth / 2.0f)       nShade = 0x2592;
            else if (fDistanceToWall < fDepth)              nShade = 0x2591;
            else                                            nShade = ' ';

            if (bBoundary) nShade = '|';

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                    screen[y * nScreenWidth + x] = ' ';
                else if (y > nCeiling && y <= nFloor)
                    screen[y * nScreenWidth + x] = nShade;
                else 
                {
                    float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
                    if (b < 0.25)       gShade = '#';
                    else if (b < 0.5)   gShade = 'x';
                    else if (b < 0.75)  gShade = '.';
                    else if (b < 0.9)   gShade = '-';
                    else                gShade = ' ';

                    screen[y * nScreenWidth + x] = gShade;
                }
                    
            }
        }
        // Display Stats
        swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);

        // Display Mini-Map
        for (int nx = 0; nx < nMapWidth; nx++)
            for (int ny = 0; ny < nMapWidth; ny++)
            {
                screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
            }

        screen[((int)fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'P';
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

    }

    return 0;
}