#include "game_scene.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

GameScene::GameScene()
{
}

GameScene::GameScene(const char* file_name, MeshLib* mesh_lib, MeshLib* object_lib)
{
	//Create players
	CustomLevel level(file_name);
	CustomLevel objects("../resources/level/objects.ssp");
	models.reserve(sizeof(Model) * (level.counterReader.levelObjectCount + 4));

	using glm::vec3;
	glm::mat4 model{ 1.0f };

	v[0] = { 10, 15 };
	v[1] = { -3, 0 };
	v[2] = { 3, 0 };
	v[3] = { -9, 0 };

	models.emplace_back(glm::translate(model, vec3{ v[0], 0 }), vec3{ 1.0f, 0.0f, 0.0f}, mesh_lib->get_mesh(0));
	models.emplace_back(glm::translate(model, vec3{ v[1], 0 }), vec3{ 0.2f, 0.9f, 0.1f}, mesh_lib->get_mesh(0));
	models.emplace_back(glm::translate(model, vec3{ v[2], 0 }), vec3{ 0.1f, 0.1f, 0.9f}, mesh_lib->get_mesh(0));
	models.emplace_back(glm::translate(model, vec3{ v[3], 0 }), vec3{ 0.9f, 0.8f, 0.1f}, mesh_lib->get_mesh(0));

	//for (int i = 0; i < 4; i++)
	//{
	models[0].create_animation_data("Robot_idle.sspAnim", MODEL_STATE::IDLE);
	models[0].create_animation_data("robot_run.sspAnim", MODEL_STATE::RUNNING);
	models[0].create_animation_data("Robot_Jump_1.sspAnim", MODEL_STATE::START_JUMP);
	models[0].create_animation_data("Robot_Jump_2.sspAnim", MODEL_STATE::FALLING);
	models[0].create_animation_data("Robot_Jump_3.sspAnim", MODEL_STATE::IN_JUMP);
	models[0].create_animation_data("Robot_Jump_4.sspAnim", MODEL_STATE::LANDING);
	models[0].create_animation_data("Robot_turn.sspAnim", MODEL_STATE::TURN);
	models[0].create_animation_data("Robot_wj1.sspAnim", MODEL_STATE::CONNECT_WALL);
	models[0].create_animation_data("Robot_wj2.sspAnim", MODEL_STATE::JUMP_FROM_WALL);

	//}

	for (unsigned int i = 0; i < level.counterReader.levelObjectCount; i++)
	{
		glm::mat4 model{ 1.0f };

		model = glm::translate(model, glm::vec3(level.levelObjects[i].position[0], level.levelObjects[i].position[1], level.levelObjects[i].position[2]));
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[2]), glm::vec3{ 0,0,1 });
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[1]), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[0]), glm::vec3{ 1,0,0 });

		models.emplace_back(model, glm::vec3(0, 0, 0), mesh_lib->get_mesh(level.levelObjects[i].id));

		if(level.levelObjects[i].position[2] > -0.01 && level.levelObjects[i].position[2] < 0.01)
		{ 
			float width = level.levelObjects[i].collisionBox[1];
			float height = level.levelObjects[i].collisionBox[0];
			auto* ptr = level.levelObjects[i].centerPivot;
			
			coll_data.emplace_back(collision_data{ 
				glm::vec2{ ptr[0], ptr[1] }, width, height, false });
		}
	}

	// Lucas/Vincent Test f�r placering av object.
	inititate_object(objects, object_lib);
}

GameScene::~GameScene()
{
}

void GameScene::inititate_object(CustomLevel& objects, MeshLib* object_lib)
{
	for (unsigned int i = 0; i < objects.counterReader.levelObjectCount; i++)
	{
		glm::mat4 model{ 1.0f };

		model = glm::translate(model, glm::vec3(objects.levelObjects[i].position[0], objects.levelObjects[i].position[1], objects.levelObjects[i].position[2]));
		model = glm::rotate(model, glm::radians(objects.levelObjects[i].rotation[2]), glm::vec3{ 0,0,1 });
		model = glm::rotate(model, glm::radians(objects.levelObjects[i].rotation[1]), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(objects.levelObjects[i].rotation[0]), glm::vec3{ 1,0,0 });

		int id = objects.levelObjects[i].id;

		float width = objects.levelObjects[i].collisionBox[1];
		float height = objects.levelObjects[i].collisionBox[0];
		auto* ptr = objects.levelObjects[i].centerPivot;

		Model tempModel = { model, glm::vec3(0, 0, 0), object_lib->get_mesh(i) };
		collision_data tempInfo = { glm::vec2{ ptr[0], ptr[1] }, width, height, false };
		placeableObjectInfo info = { tempModel, tempInfo };
		this->objects.emplace_back(info);
	}
}

int GameScene::add_object(collision_data& physics_data, int id)
{
	physics_data = objects[id].data;

	models.emplace_back(objects[id].model);

	return models.size() - 1;
}

void GameScene::rotate_object(int model_id)
{
	models[model_id].rotate(90.0f);
}

}


