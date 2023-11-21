#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace objects2D
{

    // Here we create meshes for different 2D objects
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
    Mesh* CreateStar(const std::string& name, glm::vec3 center, float sideLength, float height, glm::vec3 color);
    Mesh* CreateTurret(const std::string& name, glm::vec3 center, float turretHeight, float turretWidth, float baseSideLength, float baseHeight, glm::vec3 color);
    Mesh* CreateHexagon(const std::string& name, glm::vec3 center, float sideLength, glm::vec3 color);
    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, int numSegments);
    Mesh* CreateCircleOutline(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, int numSegments);
}
