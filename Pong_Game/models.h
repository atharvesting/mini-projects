//#pragma once
//
//class Fallable {
//public:
//    int coordX;
//    float coordY;
//    int speed;
//    bool inScene;
//    short skin;
//
//    virtual void update(float elapsedTime) {
//        coordY += speed * elapsedTime;
//    }
//
//    virtual void draw(wchar_t* screen, int nScreenWidth) {
//        screen[(int)coordY * nScreenWidth + coordX] = skin;
//    }
//
//    virtual bool is_collision() {
//        if ((int)this->coordY == nBarLevel
//            && nBarStartIdx - 1 <= this->coordX
//            && this->coordX <= nBarStartIdx + nBarWidth)
//        {
//            return true;
//        }
//    }
//};
