#pragma once
// usable packet types
enum PacketType
{
	Error = 0,		// when an error is received
	Connect = 1,	// when a user is connected
	Disconnect = 2, // when a user is disconnected
	Message = 3,	// when a message is send
	Custom = 4		// when a custom message is send
};

class Packet
{
public:
	Packet(PacketType type);

	// adds an char to the packet
	void AddChar(const char c);
	// adds an int to the packet
	void AddInt(const int i);
	// adds an float to the packet
	void AddFloat(const float f);
	// adds an string to the packet
	void AddString(const std::string str);

	// gets the build packet
	char* GetPacket();
	//gets the lenght of the packet
	int GetLength();
private:
	// the data of the build packet
	vector<char> m_dataArr;
};

