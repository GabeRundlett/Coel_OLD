#pragma once

namespace Engine {
	struct KeyPressedEvent {
		int key, scancode, mods;
	};
	struct KeyReleasedEvent {
		int key, scancode, mods;
	};
	struct KeyRepeatedEvent {
		int key, scancode, mods;
	};

	struct MousePressedEvent {
		int button, mods;
	};
	struct MouseReleasedEvent {
		int button, mods;
	};
	struct MouseScrolledEvent {
		double xPos, yPos;
	};
	struct MouseMovedEvent {
		double xOffset, yOffset;
	};

	struct WindowMovedEvent {
		int xPos, yPos;
	};
	struct WindowResizedEvent {
		int width, height;
	};
	struct WindowClosedEvent {
		int xPos, yPos;
	};
	struct WindowFocusedEvent {
		int width, height;
	};
	struct WindowLostFocusEvent {
		int width, height;
	};
} // namespace Engine
