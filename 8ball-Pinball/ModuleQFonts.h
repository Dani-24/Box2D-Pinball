#ifndef __MODULE_QFONTS_H__
#define __MODULE_QFONTS_H__

#include "Module.h"
#include "SDL_ttf/include/SDL_ttf.h"

class ModuleQFonts :public Module {
public:
	ModuleQFonts(Application* app, bool start_enabled = true);

	~ModuleQFonts();

	bool Init();

	bool Start();

	void LoadFont(const char* fontPath, char size[10]);

	void UnloadFont();

	void RenderText(const char* textToRender,int x, int y, char size[10] = "normal", Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

	bool CleanUp();
	
private:
	TTF_Font* font;
	TTF_Font* lilFont;
	TTF_Font* whichOne;

	SDL_Color color;
};

#endif