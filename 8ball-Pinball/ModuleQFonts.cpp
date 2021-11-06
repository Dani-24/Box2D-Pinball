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

	fontTexture = nullptr;

	return true;
}

bool ModuleQFonts::Start() {

	LOG("Starting Fonts Module");
	return true;
}

// For the future Dani, i know you will come back there to take this code for another game, BUT remember, this code is made as bad as you could do it.
// If you declare more fonts you can load them at LoadFont() with their size stuff.
// And renderText(), fix this shit pls, make not necesary to send an "                 " char every time you call Fonts.
// SDL_UpdateTexture and SDL_FreeSurface free memory on each loop.

void ModuleQFonts::LoadFont(const char* fontPath) {
	font = TTF_OpenFont(fontPath, 30);
	if (!font) {
		LOG("Error loading font || TTF_OpenFont: %s", TTF_GetError());
	}
	else {
		LOG("Fonts loaded");
	}
}

void ModuleQFonts::RenderText(const char* textToRender, int x, int y, Uint8 r , Uint8 g , Uint8 b) {
	// Text Color
	color = { r,g,b };

	// Create the text on surface
	if (!(fontSurface = TTF_RenderText_Blended(font, textToRender, color))) {	// Blended sale menos pixelado q Solid
		LOG("Error Rendering Text || TTF_OpenFont: %s", TTF_GetError());
	}
	else {
		if (fontTexture == nullptr) {
			// Transform the text surface to texture
			fontTexture = SDL_CreateTextureFromSurface(App->renderer->renderer, fontSurface);
			LOG("Surface to Texture");
		}
		else {
			SDL_UpdateTexture(fontTexture, nullptr, fontSurface->pixels, fontSurface->pitch);
		}

		// Draw the text at X, Y
		App->renderer->Blit(fontTexture, x, y);
	}
	SDL_FreeSurface(fontSurface);
}

void ModuleQFonts::UnloadFont()
{
	TTF_CloseFont(font);

	App->textures->Unload(fontTexture);

	font = NULL; // to be safe..

	LOG("Fonts unloaded");
}

bool ModuleQFonts::CleanUp() {

	TTF_Quit();

	return true;
}