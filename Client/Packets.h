#pragma once
#include "Singleton.h"
#include "Packet.h"

class Packets : public Singleton<Packets>
{
public:
	// gets a string from the connection
	string GetString(SOCKET conn);
	// gets a integer from the connection
	int GetInt(SOCKET conn);
	// gets a float from the connection
	float GetFloat(SOCKET conn);
	// get the pocket type from the connection
	PacketType GetPacketType(SOCKET conn);

	// sends a packet
	bool SendPacket(Packet packet, SOCKET conn);
	// processes a packet
	bool ProcessPacket(const PacketType type, SOCKET conn);

	// gets all the current messages
	void getMessages(function<void(string)> function);
	// adds a message to the message list 
	void addMessage(string str);
	// show a better error message than the error code 
	void ShowBetterError(int error);
private:

	// the list of currently saved messages
	list<string> m_arrMessages;
};

