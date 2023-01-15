#pragma once

#include "PlayerList.h"
#include <RakNet/BitStream.h>

class PluginRPC {
    PlayerList& playerList;
public:
    PluginRPC(PlayerList& _playerList);
    bool onJoinClient(unsigned char& id, RakNet::BitStream* bs);
    bool onQuitClient(unsigned char& id, RakNet::BitStream* bs);
    bool onSetPlayerColor(unsigned char& id, RakNet::BitStream* bs);
    bool onSetPlayerName(unsigned char& id, RakNet::BitStream* bs);
    bool onUpdateScoresAndPings(unsigned char& id, RakNet::BitStream* bs);
};