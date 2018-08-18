#include "stdafx.h"
#include "Packet.h"
#include "Packets.h"
#include "Graphic.h"

string Packets::GetString(SOCKET connection)
{
	int length = GetInt(connection);
	if (length <= 0) return "";
	char* buffer = new char[length+1];
	if (recv(connection, buffer, length, NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError());
		return "";
	}
	buffer[length] = '\0';
	string str = string(buffer);
	delete[] buffer;
	return str;
}

int Packets::GetInt(SOCKET connection)
{
	int length;
	if (recv(connection, (char*)&length, sizeof(int), NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError());
		return 0;
	}
	return length;
}

PacketType Packets::GetPacketType(SOCKET connection)
{
	return (PacketType)GetInt(connection);
}

bool Packets::SendPacket(Packet packet, SOCKET conn)
{
	if (send(conn, packet.GetPacket(), packet.GetLength() , NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError());
		return false;
	}
	return true;
}

bool Packets::ProcessPacket(PacketType type, SOCKET conn)
{
	switch (type)
	{
	case PacketType::Connect:
		m_arrMessages.push_back(GetString(conn));
		return true;
		break;
	case PacketType::Message :
		m_arrMessages.push_back(GetString(conn));
		Graphic::GetInstance()->redraw();
		return true;
		break;
	default:
		break;
	}
	return false;
}

void Packets::getMessages(function<void(string)> function )
{
	for_each(m_arrMessages.rbegin(), m_arrMessages.rend(), function);
}

void Packets::addMessage(string str)
{
	m_arrMessages.push_back(str);
	if (m_arrMessages.size() > 20)
		m_arrMessages.pop_front();
}

void Packets::ShowBetterError(int error)
{
	cout << "error: ";
	switch (error)
	{
	case WSAETIMEDOUT:
		cout << "Connection timed out." << endl;
		break;
	case WSAECONNABORTED:
		cout << "Connection abort.";
	default:
		cout << error << endl;
		break;
	}
}