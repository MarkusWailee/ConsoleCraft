#pragma once
#include <chrono>
#include <iostream>


class DeltaTime
{
	std::chrono::high_resolution_clock timer;
	using ms = std::chrono::duration<float, std::milli>;
public:
	static void HandleTime() { Get().HandleTime_i(); }
	static void ShowFPS() { Get().ShowFPS_i(); }
	static float GetFrameTime() { return Get().deltaTime; }
	static void SetTargetFPS(float FPS_TARGET) { Get().SetTargetFPS_i(FPS_TARGET); }
private:
	float dt_target = 0;
	float deltaTime = 0;
	float FPS = 0;
	float AVG_FPS = 0;
	void HandleTime_i();
	void ShowFPS_i();
	void SetTargetFPS_i(float FPS_TARGET);
	DeltaTime() {};
	DeltaTime(const DeltaTime&) = delete;
	static DeltaTime& Get() { static DeltaTime instance;  return instance; }
};

inline void DeltaTime::HandleTime_i()
{
	static auto start = timer.now();
	while (std::chrono::duration_cast<ms>(timer.now() - start).count() < dt_target) {}
	deltaTime = std::chrono::duration_cast<ms>(timer.now() - start).count() * 0.001;
	start = timer.now();
}
inline void DeltaTime::ShowFPS_i()
{
	static int fps_counter = 0;
	fps_counter++;
	FPS += 1 / deltaTime;
	if (fps_counter >= 100)
	{
		//std::cout << FPS / float(100) << '\n';
		AVG_FPS = FPS / float(100);
		FPS = 0;
		fps_counter = 0;
	}
	std::cout << AVG_FPS << '\n';
}
inline void DeltaTime::SetTargetFPS_i(float FPS_TARGET)
{
	if (FPS_TARGET == 0)
	{
		dt_target = 0;
		return;
	}
	dt_target = 1000 / FPS_TARGET;
}