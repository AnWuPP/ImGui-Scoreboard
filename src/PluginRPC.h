#pragma once

#include "PlayerList.h"
#include <RakNet/BitStream.h>

class PluginRPC {
    PlayerList& playerList;
public:
    PluginRPC(PlayerList& _playerList);
    bool onJoinClient(unsigned char& _id, RakNet::BitStream* bs);
    bool onQuitClient(unsigned char& _id, RakNet::BitStream* bs);
    bool onConnectionAccept(Packet *p);
};