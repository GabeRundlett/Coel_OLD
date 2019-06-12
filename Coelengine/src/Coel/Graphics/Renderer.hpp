#pragma once

namespace Coel {
	namespace Graphics {
		namespace Renderer {
			int init();
			void clear();
			void begin();
			void end();
			void flush();

			void drawRect(float x, float y, float w, float h, unsigned int color = -1);
			void resizeViewport(int x, int y, int w, int h);
		} // namespace Renderer
	} // namespace Graphics
} // namespace Coel
