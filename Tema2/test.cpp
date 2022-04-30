// #include <iostream>
// #include <set>
// #include <vector>

// struct Pair {
//     int first, second;
// };
// #define SIZE 10
// using namespace std;

// int main() {
//     vector<vector<int>> maze(3 * SIZE, vector<int>(3 * SIZE, 0));

//     vector<Pair> toBeVisited;

//     // initial cell
//     int x, z;
//     x = (rand() % SIZE) * 2 + 1;
//     z = (rand() % SIZE) * 2 + 1;

//     toBeVisited.push_back({x, z});
//     int pos;

//     while (toBeVisited.size()) {
//         pos = rand() % toBeVisited.size();
//         Pair pair = toBeVisited[pos];

//         maze[pair.first][pair.second] = 1;
//         toBeVisited.erase(toBeVisited.begin() + pos);

//         vector<Pair> neighbours;

//         if (pair.first - 3 >= 0 && maze[pair.first - 3][pair.second] != 0)
//             neighbours.push_back({pair.first - 3, pair.second});

//         if (pair.first + 3 <= SIZE * 3 && maze[pair.first + 3][pair.second] != 0)
//             neighbours.push_back({pair.first + 3, pair.second});

//         if (pair.second - 3 >= 0 && maze[pair.first][pair.second - 3] != 0)
//             neighbours.push_back({pair.first, pair.second - 3});

//         if (pair.second + 3 <= SIZE * 3 && maze[pair.first][pair.second + 3] != 0)
//             neighbours.push_back({pair.first, pair.second + 3});

//         if (neighbours.size()) {
//             int poz = rand() % neighbours.size();
//             Pair neighbour = neighbours[poz];
//             if (neighbour.first == pair.first && neighbour.second == pair.second + 3) {
//                 maze[pair.first][pair.second + 1] = 1;
//                 maze[pair.first][pair.second + 2] = 1;
//             } else if (neighbour.first == pair.first && neighbour.second == pair.second - 3) {
//                 maze[pair.first][pair.second - 1] = 1;
//                 maze[pair.first][pair.second - 2] = 1;
//             } else if (neighbour.first == pair.first + 3 && neighbour.second == pair.second) {
//                 maze[pair.first + 1][pair.second] = 1;
//                 maze[pair.first + 2][pair.second] = 1;
//             } else {
//                 maze[pair.first - 1][pair.second] = 1;
//                 maze[pair.first - 2][pair.second] = 1;
//             }
//         }

//         // upside neighbour
//         if (pair.first - 3 >= 0 && maze[pair.first - 3][pair.second] == 0) {
//             toBeVisited.push_back({pair.first - 3, pair.second});
//         }
//         // leftside neighbour
//         if (pair.second - 3 >= 0 && maze[pair.first][pair.second - 3] == 0) {
//             toBeVisited.push_back({pair.first, pair.second - 3});
//         }
//         // downside neighbour
//         if (pair.first + 3 <= SIZE * 3 && maze[pair.first + 3][pair.second] == 0) {
//             toBeVisited.push_back({pair.first + 3, pair.second});
//         }
//         // rightside neighbour
//         if (pair.second + 3 <= SIZE * 3 && maze[pair.first][pair.second + 3] == 0) {
//             toBeVisited.push_back({pair.first, pair.second + 3});
//         }
//     }

//     for (int i = 3; i < maze.size(); i += 3) {
//         maze.erase(maze.begin() + i);
//         i--;
//     }

//     for (int i = 0; i < maze.size(); i++) {
//         for (int j = 3; j < maze[i].size(); j += 3) {
//             maze[i].erase(maze[i].begin() + j);
//             j--;
//         }
//     }
//     for (size_t i = 0; i < maze.size(); i++) {
//         for (size_t j = 0; j < maze[i].size(); j++) {
//             cout << maze[i][j] << " ";
//         }
//         cout << "\n";
//     }
// }