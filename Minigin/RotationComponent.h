#pragma once
#include "BaseComponent.h"

#include <glm/vec3.hpp>


namespace dae
{
	class TransformComponent;

	class RotationComponent final : public BaseComponent
	{
	public:
		RotationComponent(GameObject* gameObject, float distanceFromTarget, float rotationSpeed = 1.0f, glm::vec3 rotateAroundPoint = { 0,0,0 });
		~RotationComponent() = default;

		void Update() override;

		void SetRotateImage(const bool rotateImage) { m_RotateImage = rotateImage; }

	private:
		float m_RotationSpeed{};
		float m_DistanceFromTarget{};
		glm::vec3 m_RotationPoint{};
		TransformComponent* m_TransformComponent;

		bool m_RotateImage;
	};
}
