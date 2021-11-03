#ifndef __MODULE_FONTS_H__
#define __MODULE_FONTS_H__

#include "Module.h"
#include "SDL/include/SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font {
	//Lookup table. all the player in the same order as the texture
	char table[MAX_FONT_CHARS];

	//FONT TEXTURE
	SDL_Texture* texture = nullptr;

	//font setup data

	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};

class ModuleFonts :public Module {
public:
	ModuleFonts(Application* app, bool start_enabled = true);

	~ModuleFonts();

	// loads a font file from a texture
	//returns a font index from  the fonts array
	//Parametre texturePath - the path to the texture file
	//parametre characters - the lookup table. all characters displayed in the same order as the texture
	//parametre rows - the amount of characters rows in the texture

	int Load(const char* texturePath, const char* characters, uint rows = 1);
	//removes a font by its index
	//unloads the texture and removes it from the fonts array
	void UnLoad(/*int fontIndex*/);

	void BlitText(int x, int y, int fontIndex, const char* text) const;
private:
	//array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];
};

#endif