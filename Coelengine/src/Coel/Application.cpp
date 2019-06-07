#include "pch.hpp"

#include "WindowManager.hpp"
#include "Internals/Utilities/Log.hpp"
#include "Internals/AppCallbacks.hpp"
#include "Graphics/Renderer.hpp"

#include <time.h>
#include "Application.hpp"

namespace Coel {
	namespace Application {
		constexpr static const float MILLIS_PER_TICK = 1000.f / TICK_RATE;
		static float totalTickTime = 0, millisElapsed;
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
				while (WindowManager::shouldRun()) {
					WindowManager::update();
					Graphics::Renderer::clear();
					Graphics::Renderer::begin();
					Internals::onUpdateCallback();
					Graphics::Renderer::end();
					Graphics::Renderer::flush();
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
			LOG_INFO(Application, "Resetting application...\n");
			totalTickTime = (float)clock();
			Internals::onResetCallback();
			LOG_SUCCESS(Application, "Reset application\n");
		}
	} // namespace Application
	void setOnStartCallback(void (*func)()) { Internals::onStartCallback = func; }
	void setOnResetCallback(void (*func)()) { Internals::onResetCallback = func; }
	void setOnCloseCallback(void (*func)()) { Internals::onCloseCallback = func; }
	void setOnUpdateCallback(void (*func)()) { Internals::onUpdateCallback = func; }
	void setOnTickCallback(void (*func)()) { Internals::onTickCallback = func; }
} // namespace Coel
