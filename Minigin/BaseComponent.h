#pragma once
#include <memory>

namespace dae
{
	class GameObject;
}

class BaseComponent
{
public:
    BaseComponent(const std::shared_ptr<dae::GameObject>& gameObject);
    virtual ~BaseComponent() = default;

    virtual void Update() {}
    virtual void Render() const {}

protected:
    std::shared_ptr<dae::GameObject> GetGameObject() const { return std::shared_ptr<dae::GameObject>(m_GameObject); }

private:
    std::weak_ptr<dae::GameObject> m_GameObject;
};