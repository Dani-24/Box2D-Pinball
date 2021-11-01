#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class ModuleSceneTitle : public Module
{
public:

	// Constructors & Destructors
	ModuleSceneTitle(Application* app, bool start_enabled);
	~ModuleSceneTitle();

	// Main module steps
	bool Start();
	update_status Update();
	bool CleanUp();

public:

	// Textures
	SDL_Texture* octoling;

	// Animation
	Animation octoAnim;

	// FX

};
