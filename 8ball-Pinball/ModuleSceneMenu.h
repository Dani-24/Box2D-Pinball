#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

enum MenuState {
	PAUSE,
	GAMEOVER,
	DISABLED
};

class ModuleSceneMenu : public Module
{
public:
	// Constructors & Destructors
	ModuleSceneMenu(Application* app, bool start_enabled = true);
	~ModuleSceneMenu();

	// Main module steps
	bool Start();
	update_status Update();
	bool CleanUp();

public:
	MenuState currentState;

	// Textures

	SDL_Texture* prueba;

	SDL_Texture* gO;

	// Animation


	// FX


	// Font

};
