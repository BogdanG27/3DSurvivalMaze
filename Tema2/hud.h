
#include <iostream>

using namespace std;

class Hud {
   public:
    float timeBarCurTime = 2.f;
    float hudRotationX = 0, hudRotationY = 0;
    float healthBar = 2.f;

    Hud();
    ~Hud();
};