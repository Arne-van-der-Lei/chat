#include "stdafx.h"
#include "Graphic.h"
#include "Packets.h"
#include "TextRenderer.h"

SOCKET Graphic::m_connecection = 0;

void Graphic::redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity(); // Reset The Current Modelview Matrix

	m_pTextRenderer->addText(" ", 0, 0, vec3(0, 1, 0));

	if (m_username == "")
	{
		UsernameInput();
	}
	else if (m_connecection == 0)
	{
		doLogin();
	}

	m_pTextRenderer->addText(m_username + "> "+ m_currentLine, 0, 41, vec3(0, 1, 0));
	int i = 0;
	Packets::GetInstance()->getMessages([this,&i](string message) { m_pTextRenderer->addText(message, 0, 40 - i, vec3(0, 1, 0)); i++; });
	
	if (m_Enter)
	{
		Packet p = Packet(PacketType::Message);
		p.AddString(m_username + "> " + m_currentLine);
		Packets::GetInstance()->addMessage(m_username + "> " + m_currentLine);
		m_currentLine = "";
		m_Enter = false;
		Packets::GetInstance()->SendPacket(p,m_connecection);
	}
	
	
	m_pTextRenderer->Draw();

	SDL_GL_SwapWindow(m_pWindow);
}

//returns the current written line
string Graphic::getCurrentLine()
{
	return m_currentLine;
}

// clears the current written line
void Graphic::clearCurrentLine()
{
	m_currentLine = "";
}

// adds a character to the current written line 
void Graphic::addCharacter(char chr)
{
	m_currentLine.append(chr + "");
}

// sets the username of the current user
void Graphic::setUserName(string str)
{
	m_username = str;
}

void Graphic::UsernameInput()
{
	m_pTextRenderer->addText("username:", 0, 0, vec3(0, 1, 0));
	m_pTextRenderer->addText(m_currentLine, 10, 0, vec3(0, 1, 0));

	if (m_Enter)
	{
		setUserName(m_currentLine);
		m_currentLine = "";
		m_Enter = false;
	}

}

void Graphic::doLogin()
{
	m_pTextRenderer->addText("adress:", 0, 0, vec3(0, 1, 0));
	m_pTextRenderer->addText(m_currentLine, 10, 0, vec3(0, 1, 0));

	if (!m_Enter)return;

	m_Enter = false;
	string adress = m_currentLine;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		wcout << L"SocketUtil::WSAStart()" << endl;
	}

	sockaddr_in addr;
	int addrLen = sizeof(addr);

#ifdef _DEBUG
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#else
	addr.sin_addr.s_addr = inet_addr(adress.data());
#endif // debug

	addr.sin_port = htons(48000);
	addr.sin_family = AF_INET;

	m_connecection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(m_connecection, (SOCKADDR*)&addr, addrLen) == SOCKET_ERROR)
	{
		wcout << L"Failed to connect" << endl;
		Packets::GetInstance()->ShowBetterError(WSAGetLastError());
	}
	else
	{
		wcout << L"connected" << endl;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Graphic::receiveLoop, NULL, NULL, NULL);
	}

}

void Graphic::Initilize()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_Init: " << SDL_GetError() << std::endl;
		return;
	}

	// Use OpenGL 4.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Create window
	m_pWindow = SDL_CreateWindow(
		"chat",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int(800),
		int(800),
		SDL_WINDOW_OPENGL);

	if (m_pWindow == nullptr)
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_CreateWindow: " << SDL_GetError() << std::endl;
		return;
	}

	// Create OpenGL context 
	m_Context = SDL_GL_CreateContext(m_pWindow);
	if (m_Context == nullptr)
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
		return;
	}

	// Set the swap interval for the current OpenGL context,
	// synchronize it with the vertical retrace
	SDL_GL_SetSwapInterval(0);

	// Set the Projection matrix to the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	gluOrtho2D(0, 800, 0, 800); // y from bottom to top

													   // Set the viewport to the client window area
													   // The viewport is the rectangular region of the window where the image is drawn.
	glViewport(0, 0, int(800), int(800));

	// Set the Modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cerr << "Core::Initialize( ), error when calling IMG_Init: " << IMG_GetError() << std::endl;
		return;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "Core::Initialize( ), error when calling TTF_Init: " << TTF_GetError() << std::endl;
		return;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cerr << "Core::Initialize( ), error when calling Mix_OpenAudio: " << Mix_GetError() << std::endl;
		return;
	}

	glewInit();
	
	m_pTextRenderer = new TextRenderer();
}

void Graphic::Cleanup()
{
	SDL_GL_DeleteContext(m_Context);

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//receaves internet messages
void Graphic::receiveLoop()
{
	while (true)
	{
		if (!Packets::GetInstance()->ProcessPacket(Packets::GetInstance()->GetPacketType(m_connecection), m_connecection))
		{
			break;
		}
	}

	closesocket(m_connecection);
}

void Graphic::textInputEvent(SDL_TextInputEvent key)
{
	m_currentLine.append(key.text);
}

void Graphic::textEditingEvent(SDL_TextEditingEvent key)
{

}

void Graphic::keyUpEvent(SDL_KeyboardEvent key)
{
	if (key.keysym.sym == SDLK_RETURN)
	{
		m_Enter = true;
	}
}