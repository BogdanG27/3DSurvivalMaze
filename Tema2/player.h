#include <stdio.h>

#include <vector>

#include "projectile.h"
using namespace std;

class Player {
   public:
    float posX, posY, pozZ;
    float width = 1.1f, depth = .4f, height = 2.6f;
    float rotation = 0;
    bool touched = false;
    float timeTouched = 0.f;
    Player(float x, float y);
    ~Player();
    void update(float dX, float dY);
    bool wallsCollision(vector<Wall> walls, float dx, float dz);
};