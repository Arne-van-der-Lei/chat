#pragma once
#include "Singleton.h"

class TextRenderer;

class Graphic: public Singleton<Graphic>
{
public:
	//redraws the screen
	void redraw();
	//gets the current written line
	string getCurrentLine();
	// clears the current line
	void clearCurrentLine();

	// add a character to the written line
	void addCharacter(char chr);
	// sets the username of the user
	void setUserName(string str);

	//gets the username from input
	void UsernameInput();

	// handels login in
	void doLogin();

	// initilizes everything
	void Initilize();
	// destrois evrything
	void Cleanup();
	
	// text input event
	void textInputEvent(SDL_TextInputEvent key);

	// text editing event
	void textEditingEvent(SDL_TextEditingEvent key);

	// keyup event
	void keyUpEvent(SDL_KeyboardEvent key);

	// threaded function for receiving data
	static void receiveLoop();
private:
	string m_currentLine;
	string m_username;

	SDL_Window* m_pWindow;
	SDL_GLContext m_Context;

	TextRenderer* m_pTextRenderer;
	static SOCKET m_connecection;

	//is enter pressed
	bool m_Enter;
};