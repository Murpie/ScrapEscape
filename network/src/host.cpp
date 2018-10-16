#include "host.hpp"
#include <iostream>

namespace network
{

EnetInit::EnetInit()
{
	enet_initialize();
}

EnetInit::~EnetInit()
{
	enet_deinitialize();
}

Host::Host(const char* ip_address)
{
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	enet_address_set_host(&address, ip_address);
	address.port = 1234;
}

Host::Host()
{
	address.host = ENET_HOST_ANY;
	address.port = 1234;
	enet_host = enet_host_create(&address, 32, 2, 0, 0);
}

Host::~Host()
{
	enet_host_destroy(enet_host);
}

Host::Host(const Host& other)
{
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	address = other.address;
	peers[0] = enet_host_connect(enet_host, &address, 2, 0);
}

Host& Host::operator=(const Host& other)
{
	enet_host_destroy(enet_host);
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	address = other.address;
	for (auto& peer : peers)
		peer = nullptr;

	peers[0] = enet_host_connect(enet_host, &address, 2, 0);

	return *this;
}

bool Host::connected() const
{
	for (auto* peer : peers)
		if (peer)
			return true;
		
	return false;
}

void Host::update(GameState& state)
{
	send(state);
	receive(state);
}

void Host::send(GameState& state)
{
	for (auto* peer : peers)
	{
		if (peer)
		{
			ENetPacket* enet_packet =
				enet_packet_create(&state, 
				sizeof(GameState) + 1,
				ENET_PACKET_FLAG_UNSEQUENCED
				| ENET_PACKET_FLAG_NO_ALLOCATE);
			enet_peer_send(peer, 0, enet_packet);
		}
	}
}

void Host::receive(GameState& state)
{
	ENetEvent event;
	while (enet_host_service(enet_host, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE: 
			state = *reinterpret_cast<GameState*>(event.packet->data);		
			break;
		case ENET_EVENT_TYPE_CONNECT: connect(event); break;
		case ENET_EVENT_TYPE_DISCONNECT: disconnect(event); break;
		}
		enet_packet_destroy(event.packet);
	}
}

void Host::connect(const ENetEvent& event)
{
	std::cout << "working";
	++num_players;
	for (auto& peer : peers)
		if (peer == nullptr)
			peer = event.peer;
}

void Host::disconnect(const ENetEvent& event)
{
	--num_players;
	for (auto& peer : peers)
		if (peer == event.peer)
			peer = nullptr;
}


}

