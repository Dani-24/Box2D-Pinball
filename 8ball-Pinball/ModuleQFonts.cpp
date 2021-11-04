#include "Application.h"
#include "ModuleQFonts.h"
#include "Globals.h"
#include "ModuleRender.h"

ModuleQFonts::ModuleQFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleQFonts::~ModuleQFonts()
{

}

int ModuleQFonts::LoadFont(const char* font_path, uint size){

	int id = -1;
	
	font = TTF_OpenFont(font_path, size);

	if (!font) {
		LOG("Error loading font || TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
		return id;
	}
	return id;
}

void ModuleQFonts::UnloadFont() {

	// free the font

	TTF_CloseFont(font);
	font = NULL; // to be safe..

	LOG("Font cleaned");
}

void ModuleQFonts::RenderText(const char* textToRender, int x, int y, int r , int g , int b ) {

	SDL_Color color = { r,g,b };
	SDL_Surface* text_surface;
	if (!(text_surface = TTF_RenderText_Solid(font, textToRender, color))) {	// Cambiar Solid por Blended para probar
		LOG("Error Rendering Text || TTF_OpenFont: %s\n", TTF_GetError());
	}
	else {

		App->renderer->Blit((SDL_Texture*)text_surface, x, y);
	}
}