#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	: window({1280, 720}, "Scrap Escape")
	, level{"../resources/level/level.ssp", &mesh_lib}
	, renderer{&level}
{
	window.assign_key(button::up, GLFW_KEY_W);
	window.assign_key(button::left, GLFW_KEY_A);
	window.assign_key(button::down, GLFW_KEY_S);
	window.assign_key(button::right, GLFW_KEY_D);
	window.assign_key(button::jump, GLFW_KEY_SPACE);
	window.assign_key(button::glow, GLFW_KEY_G);
	window.assign_key(button::refresh, GLFW_KEY_F5);
	window.assign_key(button::menu, GLFW_KEY_F1);
	window.assign_key(button::debug, GLFW_KEY_F3);
	window.assign_key(button::switch_camera, GLFW_KEY_F4);
	window.assign_key(button::reset, GLFW_KEY_R);
	window.assign_key(button::quit, GLFW_KEY_ESCAPE);

	net_out.player_id = 0;
	net_out.player_count = 1;
	net_out.directions.fill({ 0.0f, 0.0f, 0.0f });
}

void Game::run()
{
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();
	auto delta_time = 0ns;

	/*physics.add_dynamic_body({ 10, 10 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ -5.0, -5.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ 14.0, 2.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ -4.0, -20.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });*/

	physics.add_dynamic_body({ 2, 10 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ -5.0, -5.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ 14.0, 2.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ -4.0, -20.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	physics.add_static_body({ 0.0, -22.0 }, { 0.0, 0.0 }, 1000, 2, false);

	auto frames = 0;
	std::chrono::duration<float> seconds = 0s;

	while (window.is_open() && 
		!menu.exit() &&
		(*local_input)[button::quit] != button_state::pressed)
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

		std::vector<glm::vec2> debug_positions = physics.get_all_debug();
		render(debug_positions);
		window.swap_buffers();
		window.poll_events();		
	}
}

void Game::render(std::vector<glm::vec2> debug_positions)
{
	renderer.render(chat.begin(), chat.end(), 
		menu.button_data(), 
		menu.on(),
		net_out.player_count > 1, menu.debug());
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	auto& direction = net_out.directions[net_out.player_id];
	direction = { 0.0f, 0.0f, 0.0f };

	if ((*local_input)[button::up] >= button_state::pressed)
		direction.z += 1.0f;
	if ((*local_input)[button::left] >= button_state::pressed)
		direction.x -= 1.0f;
	if ((*local_input)[button::down] >= button_state::pressed)
		direction.z -= 1.0f;
	if ((*local_input)[button::right] >= button_state::pressed)
		direction.x += 1.0f;

	if (!menu.on())
		window.hide_cursor();

	if ((*local_input)[button::menu] == button_state::pressed)
		window.show_cursor();

	net_out = net.update({ chat[1], net_out.directions });
	local_input = &player_inputs.components[net_out.player_id];

	chat.update(delta);
	menu.update(delta, *local_input);

	logic_out = gameplay.update({delta, local_input});
	glm::vec2 combined_force = logic_out.force;

	//Update new direction force of player to physics
	if (physics.dynamic_rigidbodies[0].can_jump == true)
		physics.dynamic_rigidbodies[0].add_force(combined_force);
	else
		physics.dynamic_rigidbodies[0].add_force({ combined_force.x, 0 });


	//std::cout << physics.dynamic_rigidbodies[0]. << std::endl;

	std::vector<glm::vec2> dynamic_pos = physics.update(delta);
	
	 std::cout << dynamic_pos[0].x << dynamic_pos[0].y << std::endl;

	renderer.update(delta, 
		std::begin(player_inputs.components),
		std::end(player_inputs.components),
		net_out.directions,
		chat[1], net_out.player_count,
		net_out.player_id, chat.is_on(),
		net_out.player_count >= 1);

}
