#include "stdafx.h"
#include "Packet.h"

Packet::Packet(PacketType type)
{
	AddChar(type);
}

//adds an char to the packet
void Packet::AddChar(const char c)
{
	m_dataArr.push_back(c);
}

// adds an int to the packet
void Packet::AddInt(const int i)
{
	m_dataArr.push_back((char)i);
	m_dataArr.push_back((char)(i<<8));
	m_dataArr.push_back((char)(i<<16));
	m_dataArr.push_back((char)(i<<24));
}

// adds a float to the packet
void Packet::AddFloat(const float f)
{
	AddInt(static_cast<int>(f));
}

// adds a string to the packet (max length is 256)
void Packet::AddString(const std::string str)
{
	AddChar((char)str.size());												// adds the string length to the array
	std::for_each(str.begin(), str.end(), [this](char c) { AddChar(c); });	// adds the actual characters
}

char * Packet::GetPacket()
{
	return m_dataArr.data();
}

int Packet::GetLength()
{
	return (int)m_dataArr.size();
}
