#include <iostream>

using namespace std;

class Enemy2 {
   public:
    float posX, posY, posZ;
    float size;
    float speed;
    float color = 1.f;
    float time = 0.f;
    bool hit = false;
    float timeElapsed = 0.f;

    Enemy2(float x, float y, float z, float s, float sp);
    ~Enemy2();
    void update(float dx, float dy, float dz);
};