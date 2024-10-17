#pragma once

#include <regex>
#include <string>

namespace Cine
{
	namespace Utils
	{
		template <class T>
		constexpr std::string  GetClassTypenameWithSpaces()
		{
			// Get the demangled type name
			std::string name = typeid(T).name();

			// Transform the type name into a readable format
			// 1. Remove any leading "class", "struct", etc.
			static const std::regex prefixRegex(R"(^class |^struct |^enum )");
			name = std::regex_replace(name, prefixRegex, "");

			// 2. Remove namespace, if present (e.g., "Cine::")
			auto pos = name.rfind("::");
			if (pos != std::string::npos)
			{
				name = name.substr(pos + 2);
			}

			// 3. Insert spaces before capital letters (to make camel case readable)
			static const std::regex camelCaseRegex(R"(([a-z])([A-Z]))");
			name = std::regex_replace(name, camelCaseRegex, "$1 $2");

			return name;
		}

		template <class T>
		constexpr std::string  GetClassTypename()
		{
			// Get the demangled type name
			std::string name = typeid(T).name();

			// Transform the type name into a readable format
			// 1. Remove any leading "class", "struct", etc.
			static const std::regex prefixRegex(R"(^class |^struct |^enum )");
			name = std::regex_replace(name, prefixRegex, "");

			// 2. Remove namespace, if present (e.g., "Cine::")
			auto pos = name.rfind("::");
			if (pos != std::string::npos)
			{
				name = name.substr(pos + 2);
			};
			return name;
		}

	}
}