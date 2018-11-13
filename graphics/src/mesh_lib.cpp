#include "mesh_lib.hpp"

namespace graphics
{

MeshLib::MeshLib(const int listType)
{
	if (listType == 0)
	{
		create_mesh_list();
	}
	else if (listType == 1)
	{
		create_object_list();
	}
}

MeshLib::~MeshLib()
{
	for (auto& mesh : meshes)
		delete mesh;
}

void MeshLib::create_mesh_list()
{
	std::string meshLoader[] =
	{
		"robot.ssp",						//0
		"door.ssp",							//1
		"floor.ssp",						//2
		"roller_belt.ssp",					//3
		"surveillencecamera.ssp",			//4
		"plank.ssp",						//5
		"platform.ssp",						//6
		"steel_beam.ssp",					//7
		"ladder.ssp",						//8
		"box_small.ssp",					//9
		"valve.ssp",						//10
		"wall_background.ssp",				//11
		"fan_shell.ssp",					//12
		"propeller.ssp",					//13
		"ventilation.ssp",					//14
		"controlpanel_1_wide.ssp",			//15
		"controlpanel_2_small.ssp",			//16
		"controlpanel_3_big.ssp",			//17
		"pipe_bent.ssp",					//18
		"pipe_straight.ssp",				//19
		"pipe_angled.ssp",					//20
		"wire_bent_1.ssp",					//21
		"wire_bent_2.ssp",					//22
		"wire_broken.ssp",					//23
		"wire_pile.ssp",					//24
		"wire_straight.ssp",				//25
		"wall_doorhole.ssp",				//26
		"wall_port.ssp",					//27
		"wall.ssp",							//28
		"ventilationdrum_bent.ssp",			//29
		"ventilationdrum_straight.ssp",		//30
		"crane.ssp",				        //31
		"crane_lower_part.ssp",				//32
		"grinder.ssp",					    //33
		"wall_hole.ssp",				    //34
		"box_medium.ssp",					//35
		"box_big.ssp",						//36
		"cog_1.ssp",						//37
		"cog_2.ssp",						//38
		"cog_3.ssp",						//39 
		"machine_1.ssp",					//40
		"machine_2.ssp",					//41
		"wall_slim.ssp",					//42
		"hatch.ssp",						//43
		"wall_single.ssp",					//44
		"Shit_robot.ssp",					//45
		"big_propeller.ssp",				//46
		"big_fan_shell.ssp",				//47
		"platform_fence.ssp",               //48
		"melting_pot.ssp"                   //49
	};

	for (auto i = 0u; i < sizeof(meshLoader) / sizeof(meshLoader[0]); ++i)
	{
		mesh_list.emplace_back(meshLoader[i]);
	}

	for (auto i = 0u; i < mesh_list.size(); ++i)
	{
		add_mesh(mesh_list[i]);
	}
}

void MeshLib::create_object_list()
{
	std::string meshLoader[] =
	{
		"spike_trap.ssp",					//0
		"saw.ssp",							//1
		"platform_oil.ssp",					//2
		"sticky_platform.ssp",				//3
		"shock_trap.ssp",					//4
		"treadmill.ssp",					//5
		"standard_platform.ssp",			//6
		"speed_boost.ssp",					//7
		"steam_boost.ssp",					//8
		"trampolin.ssp",					//9
		"turret.ssp"						//10
	};

	for (auto i = 0u; i < sizeof(meshLoader) / sizeof(meshLoader[0]); ++i)
	{
		mesh_list.emplace_back(meshLoader[i]);
	}

	for (auto i = 0u; i < mesh_list.size(); ++i)
	{
		add_mesh(mesh_list[i]);
	}
}

void MeshLib::add_mesh(const std::string& file_path)
{
	Mesh* new_mesh = new Mesh(file_path);
	meshes.emplace_back(new_mesh);
}

Mesh* MeshLib::get_mesh(int key)
{
	return meshes.at(key);
}

int MeshLib::get_number_of_meshes()const
{
	return meshes.size();
}

}