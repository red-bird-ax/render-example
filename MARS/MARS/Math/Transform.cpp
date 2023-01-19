#include <Math/Transform.h>

#include <glm/gtc/matrix_transform.hpp>

Matrix Transform::GetModelMatrix() const
{
	Matrix matrix = Matrix{ 1.f };

	matrix = glm::translate(matrix, glm::vec3{ Location.X, Location.Y, Location.Z });
	matrix = glm::rotate(matrix, glm::radians(Rotation.Yaw),   glm::vec3{ 0.f, 0.f, 1.f });
	matrix = glm::rotate(matrix, glm::radians(Rotation.Roll),  glm::vec3{ 1.f, 0.f, 0.f });
	matrix = glm::rotate(matrix, glm::radians(Rotation.Pitch), glm::vec3{ 0.f, 1.f, 0.f });
	matrix = glm::scale(matrix, glm::vec3{ Scale.X, Scale.Y, Scale.Z });

	return matrix;
}
