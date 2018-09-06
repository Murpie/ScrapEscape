#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vector>
#include "model.hpp"
//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render()const;
private:
	std::vector<Model*>models;
};

#endif