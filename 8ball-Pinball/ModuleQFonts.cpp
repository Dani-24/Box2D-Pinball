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
	font = TTF_OpenFont("pinball/font/Paintball.ttf", size);

	if (!font) {
		LOG("Error loading font || TTF_OpenFont: %s", TTF_GetError());

		return false;
	}

	return true;
}

void ModuleQFonts::RenderText(const char* textToRender, int x, int y, int r , int g , int b ) {

	// Text Color
	SDL_Color color = { r,g,b };

	// Create the text on surface
	if (!(fontSurface = TTF_RenderText_Solid(font, textToRender, color))) {	// Cambiar Solid por Blended para probar
		LOG("Error Rendering Text || TTF_OpenFont: %s", TTF_GetError());
	}
	else {

		// Transform the text surface to texture
		fontTexture = SDL_CreateTextureFromSurface(App->renderer->renderer, fontSurface);
		
		// Draw the text at X, Y
		App->renderer->Blit(fontTexture, x, y);
	}
}

void ModuleQFonts::UnloadFont()
{
	// clean & free memory

	SDL_FreeSurface(fontSurface);

	App->textures->Unload(fontTexture);

	TTF_CloseFont(font);
	font = NULL; // to be safe..

	TTF_Quit();

	LOG("Font cleaned");
}