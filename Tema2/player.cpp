#include "player.h"

Player::Player(float x, float y) {
    posX = x;
    posY = y;
}

Player::~Player() {}

void Player::update(float dX, float dY) {
    posX += dX;
    posY += dY;
}

// check the collision with the walls (aproximating the player with a sphere)
bool Player::wallsCollision(vector<Wall> walls, float dx, float dz) {
    for (int i = 0; i < walls.size(); i++) {
        float minX = walls[i].posX - WALLSIZE / 2;
        float minZ = walls[i].posZ - WALLSIZE / 2;
        float maxX = walls[i].posX + WALLSIZE / 2;
        float maxZ = walls[i].posZ + WALLSIZE / 2;
        float x = max(minX, min(posX + dx, maxX));
        float z = max(minZ, min(posY + dz, maxZ));

        float distance = sqrt((x - posX - dx) * (x - posX - dx) +
                              (z - posY - dz) * (z - posY - dz));

        if (distance < width / 2)
            return true;
    }

    return false;
}
