#include "Application.h"
#include "ModuleQFonts.h"
#include "Globals.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModuleQFonts::ModuleQFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleQFonts::~ModuleQFonts()
{
}

bool ModuleQFonts::Init() {
	LOG("Init Module Fonts");
	if (TTF_Init() == -1) {
		LOG("Fonts can't initialize || TTF_Init: %s", TTF_GetError());
		return false;
	}
	return true;
}

bool ModuleQFonts::Start() {

	LOG("Starting Fonts Module");

	// Load Font
	//font = TTF_OpenFont("pinball/font/Paintball.ttf", 30);
	//lilFont = TTF_OpenFont("pinball/font/Paintball.ttf", 20);

	return true;
}

void ModuleQFonts::LoadFont(const char* fontPath, char size[10]) {
	if (size != "chikita") {
		font = TTF_OpenFont(fontPath, 30);

		if (!font) {
			LOG("Error loading font || TTF_OpenFont: %s", TTF_GetError());
		}
	}
	else {
		lilFont = TTF_OpenFont(fontPath, 20);

		if (!lilFont) {
			LOG("Error loading little font || TTF_OpenFont: %s", TTF_GetError());
		}
	}
	LOG("Fonts loaded");
}

void ModuleQFonts::RenderText(const char* textToRender, int x, int y, char size[10], Uint8 r , Uint8 g , Uint8 b) {
	// Text Color
	color = { r,g,b };

	// by default use font at normal size
	whichOne = font;

	if (size == "chikita") {
		whichOne = lilFont;
	}
	SDL_Surface* fontSurface;
	// Create the text on surface
	if (!(fontSurface = TTF_RenderText_Blended(whichOne, textToRender, color))) {	// Blended sale menos pixelado q Solid
		LOG("Error Rendering Text || TTF_OpenFont: %s", TTF_GetError());
	}
	else {

		// Transform the text surface to texture
		SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(App->renderer->renderer, fontSurface);
		
		// Draw the text at X, Y
		App->renderer->Blit(fontTexture, x, y);
	}
	SDL_FreeSurface(fontSurface);
}

void ModuleQFonts::UnloadFont()
{
	TTF_CloseFont(font);
	TTF_CloseFont(lilFont);

	font = lilFont = whichOne = NULL; // to be safe..

	LOG("Fonts unloaded");
}

bool ModuleQFonts::CleanUp() {

	TTF_Quit();

	return true;
}