#include <string>
#include <cstdint>
namespace glash
{
		struct Color
		{
			uint8_t r, g, b, y;

			static constexpr Color RED() { return { 255, 0, 0, 0 }; }
			static constexpr Color GREEN() { return { 0, 255, 0, 0 }; }
			static constexpr Color BLUE() { return { 0, 0, 255, 0 }; }
			static constexpr Color YELLOW() { return { 255, 255, 0, 0 }; }

			inline Color& operator+(const Color& color)
			{
				r += color.r;
				g += color.g;
				b += color.b;
				y += color.y;
				return* this;
			}
			inline Color& operator+=(const Color& color)
			{
				*this = *this + color;
				return *this;
			}
		};
}