#pragma once
#include "Singleton.h"

enum PacketType
{
	Error = 0,		// when an error is received
	Connect = 1,	// when a user is connected
	Disconnect = 2, // when a user is disconnected
	Message = 3,	// when a message is send
	Custom = 4		// when a custom message is send
};

struct float3
{
	float3(float x, float y, float z) :x(x), y(y), z(z) {};
	float3() {}
	float x, y, z;
};

class Packets : public Singleton<Packets>
{
public:
	std::string GetString(SOCKET conn);
	int GetInt(SOCKET conn);
	float GetFloat(SOCKET conn);
	float3 GetFloat3(SOCKET conn);
	PacketType GetPacketType(SOCKET conn);

	bool SetString(const std::string str, SOCKET conn);
	bool SetInt(const int str, SOCKET conn);
	bool SetFloat(const float str, SOCKET conn);
	bool SetFloat3(const float3 str, SOCKET conn);
	bool SetPacketType(const PacketType str, SOCKET conn);

	bool ProcessPacket(const PacketType type, SOCKET conn);
	bool BroadCastString(const std::string str, SOCKET conn);
	bool BroadCast(std::function<bool(SOCKET)> callback, SOCKET conn);
	void addSocket(SOCKET conn);
	void removeSocket(SOCKET conn);
	void ShowBetterError(int error, std::string at);

	int getLength();
private:
	std::list<SOCKET> m_arrSockets;
};

