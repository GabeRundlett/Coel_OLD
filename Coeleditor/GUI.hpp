#pragma once
#include "Coel.hpp"
#include "clm.hpp"

namespace GUI {
	namespace Button {
		namespace Interals {
			static void emptyFunc() {}
		} // namespace Interals
		struct Data {
			void (*onPressCallback)();
			float x, y, w, h;
			unsigned int col;
			Data(float x, float y, float w, float h, unsigned int c)
				: x(x), y(y), w(w), h(h), col(c), onPressCallback(&Interals::emptyFunc) {}
			Data(float x, float y, float w, float h, Coel::cvec4 c)
				: x(x), y(y), w(w), h(h), col(c), onPressCallback(&Interals::emptyFunc) {}
		};
		inline void draw(float x, float y, float w, float h, unsigned int c)
		{
			Coel::Graphics::Renderer::drawRect(x, y, w, h, c);
		}
		inline void draw(const Coel::vec2 &pos, const Coel::vec2 &size, unsigned int c)
		{
			Coel::Graphics::Renderer::drawRect(pos.x, pos.y, size.x, size.y, c);
		}
		inline void draw(const Data &d)
		{
			Coel::Graphics::Renderer::drawRect(d.x, d.y, d.w, d.h, d.col);
		}
		inline void checkIfPressed(const Data &d, const Coel::vec2 &pos)
		{
			if (!(pos.x < d.x || pos.y < d.y || pos.x > d.x + d.w || pos.y > d.y + d.h))
				d.onPressCallback();
		}
		inline bool checkIfHovered(const Data &d, const Coel::vec2 &pos)
		{
			return !(pos.x < d.x || pos.y < d.y || pos.x > d.x + d.w || pos.y > d.y + d.h);
		}
		inline void setPressCallback(Data &d, void (*func)())
		{
			d.onPressCallback = func;
		}
	} // namespace Button
} // namespace GUI
