#pragma once

namespace Coel {
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
		double xOffset, yOffset;
	};
	struct MouseMovedEvent {
		double xPos, yPos;
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
} // namespace Coel

// Key codes
#pragma once

#define COEL_KEY_SPACE 32
#define COEL_KEY_APOSTROPHE 39
#define COEL_KEY_COMMA 44
#define COEL_KEY_MINUS 45
#define COEL_KEY_PERIOD 46
#define COEL_KEY_SLASH 47
#define COEL_KEY_0 48
#define COEL_KEY_1 49
#define COEL_KEY_2 50
#define COEL_KEY_3 51
#define COEL_KEY_4 52
#define COEL_KEY_5 53
#define COEL_KEY_6 54
#define COEL_KEY_7 55
#define COEL_KEY_8 56
#define COEL_KEY_9 57
#define COEL_KEY_SEMICOLON 59
#define COEL_KEY_EQUAL 61
#define COEL_KEY_A 65
#define COEL_KEY_B 66
#define COEL_KEY_C 67
#define COEL_KEY_D 68
#define COEL_KEY_E 69
#define COEL_KEY_F 70
#define COEL_KEY_G 71
#define COEL_KEY_H 72
#define COEL_KEY_I 73
#define COEL_KEY_J 74
#define COEL_KEY_K 75
#define COEL_KEY_L 76
#define COEL_KEY_M 77
#define COEL_KEY_N 78
#define COEL_KEY_O 79
#define COEL_KEY_P 80
#define COEL_KEY_Q 81
#define COEL_KEY_R 82
#define COEL_KEY_S 83
#define COEL_KEY_T 84
#define COEL_KEY_U 85
#define COEL_KEY_V 86
#define COEL_KEY_W 87
#define COEL_KEY_X 88
#define COEL_KEY_Y 89
#define COEL_KEY_Z 90
#define COEL_KEY_LEFT_BRACKET 91
#define COEL_KEY_BACKSLASH 92
#define COEL_KEY_RIGHT_BRACKET 93
#define COEL_KEY_GRAVE_ACCENT 96
#define COEL_KEY_WORLD_1 161 /* non-US #1 */
#define COEL_KEY_WORLD_2 162 /* non-US #2 */

/* Function keys */
#define COEL_KEY_ESCAPE 256
#define COEL_KEY_ENTER 257
#define COEL_KEY_TAB 258
#define COEL_KEY_BACKSPACE 259
#define COEL_KEY_INSERT 260
#define COEL_KEY_DELETE 261
#define COEL_KEY_RIGHT 262
#define COEL_KEY_LEFT 263
#define COEL_KEY_DOWN 264
#define COEL_KEY_UP 265
#define COEL_KEY_PAGE_UP 266
#define COEL_KEY_PAGE_DOWN 267
#define COEL_KEY_HOME 268
#define COEL_KEY_END 269
#define COEL_KEY_CAPS_LOCK 280
#define COEL_KEY_SCROLL_LOCK 281
#define COEL_KEY_NUM_LOCK 282
#define COEL_KEY_PRINT_SCREEN 283
#define COEL_KEY_PAUSE 284
#define COEL_KEY_F1 290
#define COEL_KEY_F2 291
#define COEL_KEY_F3 292
#define COEL_KEY_F4 293
#define COEL_KEY_F5 294
#define COEL_KEY_F6 295
#define COEL_KEY_F7 296
#define COEL_KEY_F8 297
#define COEL_KEY_F9 298
#define COEL_KEY_F10 299
#define COEL_KEY_F11 300
#define COEL_KEY_F12 301
#define COEL_KEY_F13 302
#define COEL_KEY_F14 303
#define COEL_KEY_F15 304
#define COEL_KEY_F16 305
#define COEL_KEY_F17 306
#define COEL_KEY_F18 307
#define COEL_KEY_F19 308
#define COEL_KEY_F20 309
#define COEL_KEY_F21 310
#define COEL_KEY_F22 311
#define COEL_KEY_F23 312
#define COEL_KEY_F24 313
#define COEL_KEY_F25 314
#define COEL_KEY_KP_0 320
#define COEL_KEY_KP_1 321
#define COEL_KEY_KP_2 322
#define COEL_KEY_KP_3 323
#define COEL_KEY_KP_4 324
#define COEL_KEY_KP_5 325
#define COEL_KEY_KP_6 326
#define COEL_KEY_KP_7 327
#define COEL_KEY_KP_8 328
#define COEL_KEY_KP_9 329
#define COEL_KEY_KP_DECIMAL 330
#define COEL_KEY_KP_DIVIDE 331
#define COEL_KEY_KP_MULTIPLY 332
#define COEL_KEY_KP_SUBTRACT 333
#define COEL_KEY_KP_ADD 334
#define COEL_KEY_KP_ENTER 335
#define COEL_KEY_KP_EQUAL 336
#define COEL_KEY_LEFT_SHIFT 340
#define COEL_KEY_LEFT_CONTROL 341
#define COEL_KEY_LEFT_ALT 342
#define COEL_KEY_LEFT_SUPER 343
#define COEL_KEY_RIGHT_SHIFT 344
#define COEL_KEY_RIGHT_CONTROL 345
#define COEL_KEY_RIGHT_ALT 346
#define COEL_KEY_RIGHT_SUPER 347
#define COEL_KEY_MENU 348
