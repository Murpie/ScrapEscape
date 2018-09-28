#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <array>
#include <vector>
#include <chrono>

#include <renderer.hpp>
#include <host.hpp>
#include "ecs.hpp"
#include "mesh_lib.hpp"
#include "game_scene.hpp"
#include "window.hpp"
#include "input.hpp"
#include "gui.hpp"
#include "timer.hpp"
#include "config.hpp"

//::.. authors ..:://
// Edvard
// Olle

template <int N>
struct input_array
{
	int player_index;
	std::array<ecs::entity, N> entities;
	input components[N];
};

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void render();
	void update(std::chrono::milliseconds delta);
	void create_scene(const char* file_name);
	
	std::unique_ptr<Host> host;
	std::chrono::milliseconds timestep{16};

	MeshLib* mesh_lib;
	GameScene* level;

	Window window;
	input player_input;
	
	Renderer* renderer;
	
	input_array<4> player_inputs;
	input* local_input{ &player_inputs.components[0] };
	gui::chat chat;
	gui::Menu menu;
	
};



#endif