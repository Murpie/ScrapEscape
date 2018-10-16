#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "host.hpp"
#include "packet.hpp"

namespace network
{

class Messenger
{
public:
	Messenger() : init{}, player_host{} {}

	int connected_players() const;
	int id() const;
	bool connected() const;
	void update(GameState& state, const char* ip_address);

private:
	void write_state(GameState& state);
	void read_state(GameState& state);

	EnetInit init;
	Host player_host;
	int player_id = 0;
	int num_players = 1;
};

}
#endif

