#include "Subject.h"

dae::Subject::Subject(GameObject* gameObject)
	: m_GameObject(gameObject)
{
}

dae::Subject::~Subject()
{
}

void dae::Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
	observer->Notify(Initialize, this);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::ranges::remove(m_Observers, observer).begin(), m_Observers.end());
}

void dae::Subject::MarkForDestroy(Observer* observer)
{
	const auto it{ std::ranges::find(m_Observers, observer) };
	if (it != m_Observers.end())
	{
		m_MarkedForDestroy = true;
		*it = nullptr;
	}
}

void dae::Subject::Invoke(const Event& event)
{
	for (size_t i{}; i < m_Observers.size(); ++i)
	{
		if (const auto Observer{ m_Observers[i] })
			Observer->Notify(event, this);
	}
	if (m_MarkedForDestroy)
		RemoveInvalidObservers();
}

void dae::Subject::Clear()
{
	m_Observers.clear();
}

void dae::Subject::RemoveInvalidObservers()
{
	m_Observers.erase(std::ranges::remove_if(m_Observers, [](const auto& observer)
	{
		return !observer;
	}).begin(), m_Observers.end());

	m_MarkedForDestroy = false;
}
