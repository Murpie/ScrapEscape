#include <laser.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
namespace graphics
{

Laser::Laser()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);

	laser_textures[0].load_texture("../resources/textures/laser/start_background_texture.png", true);
	laser_textures[1].load_texture("../resources/textures/laser/start_overlay_texture.png", true);
	laser_textures[2].load_texture("../resources/textures/laser/middle_background_texture.png", true);
	laser_textures[3].load_texture("../resources/textures/laser/middle_overlay_texture.png", true);
	laser_textures[4].load_texture("../resources/textures/laser/end_background_texture.png", true);
	laser_textures[5].load_texture("../resources/textures/laser/end_overlay_texture.png", true);

}

void Laser::update(const glm::vec2& start, const glm::vec2& end)
{
	model[3][0] = start.x;
	model[3][1] = start.y;
	
	glm::vec2 direction = end - start;
	float distance = glm::distance(start, end);

	glm::vec2 nor_dir = glm::normalize(direction);
	float aco = glm::acos(glm::dot(nor_dir, glm::vec2(1,0)));

	model = glm::mat4(1.f);

	model = glm::translate(model, glm::vec3(start + direction * 0.5f, 0.f));
	
	model = glm::rotate(model, aco, glm::vec3(0,0,-1));

	model = glm::scale(model, glm::vec3(distance * 0.5f, 1.f, 1.f));

}

void Laser::render(const Shader &shader, const Camera& cam)const
{
	shader.use();
	shader.uniform("model", model);
	shader.uniform("view", cam.view());
	shader.uniform("projection", cam.projection);
	glm::vec4 color{1,0,0,1};
	shader.uniform("color", color);

	shader.uniform("distance", distance);

	for (int i = 0; i < 6; i++)
	{
		laser_textures[i].bind(i);
	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}