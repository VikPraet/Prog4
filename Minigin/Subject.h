#pragma once
#include "Observer.h"
#include "GameObject.h"
#include <vector>
#include <queue>

namespace dae
{
	class Subject
	{
	public:
		Subject(GameObject* gameObject);
		~Subject();

		void AddObserver(Observer* observer);
		//removes the observer immediately
		void RemoveObserver(Observer* observer);
		//removes the observer when all observers are invoked
		void MarkForDestroy(Observer* observer);
		void Invoke(const Event& event);
		void Clear();

		GameObject* GetParent() const { return m_GameObject; }

	private:
		void RemoveInvalidObservers();

		std::vector<Observer*> m_Observers;
		bool m_MarkedForDestroy{ false };

		GameObject* m_GameObject;
	};
}
