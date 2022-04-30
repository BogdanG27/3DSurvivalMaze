#include "projectile.h"

Projectile2::Projectile2(float x, float y, float z, float dX, float dY, float dZ, float sp) {
    posX = x, posY = y, posZ = z;
    dirX = dX, dirY = dY, dirZ = dZ;
    speed = sp;
}

Projectile2::~Projectile2() {}

void Projectile2::update(float dTS) {
    posX += dirX * speed * dTS;
    posY += dirY * speed * dTS;
    posZ += dirZ * speed * dTS;
    timeActual += dTS;
}

bool Projectile2::wallCollision(vector<Wall> walls) {
    for (int i = 0; i < walls.size(); i++) {
        if (posX - size / 2 < walls[i].posX + WALLSIZE / 2 &&
            posX + size / 2 > walls[i].posX - WALLSIZE / 2 &&
            posZ - size / 2 < walls[i].posZ + WALLSIZE / 2 &&
            posZ + size / 2 > walls[i].posZ - WALLSIZE / 2)
            return true;
    }
    return false;
}

int Projectile2::enemyCollision(vector<Enemy2> enemies) {
    for (int i = 0; i < enemies.size(); i++) {
        if (posX - size / 2 < enemies[i].posX + enemies[i].size / 2 &&
            posX + size / 2 > enemies[i].posX - enemies[i].size / 2 &&
            posZ - size / 2 < enemies[i].posZ + enemies[i].size / 2 &&
            posZ + size / 2 > enemies[i].posZ - enemies[i].size / 2)
            return i;
    }
    return -1;
}