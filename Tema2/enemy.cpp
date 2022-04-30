#include "enemy.h"

Enemy2::Enemy2(float x, float y, float z, float s, float sp) {
    posX = x;
    posY = y;
    posZ = z;
    size = s;
    speed = sp;
}

Enemy2::~Enemy2() {}

// updating the position of the enemy
void Enemy2::update(float dx, float dy, float dz) {
    posX += speed * dx;
    posY += speed * dy;
    posZ += speed * dz;
}