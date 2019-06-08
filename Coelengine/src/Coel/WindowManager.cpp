#include "pch.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Internals/Utilities/Log.hpp"
#include "Internals/EventCallbacks.hpp"

namespace Coel {
	namespace WindowManager {
		static GLFWwindow *s_window;

		unsigned int init()
		{
			LOG_INFO(WindowManager, "Initializing window manager...\n");
			if (!glfwInit()) {
				LOG_ERROR(WindowManager, "Failed to initialize GLFW\n");
				return 0;
			}
			LOG_INFO(WindowManager, "Creating window...\n");
			s_window = glfwCreateWindow(800, 600, "title", 0, 0);
			if (!s_window) {
				LOG_ERROR(WindowManager, "Failed to create window\n");
				return 0;
			}
			glfwMakeContextCurrent(s_window);
			LOG_SUCCESS(WindowManager, "Created window\n");
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				LOG_ERROR(WindowManager, "Failed to initalize glad\n");
				return 0;
			}
			glfwSwapInterval(0);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);

			glfwSetKeyCallback(s_window, [](GLFWwindow *w, int key, int scancode, int action, int mods) {
				switch (action) {
				case GLFW_PRESS:
					Internals::onKeyPressCallback({key, scancode, mods});
					break;
				case GLFW_RELEASE:
					Internals::onKeyReleaseCallback({key, scancode, mods});
					break;
				case GLFW_REPEAT:
					Internals::onKeyRepeatCallback({key, scancode, mods});
					break;
				}
			});

			glfwSetMouseButtonCallback(s_window, [](GLFWwindow *w, int button, int action, int mods) {
				switch (action) {
				case GLFW_PRESS:
					Internals::onMousePressCallback({button, mods});
					break;
				case GLFW_RELEASE:
					Internals::onMouseReleaseCallback({button, mods});
					break;
				}
			});
			glfwSetCursorPosCallback(s_window, [](GLFWwindow *w, double xPos, double yPos) {
				Internals::onMouseMoveCallback({xPos, yPos});
			});
			glfwSetScrollCallback(s_window, [](GLFWwindow *w, double xOffset, double yOffset) {
				Internals::onMouseScrollCallback({xOffset, yOffset});
			});

			glfwSetWindowPosCallback(s_window, [](GLFWwindow *w, int xPos, int yPos) {
				Internals::onWindowMoveCallback({xPos, yPos});
			});
			glfwSetWindowSizeCallback(s_window, [](GLFWwindow *w, int width, int height) {
				Internals::onWindowResizeCallback({width, height});
			});

			LOG_SUCCESS(WindowManager, "Initialized window manager\n");
			return 1;
		}
		void update()
		{
			glfwPollEvents();
			glfwSwapBuffers(s_window);
		}
		int shouldRun() { return !glfwWindowShouldClose(s_window); }
		void close() { glfwTerminate(); }
	} // namespace WindowManager
#define __ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(_namespace_, _eventName_) \
	void setOn##_namespace_##_eventName_##Callback(void (*func)(const Event::_namespace_::_eventName_ &)) { Internals::on##_namespace_##_eventName_##Callback = func; }

	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Key, Press)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Key, Release)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Key, Repeat)

	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Mouse, Move)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Mouse, Press)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Mouse, Release)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Mouse, Scroll)

	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Window, Move)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Window, Resize)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Window, Close)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Window, Focus)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(Window, Defocus)
} // namespace Coel
