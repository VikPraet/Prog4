#pragma once
#include "imgui.h"
#include <vector>

namespace ttc
{
	struct Transform {
		float matrix[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	};

	class GameObject3D {
	public:
		Transform local;
		int id;
	};

	class GameObject3DAlt {
	public:
		Transform* local;
		int id;
	};

	struct GraphData {
		std::vector<int> step_sizes;
		std::vector<float> average_times;
	};

	static ImU32 graphColors[3] = { ImColor(255, 0, 85),
									ImColor(0, 255, 217),
									ImColor(191, 255, 0) };

	void ShowIntGraphWindow();
	void ShowGOGraphWindow();

	GraphData TrashTheCacheInt(int samples);
	GraphData TrashTheCacheGameObject(int samples);
	GraphData TrashTheCacheGameObjectAlt(int samples);
}
