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
	UnloadFont();
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
		lilFont = TTF_OpenFont(fontPath, 30);

		if (!lilFont) {
			LOG("Error loading little font || TTF_OpenFont: %s", TTF_GetError());
		}
	}
}

void ModuleQFonts::RenderText(const char* textToRender, int x, int y, char size[10], int r , int g , int b) {
	// Text Color
	SDL_Color color = { r,g,b };

	// by default use font at normal size
	whichOne = font;

	if (size == "chikita") {
		whichOne = lilFont;
	}

	// Create the text on surface
	if (!(fontSurface = TTF_RenderText_Solid(whichOne, textToRender, color))) {	// Cambiar Solid por Blended para probar
		LOG("Error Rendering Text || TTF_OpenFont: %s", TTF_GetError());
	}
	else {

		// Transform the text surface to texture
		fontTexture = SDL_CreateTextureFromSurface(App->renderer->renderer, fontSurface);
		
		// Draw the text at X, Y
		App->renderer->Blit(fontTexture, x, y);
	}

	SDL_FreeSurface(fontSurface);
}

void ModuleQFonts::UnloadFont()
{
	TTF_CloseFont(font);
	TTF_CloseFont(lilFont);

	font = NULL; // to be safe..
	lilFont = NULL;

	whichOne = NULL;

	LOG("Fonts unloaded");
}

bool ModuleQFonts::CleanUp() {

	// clean & free memory

	if (fontSurface != nullptr) {
		SDL_FreeSurface(fontSurface);
	}

	App->textures->Unload(fontTexture);

	TTF_Quit();

	return true;
}