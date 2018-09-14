#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <array>
#include <vector>
#include <chrono>

#include <renderer.hpp>
#include <host.hpp>
#include "window.hpp"
#include "input.hpp"
#include <lua_load.hpp>

//::.. authors ..:://
// Edvard
// Olle

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void render();
	void update(std::chrono::milliseconds delta);

	std::string s;
	
	std::unique_ptr<host> host;
	std::chrono::milliseconds timestep{16};
	Window window;
	input player_input;
	LuaLoad luaLoad; //Test
	Renderer* renderer;
	
	
};



#endif

