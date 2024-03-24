#pragma once
#include "Events.h"

namespace dae
{
	enum Event
	{
		Initialize,
		OnHealthChange,
		OnScoreChange,
	};

	class Subject;
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		virtual void Notify(const Event& event, Subject* pSubject) = 0;
	};
}
