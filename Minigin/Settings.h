#pragma once
#include <glm/vec2.hpp>

namespace dae
{
	class Settings final
	{
	public:
		static inline char window_title[] = "Programming 4 assignment";

		static inline int window_width = 720;
		static inline int window_height = 570;

		static inline float fixed_time_step = 0.02f;
	};
}
