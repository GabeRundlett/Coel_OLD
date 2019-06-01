#include "pch.hpp"

#include "WindowManager.hpp"
#include "Internals/Utilities/Log.hpp"
#include "Internals/AppCallbacks.hpp"

#include <time.h>

namespace Coel {
	namespace Application {
		constexpr static const unsigned int TICK_RATE = 64;
		constexpr static const float MILLIS_PER_TICK = 1000.f / TICK_RATE;
		static float totalTickTime = 0, millisElapsed;
		static unsigned char shouldOpen;
		unsigned int init()
		{
			LOG_INFO(Application, "Initializing application...\n");
			LOG_INFO(Application, "Initializing window manager...\n");
			if (!WindowManager::init()) {
				LOG_ERROR(Application, "Window manager failed to initialize\n");
				shouldOpen = 0;
				return 0;
			}
			LOG_SUCCESS(Application, "Initialized window manager\n");
			LOG_SUCCESS(Application, "Initialized application\n");
			shouldOpen = 1;
			return 1;
		}
		void start()
		{
			if (shouldOpen) {
				LOG_INFO(Application, "Starting application...\n");
				Internals::onStartCallback();
				LOG_SUCCESS(Application, "Started application\n");
				while (WindowManager::shouldRun()) {
					WindowManager::update();
					Internals::onUpdateCallback();
					millisElapsed = clock() - totalTickTime;
					if (millisElapsed > MILLIS_PER_TICK) {
						totalTickTime += MILLIS_PER_TICK;
						Internals::onTickCallback();
					}
				}
				LOG_INFO(Application, "Closing application...\n");
				Internals::onCloseCallback();
				WindowManager::close();
				LOG_SUCCESS(Application, "Closed application\n");
			}
		}
		void reset()
		{
			if (shouldOpen) {
				LOG_INFO(Application, "Resetting application...\n");
				totalTickTime = (float)clock();
				Internals::onResetCallback();
				LOG_SUCCESS(Application, "Reset application\n");
			}
		}
	} // namespace Application
	void setOnStartCallback(void (*func)()) { Internals::onStartCallback = func; }
	void setOnResetCallback(void (*func)()) { Internals::onResetCallback = func; }
	void setOnCloseCallback(void (*func)()) { Internals::onCloseCallback = func; }
	void setOnUpdateCallback(void (*func)()) { Internals::onUpdateCallback = func; }
	void setOnTickCallback(void (*func)()) { Internals::onTickCallback = func; }
} // namespace Coel
