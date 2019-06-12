#include "pch.hpp"

#include "WindowManager.hpp"
#include "Internals/Utilities/Log.hpp"
#include "Internals/AppCallbacks.hpp"
#include "Graphics/Renderer.hpp"

//#include <chrono>
#include <time.h>
#include "Application.hpp"

namespace Coel {
	namespace Application {
		//std::chrono::high_resolution_clock::time_point timeElapsed, startTime;
		constexpr static const double TIME_PER_TICK = 1000.f / TICK_RATE;
		static unsigned int totalTickCount = 0, millisElapsed = 0;
		static bool s_shouldRun = true;
		static unsigned int init()
		{
			LOG_INFO(Application, "Initializing application...\n");
			LOG_INFO(Application, "Initializing window manager...\n");
			if (!WindowManager::init()) {
				LOG_ERROR(Application, "Window manager failed to initialize\n");
				return 0;
			}
			LOG_SUCCESS(Application, "Initialized window manager\n");
			LOG_INFO(Application, "Initializing Renderer...\n");
			if (!Graphics::Renderer::init()) {
				LOG_ERROR(Application, "Renderer failed to initialize\n");
				return 0;
			}
			LOG_SUCCESS(Application, "Initialized Renderer\n");
			LOG_SUCCESS(Application, "Initialized application\n");
			return 1;
		}
		void start()
		{
			if (init()) {
				LOG_INFO(Application, "Starting application...\n");
				Internals::onStartCallback();
				LOG_SUCCESS(Application, "Started application\n");
				reset();
				while (WindowManager::shouldRun() && s_shouldRun) {
					WindowManager::update();
					Graphics::Renderer::clear();
					Graphics::Renderer::begin();
					Internals::onUpdateCallback();
					Graphics::Renderer::end();
					Graphics::Renderer::flush();
					//timeElapsed = std::chrono::high_resolution_clock::now();
					//std::chrono::duration<double> timeSinceStart = timeElapsed - startTime;
					//if (timeSinceStart.count() > SECONDS_PER_TICK + totalTickCount * SECONDS_PER_TICK) {
					millisElapsed = unsigned int(clock() - TIME_PER_TICK * totalTickCount);
					if (millisElapsed > TIME_PER_TICK) {
						Internals::onTickCallback();
						totalTickCount++;
					}
				}
				LOG_INFO(Application, "Closing application...\n");
				Internals::onCloseCallback();
				WindowManager::shutdown();
				LOG_SUCCESS(Application, "Closed application\n");
			}
		}
		void reset()
		{
			LOG_INFO(Application, "Resetting application...\n");
			totalTickCount = unsigned int((double)clock() / TIME_PER_TICK);
			millisElapsed = 0;
			//startTime = std::chrono::high_resolution_clock::now();
			//timeElapsed = std::chrono::high_resolution_clock::now();
			Internals::onResetCallback();
			LOG_SUCCESS(Application, "Reset application\n");
		}
		void close()
		{
			s_shouldRun = false;
		}
		void maximize()
		{
			WindowManager::maximize();
		}
		void minimize()
		{
			WindowManager::minimize();
		}
	} // namespace Application
	void setOnStartCallback(void (*func)()) { Internals::onStartCallback = func; }
	void setOnResetCallback(void (*func)()) { Internals::onResetCallback = func; }
	void setOnCloseCallback(void (*func)()) { Internals::onCloseCallback = func; }
	void setOnUpdateCallback(void (*func)()) { Internals::onUpdateCallback = func; }
	void setOnTickCallback(void (*func)()) { Internals::onTickCallback = func; }
} // namespace Coel
