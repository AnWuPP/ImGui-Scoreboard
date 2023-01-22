#include "PluginRPC.h"
#include <d3d9.h>
#include <sampapi/CChat.h>
#include <string>
#include <format>

namespace samp = sampapi::v037r1;

template <typename T>
std::string readWithSize(RakNet::BitStream& bs) {
    T size;
    if (!bs.Read(size) || size <= 0)
        return {};
    std::string str(size, '\0');
    bs.Read(str.data(), size);
    return str;
}

template <typename T>
void writeWithSize(RakNet::BitStream& bs, std::string_view str) {
    T size = static_cast<T>(str.size());
    bs.Write(size);
    bs.Write(str.data(), size);
}

PluginRPC::PluginRPC(PlayerList& _playerList) : playerList(_playerList) { }

bool PluginRPC::onJoinClient(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 137) {
        return true;
    }

    uint16_t id;
    uint32_t color;
    char isNpc;
    std::string nickname;

    bs->Read(id);
    bs->Read(color);
    bs->Read(isNpc);
    nickname = readWithSize<char>(*bs);
    if (!isNpc && !playerList.IsPlayerExists(id)) {
        playerList.AddPlayer(id);
    }
    return true;
}

bool PluginRPC::onQuitClient(unsigned char& _id, RakNet::BitStream* bs) {
    if (_id != 138) {
        return true;
    }

    uint16_t id;
    char reason;

    bs->Read(id);
    bs->Read(reason);
    if (playerList.IsPlayerExists(id)) {
        playerList.RemovePlayer(id);
    }
    return true;
}

bool PluginRPC::onConnectionAccept(Packet* p) {
    RakNet::BitStream bs(p->data, p->length, false);
    uint8_t packetId;
    bs.Read(packetId);
    if (packetId != 34) {
        return true;
    }
    if (playerList.Count() > 0) {
        playerList.ClearList();
    }
    return true;
}