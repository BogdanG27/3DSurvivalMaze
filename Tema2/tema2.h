#pragma once

// #include <vector>

#include "camera.h"
#include "components/simple_scene.h"
#include "hud.h"
#include "maze.h"
// #include "projectile.h"

using namespace std;

#define MODE_CENTER 1
#define MODE_CORNER 0

namespace m1 {
class Tema2 : public gfxc::SimpleScene {
   public:
    camera::Camera* camera;

    Tema2();
    ~Tema2();
    void Init() override;
    struct ViewportSpace {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    struct LogicSpace {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

   private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void Draw(float deltaTimeSeconds);
    void FrameEnd() override;
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
    void DrawWall(Wall wall);
    void DrawWalls();
    void DrawFloor();
    void DrawPlayer();
    void DrawEnemies();
    void DrawProjectiles(float deltaTimeSeconds);
    void DrawHud();
    void DrawExitPoint();
    bool PlayerEnemyCollision();

    glm::mat4 projectionMatrix;
    Maze maze;
    bool renderCameraTarget;
    bool projectionType;
    GLfloat fov;
    GLfloat right;
    GLfloat left;
    GLfloat bottom;
    GLfloat top;
    float scale = 0.6f;
    ViewportSpace viewSpace;
    LogicSpace logicSpace;

    Player* player;
    vector<Projectile2> projectiles;
    Hud hud;
    float hudTimer = 0.f;
    bool hide = false;
};
}  // namespace m1