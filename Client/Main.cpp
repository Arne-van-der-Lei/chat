//client
#include "stdafx.h"
#include "Packets.h"
#include "Graphic.h"
#include "Main.h"

int main(int argc, char *argv[])
{
	// starts up the main running loop
	Graphic::GetInstance()->Initilize();
	
	bool quit = false;
	SDL_Event e{};
	while (!quit)
	{
		// Poll next event from queue
		while (SDL_PollEvent(&e) != 0)
		{
			// Handle the polled event
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				//game.ProcessKeyDownEvent(e.key);
				break;
			case SDL_KEYUP:
				Graphic::GetInstance()->keyUpEvent(e.key);
				break;
			case SDL_MOUSEMOTION:
				//game.ProcessMouseMotionEvent(e.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				//game.ProcessMouseDownEvent(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				//game.ProcessMouseUpEvent(e.button);
				break; 
			case SDL_TEXTINPUT:
				Graphic::GetInstance()->textInputEvent(e.text);
					break;
			case SDL_TEXTEDITING:
					Graphic::GetInstance()->textEditingEvent(e.edit);
					break;
			}
		}

		Graphic::GetInstance()->redraw();
	}

	Graphic::GetInstance()->Cleanup();
	return 0;
}