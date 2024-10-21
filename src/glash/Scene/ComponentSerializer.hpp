#pragma once

#include <unordered_map>
#include <sstream>
#include <string>
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include "glash/Utils/StringUtils.hpp"

#define SERIALIZE_FIELD(field) visitor(#field, field);
#define BEGIN_SERIALIZE() template <typename Visitor> void Serialize(Visitor& visitor) {
#define END_SERIALIZE() }

template <typename T>
struct is_serializable : std::disjunction<
	std::is_arithmetic<T>,
	std::is_same<T, glm::vec2>,
	std::is_same<T, glm::vec3>,
	std::is_same<T, glm::vec4>,
	std::is_same<T, std::string>
> {};

template <typename T>
struct is_serializable<std::vector<T>> {
	static constexpr bool value = is_serializable<T>::value;
};

template<>
struct YAML::convert<glm::vec2>
{
	static Node encode(const glm::vec2& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		return node;
	}

	static bool decode(const Node& node, glm::vec2& rhs)
	{

		if (!node.IsSequence() || node.size() != 2)
		{
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();

		return true;
	}
};

template<>
struct YAML::convert<glm::vec3>
{
	static Node encode(const glm::vec3& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		return node;
	}

	static bool decode(const Node& node, glm::vec3& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
		{
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();

		return true;
	}
};

template<>
struct YAML::convert<glm::vec4>
{
	static Node encode(const glm::vec4& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);

		return node;
	}

	static bool decode(const Node& node, glm::vec4& rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
		{
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();

		return true;
	}
};

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

namespace Cine
{
	class Serializer {
	public:
		YAML::Emitter emitter;

		// Start emitting a new map for the object

		template <typename T>
		void StartObject(const T& obj) {

			emitter << YAML::BeginMap << YAML::Key << Utils::GetClassTypename<T>() << YAML::Value << YAML::BeginMap;
		}

		// End emitting the map
		void EndObject() {
			emitter << YAML::EndMap << YAML::EndMap;
		}

		// Serialize a field
		template <typename T>
		void operator()(const std::string& name, T& value) {
			emitter << YAML::Key << name;

			if constexpr (is_serializable<T>::value)
			{

				emitter << YAML::Value << value;
			}
			else 
			{
				// Call the Serialize function of the nested class
				Serialize(value);
			}
		}

		// Serialize function that includes the class name
		template <typename T>
		void Serialize(T& obj) {
			StartObject(obj); // Start emitting the object map
			obj.Serialize(*this); // Serialize fields
			EndObject(); // End emitting the object map
		}

		YAML::Node GetNode() {
			return YAML::Load(emitter.c_str());
		}
	};

	class Deserializer {
	public:
		const YAML::Node& node;

		Deserializer(const YAML::Node& node) : node(node) {}

		// Deserialize a field
		template <typename T>
		void operator()(const std::string& name, T& value) {
			if (node[name]) {
				if constexpr (is_serializable<T>::value) {
					value = node[name].as<T>();
				}
				else {
					// Call the Deserialize function of the nested class
					Deserialize(value, node[name]);
				}
			}
			else {
				throw std::runtime_error("Field not found: " + name);
			}
		}

		// Deserialize function
		template <typename T>
		void Deserialize(T& obj, const YAML::Node& node) {
			Deserializer deserializer(node[Utils::GetClassTypename<T>()]);
			obj.Serialize(deserializer);
		}
	};

	// Serialization and Deserialization functions
	template <typename T>
	YAML::Node Serialize(T& obj) {
		Serializer serializer;
		serializer.Serialize(obj); // Start emitting the object map
		return serializer.GetNode();
	}

	template <typename T>
	void Deserialize(T& obj, const YAML::Node& node) {
		Deserializer deserializer(node[Utils::GetClassTypename<T>()]);
		obj.Serialize(deserializer);
	}
}

