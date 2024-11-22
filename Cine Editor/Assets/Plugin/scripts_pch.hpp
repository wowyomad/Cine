#pragma once
//input/output
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <fmt/format.h>
#include <format>
#include <print>

//data structures
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <tuple>
#include <variant>
#include <any>

//other
#include <thread>
#include <memory>
#include <filesystem>
#include <stdio.h>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <type_traits>

//maths
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

//Library
#include "glash/Cine.hpp"
#include "glash/Scene/Components.hpp"
#include "glash/Core/Application.hpp"
#include "glash/Core/Log.hpp"