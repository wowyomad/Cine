#include "some.hpp"
#include <fmt/format.h>

namespace glash
{
	void SomeClass::print() const
	{
		fmt::println("val: {}", m_Val);
	}
}