#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>
#include <vector>
#include "game_scene.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "framebuffer.hpp"
#include "post_processing_effects.hpp"
#include "user_interface.hpp"
#include "lights.hpp"
#include "../../engine/include/timer.hpp"
#include "../../engine/include/gui.hpp"
#include "primitive_factory.hpp"
#include "skybox.hpp"

namespace graphics
{

//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer(GameScene* scene);

	void render(
		const std::string* begin,
		const std::string* end,
		const gui::button_array& buttons,
		const std::vector<glm::vec2>& debug_positions,
		bool is_menu,
		bool connected,
		bool debug) const;

	void update(std::chrono::milliseconds delta,
		const input* begin,
		const input* end,
		const std::array<glm::vec3, 4>& directions,
		const std::string& data,
		int num_players,
		int id,
		bool is_on,
		bool move_char);

	static void line_debug(const std::vector<glm::vec2>& lines)
	{
		VertexArray vao;
		Buffer vertex_buffer;

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		gl_buffer_data(GL_ARRAY_BUFFER, lines, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, lines.size());
		glBindVertexArray(0);
	}

private:

	GameScene* scene;
	DebugCamera db_camera;
	GameCamera game_camera;
	std::vector<Model> models;

	Box light_box;
	Skybox skybox;

	Text text;
	UserInterface ui;
	std::vector<Shader> shaders;
	std::chrono::milliseconds time{10000};

	std::string log;
	Timer t;

	int new_player_count{0};

	glm::vec2 v[4];
	bool is_chat_visible{false};

	bool game_over{false};

	bool show_start{false};


	Framebuffer scene_texture;
	PostProcessingEffects post_processing_effects;

	bool want_glow{false};
	PointLight light{ glm::vec3(0,2,4), glm::vec3(1,1,1) };

	bool debug_active{ false };
};


template <typename T>
void render_type(const Shader& shader, const Camera& camera, const PointLight& light, const T& data)
{
	shader.use();
	for (auto i = 4u; i < data.size(); ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader, camera, light);
	}
}

template <typename T>
void render_character(const Shader& shader, const Camera& camera, const PointLight& light, const T& data, int num_players)
{
	shader.use();
	for (auto i = 0; i < num_players; ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader, camera, light);
	}
}

}

#endif