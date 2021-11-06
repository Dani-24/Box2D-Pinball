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

	bool changeSize = false;

	if (theSize != size) {
		changeSize = true;
	}

	if (size == "chikita") {
		wFont = lilFont;
	}
	else {
		// by default use font at normal size
		wFont = font;
	}

	// Create the text on surface
	if (!(fontSurface = TTF_RenderText_Blended(wFont, textToRender, color))) {	// Blended sale menos pixelado q Solid
		LOG("Error Rendering Text || TTF_OpenFont: %s", TTF_GetError());
	}
	else {
		if (fontTexture == nullptr || changeSize == true) {
			changeSize = false;
			// Transform the text surface to texture
			fontTexture = SDL_CreateTextureFromSurface(App->renderer->renderer, fontSurface);
			LOG("Surface to Texture");
		}
		else {
			SDL_UpdateTexture(fontTexture, nullptr, fontSurface->pixels, fontSurface->pitch);
			LOG("Updating Texture");
		}

		// Draw the text at X, Y
		App->renderer->Blit(fontTexture, x, y);
	}
	SDL_FreeSurface(fontSurface);
}

void ModuleQFonts::UnloadFont()
{
	TTF_CloseFont(font);
	TTF_CloseFont(lilFont);

	App->textures->Unload(fontTexture);

	font = lilFont = wFont = NULL; // to be safe..

	LOG("Fonts unloaded");
}

bool ModuleQFonts::CleanUp() {

	TTF_Quit();

	return true;
}