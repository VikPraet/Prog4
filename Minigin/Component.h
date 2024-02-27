#pragma once
#include <memory>

#include "Transform.h"

namespace dae
{
	class GameObject;
}

class Component
{
public:
    Component(const std::shared_ptr<dae::GameObject>& gameObject);
    virtual ~Component() = default;

    virtual void Update() {}
    virtual void Render() const {}

    virtual void SetPosition(float x, float y, float z);

protected:
    std::shared_ptr<dae::GameObject> GetGameObject() const { return std::shared_ptr<dae::GameObject>(m_GameObject); }

    // todo: make private or make new component for transform
    dae::Transform m_transform{};

private:
    std::weak_ptr<dae::GameObject> m_GameObject;
};