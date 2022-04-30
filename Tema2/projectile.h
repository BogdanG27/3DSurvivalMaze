#include <iostream>
#include <vector>

#include "enemy.h"
#include "wall.h"

using namespace std;

class Projectile2 {
   public:
    float posX, posY, posZ, dirX, dirY, dirZ;
    float speed;
    float timeMax = 1.f;
    float timeActual = 0.f;
    float size = 0.3f;

    Projectile2(float x, float y, float z, float dX, float dY, float dZ, float sp);
    ~Projectile2();
    void update(float dTS);
    bool wallCollision(vector<Wall> walls);
    int enemyCollision(vector<Enemy2> enemies);
};