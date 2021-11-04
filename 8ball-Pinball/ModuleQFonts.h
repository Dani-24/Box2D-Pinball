#ifndef __MODULE_QFONTS_H__
#define __MODULE_QFONTS_H__

#include "Module.h"
#include "SDL_ttf/include/SDL_ttf.h"

class ModuleQFonts :public Module {
public:
	ModuleQFonts(Application* app, bool start_enabled = true);

	~ModuleQFonts();

	int LoadFont(const char* font_path, uint size = 16);

	void UnloadFont();

	void RenderText(const char* textToRender,int x, int y, int r = 0, int g = 0, int b = 0);
	
private:
	TTF_Font* font;
};

#endif