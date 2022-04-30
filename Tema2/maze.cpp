#include "maze.h"

Maze::Maze() {
}

Maze::~Maze() {}

// generating the maze
void Maze::GenerateMaze() {
    // we use Prim's algorithm to generate the maze
    vector<vector<int>> maze(3 * SIZE, vector<int>(3 * SIZE, 0));

    // selecting an initial cell
    int x, z;
    x = (rand() % SIZE) * 3 + 1;
    z = (rand() % SIZE) * 3 + 1;

    // adding the position to the toVisit vector
    vector<Pair> toBeVisited;
    toBeVisited.push_back({x, z});
    int pos;

    // while visited has elements
    // select a random element
    // conect it to it's neighbors if it has any (3 cells diffence)
    // if a neighbor was not marked as clear path the add it to the toVisit vector
    // repet the process
    while (toBeVisited.size()) {
        pos = rand() % toBeVisited.size();
        Pair pair = toBeVisited[pos];

        maze[pair.first][pair.second] = 1;
        toBeVisited.erase(toBeVisited.begin() + pos);

        vector<Pair> neighbours;

        if (pair.first - 3 >= 0 && maze[pair.first - 3][pair.second] != 0)
            neighbours.push_back({pair.first - 3, pair.second});

        if (pair.first + 3 < SIZE * 3 && maze[pair.first + 3][pair.second] != 0)
            neighbours.push_back({pair.first + 3, pair.second});

        if (pair.second - 3 >= 0 && maze[pair.first][pair.second - 3] != 0)
            neighbours.push_back({pair.first, pair.second - 3});

        if (pair.second + 3 < SIZE * 3 && maze[pair.first][pair.second + 3] != 0)
            neighbours.push_back({pair.first, pair.second + 3});

        if (neighbours.size()) {
            int poz = rand() % neighbours.size();
            Pair neighbour = neighbours[poz];
            if (neighbour.first == pair.first && neighbour.second == pair.second + 3) {
                maze[pair.first][pair.second + 1] = 1;
                maze[pair.first][pair.second + 2] = 1;
            } else if (neighbour.first == pair.first && neighbour.second == pair.second - 3) {
                maze[pair.first][pair.second - 1] = 1;
                maze[pair.first][pair.second - 2] = 1;
            } else if (neighbour.first == pair.first + 3 && neighbour.second == pair.second) {
                maze[pair.first + 1][pair.second] = 1;
                maze[pair.first + 2][pair.second] = 1;
            } else {
                maze[pair.first - 1][pair.second] = 1;
                maze[pair.first - 2][pair.second] = 1;
            }
        }

        if (pair.first - 3 >= 0 && maze[pair.first - 3][pair.second] == 0) {
            toBeVisited.push_back({pair.first - 3, pair.second});
        }
        if (pair.second - 3 >= 0 && maze[pair.first][pair.second - 3] == 0) {
            toBeVisited.push_back({pair.first, pair.second - 3});
        }
        if (pair.first + 3 < SIZE * 3 && maze[pair.first + 3][pair.second] == 0) {
            toBeVisited.push_back({pair.first + 3, pair.second});
        }
        if (pair.second + 3 < SIZE * 3 && maze[pair.first][pair.second + 3] == 0) {
            toBeVisited.push_back({pair.first, pair.second + 3});
        }
    }

    // select a random side for the exit
    int randSide = rand() % 4;
    int i = -1, j = -1;

    if (randSide == 0) {
        i = 0;
        do {
            j = rand() % maze.size();
        } while (maze[i + 1][j] == 0);
        maze[i][j] = 3;
    } else if (randSide == 1) {
        i = maze.size() - 1;
        do {
            j = rand() % maze.size();
        } while (maze[i - 1][j] == 0);
        maze[i][j] = 3;
    } else if (randSide == 2) {
        j = 0;
        do {
            i = rand() % maze.size();
        } while (maze[i][j + 1] == 0);
        maze[i][j] = 3;
    } else {
        j = maze.size() - 1;
        do {
            i = rand() % maze.size();
        } while (maze[i][j - 1] == 0);
        maze[i][j] = 3;
    }

    exitPoint = new Wall(j, i);

    // set the initial position
    initialPosition.first = x;
    initialPosition.second = z;

    maze[x][z] = 8;
    int k = 0;

    // create 10 enemies places random on the map
    do {
        int a = rand() % (SIZE * 3);
        int b = rand() % (SIZE * 3);
        if (maze[a][b] == 1) {
            maze[a][b] = 2;
            Enemy2 enemy(((float)a + 0.5f) * WALLSIZE, WALLSIZE / 2, ((float)b + 0.5f) * WALLSIZE, 1.f, .5f);
            enemies.push_back(enemy);
            k++;
        }
    } while (k != 10);

    arr = maze;
}

// creating the walls
void Maze::CreateWalls() {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = 0; j < arr[i].size(); j++) {
            if (arr[i][j] == 0) {
                Wall wall(((float)j + 0.5f) * WALLSIZE, ((float)i + 0.5f) * WALLSIZE);
                walls.push_back(wall);
            }
        }
    }
}