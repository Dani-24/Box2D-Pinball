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

	void LoadFont(const char* fontPath);

	void UnloadFont();

	void drawText(const char* textToRender, int x, int y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

private:
	void RenderText(const char* textToRender,int x, int y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

public:
	bool CleanUp();
	
private:
	SDL_Color color;

	TTF_Font* font;
	TTF_Font* lilFont;

	SDL_Texture* fontTexture;
	SDL_Surface* fontSurface;

	TTF_Font* wFont;

	int theS;
};

#endif