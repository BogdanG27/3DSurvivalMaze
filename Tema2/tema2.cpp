#include "tema2.h"

#include <Core/Engine.h>

#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2() {
}

Tema2::~Tema2() {
}

void Tema2::Init() {
    renderCameraTarget = false;
    // generating the maze
    // and creating the walls
    maze.GenerateMaze();
    maze.CreateWalls();

    // setting the camera to a random initial position determined when
    // creating the maze
    float cameraX = WALLSIZE * ((float)maze.initialPosition.first + 0.5f);
    float cameraZ = WALLSIZE * ((float)maze.initialPosition.second + 0.5f);
    camera = new camera::Camera();
    camera->Set(glm::vec3(cameraX, 1.5, cameraZ), glm::vec3(cameraX, 1, 0), glm::vec3(0, 1, 0));

    // importing the meshes needed (box and sphere(for projectiles))
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    fov = 60.f;

    // setting the player initial pozition
    player = new Player(cameraX, cameraZ);

    {
        Shader* shader = new Shader("PlayerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "PlayerVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "PlayerFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;

        shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "EnemyVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "EnemyFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // printing the maze to se it's state
    for (size_t i = 0; i < maze.arr.size(); i++) {
        for (size_t j = 0; j < maze.arr[i].size(); j++) {
            cout << maze.arr[i][j] << " ";
        }
        cout << "\n";
    }

    // setting the view to perspective
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
}

// funtion to draw a wall
void Tema2::DrawWall(Wall wall) {
    // translate to the right position and scaling to the wall size
    // render the mesh
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(wall.posX, WALLSIZE / 2, wall.posZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(WALLSIZE, WALLSIZE, WALLSIZE));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0, 0, 1));
    }
}

// function from lab4 for rotation on OY axes
// had a problem with glm::rotate function, had to use this one
glm::mat4 RotateOY(float radians) {
    return glm::transpose(
        glm::mat4(cos(radians), 0, sin(radians), 0,
                  0, 1, 0, 0,
                  -sin(radians), 0, cos(radians), 0,
                  0, 0, 0, 1));
}

// same for OX
glm::mat4 RotateOX(float radians) {
    return glm::transpose(glm::mat4(1, 0, 0, 0,
                                    0, cos(radians), -sin(radians), 0,
                                    0, sin(radians), cos(radians), 0,
                                    0, 0, 0, 1));
}

// Drawing the Hud
void Tema2::DrawHud() {
    // changing the view to ortho
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.01f, 200.0f);
    // drwaing the time bar
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix *= RotateOX(hud.hudRotationX);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(3 - (2.f - hud.timeBarCurTime) / 2, 3, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(hud.timeBarCurTime, 0.5, 1));

        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.2, 0.8, 1));
    }

    // drawing the health bar
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix *= RotateOX(hud.hudRotationX);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(-(3 - (2.f - hud.healthBar) / 2), 3, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(hud.healthBar, 0.5, 1));

        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0, 1, 0));
    }
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
}

// funtion to draw all the walls
void Tema2::DrawWalls() {
    for (auto wall : maze.walls)
        DrawWall(wall);
}

// function to draw the exit point
void Tema2::DrawExitPoint() {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(((float)maze.exitPoint->posX + 0.5f) * WALLSIZE, WALLSIZE / 2, ((float)maze.exitPoint->posZ + 0.5) * WALLSIZE));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(WALLSIZE, WALLSIZE, WALLSIZE));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0, 1, 0));
    }
}

// function to draw the floor (a scaled box)
void Tema2::DrawFloor() {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(maze.arr.size() * WALLSIZE / 2, 0, maze.arr.size() * WALLSIZE / 2));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(maze.arr.size() * WALLSIZE, 0.1, maze.arr.size() * WALLSIZE));

        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(1, 1, 1));
    }
}

void Tema2::FrameStart() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

// function to draw all the enemies
void Tema2::DrawEnemies() {
    for (auto enemy : maze.enemies) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemy.posX, enemy.posY, enemy.posZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(enemy.size, enemy.size, enemy.size));
        // if the enemy was hit
        // render it with the shader that applies the deformation
        // otherwise render it normally
        if (enemy.hit == false)
            RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(1, 0, 0));
        else {
            RenderMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 1, 0));
        }
    }
}

// draw all the parts of the body
void Tema2::DrawPlayer() {
    // the body
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX, 2 * scale, player->posY));
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1 * scale, 2 * scale, 0.4 * scale));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    }
    // the left leg
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX, 0.5 * scale, player->posY));
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.15, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4 * scale, 1 * scale, 0.4 * scale));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }
    // the right leg
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3((player->posX), 0.5 * scale, player->posY));
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.15, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4 * scale, 1 * scale, 0.4 * scale));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }

    // the left arm
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX, 2 * scale, player->posY));
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.43, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4 * scale, 2 * scale, 0.4 * scale));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }

    // the right arm
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX, 2 * scale, player->posY));
        modelMatrix *= RotateOY(player->rotation);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.43, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4 * scale, 2 * scale, 0.4 * scale));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }

    // the head
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX, 3.4 * scale, player->posY));
        modelMatrix = glm::rotate(modelMatrix, player->rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8 * scale, 0.8 * scale, 0.4 * scale));
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }
}

// function to draw all scene elements
void Tema2::Draw(float deltaTimeSeconds) {
    // pressing O key hides the walls and the enemies
    // useful to find the exit faster
    // only for demonstation purposes
    if (!hide) {
        DrawWalls();
        DrawEnemies();
        DrawHud();
    }
    DrawFloor();
    DrawExitPoint();
    // draw the player only in 3rd person
    DrawProjectiles(deltaTimeSeconds);
    if (camera->person == PERSON::THIRD)
        DrawPlayer();
}

// function to draw the projectile
void Tema2::DrawProjectiles(float deltaTimeSeconds) {
    for (auto& projectile : projectiles) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(projectile.posX, projectile.posY, projectile.posZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(projectile.size));
        RenderMesh(meshes["sphere"], shaders["PlayerShader"], modelMatrix, glm::vec3(0.2, 0.5, 0.4));
    }
}

bool Tema2::PlayerEnemyCollision() {
    for (auto enemy : maze.enemies) {
        if (player->posX - player->width < enemy.posX + enemy.size / 2 &&
            player->posX + player->width > enemy.posX - enemy.size / 2 &&
            player->posY - player->depth < enemy.posZ + enemy.size / 2 &&
            player->posY + player->depth > enemy.posZ - enemy.size / 2 &&
            enemy.hit == false)
            return true;
    }
    return false;
}

void Tema2::Update(float deltaTimeSeconds) {
    // disabling the cursor by default, so the user does not have to hold
    // mouse right button to rotate the camera
    window->DisablePointer();

    hudTimer += deltaTimeSeconds;

    // if the player finds the exit stop and print the winning message
    if (player->posX >= (SIZE * 3 - 1) * WALLSIZE ||
        player->posY >= (SIZE * 3 - 1) * WALLSIZE ||
        player->posX <= (1) * WALLSIZE ||
        player->posY <= (1) * WALLSIZE) {
        cout << "You won!\n";
        exit(EXIT_SUCCESS);
    }
    // every second decreases the timer
    // if the timer reaches 0, then the game is over
    if (hudTimer >= 1.f) {
        hud.timeBarCurTime -= 0.01f;
        if (hud.timeBarCurTime <= 0) {
            cout << "Time's up! You Lost!\n";
            exit(EXIT_SUCCESS);
        }
        hudTimer = 0;
    }

    // setting the player position depending on the camera
    glm::vec3 targetPosition = camera->GetTargetPosition();
    if (camera->person == PERSON::THIRD) {
        player->posY = targetPosition[2];
        player->posX = targetPosition[0];
    } else {
        player->posY = camera->position[2];
        player->posX = camera->position[0];
    }

    // updating the projectiles and checking the collision with walls or enemies
    // the projectiles have a certain amount of time to exist then they are deleted
    // if a bullet touches a wall or an enemy, it is removed
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].update(deltaTimeSeconds);
        int collisionEnemy = projectiles[i].enemyCollision(maze.enemies);
        if (collisionEnemy != -1) {
            projectiles.erase(projectiles.begin() + i);
            i--;
            maze.enemies[collisionEnemy].hit = true;

            continue;
        }
        if (projectiles[i].timeActual >= projectiles[i].timeMax || projectiles[i].wallCollision(maze.walls)) {
            projectiles.erase(projectiles.begin() + i);
            i--;
        }
    }

    // check collision with enemies every second, so the health bar does not
    // imediatelly drops to 0
    if (PlayerEnemyCollision() && !player->touched) {
        player->touched = true;
        hud.healthBar -= 0.2;
    }

    if (player->touched) {
        player->timeTouched += deltaTimeSeconds;
    }

    if (player->timeTouched >= 1.f) {
        player->touched = false;
        player->timeTouched = 0;
    }

    // if the health bar reached 0, then the game is lost
    if (hud.healthBar <= 0) {
        cout << "You lost!\n";
        exit(EXIT_SUCCESS);
    }

    // creating the animation for the oponents
    for (int i = 0; i < maze.enemies.size(); i++) {
        maze.enemies[i].time += deltaTimeSeconds;
        if (maze.enemies[i].hit) {
            maze.enemies[i].timeElapsed += deltaTimeSeconds;
            if (maze.enemies[i].timeElapsed >= 1.f) {
                maze.enemies.erase(maze.enemies.begin() + i);
                i--;
                continue;
            }
        }
        /**
         *  for each interval the enemy executes some sort of a translation
         * up - down : 0 - 4
         * left - right : 4 - 8
         * front - back : 8 - 12
         */
        if (maze.enemies[i].time < 1.0f) {
            maze.enemies[i].update(0, deltaTimeSeconds, 0);
        } else if (maze.enemies[i].time < 3.0f) {
            maze.enemies[i].update(0, -deltaTimeSeconds, 0);
        } else if (maze.enemies[i].time < 4.0f) {
            maze.enemies[i].update(0, deltaTimeSeconds, 0);
        } else if (maze.enemies[i].time < 5.0f) {
            maze.enemies[i].update(deltaTimeSeconds, 0, 0);
        } else if (maze.enemies[i].time < 7.0f) {
            maze.enemies[i].update(-deltaTimeSeconds, 0, 0);
        } else if (maze.enemies[i].time < 8.0f) {
            maze.enemies[i].update(deltaTimeSeconds, 0, 0);
        } else if (maze.enemies[i].time < 9.0f) {
            maze.enemies[i].update(0, 0, deltaTimeSeconds);
        } else if (maze.enemies[i].time < 11.0f) {
            maze.enemies[i].update(0, 0, -deltaTimeSeconds);
        } else if (maze.enemies[i].time < 12.0f) {
            maze.enemies[i].update(0, 0, deltaTimeSeconds);
        } else
            maze.enemies[i].time = 0.f;
    }
    // draw all the elements
    Draw(deltaTimeSeconds);
}

void Tema2::FrameEnd() {
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

// function to render a mesh (got it from previous labs)
void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shader->program, "v_color"), 1, glm::value_ptr(color));
    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    float cameraSpeed = 10.0f;

    /**
     * the movement of the player using WASD
     * each time we press one of these keys we check if that movement results
     * in a collision
     * If it does, then return the player and the camera to the previous pozition,
     * before the collision happend
     * It's buggy sometimes, the player gets stuck in the wall
     * but it can get out by rotating and presing other keys
     */

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(deltaTime * cameraSpeed);
        glm::vec3 targetPosition = camera->GetTargetPosition();
        if (camera->person == PERSON::THIRD) {
            player->posY = targetPosition[2];
            player->posX = targetPosition[0];
        } else {
            player->posY = camera->position[2];
            player->posX = camera->position[0];
        }
        if (player->wallsCollision(maze.walls, 0, deltaTime * cameraSpeed)) {
            camera->MoveForward(-deltaTime * cameraSpeed);
            targetPosition = camera->GetTargetPosition();
            if (camera->person == PERSON::THIRD) {
                player->posY = targetPosition[2];
                player->posX = targetPosition[0];
            } else {
                player->posY = camera->position[2];
                player->posX = camera->position[0];
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->TranslateRight(-deltaTime * cameraSpeed);
        glm::vec3 targetPosition = camera->GetTargetPosition();
        if (camera->person == PERSON::THIRD) {
            player->posY = targetPosition[2];
            player->posX = targetPosition[0];
        } else {
            player->posY = camera->position[2];
            player->posX = camera->position[0];
        }
        if (player->wallsCollision(maze.walls, -deltaTime * cameraSpeed, 0)) {
            camera->TranslateRight(deltaTime * cameraSpeed);
            targetPosition = camera->GetTargetPosition();
            if (camera->person == PERSON::THIRD) {
                player->posY = targetPosition[2];
                player->posX = targetPosition[0];
            } else {
                player->posY = camera->position[2];
                player->posX = camera->position[0];
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-deltaTime * cameraSpeed);
        glm::vec3 targetPosition = camera->GetTargetPosition();
        if (camera->person == PERSON::THIRD) {
            player->posY = targetPosition[2];
            player->posX = targetPosition[0];
        } else {
            player->posY = camera->position[2];
            player->posX = camera->position[0];
        }

        if (player->wallsCollision(maze.walls, 0, -deltaTime * cameraSpeed)) {
            camera->MoveForward(deltaTime * cameraSpeed);
            targetPosition = camera->GetTargetPosition();
            if (camera->person == PERSON::THIRD) {
                player->posY = targetPosition[2];
                player->posX = targetPosition[0];
            } else {
                player->posY = camera->position[2];
                player->posX = camera->position[0];
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->TranslateRight(deltaTime * cameraSpeed);
        glm::vec3 targetPosition = camera->GetTargetPosition();
        if (camera->person == PERSON::THIRD) {
            player->posY = targetPosition[2];
            player->posX = targetPosition[0];
        } else {
            player->posY = camera->position[2];
            player->posX = camera->position[0];
        }
        if (player->wallsCollision(maze.walls, deltaTime * cameraSpeed, 0)) {
            camera->TranslateRight(-deltaTime * cameraSpeed);
            targetPosition = camera->GetTargetPosition();
            if (camera->person == PERSON::THIRD) {
                player->posY = targetPosition[2];
                player->posX = targetPosition[0];
            } else {
                player->posY = camera->position[2];
                player->posX = camera->position[0];
            }
        }
    }
}

void Tema2::OnKeyPress(int key, int mods) {
    // Pressing the CTRL key changes the camera person
    if (mods == GLFW_MOD_CONTROL) {
        if (camera->person == PERSON::THIRD) {
            camera->TranslateForward(camera->distanceToTarget);
            camera->person = PERSON::FIRST;
        } else {
            camera->TranslateForward(-camera->distanceToTarget);
            camera->person = PERSON::THIRD;
        }
    }
    // Pressing the O keys hides the walls to find the exit quicker
    if (key == GLFW_KEY_O) {
        hide = !hide;
    }
}

void Tema2::OnKeyRelease(int key, int mods) {
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    // rotation of the camera, player and hud
    if (camera->person == PERSON::FIRST) {
        camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
        camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        player->rotation += sensivityOY * -deltaX;
        hud.hudRotationY += sensivityOY * -deltaX;
        hud.hudRotationX += sensivityOX * -deltaY;
    }

    if (camera->person == PERSON::THIRD) {
        camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
        camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        player->rotation += sensivityOY * -deltaX;
        hud.hudRotationY += sensivityOY * -deltaX;
        hud.hudRotationX += sensivityOX * -deltaY;
    }
}

// pressing the mouse button launches a projectile
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (button == 1) {
        glm::vec3 normDir = glm::normalize(camera->forward);

        camera->person = PERSON::FIRST;
        Projectile2 proj(camera->position.x,
                         camera->position.y,
                         camera->position.z,
                         normDir.x,
                         normDir.y,
                         normDir.z,
                         10.f);
        projectiles.push_back(proj);
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}