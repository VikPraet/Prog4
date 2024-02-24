#include "GameTime.h"

void GameTime::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

void GameTime::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
	m_lastTime = currentTime;

	// Smooth the delta time using a circular buffer
	m_DeltaTimeBuffer[m_BufferIndex] = m_DeltaTime;
	m_BufferIndex = (m_BufferIndex + 1) % m_DeltaTimeWindow;

	m_SmoothedDeltaTime = 0.0f;
	for (int i = 0; i < m_DeltaTimeWindow; ++i)
	{
		m_SmoothedDeltaTime += m_DeltaTimeBuffer[i];
	}
	m_SmoothedDeltaTime /= m_DeltaTimeWindow;
}
