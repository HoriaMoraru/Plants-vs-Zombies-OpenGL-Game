#include "lab_m1/Tema1/objects2D.h"
#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/transforms2D.h"
#include "lab_m1/Tema1/utils.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    resolution = window->GetResolution();
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, static_cast<float>(resolution.x), 0, static_cast<float>(resolution.y), 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Create the red enemy zone and add it to the list.
    Mesh* enemy_zone = objects2D::CreateRectangle("enemy_zone", origin, (float)resolution.x / 20, 2 * distanceBetweenSquares + 3 * squareSize, glm::vec3(1, 0, 0), true);
    AddMeshToList(enemy_zone);

    //Create the squares for placing the turrets as a 3x3 GRID.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::string squareName = "square_slot" + std::to_string(i) + std::to_string(j);
            Mesh* square = objects2D::CreateSquare(squareName, origin, squareSize, glm::vec3(0, 1, 0), true);
            glm::vec3 squareBlc = initialSquareGridPosition + glm::vec3((squareSize + distanceBetweenSquares) * j, (squareSize + distanceBetweenSquares) * i, 0.0f);
            glm::vec3 squareCenter = squareBlc + glm::vec3(squareSize / 2, squareSize / 2, 0.0f);
            turretSquaresShoot.push_back(SquareInstance(square, squareSize, squareBlc, squareCenter));
        }
    }

    for (int k = 0; k < 4; ++k) {
        std::string squareName = "square_line" + std::to_string(k + 1);
        std::string turretName = "turret" + std::to_string(k + 1);
        std::string lifeName = "life" + std::to_string(k + 1);

        glm::vec3 squareBlc = glm::vec3(lineStartX + (squareSize + distanceBetweenSquares * 2) * k, lineY, 0.0f);
        glm::vec3 squareCenter = squareBlc + glm::vec3(squareSize / 3, squareSize / 2, 0.0f);
        glm::vec3 turretCenter = squareCenter + glm::vec3(0, 0, 1.0f); // Add the turret above the square
        glm::vec3 circleCenter = glm::vec3(circleStartX + (circleDiameter + distanceBetweenSquares * 2) * k, lineY, 0.0f) + glm::vec3(0, squareSize / 2, 0.0f);

        // Create the placing square and add it to the list
        Mesh* square = objects2D::CreateSquare(squareName, origin, squareSize, glm::vec3(0, 0, 0), false);
        turretSquaresSpawn.push_back(SquareInstance(square, squareSize, squareBlc, squareCenter));

        // Create the turret and add it to the list
        Mesh* turret = objects2D::CreateTurret(turretName, origin + glm::vec3(0, 0, 1), 25.0f, 50.0f, 90.0f, 90.0f, turretsColors[k]);
        turrets.push_back(Turret(turret, glm::mat3(1) * transforms2D::Translate(squareCenter.x, squareCenter.y), turretName, turretCenter, turretsColors[k],
            CollisionCircle(turretCenter, 50.0f + 90.0f)));

        if (k < 3)
        {
            Mesh* life = objects2D::CreateCircle(lifeName, origin, circleDiameter / 2, glm::vec3(1, 0, 0), 100);
            lives.push_back(Life(life, circleCenter));
        }

        // Create stars below each square
        int numStars = k + 1; // Adjust the number of stars for each square
        if (k > 1)
        {
            numStars--;
        }

        for (int s = 0; s < numStars; ++s) {
            std::string turretPointName = "turret_point" + std::to_string(k) + std::to_string(s);
            // Calculate the position for each star below the square
            glm::vec3 turretPointCenter = glm::vec3(squareBlc.x + starDistance * s + 20.0f, squareBlc.y - 20.0f, 0.0f);

            Mesh* turretPoint = objects2D::CreateStar(turretPointName, origin, 30.0f, 15.0f * 3 / 2, glm::vec3(1.0f, 1.0f, 0.0f));
            turretRequiredPoints.push_back(Point(turretPoint, turretPointCenter));
        }
    }

    for (int p = 0; p < startPoints; ++p)
    {
        std::string playerPointName = "player_point";
        glm::vec3 playerPointCenter = glm::vec3(circleStartX - circleDiameter / 2 + (30.0f + distanceBetweenPlayerPoints) * p, lineY, 0.0f);

        Mesh* playerPoint = objects2D::CreateStar(playerPointName, origin, 30.0f, 15.0f * 3 / 2, glm::vec3(1.0f, 1.0f, 0.0f));
        playerPoints.push_back(Point(playerPoint, playerPointCenter));
    }
}



void Tema1::FrameStart()
{
    ClearScreen(glm::vec3(0.3f, 0.3f, 0.3f));
}

void Tema1::Update(float deltaTimeSeconds)
{
    randomSpawnTime -= deltaTimeSeconds;
    pointsSpawnTime -= deltaTimeSeconds;
    rotate += deltaTimeSeconds * 1.0f;
    translate = 50 * deltaTimeSeconds;
    glm::vec3 targetPosition = glm::vec3(10.0f, 40.0f, 0.0f);

    //Enemy_zone
    RenderMesh2D(meshes["enemy_zone"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(targetPosition.x, targetPosition.y));

    //Turret placing spots grid
    for (auto const& tss : turretSquaresShoot) {
        RenderMesh2D(tss.mesh, shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(tss.bottomLeftCorner.x, tss.bottomLeftCorner.y));
    }

    //Turret placing spots & turrets & lives
    for (int i = 0; i < 4; ++i) {
        RenderMesh2D(turretSquaresSpawn[i].mesh, shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(turretSquaresSpawn[i].bottomLeftCorner.x, turretSquaresSpawn[i].bottomLeftCorner.y));
        RenderMesh2D(turrets[i].mesh, shaders["VertexColor"], turrets[i].modelMatrix);
        if (i < lives.size())
        {
            RenderMesh2D(lives[i].mesh, shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(lives[i].center.x, lives[i].center.y));
        }
    }

    //Turret required points
    for (auto const& trp : turretRequiredPoints) {
        RenderMesh2D(trp.mesh, shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(trp.center.x, trp.center.y));
    }

    for (auto const& pp : playerPoints) {
        RenderMesh2D(pp.mesh, shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(pp.center.x, pp.center.y));
    }

    if (randomSpawnTime <= 0 && enemies.size() < maxEnemies) {

        int randomEnemy = (int)getRandomValue(0, 4);
        int randomPosition = (int)getRandomValue(0, 3);
        std::string enemyNameInner = "enemy_inner" + std::to_string(randomPosition);
        std::string enemyNameOuter = "enemy_outer" + std::to_string(randomPosition);
        nextEnemySpawnPosition = enemySpawnPositions[randomPosition];

        Mesh* enemyInner = objects2D::CreateHexagon(enemyNameInner, origin + (0, 0, 2.0f), 20.0f, enemyColors[randomEnemy].first);
        Mesh* enemyOuter = objects2D::CreateHexagon(enemyNameOuter, origin + (0, 0, 2.0f), 40.0f, enemyColors[randomEnemy].second);
        Enemy newEnemy = Enemy(enemyInner, enemyOuter, randomPosition, glm::mat3(1) * transforms2D::Translate(nextEnemySpawnPosition.x, nextEnemySpawnPosition.y), nextEnemySpawnPosition, enemyColors[randomEnemy].second,
            CollisionCircle(nextEnemySpawnPosition, 40.0f));
        trackEnemyColors[randomPosition].first.push_back(enemyColors[randomEnemy].second);
        trackEnemyColors[randomPosition].second++;

        enemies.push_back(newEnemy);

        randomSpawnTime = getRandomValue(4.0f, 5.5f);
    }

    if (pointsSpawnTime <= 0)
    {
        std::string pointName = "player_point_to_pick";
        glm::vec3 pointCenter = glm::vec3(getRandomValue(600, 1100), getRandomValue(20, 400), 0.0f);

        for (int i = 0; i < nrOfPointsToSpawn; ++i) {
            Mesh* playerPoint = objects2D::CreateStar(pointName, origin + glm::vec3(0, 0, 4.0f), 30.0f, 15.0f * 3 / 2, glm::vec3(1.0f, 1.0f, 0.0f));
            playerPointsToPick.push_back(Point(playerPoint, pointCenter));
            pointCenter = glm::vec3(getRandomValue(600, 1100), getRandomValue(20, 400), 0.0f);
        }

        nrOfPointsToSpawn = (int)getRandomValue(1, 4);
        pointsSpawnTime = getRandomValue(8.0f, 12.0f);
    }

    for (auto const& p : playerPointsToPick) {
        RenderMesh2D(p.mesh, shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(p.center.x, p.center.y));
    }

    for (auto& it = enemies.begin(); it != enemies.end(); ) {
        Enemy& enemy = *it;

        if (enemy.isDissappearing())
        {
            enemy.modelMatrix = glm::mat3(1) * transforms2D::Translate(enemy.center.x, enemy.center.y) * transforms2D::Scale(enemy.scale, enemy.scale);
            enemy.scale -= deltaTimeSeconds;
            enemy.dissappearingTime -= deltaTimeSeconds;
            if (enemy.dissappearingTime <= 0)
            {
                it = enemies.erase(it);
                continue;
            }
        }

        RenderMesh2D(enemy.inner, shaders["VertexColor"], enemy.modelMatrix);
        RenderMesh2D(enemy.outer, shaders["VertexColor"], enemy.modelMatrix);
        enemy.modelMatrix = glm::mat3(1) * transforms2D::Translate(enemy.center.x, enemy.center.y);
        enemy.center.x -= translate;
        enemy.collisionCircle.center.x -= translate;

        if (enemy.center.x <= origin.x + 10.0f) {
            it = enemies.erase(it);
            trackEnemyColors[enemy.line].second--;
            if (!lives.empty()) {
                lives.pop_back();
            }
        }
        else {
            ++it;
        }
    }

    if (!draggedTurret.isNullTurret()) {
        glm::ivec2 cursorPosition = window->GetCursorPosition();
        draggedTurret.modelMatrix = glm::mat3(1) * transforms2D::Translate(cursorPosition.x, resolution.y - cursorPosition.y);
        RenderMesh2D(draggedTurret.mesh, shaders["VertexColor"], draggedTurret.modelMatrix);
    }

    for (auto& it = placedTurrets.begin(); it != placedTurrets.end(); ) {
        Turret& turret = *it;
        for (auto& it2 = enemies.begin(); it2 != enemies.end(); ++it2) {
            Enemy& enemy = *it2;
            if (enemy.collisionCircle.isColliding(turret.collisionCircle) && !turret.isDissappearing())
            {
                turret.startDissappearing();
                int line = turret.name[13] - '0';
                int column = turret.name[14] - '0';
                turretSquaresShoot[line * 3 + column].isOccupied = false;
                continue;
            }
        }
        if (turret.isDissappearing())
        {
            turret.modelMatrix = glm::mat3(1) * transforms2D::Translate(turret.center.x, turret.center.y) * transforms2D::Scale(turret.scale, turret.scale);
            turret.scale -= deltaTimeSeconds;
            turret.dissappearingTime -= deltaTimeSeconds;
            if (turret.dissappearingTime <= 0)
            {
                it = placedTurrets.erase(it);
                continue;
            }
        }
        RenderMesh2D(turret.mesh, shaders["VertexColor"], turret.modelMatrix);
        int line = getLineFromTurretNameString(turret.name);
        auto color_it = std::find(trackEnemyColors[line].first.begin(), trackEnemyColors[line].first.end(), turret.color);
        if (trackEnemyColors[line].second && color_it != trackEnemyColors[line].first.end())
        {
            if (turret.shootingTimeInterval <= 0 && !turret.isDissappearing())
            {
                turret.shootingTimeInterval = shootingInterval;
                glm::vec3 bulletCenter = turret.center + glm::vec3(105.0f, 0, 2.0f);
                Bullet newBullet = Bullet(objects2D::CreateStar("bullet", origin + glm::vec3(0, 0, 3.0f), 60.0f, 30.0f * 3 / 2, turret.color),
                    glm::mat3(1) * transforms2D::Translate(bulletCenter.x, bulletCenter.y),
                    bulletCenter, turret.color, CollisionCircle(bulletCenter, 60.0f / 2));
                bulletsShot.push_back(newBullet);
            }
            else
            {
                turret.shootingTimeInterval -= deltaTimeSeconds;
            }
        }
        ++it;
    }

    for (auto& it2 = bulletsShot.begin(); it2 != bulletsShot.end(); ) {
        int continueFlag = 0;
        Bullet& bullet = *it2;
        bullet.modelMatrix = glm::mat3(1) * transforms2D::Translate(bullet.center.x, bullet.center.y) * transforms2D::Rotate(rotate);
        bullet.center.x += translate;
        bullet.collisionCircle.center.x += translate;
        for (auto& it = enemies.begin(); it != enemies.end(); ) {
            Enemy& enemy = *it;
            if (enemy.collisionCircle.isColliding(bullet.collisionCircle) && checkIfGlmVectorAreEqual(enemy.color, bullet.color))
            {
                it2 = bulletsShot.erase(it2);
                enemy.hp--;
                if (enemy.hp == 0) {
                    enemy.startDissappearing();
                    trackEnemyColors[enemy.line].second--;
                    for (auto& it3 = trackEnemyColors[enemy.line].first.begin(); it3 != trackEnemyColors[enemy.line].first.end(); ++it3)
                    {
                        if (checkIfGlmVectorAreEqual(*it3, enemy.color))
                        {
							it3 = trackEnemyColors[enemy.line].first.erase(it3);
							break;
						}
					}
                }
                continueFlag = 1;
                break;
            }
            ++it;
        }
        if (continueFlag)
        {
            continue;
        }
        ++it2;
        RenderMesh2D(bullet.mesh, shaders["VertexColor"], bullet.modelMatrix);
    }
}


void Tema1::FrameEnd()
{
    if (lives.empty()) {
        ClearScreen(glm::vec3(0, 0, 0));
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = resolution.y - mouseY;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        // Check if the mouse click is within the bounding box of a turret
        int index = 0;
        for (SquareInstance& square : turretSquaresSpawn) {
            if (square.CheckMouseInBoundingBox(mouseX, mouseY)) {
                draggedTurret = Turret(turrets[index].mesh, turrets[index].modelMatrix, turrets[index].name, turrets[index].center, turrets[index].color);
                draggedTurretIndex = index + 1;
                break;
            }
            ++index;
        }
        for (auto it = playerPointsToPick.begin(); it != playerPointsToPick.end(); ++it) {
            auto& pp = *it;
            if (pp.CheckMouseInBoundingBox(mouseX, mouseY) && playerPoints.size() < startPoints) {
                glm::vec3 playerPointCenter = glm::vec3(circleStartX - circleDiameter / 2 + (30.0f + distanceBetweenPlayerPoints) * playerPoints.size(), lineY, 0.0f);
                playerPoints.push_back(Point(it->mesh, playerPointCenter));
                it = playerPointsToPick.erase(it);
                break;
            }
        }
    }
    else if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        for (SquareInstance& square : turretSquaresShoot) {
            if (square.CheckMouseInBoundingBox(mouseX, mouseY)) {
                std::pair<int, int> lineAndColumn = square.getSquareLineAndColumn();
                const std::string turretName = "placed_turret" + std::to_string(lineAndColumn.first) + std::to_string(lineAndColumn.second);
                for (auto it = placedTurrets.begin(); it != placedTurrets.end(); ++it) {
                    auto& turret = *it;
                    if (turret.name == turretName) {
                        turret.startDissappearing();
                        square.isOccupied = false;
                        break;
                    }
                }
                break;
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    mouseY = resolution.y - mouseY;
    int nrPointsToRemove = getNumberOfPointsToRemove(draggedTurretIndex);
    if (!draggedTurret.isNullTurret() && playerPoints.size() >= nrPointsToRemove) {
        // Find the turret that was being dragged
        for (SquareInstance& square : turretSquaresShoot) {
            if (square.CheckMouseInBoundingBox(mouseX, mouseY)) {
                // Update the position of the turret based on the cursor position
                if (!square.isOccupied) {
                    glm::vec3 turretCenter = square.bottomLeftCorner + glm::vec3(squareSize / 3, squareSize / 2, 0.0f);
                    glm::vec3 squareCenter = turretCenter + glm::vec3(squareSize / 6, 0, 0.0f); // Add the turret above the square
                    std::pair<int, int> lineAndColumn = square.getSquareLineAndColumn();
                    const std::string turretName = "placed_turret" + std::to_string(lineAndColumn.first) + std::to_string(lineAndColumn.second);

                    Mesh* placedTurret = objects2D::CreateTurret(turretName, origin + glm::vec3(0, 0, 1.0f), 25.0f, 50.0f, 90.0f, 90.0f, draggedTurret.color);
                    placedTurrets.push_back(Turret(placedTurret, glm::mat3(1) * transforms2D::Translate(turretCenter.x, turretCenter.y), turretName, turretCenter, draggedTurret.color,
                        CollisionCircle(squareCenter, (50.0f + 90.0f) / 2)));

                    for (int i = 0; i < nrPointsToRemove; ++i) {
                        playerPoints.pop_back();
                    }

                    square.isOccupied = true;
                }
                break;
            }
        }

    }
    draggedTurret = Turret(); // null turret
    draggedTurretIndex = -1;
}

void Tema1::OnWindowResize(int width, int height)
{
    resolution = glm::ivec2(width, height);

    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, static_cast<float>(width), 0, static_cast<float>(height), 0.01f, 400);
    camera->Update();

    ClearScreen(glm::vec3(0.3f, 0.3f, 0.3f));
}

