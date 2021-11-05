#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

enum GameState {
	MENU,
	SETTINGS,
	SCORES
};

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
	GameState currentState;

	// Textures
	SDL_Texture* octoling;

	// Animation
	Animation octoAnim;

	// FX


	// Font
	
	int textSize = 16;

	char textPlay[10];
	
	char textSettings[10];

	char textScores[10];

	char githubLink[100];

};
