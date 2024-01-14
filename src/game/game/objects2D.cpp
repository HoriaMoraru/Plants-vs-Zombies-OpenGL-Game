#include "objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* objects2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* objects2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
	return CreateRectangle(name, leftBottomCorner, length, length, color, fill);
}

Mesh* objects2D::CreateTurret(const std::string& name,
    glm::vec3 center,
    float turretHeight,
    float turretWidth,
    float baseSideLength,
    float baseHeight,
    glm::vec3 color)
{
    float halfBaseSideLength = baseSideLength / 2.0f;
    float halfBaseHeight = baseHeight / 2.0f;
    glm::vec3 turretStart = center + glm::vec3(halfBaseSideLength * 2 / 3,-(halfBaseHeight * 1 / 3), 0);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-halfBaseSideLength, 0, 0), color),
        VertexFormat(center + glm::vec3(0, -halfBaseHeight, 0), color),
        VertexFormat(center + glm::vec3(halfBaseSideLength, 0, 0), color),
        VertexFormat(center + glm::vec3(0, halfBaseHeight, 0), color),
        VertexFormat(turretStart, color),
        VertexFormat(turretStart + glm::vec3(turretWidth, 0, 0), color),
        VertexFormat(turretStart + glm::vec3(turretWidth, turretHeight, 0), color),
        VertexFormat(turretStart + glm::vec3(0, turretHeight, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2, 4, 5, 6, 7, 4, 6 };

    Mesh* turret = new Mesh(name);
    turret->InitFromData(vertices, indices);
    return turret;
}

Mesh* objects2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float sideLength,
    float height,
    glm::vec3 color)
{
    float halfSideLength = sideLength / 2.0f;
    float halfHeight = height / 2.0f;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-halfSideLength, 0, 0), color), // E 0
        VertexFormat(center + glm::vec3(halfSideLength, 0, 0), color), // D 1
        VertexFormat(center + glm::vec3(-halfSideLength / 4, 0, 0), color), // H 2
        VertexFormat(center + glm::vec3(halfSideLength / 4, 0, 0), color), // I 3
        VertexFormat(center + glm::vec3(0, halfHeight, 0), color), // B 4
        VertexFormat(center + glm::vec3(0, -halfHeight / 2, 0), color), // L 5
        VertexFormat(center + glm::vec3(-halfSideLength / 2, -halfHeight, 0), color), // F 6
        VertexFormat(center + glm::vec3(halfSideLength / 2, -halfHeight, 0), color), // G 7
    };

    Mesh* star = new Mesh(name);

    std::vector<unsigned int> indices = {
        4, 2, 3, // Triangle 1 (B, H, I)
        0, 2, 5, // Triangle 2 (E, H, L)
        5, 3, 1, // Triangle 3 (L, I, D)
        3, 5, 7, // Triangle 4 (I, L, G)
        2, 6, 5, // Triangle 5 (H, F, L)
        2, 3, 5, // Triangle 6 (H, I, L)
    };

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* objects2D::CreateHexagon(
    const std::string& name,
    glm::vec3 center,
    float sideLength,
    glm::vec3 color)
{
    float halfSideLength = sideLength / 2.0f;
    float apothem = halfSideLength * 1.8f;

    std::vector<VertexFormat> vertices = {
        VertexFormat(center + glm::vec3(-halfSideLength, apothem, 0), color), // A 0
        VertexFormat(center + glm::vec3(halfSideLength, apothem, 0), color),  // B 1
        VertexFormat(center + glm::vec3(sideLength, 0, 0), color),            // C 2
        VertexFormat(center + glm::vec3(halfSideLength, -apothem, 0), color), // D 3
        VertexFormat(center + glm::vec3(-halfSideLength, -apothem, 0), color),// E 4
        VertexFormat(center + glm::vec3(-sideLength, 0, 0), color)             // F 5
    };

    Mesh* hexagon = new Mesh(name);

    std::vector<unsigned int> indices = {
        0, 1, 2, // Triangle 1 (A, B, C)
        0, 2, 3, // Triangle 2 (A, C, D)
        0, 3, 4, // Triangle 3 (A, D, E)
        0, 4, 5  // Triangle 4 (A, E, F)
    };

    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

Mesh* objects2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments)
{
    std::vector<VertexFormat> vertices;

    // Add center vertex
    vertices.push_back(VertexFormat(center, color));

    // Add vertices around the circle
    for (int i = 0; i <= numSegments; ++i) {
        float theta = static_cast<float>(i) / static_cast<float>(numSegments) * 2.0f * glm::pi<float>();
        float x = center.x + radius * glm::cos(theta);
        float y = center.y + radius * glm::sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
    }

    Mesh* filledCircle = new Mesh(name);
    std::vector<unsigned int> indices;

    // Connect the vertices to form a triangle fan
    for (unsigned int i = 1; i <= numSegments; ++i) {
        indices.push_back(i);
    }

    filledCircle->SetDrawMode(GL_TRIANGLE_FAN);
    filledCircle->InitFromData(vertices, indices);
    return filledCircle;
}

Mesh* objects2D::CreateCircleOutline(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments)
{
    std::vector<VertexFormat> vertices;

    // Add vertices around the circle
    for (int i = 0; i < numSegments; ++i) {
        float theta = static_cast<float>(i) / static_cast<float>(numSegments) * 2.0f * glm::pi<float>();
        float x = center.x + radius * glm::cos(theta);
        float y = center.y + radius * glm::sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
    }

    Mesh* circleOutline = new Mesh(name);
    std::vector<unsigned int> indices;

    // Connect the vertices to form a line strip
    for (unsigned int i = 0; i < numSegments; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    circleOutline->SetDrawMode(GL_LINE_STRIP);
    circleOutline->InitFromData(vertices, indices);
    return circleOutline;
}
