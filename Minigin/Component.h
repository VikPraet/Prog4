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
    virtual ~Component() = default;

    virtual void Update() {}
    virtual void Render() const {}

    virtual void SetPosition(float x, float y, float z);

protected:
    Component() = default;

    dae::Transform m_transform{};
};