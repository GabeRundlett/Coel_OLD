#pragma once

#include "Events.hpp"

namespace Engine {
	namespace Application {
		unsigned int init();
		void start();
		void reset();
	} // namespace Application
	void setOnStartCallback(void (*func)());
	void setOnResetCallback(void (*func)());
	void setOnCloseCallback(void (*func)());
	void setOnUpdateCallback(void (*func)());
	void setOnTickCallback(void (*func)());
} // namespace Engine
