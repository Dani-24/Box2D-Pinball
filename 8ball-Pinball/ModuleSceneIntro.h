#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:

	// Constructors & Destructors
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	// Main module steps
	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void BallManager();
	void CreateBG();

public:

	// Lists of physics objects
	p2List<PhysBody*> balls;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* tableroColliders;

	// Textures
	SDL_Texture* ball;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* tablero;

	// Animation

	Animation* currentAnim;

	Animation ballLightAnim;

	int N = 24;

	// FX
	uint bonus_fx;

	// Raycast
	p2Point<int> ray;
	bool ray_on;


};
