#include "TrashTheCache.h"

#include <algorithm>
#include <chrono>
#include <future>

#include "imgui_plot.h"

void ttc::ShowIntGraphWindow()
{
	ImGui::Begin("Exercise 1");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -6);

	static int numberOfSamples = 10;
	ImGui::InputInt("# Samples", &numberOfSamples);
	if (numberOfSamples < 1) numberOfSamples = 1;

	static GraphData graphData{};
	static bool drawGraph = false;

	// multi threading
	static std::future<GraphData> futureData;
	static bool isCalculating = false;

	if (isCalculating)
	{
		if (futureData.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			graphData = futureData.get();
			isCalculating = false;
		}
		else
		{
			ImGui::Text("Calculating values...");
		}
	}
	else
	{
		if (ImGui::Button("Trash the cache") && !isCalculating)
		{
			drawGraph = true;
			isCalculating = true;
			futureData = std::async(std::launch::async, &ttc::TrashTheCacheInt, numberOfSamples);
		}
	}

	if(drawGraph && !isCalculating)
	{
		constexpr int dataCount = 11;
		float maxTime = 0;

		for (const auto time : graphData.average_times) 
		{
			if (time > maxTime) maxTime = time;
		}

		static float x_data[dataCount];
		static float Y_data[dataCount];

		for (int i{}; i < static_cast<int>(graphData.step_sizes.size()); ++i) 
		{
			x_data[i] = static_cast<float>(graphData.step_sizes[i]);
			Y_data[i] = graphData.average_times[i];
		}

		ImGui::PlotConfig conf;
		conf.values.xs = x_data;
		conf.values.ys = Y_data;
		conf.values.count = dataCount;
		conf.values.color = graphColors[2];
		conf.scale.min = 0;
		conf.scale.max = maxTime;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.0f, y=%.2f";
		conf.grid_x.show = false;
		conf.grid_y.show = true;
		conf.grid_y.size = maxTime / 5.f;
		conf.grid_y.subticks = 5;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 1.5f;
		Plot("plot1", conf);
	}

	ImGui::End();
}

void ttc::ShowGOGraphWindow()
{
	ImGui::Begin("Exercise 2");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -6);

	static int numberOfSamples = 50;
	ImGui::InputInt("# Samples", &numberOfSamples);
	if (numberOfSamples < 1) numberOfSamples = 1;

	// GameObject
	static GraphData graphDataGO{};
	static bool drawGraphGO = false;

	static std::future<GraphData> futureDataGO;
	static bool isCalculatingGo = false;

	if (isCalculatingGo)
	{
		if (futureDataGO.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			graphDataGO = futureDataGO.get();
			isCalculatingGo = false;
		}
		else
		{
			ImGui::Text("Calculating values...");
		}
	}
	else
	{
		if (ImGui::Button("Trash the cache with GameObject3D") && !isCalculatingGo)
		{
			drawGraphGO = true;
			isCalculatingGo = true;
			futureDataGO = std::async(std::launch::async, &ttc::TrashTheCacheGameObject, numberOfSamples);
		}
	}

	if (drawGraphGO && !isCalculatingGo)
	{
		constexpr int dataCount = 11;
		float maxTime = 0;

		for (const auto time : graphDataGO.average_times)
		{
			if (time > maxTime) maxTime = time;
		}

		static float x_data[dataCount];
		static float Y_data[dataCount];

		for (int i{}; i < static_cast<int>(graphDataGO.step_sizes.size()); ++i)
		{
			x_data[i] = static_cast<float>(graphDataGO.step_sizes[i]);
			Y_data[i] = graphDataGO.average_times[i];
		}

		ImGui::PlotConfig conf;
		conf.values.xs = x_data;
		conf.values.ys = Y_data;
		conf.values.count = dataCount;
		conf.values.color = graphColors[0];
		conf.scale.min = 0;
		conf.scale.max = maxTime;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.0f, y=%.2f";
		conf.grid_x.show = false;
		conf.grid_y.show = true;
		conf.grid_y.size = maxTime / 5.f;
		conf.grid_y.subticks = 5;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 1.5f;
		Plot("plot1", conf);
	}

	// GameObjectAlt
	static GraphData graphDataGOAlt{};
	static bool drawGraphGOAlt = false;

	static std::future<GraphData> futureDataGOAlt;
	static bool isCalculatingGoAlt = false;

	if (isCalculatingGoAlt)
	{
		if (futureDataGOAlt.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			graphDataGOAlt = futureDataGOAlt.get();
			isCalculatingGoAlt = false;
		}
		else
		{
			ImGui::Text("Calculating values...");
		}
	}
	else
	{
		if (ImGui::Button("Trash the cache with GameObject3DAlt") && !isCalculatingGoAlt)
		{
			drawGraphGOAlt = true;
			isCalculatingGoAlt = true;
			futureDataGOAlt = std::async(std::launch::async, &ttc::TrashTheCacheGameObjectAlt, numberOfSamples);
		}
	}

	if (drawGraphGOAlt && !isCalculatingGoAlt)
	{
		constexpr int dataCount = 11;
		float maxTime = 0;

		for (const auto time : graphDataGOAlt.average_times)
		{
			if (time > maxTime) maxTime = time;
		}

		static float x_data[dataCount];
		static float Y_data[dataCount];

		for (int i{}; i < static_cast<int>(graphDataGOAlt.step_sizes.size()); ++i)
		{
			x_data[i] = static_cast<float>(graphDataGOAlt.step_sizes[i]);
			Y_data[i] = graphDataGOAlt.average_times[i];
		}

		ImGui::PlotConfig conf;
		conf.values.xs = x_data;
		conf.values.ys = Y_data;
		conf.values.count = dataCount;
		conf.values.color = graphColors[1];
		conf.scale.min = 0;
		conf.scale.max = maxTime;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.0f, y=%.2f";
		conf.grid_x.show = false;
		conf.grid_y.show = true;
		conf.grid_y.size = maxTime / 5.f;
		conf.grid_y.subticks = 5;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 1.5f;
		Plot("plot2", conf);
	}

	if (drawGraphGO && drawGraphGOAlt && !isCalculatingGo && !isCalculatingGoAlt)
	{
		ImGui::Text("Combined:");

		constexpr int dataCount = 11;
		float maxTime = 0;

		// Find the maximum time across both datasets
		for (const auto& time : graphDataGO.average_times)
		{
			if (time > maxTime) maxTime = time;
		}
		for (const auto& time : graphDataGOAlt.average_times)
		{
			if (time > maxTime) maxTime = time;
		}

		static float x_data[dataCount];
		static float Y_dataGO[dataCount];
		static float Y_dataGOAlt[dataCount];

		// Assuming both data series have the same step sizes
		for (int i{}; i < static_cast<int>(graphDataGO.step_sizes.size()); ++i)
		{
			x_data[i] = static_cast<float>(graphDataGO.step_sizes[i]);
			Y_dataGO[i] = graphDataGO.average_times[i];
			Y_dataGOAlt[i] = graphDataGOAlt.average_times[i];
		}

		static const float* y_data_arr[] = { Y_dataGO, Y_dataGOAlt };

		ImGui::PlotConfig conf;
		conf.values.xs = x_data;
		conf.values.ys_list = y_data_arr;
		conf.values.ys_count = 2;
		conf.values.count = dataCount;
		conf.values.colors = graphColors;
		conf.scale.min = 0;
		conf.scale.max = maxTime;
		conf.tooltip.show = false;
		conf.tooltip.format = "x=%.0f, y=%.2f";
		conf.grid_x.show = false;
		conf.grid_y.show = true;
		conf.grid_y.size = maxTime / 5.f;
		conf.grid_y.subticks = 5;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 1.5f;
		Plot("Combined Plot", conf);
	}

	ImGui::End();
}

ttc::GraphData ttc::TrashTheCacheInt(const int samples)
{
	GraphData graphData{};

	constexpr int size = 67108864;
	int* arr = new int[size];
	const int numRuns = samples;

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<long long> times;

		for (int run{}; run < numRuns; ++run)
		{
			for (int i{}; i < size; ++i)
			{
				arr[i] = 1;
			}

			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < size; i += stepSize)
			{
				arr[i] *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			times.push_back(elapsed);
		}

		std::sort(times.begin(), times.end());

		if (times.size() > 2)
		{
			times.erase(times.begin());
			times.pop_back();
		}

		float sum = 0;
		for (const auto t : times)
		{
			sum += t;
		}
		const auto average = sum / static_cast<float>(times.size());

		// Store step size and average time
		graphData.step_sizes.push_back(stepSize);
		graphData.average_times.push_back(average);
	}

	delete[] arr;

	return graphData;
}

ttc::GraphData ttc::TrashTheCacheGameObject(const int samples)
{
	GraphData graphData{};

	constexpr int size = 1000000;
	GameObject3D* arr = new GameObject3D[size];
	const int numRuns = samples;

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<long long> times;

		for (int run{}; run < numRuns; ++run)
		{
			for (int i{}; i < size; ++i)
			{
				arr[i].id = 1;
			}

			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < size; i += stepSize)
			{
				arr[i].id *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			times.push_back(elapsed);
		}

		std::sort(times.begin(), times.end());

		if (times.size() > 2)
		{
			times.erase(times.begin());
			times.pop_back();
		}

		float sum = 0;
		for (const auto t : times)
		{
			sum += t;
		}
		const auto average = sum / static_cast<float>(times.size());

		// Store step size and average time
		graphData.step_sizes.push_back(stepSize);
		graphData.average_times.push_back(average);
	}

	delete[] arr;

	return graphData;
}

ttc::GraphData ttc::TrashTheCacheGameObjectAlt(const int samples)
{
	GraphData graphData{};

	constexpr int size = 1000000;
	GameObject3DAlt* arr = new GameObject3DAlt[size];
	const int numRuns = samples;

	for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<long long> times;

		for (int run{}; run < numRuns; ++run)
		{
			for (int i{}; i < size; ++i)
			{
				arr[i].id = 1;
			}

			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < size; i += stepSize)
			{
				arr[i].id *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			times.push_back(elapsed);
		}

		std::sort(times.begin(), times.end());

		if (times.size() > 2)
		{
			times.erase(times.begin());
			times.pop_back();
		}

		float sum = 0;
		for (const auto t : times)
		{
			sum += t;
		}
		const auto average = sum / static_cast<float>(times.size());

		// Store step size and average time
		graphData.step_sizes.push_back(stepSize);
		graphData.average_times.push_back(average);
	}

	delete[] arr;

	return graphData;
}
