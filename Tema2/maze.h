#include <iostream>
#include <set>

#include "player.h"

#define SIZE 6
#define ENEMYNO 10

struct Pair {
    int first, second;
};

using namespace std;

class Maze {
   public:
    vector<Wall> walls;
    vector<Enemy2> enemies;
    vector<vector<int>> arr;
    Pair initialPosition;
    Wall *exitPoint;

    Maze();
    ~Maze();
    void GenerateMaze();
    void CreateWalls();
};