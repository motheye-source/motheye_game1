#pragma once
#include <chrono>
#include <optional>

namespace app::engine
{
	class ClockSource
	{
	public:

		float Tick();

	private:

		std::optional<std::chrono::steady_clock::time_point> lastTick_;
	};

	inline float ClockSource::Tick()
	{
		const auto now = std::chrono::steady_clock::now();

		if (!lastTick_.has_value())
		{
			lastTick_ = now;
			return 0.0f;
		}

		const float deltaSeconds = std::chrono::duration<float>(now - lastTick_.value()).count();
		lastTick_ = now;
		return deltaSeconds;
	}
}
