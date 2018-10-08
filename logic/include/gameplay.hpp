#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <chrono>
#include <array>
#include <glm/glm.hpp>

#include "lua_load.hpp"
#include "lua_script.hpp"
#include "../../engine/include/input.hpp"

namespace logic
{

struct Input
{
	std::chrono::milliseconds delta;
	const input* local_input;
};

struct Output
{
	glm::vec2 force;

};

template <typename T>
using script_array = std::array<T, 10>;

class Gameplay
{
public:
	Gameplay();

	void refresh();

	Output update(Input input);

private:
	script_array<int> entities;
	script_array<LuaScript> scripts;

	LuaLoad luaLoad; //Test
};

}

#endif
