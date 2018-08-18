#include "stdafx.h"
#include "Packets.h"

std::string Packets::GetString(SOCKET connection)
{
	int length = GetInt(connection);
	char* buffer = new char[length+1];
	if (recv(connection, buffer, length, NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError(),"Packets::GetString");
		removeSocket(connection);
		return "";
	}
	buffer[length] = '\0';
	std::string str = std::string(buffer);
	delete[] buffer;
	return str;
}

int Packets::GetInt(SOCKET connection)
{
	char length;
	if (recv(connection, (char*)&length, sizeof(char), NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError(), "Packets::GetInt");
		removeSocket(connection);
		return 0;
	}
	return length;
}

float Packets::GetFloat(SOCKET conn)
{
	float length;
	if (recv(conn, (char*)&length, sizeof(int), NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError(), "Packets::GetFloat");
		removeSocket(conn);
		return 0;
	}
	return length;
}

float3 Packets::GetFloat3(SOCKET conn)
{
	return float3(GetFloat(conn),GetFloat(conn),GetFloat(conn));
}

PacketType Packets::GetPacketType(SOCKET connection)
{
	return (PacketType)GetInt(connection);
}

bool Packets::SetString(std::string str, SOCKET connection)
{
	SetInt(str.length(),connection);
	if (send(connection, str.c_str(), str.length(), NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError(), "Packets::SetString");
		removeSocket(connection);
		return false;
	}
	return true;
}

bool Packets::SetInt(int str, SOCKET connection)
{
	if(send(connection, (char*)&str, sizeof(int),NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError(), "Packets::SetInt");
		removeSocket(connection);
		return false;
	}
	return true;
}

bool Packets::SetFloat(const float str, SOCKET conn)
{
	if (send(conn, (char*)&str, sizeof(int), NULL) == SOCKET_ERROR)
	{
		ShowBetterError(WSAGetLastError(), "Packets::SetFloat");
		removeSocket(conn);
		return false;
	}
	return true;
}

bool Packets::SetFloat3(const float3 str, SOCKET conn)
{
	return SetFloat(str.x, conn) && SetFloat(str.y, conn) && SetFloat(str.z, conn);
}

bool Packets::SetPacketType(PacketType str, SOCKET connection)
{
	return SetInt(str, connection);
}

bool Packets::ProcessPacket(PacketType type, SOCKET conn)
{
	switch (type)
	{
	case Message:
		return BroadCastString(GetString(conn), conn);
		break;
	case PacketType::Custom:
	{
		return true;
	}
		break;
	default:
		break;
	}
	return true;
}

bool Packets::BroadCastString(std::string str, SOCKET conn)
{
	int i = 0;
	for (auto i = m_arrSockets.begin(); i != m_arrSockets.end(); ++i)
	{
		if (conn == *i)continue;

		if (!Packets::GetInstance()->SetPacketType(Message, *i))
		{
			Packets::GetInstance()->removeSocket(conn);
			continue;
		}
		if (!Packets::GetInstance()->SetString(str, *i))
		{
			Packets::GetInstance()->removeSocket(conn);
			return false;
		}
	}
	std::cout << str << std::endl;
	return true;
}

bool Packets::BroadCast(std::function<bool(SOCKET)> callback, SOCKET conn)
{
	int i = 0;
	for (auto i = m_arrSockets.begin(); i != m_arrSockets.end(); ++i)
	{
		if (conn == *i)continue;

		if(!callback(*i)){
			Packets::GetInstance()->removeSocket(conn);
			return false;
		}
	}
	return true;
}

void Packets::addSocket(SOCKET conn)
{
	m_arrSockets.push_back(conn);
}

void Packets::removeSocket(SOCKET conn)
{
	closesocket(conn);
	m_arrSockets.remove(conn);
}

void Packets::ShowBetterError(int error, std::string at)
{
	std::cout << "error: ";
	switch (error)
	{
	case WSAETIMEDOUT:
		std::cout << "Connection timed out." << " " << at << std::endl;
		break;
	case WSAECONNRESET:
		std::cout << "Connection closed." << " " << at << std::endl;
		break;
	case WSAENOTSOCK:
		std::cout << "Socket operation on nonsocket." << " " << at << std::endl;
	default:
		std::cout << error << std::endl;
		break;
	}
}

int Packets::getLength()
{
	return m_arrSockets.size();
}
