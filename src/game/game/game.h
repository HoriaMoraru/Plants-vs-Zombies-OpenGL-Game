#pragma once

#include "components/simple_scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "game/game/utils.h"
#include <unordered_map>


namespace m1
{
    class game : public gfxc::SimpleScene
    {
    public:
        game();
        ~game();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::ivec2 resolution;
        glm::vec3 origin = glm::vec3(0, 0, 0);
        int maxEnemies = 20;
        float randomSpawnTime = 3.0f;
        float squareSize = 135.0f; // Size of each square
        glm::vec3 enemySpawnPositions[3] = { glm::vec3(1200, 40 + squareSize / 2, 1.0f), glm::vec3(1200, 40 + 20 * 1 + squareSize + squareSize / 2, 1.0f), glm::vec3(1200, 40 + 20 * 2 + squareSize * 2 + squareSize / 2, 1.0f) };
        std::pair<glm::vec3, glm::vec3> enemyColors[4] = { std::make_pair(glm::vec3(0.9f, 0.2f, 0.3f), glm::vec3(1, 0.5f, 0.0f)), std::make_pair(glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.0f, 0.0f, 1.0f)), std::make_pair(glm::vec3(0.7f, 0.6f, 0.3f), glm::vec3(1.0f, 1.0f, 0)), std::make_pair(glm::vec3(0.1f, 0.5f, 0.8f), glm::vec3(0.5f, 0, 0.5f)) };
        glm::vec3 turretsColors[4] = { glm::vec3(1.0f, 0.5f, 0), glm::vec3(0, 0, 1.0f), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.0f, 0.5f) };
        glm::vec3 nextEnemySpawnPosition = glm::vec3(1200, 40 + squareSize / 2, 0);
        std::vector<Enemy> enemies;
        std::vector<SquareInstance> turretSquaresShoot;
        std::vector<SquareInstance> turretSquaresSpawn;
        std::vector<Turret> turrets;
        std::vector<Turret> placedTurrets;
        std::vector<Life> lives;
        std::vector<Point> turretRequiredPoints;
        std::vector<Point> playerPoints;
        std::vector<Point> playerPointsToPick;
        std::vector<Bullet> bulletsShot;
        Turret draggedTurret = Turret(); // null turret
        //int trackEnemyLines[3] = { 0, 0, 0 };
        std::pair<std::vector<glm::vec3>, int> trackEnemyColors[3];
        float shootingInterval = 1.5f;
        int startPoints = 10;
        float pointsSpawnTime = 5.0f;
        int nrOfPointsToSpawn = 2;
        int draggedTurretIndex = -1;
        glm::vec3 initialSquareGridPosition = glm::vec3(100.0f, 40.0f, 0.0f); // Initial position for the grid
        float distanceBetweenSquares = 20.0f; // Distance between squares
        float lineStartX = 10.0f;
        float lineY = 570.0f;
        float circleStartX = 1000.0f;
        float circleDiameter = squareSize / 2;
        float starDistance = 32.5f; // Adjust the distance between stars
        float distanceBetweenPlayerPoints = 3.0f;
        float translate = 0.0f;
        float rotate = 0.0f;
    };
} 
