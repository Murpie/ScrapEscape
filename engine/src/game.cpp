#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	: window({1280, 720}, "Scrap Escape")
	, level{"../resources/level/level.ssp", &mesh_lib}
	, renderer{&level}
{
	window.assign_key(logic::button::up, GLFW_KEY_W);
	window.assign_key(logic::button::left, GLFW_KEY_A);
	window.assign_key(logic::button::down, GLFW_KEY_S);
	window.assign_key(logic::button::right, GLFW_KEY_D);
	window.assign_key(logic::button::jump, GLFW_KEY_SPACE);
	window.assign_key(logic::button::glow, GLFW_KEY_G);
	window.assign_key(logic::button::refresh, GLFW_KEY_F5);
	window.assign_key(logic::button::menu, GLFW_KEY_F1);
	window.assign_key(logic::button::debug, GLFW_KEY_R);
	window.assign_key(logic::button::switch_object, GLFW_KEY_F4);
	window.assign_key(logic::button::remove_object, GLFW_KEY_O);
	window.assign_key(logic::button::rotate, GLFW_KEY_F3);	
	window.assign_key(logic::button::build_mode, GLFW_KEY_B);
	window.assign_key(logic::button::place_object, GLFW_KEY_KP_ENTER);
	window.assign_key(logic::button::quit, GLFW_KEY_ESCAPE);

	logic_out.directions.fill({ 0.0f, 0.0f, 0.0f });

	physics.add_dynamic_body(level.v[0], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[1], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[2], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[3], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	for (int i = 0; i < 4; ++i)
	{
		dynamics[i].position = level.v[i];
		dynamics[i].velocity = {0.0f, 0.0f};
		dynamics[i].size = {1.0f, 3.5f};
		dynamics[i].forces = {0.0f, 0.0f};
		dynamics[i].impulse = { 0.0f, 0.0f };
	}
		

	for (auto& coll : level.coll_data)
		physics.add_static_body(coll.position, 
			glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);
}

void Game::run()
{
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();
	auto delta_time = 0ns;
	auto frames = 0;
	std::chrono::duration<float> seconds = 0s;

	while (window.is_open() && 
		!menu.exit() &&
		(*local_input)[logic::button::quit] != logic::button_state::held)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		seconds += delta_time;
		auto fps = static_cast<int>(++frames / seconds.count());
		if (seconds > 1s)
		{
			seconds = 0s;
			frames = 0;
			std::string title = 
				"Scrap Escape | FPS: " 
				+ std::to_string(fps);
			
			window.title(title.c_str());
		}
		
		while (delta_time > timestep)
		{
			delta_time -= timestep;
			window.update_input(*local_input);
			
			update(timestep);
		}

		render();
		window.swap_buffers();
		window.poll_events();		
	}
}

void Game::render()
{	
	std::vector<glm::vec3> db_coll = physics.get_all_debug();
	
	renderer.render(chat.begin(), chat.end(),
		menu.button_strings(),
		db_coll, menu.on(),
		net.connected(), menu.debug(), leader_board, showleaderboard);
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	if ((*local_input)[logic::button::debug] == logic::button_state::pressed)
	{
		renderer.debug_active = !renderer.debug_active;
	}

	if (!menu.on())
		window.hide_cursor();

	chat.update(delta);
	menu.update(delta, *local_input);
		
	const char* str = nullptr;
	if (!chat[1].empty() && !is_client)
	{
		str = chat[1].c_str();
		is_client = true;
	}

	pack_data();
	net.update(net_state, str);
	unpack_data();

	std::array<glm::vec3, 4> directions
	{ 
		glm::vec3{0.0f}, 
		glm::vec3{0.0f},
		glm::vec3{0.0f}, 
		glm::vec3{0.0f} 
	};

	{
		logic::objects_array obj;
		for (int i = 0; i < dynamics.size(); ++i)
		{
			obj[i].position = dynamics[i].position;
			obj[i].velocity = dynamics[i].velocity;
			obj[i].size = dynamics[i].size;
			obj[i].forces = dynamics[i].forces;
			obj[i].impulse = dynamics[i].impulse;
		}
		
		logic_out = gameplay.update({ delta, obj, player_inputs, directions, &level, &physics });

		for (int i = 0; i < dynamics.size(); ++i)
		{
			dynamics[i].position = obj[i].position;
			dynamics[i].velocity = obj[i].velocity;
			dynamics[i].size = obj[i].size;
			dynamics[i].forces = obj[i].forces;
			dynamics[i].impulse = obj[i].impulse;
		}
	}
	
	if (net.connected())
	{
		for (int i = 0; i < 4; ++i)
		{
			if (jump_timers[i] <= 0ms && player_inputs[i][logic::button::jump] == logic::button_state::held)
			{
				jump_timers[i] = 3s;
				dynamics[i].impulse = glm::vec2{ 0.0f, 25.0f};
			}
			
			jump_timers[i] -= delta;		
			dynamics[i].forces.x = logic_out.directions[i].x * 2000.0f;
		}
	}

	physics.update(delta, dynamics);

	if (net.connected())
	{
		for (int i = 0; i < 4; ++i)
		{
			if (logic_out.directions[i].x > 0.0f)
			{
				level.models[i].rotate({0.0f, 1.0f, 0.0f}, glm::radians(180.0f));
			}
			else if (logic_out.directions[i].x < 0.0f)
			{
				level.models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f));
			}
			
			level.v[i] = dynamics[i].position;
			level.models[i].set_position(dynamics[i].position);			
		}		
	}
	level.models[0].update_animation((float)delta.count());

	renderer.update(delta,
		player_inputs[net.id()].cursor,
		logic_out.directions,
		chat[1], player_count,
		net.id(), chat.is_on(),
		net.connected());
}

void Game::pack_data()
{	
	for (int i = 0; i < 4; ++i)
	{
		net_state.inputs[i] = static_cast<logic::uint16>(player_inputs[i]);
	}

	for (int i = 0; i < dynamics.size(); ++i)
	{
		net_state.game_objects[i].position = dynamics[i].position;
		net_state.game_objects[i].velocity = dynamics[i].velocity;
	}
}

void Game::unpack_data()
{	
	for (int i = 0; i < 4; ++i)
	{
		if (i != net.id())
		{
			player_inputs[i] = logic::input{net_state.inputs[i]};
		}		
	}
	
	if (state_sequence != net_state.sequence)
	{
		state_sequence = net_state.sequence;
		player_count = net_state.player_count;

		if (net.id())
		{
			for (int i = 0; i < dynamics.size(); ++i)
			{
				dynamics[i].position = net_state.game_objects[i].position;
				dynamics[i].velocity = net_state.game_objects[i].velocity;
			}
		}

		local_input = &player_inputs[net.id()];
	}
}
