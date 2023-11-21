#include <iostream>
#include <random>
#include "glm/glm.hpp"

float getRandomValue(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gen);
}

int getLineFromTurretNameString(std::string& turretName)
{
    return turretName[13] - '0';
}

int getNumberOfPointsToRemove(int draggedTurretIndex)
{
    switch (draggedTurretIndex)
    {
    case -1:
		return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 2;
    case 4:
        return 3;

    }
}

bool checkIfGlmVectorAreEqual(glm::vec3 v1, glm::vec3 v2)
{   
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}