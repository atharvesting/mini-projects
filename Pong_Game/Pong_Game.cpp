#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <random>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <format>

class GameManager; // Forward declaration

const int nScreenHeight{ 30 };
const int nScreenWidth{ 100 };

static double randomNumGenerator(bool generateZeroToOne, int low, int high) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if (generateZeroToOne) {
        static std::uniform_real_distribution<double> dis(0, 1);
        return dis(gen);
    }
    else {
        static std::uniform_real_distribution<double> dis(low, high);  // dis(1, nScreenWidth - 2);
        return dis(gen);
    }
}

class Fallable {
protected:  // Such that only derived classes can access them
    float coordX;
    float coordY;
    float speedX;
    int speedY;
    bool inScene;
    short skin;

public:
    Fallable(int x, float y, int speed, short skin)
        : coordX(x), coordY(y), speedX(0), speedY(speed), inScene(true), skin(skin) {}

    virtual void update(float elapsedTime) = 0;
    virtual void draw(wchar_t* screen, int nScreenWidth) = 0;
    virtual void on_collision(GameManager& game, bool hitBar) = 0;

    bool is_collision_bar(int barX, int barLevel, int barWidth) {

        if (static_cast<int>(coordY) >= barLevel 
            && barX - 1 <= coordX
            && coordX <= barX + barWidth) 
        {
            inScene = false;
            return true;
        }

        return false;
    }

    bool is_collision_ground() {
        if (static_cast<int>(coordY) >= nScreenHeight - 1) {
            inScene = false;
            return true;
        }
        return false;
    }

    bool is_in_scene() {
        return this->inScene;
    }

    void set_speedX(float newSpeedX) {
        speedX = newSpeedX;
    }

    float getCoordX() {
        return coordX;
    }

    virtual ~Fallable() {}

};

class Ball : public Fallable {
public:
    Ball(int speed, short skin)
        : Fallable(randomNumGenerator(false, 1, nScreenWidth - 1), 0, speed, skin) {
    }

    void update(float fElapsedTime) override {
        coordY += speedY * fElapsedTime;
        float nextX = coordX + (speedX * fElapsedTime);

        // Keep ball within horizontal bounds [0, nScreenWidth - 1]
        if (nextX >= 0 && nextX < nScreenWidth) {
            coordX = nextX;
        }
        else {
            speedX *= -1; // Ball will bounce off the wall
        }
    }

    void draw(wchar_t* screen, int nScreenWidth) override {
        if (inScene) screen[static_cast<int>(coordY) * nScreenWidth + static_cast<int>(coordX)] = skin;
    }

    void on_collision(GameManager& game, bool hitBar) override;

    ~Ball() {}
};

class PowerUp : public Fallable {
protected:
    int duration;
    double elapsedTime;
    bool isActive;
    std::function<void(GameManager&)> activateCallback;
    std::function<void(GameManager&)> deactivateCallback;

public:
    PowerUp(int speed, short skin, int duration,
        std::function<void(GameManager&)> onActivate,
        std::function<void(GameManager&)> onDeactivate)
        : Fallable(randomNumGenerator(false, 1, nScreenWidth - 2), 0, speed, skin), 
            duration(duration), isActive(false), elapsedTime(0),
            activateCallback(onActivate), deactivateCallback(onDeactivate) {}

    void update(float fElapsedTime) {
        coordY += fElapsedTime * speedY;
        elapsedTime += fElapsedTime;
    }

    void on_collision(GameManager& game, bool hitBar) {
        if (hitBar) {
            isActive = true; elapsedTime = 0; inScene = false;
            if (activateCallback) activateCallback(game);
        }
    }

    bool is_active() {
        return isActive;
    }

    bool time_left() {
        return duration > elapsedTime;
    }

    void draw(wchar_t* screen, int nScreenWidth) {
        if (inScene) screen[static_cast<int>(coordY) * nScreenWidth + static_cast<int>(coordX)] = skin;
    }

    void deactivate(GameManager& game) {
        if (deactivateCallback) deactivateCallback(game);
    }

    ~PowerUp() {}
};

class PUinDisguise : public PowerUp {
public:
    short realSkin;

    PUinDisguise(int speed, short skin, int duration,
        std::function<void(GameManager&)> onActivate,
        std::function<void(GameManager&)> onDeactivate,
        short realSkin)
        : PowerUp(speed, skin, duration, onActivate, onDeactivate), realSkin(realSkin) {
    }

    void update(float fElapsedTime) {
        coordY += fElapsedTime * speedY;
        elapsedTime += fElapsedTime;

        if (elapsedTime > 0.7 * (nScreenHeight / speedY)) skin = realSkin;
    }
};

enum class PowerUpType {
    PUdoubleBar,        
    PUdoublePoints,    
    PUenhancedSpeed,    
    PUmagnetBar,
    PUdaBomb,
    PUdaBlessing,
    PUantiMagnetBar,
    PUbid,
    PUcid,
    PUhalfPoints,
    PUhalfSpeed,
};

class FallableFactory {
public:
    static std::unique_ptr<Ball> create_ball();
    static std::unique_ptr<PowerUp> create_powerup(PowerUpType type);
};

class GameManager {
public:
    struct Bar {
        float fStartIdx = nScreenWidth / 2;
        int nWidth{ 10 };
        int nLevel{ nScreenHeight - 2 };
        float nSpeed{ 24 };
        short nSkin = 0x5F;  // 0x2593

        void transform(float multiplier) {
            nWidth *= multiplier;
        }

        void reset() {
            nWidth = 10;
        }

    } bar;

    std::vector<std::unique_ptr<Ball>> balls;
    std::vector<std::unique_ptr<PowerUp>> powerups;
    int nScore = 0;
    float nScoreMultiplier = 1;
    bool magnetic = false;
    bool antimagnetic = false;
    int antiMagneticFactor = 1;
    int barMid;

    std::unordered_map<PowerUpType, float> spawnTable = {
        { PowerUpType::PUdoubleBar,         0.001       },
        { PowerUpType::PUdoublePoints,      0.001       },
        { PowerUpType::PUenhancedSpeed,     0.002       },
        { PowerUpType::PUmagnetBar,         0.0001      },
        { PowerUpType::PUdaBomb,            0.5         },
        { PowerUpType::PUdaBlessing,        0.000001    },
        { PowerUpType::PUantiMagnetBar,     0.08        },
        { PowerUpType::PUbid,               0.08        },
        { PowerUpType::PUcid,               0.08        },
        { PowerUpType::PUhalfPoints,        0.08        },
        { PowerUpType::PUhalfSpeed,         0.08        },
    };

    void update_score(int num, float multiplier) {
        nScore += num * multiplier;
    }

    void update(float fElapsedTime)
    {
        
        // Bar Movement Logic
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            if (bar.fStartIdx > 0) bar.fStartIdx -= bar.nSpeed * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            if (bar.fStartIdx < nScreenWidth - bar.nWidth - 1) bar.fStartIdx += bar.nSpeed * fElapsedTime;
        }

        // Powerup spawning Logic
        for (auto& [type, rate] : spawnTable) {
            double p = rate * fElapsedTime;

            if (randomNumGenerator(true, 2, 3) < p) {
                if (powerups.size() < 12) powerups.push_back(FallableFactory::create_powerup(type));
            }
        }

        bool spawnBall = false;
        barMid = static_cast<int>((2 * bar.fStartIdx + bar.nWidth) / 2);
        
        // Updating the balls
        for (auto it = balls.begin(); it != balls.end(); ) 
        {
            if (magnetic) {
                float current_X = (*it)->getCoordX();
                antiMagneticFactor = antimagnetic ? -1 : 1;
                if (std::abs(current_X - barMid) > 4) (*it)->set_speedX((antiMagneticFactor * (barMid - current_X) / 4));
            }

            (*it)->update(fElapsedTime);

            bool hitBar = (*it)->is_collision_bar(bar.fStartIdx, bar.nLevel, bar.nWidth);
            bool hitGround = (*it)->is_collision_ground();

            if (hitBar || hitGround || !(*it)->is_in_scene()) {
                (*it)->on_collision(*this, hitBar);
                spawnBall = true;

                it = balls.erase(it);
            }
            else {
                ++it; 
            }
        }

        if (spawnBall) balls.push_back(FallableFactory::create_ball());

        // Updating the powerups
        for (auto it = powerups.begin(); it != powerups.end(); )
        {
            (*it)->update(fElapsedTime);

            // If already active
            if ((*it)->is_active()) 
            {
                if ((*it)->time_left() == false) {
                    (*it)->deactivate((*this));
                    it = powerups.erase(it);
                }
                else {
                    ++it;
                }
            }
            // If inactive
            else 
            {
                bool hitGround = (*it)->is_collision_ground();
                bool hitBar = (*it)->is_collision_bar(bar.fStartIdx, bar.nLevel, bar.nWidth);

                if (!hitGround && !hitBar) ++it;
                else if (hitBar) {
                    (*it)->on_collision((*this), hitBar);
                    ++it;
                }
                else it = powerups.erase(it);
            }
        }
    }

    void draw(wchar_t* screen, int nScreenWidth) 
    {
        screen[(bar.nLevel) * nScreenWidth + (int)bar.fStartIdx] = 0x23B9;
        screen[(bar.nLevel) * nScreenWidth + (int)bar.fStartIdx + bar.nWidth - 1] = 0x23B8;

        // Formula to write on the screen at (x, y) => screen[y * nScreenWidth + x]
        for (int i = 1; i < bar.nWidth - 1; i++)
        {
            screen[bar.nLevel * nScreenWidth + (int)bar.fStartIdx + i] = bar.nSkin;
        }
        for (auto it = balls.begin(); it != balls.end(); ++it) 
        {
            (*it)->draw(screen, nScreenWidth);
        }
        for (auto it = powerups.begin(); it != powerups.end(); ++it)
        {
            (*it)->draw(screen, nScreenWidth);
        }
    }
};

void Ball::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) game.update_score(1, game.nScoreMultiplier);
    else if (game.nScore > 0) game.update_score(-2, game.nScoreMultiplier);
}

std::unique_ptr<Ball> FallableFactory::create_ball() {
    return std::make_unique<Ball>(10, 0x2B24);
}

std::unique_ptr<PowerUp> FallableFactory::create_powerup(PowerUpType type) {
    switch (type) {
        case PowerUpType::PUdoubleBar:
            return std::make_unique<PowerUp>(12, 0x002B, 7,
                [](GameManager& game) { if (game.bar.nWidth <= 20) game.bar.transform(2); },
                [](GameManager& game) { if (game.bar.nWidth > 10) game.bar.transform(0.5); }
            );
        case PowerUpType::PUdoublePoints:
            return std::make_unique<PowerUp>(10, 0x0058, 10,
                [](GameManager& game) { game.nScoreMultiplier *= 2; },
                [](GameManager& game) { game.nScoreMultiplier /= 2; }
            );
        case PowerUpType::PUenhancedSpeed:
            return std::make_unique<PowerUp>(12, 0x2192, 7,
                [](GameManager& game) { game.bar.nSpeed *= 1.25; },
                [](GameManager& game) { game.bar.nSpeed /= 1.25; }
            );
        case PowerUpType::PUmagnetBar:
            return std::make_unique<PowerUp>(13, 0x2229, 15,
                [](GameManager& game) { 
                    if (!game.antimagnetic) game.magnetic = true;
                    else game.antimagnetic = false;
                },
                [](GameManager& game) { if (!game.antimagnetic) game.magnetic = false; }
            );
        case PowerUpType::PUdaBomb:
            return std::make_unique<PowerUp>(5, 0x2764, 1,
                [](GameManager& game) { if (game.nScore >= 50) game.nScore -= 50; else game.nScore = 0; },
                nullptr
            );
        case PowerUpType::PUdaBlessing:
            return std::make_unique<PowerUp>(12, 0x2623, 1,
                [](GameManager& game) { game.nScore += game.nScoreMultiplier * 50; },
                nullptr
            );
        case PowerUpType::PUantiMagnetBar:
            return std::make_unique<PowerUp>(12, 0x2624, 7,
                [](GameManager& game) { game.magnetic = true; game.antimagnetic = true; },
                [](GameManager& game) { game.magnetic = false; game.antimagnetic = false; }
            );
        case PowerUpType::PUbid:
            return std::make_unique<PUinDisguise>(6, 0x2764, 1,
                [](GameManager& game) { game.nScore += 20; },
                nullptr,
                0x2623
            );
        case PowerUpType::PUcid:
            return std::make_unique<PUinDisguise>(13, 0x2623, 1,
                [](GameManager& game) { if (game.nScore >= 55) game.nScore -= 55; else game.nScore = 0; },
                nullptr,
                0x2764
            );
        case PowerUpType::PUhalfPoints:
            return std::make_unique<PowerUp>(12, 0x00F7, 8,
                [](GameManager& game) { game.nScoreMultiplier /= 2; },
                [](GameManager& game) { game.nScoreMultiplier *= 2; }
            );
        case PowerUpType::PUhalfSpeed:
            return std::make_unique<PowerUp>(12, 0x2190, 7,
                [](GameManager& game) { game.bar.nSpeed /= 2; },
                [](GameManager& game) { game.bar.nSpeed *= 2; }
            );
        default:
            return nullptr; 
    }
}

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    auto tp1 = std::chrono::system_clock::now();
    auto startTime = tp1;
    auto tp2 = std::chrono::system_clock::now();

    GameManager game;
    game.balls.push_back(FallableFactory::create_ball());

    while (1) 
    {
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) return 0;
        
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
        
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        game.update(fElapsedTime);
        game.draw(screen, nScreenWidth);
        std::chrono::duration<float> gameDuration = tp1 - startTime;
        
        // Render basic stats like score and total elapsed time
        std::wstring stats = std::format(
            L"Score = {}, Time = {}",
            game.nScore, static_cast<int>(gameDuration.count())
        );
        for (size_t i = 0; i < stats.size(); i++)
            screen[i] = stats[i];

        // Render the score beneath the bar at all times
        std::wstring score = std::format(L"{}", game.nScore);
        for (size_t i = 0; i < score.size(); i++)
            screen[(game.bar.nLevel + 1) * (nScreenWidth) + game.barMid - static_cast<int>(score.size() / 2) + i] = score[i];

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }

    delete[] screen;

    return 0;
}