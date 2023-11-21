#include "utils/glm_utils.h"

namespace transforms2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // Initialize the translation matrix
        glm::mat3 translationMatrix(1.0f);  // Start with an identity matrix

        // Set the translation components in the matrix
        translationMatrix[2][0] = translateX;
        translationMatrix[2][1] = translateY;

        return translationMatrix;
    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // Initialize the scaling matrix
        glm::mat3 scalingMatrix(1.0f);  // Start with an identity matrix

        // Set the scaling components in the matrix
        scalingMatrix[0][0] = scaleX;
        scalingMatrix[1][1] = scaleY;

        return scalingMatrix;
    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // Initialize the rotation matrix
        glm::mat3 rotationMatrix(1.0f);  // Start with an identity matrix

        float cosTheta = cos(radians);
        float sinTheta = sin(radians);

        // Set the rotation components in the matrix
        rotationMatrix[0][0] = cosTheta;
        rotationMatrix[0][1] = -sinTheta;
        rotationMatrix[1][0] = sinTheta;
        rotationMatrix[1][1] = cosTheta;

        return rotationMatrix;
    }
}   // namespace transform2D
