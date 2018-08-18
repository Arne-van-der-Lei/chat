//server
#include "stdafx.h"
#include "Packets.h"


void clientMessages(SOCKET connection)
{
	while (true)
	{
		if (!Packets::GetInstance()->ProcessPacket(Packets::GetInstance()->GetPacketType(connection), connection))
		{
			break;
		}
	}
	closesocket(connection);
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(WINSOCK_VERSION, &wsaData))
	{
		std::wcout << L"SocketUtil::WSAStart()" << std::endl;
	}

	sockaddr_in addr;
	int addrLen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	addr.sin_port = htons(48000);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if(bind(sListen, (SOCKADDR*)&addr, addrLen) == SOCKET_ERROR)std::cout << "bind error: " << WSAGetLastError() << std::endl;
	if(listen(sListen, SOMAXCONN)== SOCKET_ERROR)std::cout << "listen error: " << WSAGetLastError() << std::endl;

	while (true)
	{
		SOCKET sock = accept(sListen, (SOCKADDR*)&addr, &addrLen);

		if (sock == 0)
		{
			std::wcout << L"connection failed" << std::endl;
			std::wcout << WSAGetLastError() << std::endl;
		}
		else
		{
			Packets::GetInstance()->addSocket(sock);
			std::wcout << L"connection made " << sock << std::endl;
			Packets::GetInstance()->SetPacketType(Connect, sock);
			Packets::GetInstance()->SetString("Welcome to the Shack",sock);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientMessages, (LPVOID)sock, NULL, NULL);
			Packets::GetInstance()->BroadCastString("user connected",sock);
		}
	}

	
	std::cin.get();
	return 0;
}