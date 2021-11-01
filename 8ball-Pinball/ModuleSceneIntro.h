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
	ModuleSceneIntro(Application* app, bool start_enabled);
	~ModuleSceneIntro();

	// Main module steps
	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void BallManager();
	void CreateBG();

	void CreateSpring();

public:

	// Lists of physics objects
	p2List<PhysBody*> balls;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* tableroColliders;

	PhysBody* springTop;
	PhysBody* springBot;

	int springForce = 0;

	// Textures
	SDL_Texture* ball;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* tableroBG;
	SDL_Texture* tableroNoBG;
	SDL_Texture* tableroParticles;

	float scrollerBG[2];

	// Animation

	Animation* currentAnim;

	Animation ballLightAnim;

	int N = 38;	// Ball Sprite Coord.

	// FX
	uint collision1Fx;
	uint collision2Fx;
	uint collision3Fx;
	uint collision4Fx;
	uint collision5Fx;

	int music;

	// Raycast
	p2Point<int> ray;
	bool ray_on;


};
