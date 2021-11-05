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

	void UnloadFont();

	void RenderText(const char* textToRender,int x, int y, int r = 255, int g = 255, int b = 255);
	
private:
	TTF_Font* font;

	SDL_Surface* fontSurface;

	SDL_Texture* fontTexture;

	int size = 30;
};

#endif