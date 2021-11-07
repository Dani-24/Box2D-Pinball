#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

enum GameState {
	MENU,
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

	p2List<int> scores;

	// Textures
	SDL_Texture* titleTexture;

	SDL_Texture* cursor;
	int cursorX, cursorY;

	SDL_Texture* octoling;
	SDL_Texture* bg;
	SDL_Texture* bgPart;

	SDL_Texture* metro;

	SDL_Texture* scrollerTexture;
	float scroller[2];

	float partScroll[2];

	// Animation
	Animation octoAnim;
	Animation metroAnim;

	int metroX, metroY;
	bool metroMoving;

	// FX
	uint scorefx;
	uint acceptfx;
	uint backfx;
	uint selectfx;
	uint metroFx;

	int increase = 0;

	// Font
	char githubLink[100];

	char scorePosition[100];

};
