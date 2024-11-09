#pragma once

#include <glm/glm.hpp>

namespace Cine
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
        bool IsInteger(const std::string& str);
        bool IsFloat(const std::string& str);
	}
}