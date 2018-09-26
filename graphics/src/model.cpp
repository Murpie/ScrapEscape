#include "model.hpp"

Model::Model(const glm::mat4& model, const int id)
	: model{model}
{
	mesh = new Mesh("RobotTest.ssp");
}

Model::~Model()
{
	delete mesh;
}

void Model::render(const Shader & shader, const Camera& camera)const
{
	shader.uniform("model", model);
	shader.uniform("view", camera.view_matrix());
	shader.uniform("projection", camera.projection_matrix());
	shader.uniform("view_pos", glm::vec3{ camera.view_matrix()[3][0] });
	mesh->render();
}