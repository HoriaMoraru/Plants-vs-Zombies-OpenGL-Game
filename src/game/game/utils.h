#pragma once
#include "utils/gl_utils.h"

#define SHOULD_NOT_DISSAPPEAR -9999.0f


typedef struct CollisionCircle {
	glm::vec3 center;
	float radius;

	CollisionCircle() : center(glm::vec3(0, 0, 0)), radius(0.0f) {}

	CollisionCircle(glm::vec3 center, float radius)
		: center(center), radius(radius) {
	}

	bool isColliding(CollisionCircle& other) {
		float distance = glm::distance(center, other.center);
		return distance <= radius + other.radius;
	}
};


typedef struct Enemy {
	Mesh* inner;
	Mesh* outer;
	int line;
	int hp = 3;
	glm::mat3 modelMatrix;
	glm::vec3 center;
	CollisionCircle collisionCircle;
	float dissappearingTime = SHOULD_NOT_DISSAPPEAR;
	float scale = 1.0f;
	glm::vec3 color;

	Enemy(Mesh* innerMesh, Mesh* outerMesh, int line, glm::mat3& matrix, glm::vec3 center, glm::vec3 color, CollisionCircle collisionCircle)
		: inner(innerMesh), outer(outerMesh), line(line), modelMatrix(matrix), center(center), color(color), collisionCircle(collisionCircle) {
	}

	void startDissappearing() {
		dissappearingTime = 1.0f;
	}

	bool isDissappearing() {
		return dissappearingTime > SHOULD_NOT_DISSAPPEAR && dissappearingTime <= 1.0f;
	}
} Enemy;


typedef struct Bullet {
	Mesh* mesh;
	glm::mat3 modelMatrix;
	glm::vec3 center;
	CollisionCircle collisionCircle;
	glm::vec3 color;

	Bullet(Mesh* mesh, glm::mat3& matrix, glm::vec3 center, glm::vec3 color, CollisionCircle collisionCircle)
		: mesh(mesh), modelMatrix(matrix), center(center), color(color), collisionCircle(collisionCircle) {
	}
} Bullet;


typedef struct Turret {
	Mesh* mesh;
	glm::mat3 modelMatrix;
	std::string name;
	float shootingTimeInterval;
	glm::vec3 center;
	float dissappearingTime = SHOULD_NOT_DISSAPPEAR;
	float scale = 1.0f;
	glm::vec3 color;
	CollisionCircle collisionCircle;

	Turret(Mesh* turretMesh, glm::mat3& matrix, std::string name, glm::vec3 center, glm::vec3 color, CollisionCircle collisionCircle)
		: mesh(turretMesh), modelMatrix(matrix), name(name), center(center), color(color), collisionCircle(collisionCircle) {
		shootingTimeInterval = 1.5f;
	}

	Turret(Mesh* turretMesh, glm::mat3& matrix, std::string name, glm::vec3 center, glm::vec3 color)
		: mesh(turretMesh), modelMatrix(matrix), name(name), center(center), color(color) {
		shootingTimeInterval = 1.5f;
	}



	// NullTurret constructor
	Turret() : mesh(nullptr), modelMatrix(glm::mat3(0)), name("null"), center(glm::vec3(0, 0, 0)) {
	}

	// Function to check if the turret is null
	bool isNullTurret() const {
		return mesh == nullptr;
	}

	void startDissappearing() {
		dissappearingTime = 1.0f;
	}

	bool isDissappearing() {
		return dissappearingTime > SHOULD_NOT_DISSAPPEAR && dissappearingTime <= 1.0f;
	}
} Turret;


typedef struct SquareInstance {
	Mesh* mesh;
	float length;
	glm::vec3 bottomLeftCorner;
	glm::vec3 center;
	bool isOccupied = false;

	SquareInstance(Mesh* mesh, float length, glm::vec3 bottomLeftCorner, glm::vec3 center)
		: mesh(mesh), length(length), bottomLeftCorner(bottomLeftCorner), center(center) {
	}

	std::pair<int, int> getSquareLineAndColumn()
	{
		glm::vec3 blc = bottomLeftCorner;
		float squareSize = length;
		int line = (blc.y - 40.0f) / (squareSize + 20.0f);
		int column = (blc.x - 100.0f) / (squareSize + 20.0f);
		return std::make_pair(line, column);
	}

	bool CheckMouseInBoundingBox(int mouseX, int mouseY)
	{
		//Get the position and size of the bounding box of the mesh
		float squareSize = length;
		glm::vec3 blc = bottomLeftCorner;

		//Check if the mouse is within the bounding box
		return (mouseX >= blc.x && mouseX <= blc.x + squareSize &&
			mouseY >= blc.y && mouseY <= blc.y + squareSize);
	}
};


typedef struct Life {
	Mesh* mesh;
	glm::vec3 center;

	Life(Mesh* mesh, glm::vec3 center)
		: mesh(mesh), center(center) {
	}
};


typedef struct Point {
	Mesh* mesh;
	glm::vec3 center;

	Point(Mesh* mesh, glm::vec3 center)
		: mesh(mesh), center(center) {
	}

	bool CheckMouseInBoundingBox(int mouseX, int mouseY)
	{
		//Check if the mouse is within the bounding box
		return (mouseX >= center.x - 30.0f / 2 && mouseX <= center.x + 30.0f / 2 &&
			mouseY >= center.y - 15.0f / 2 && mouseY <= center.y + 15.0f / 2);
	}
};


float getRandomValue(float min, float max);
int getLineFromTurretNameString(std::string& turretName);
int getNumberOfPointsToRemove(int draggedTurretIndex);
bool checkIfGlmVectorAreEqual(glm::vec3 v1, glm::vec3 v2);