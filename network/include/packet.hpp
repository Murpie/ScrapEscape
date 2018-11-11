#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <glm/glm.hpp>
#include "../../logic/include/input.hpp"

namespace network
{

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

enum class SessionState : uint8
{
	none,
	waiting,
	building,
	playing
};

class GameInput
{
public:
	int id = 0;
	logic::input data;
};

class GameObject
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
};

class GameState
{
public:

	GameObject game_objects[100];
	logic::input inputs[4]{};
	uint32 sequence = 0;
	uint8 player_count = 1;
	uint8 player_id = 0;
	SessionState state = SessionState::none;
};

class Queue
{
public:
	bool empty() const;
	void enqueue(const GameState& value);
	const GameState& dequeue();

private:
	int front = 0;
	int back = 0;
	static constexpr int size = 100;
	GameState data[size];
};

}

#endif


