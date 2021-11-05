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

	// -------- Constructors & Destructors --------
	ModuleSceneIntro(Application* app, bool start_enabled);
	~ModuleSceneIntro();

	// -------- Main module steps --------
	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	// -------- Extra functions --------
	void BallManager();
	void CreateBG();
	void CreateSpring();
	void CreateFlippers();
	void CreateSensors();
	void CreateBumpers();

public:

	// -------- HUD -----------------------------

	int score;
	int lives;

	// -------- Lists of physics objects --------
	p2List<PhysBody*> balls;

	PhysBody* tableroColliders[11];

	// Spring
	PhysBody* springTop;
	PhysBody* springBot;
	int springForce;

	// Flippers
	PhysBody* flipperLeft;
	PhysBody* flipperRight;
	PhysBody* flipperLeftPoint;
	PhysBody* flipperRightPoint;
	int flipperforce;

	// Sensors
	PhysBody* leftSideKicker;
	PhysBody* rightSideKicker;

	PhysBody* leftPad;
	PhysBody* rightPad;

	PhysBody* leftPlat;
	PhysBody* rightPlat;

	PhysBody* antiBugPad;

	// Bumpers
	PhysBody* bumperTop;
	PhysBody* bumperMid; // jungler diff

	float bumperTopX;
	float bumperMidX;
	float bumperTopY;
	float bumperMidY;
	int count;
	bool dir;
	bool axis;
	bool moveX;
	int changes;

	// -------- Textures --------

	// Ball
	SDL_Texture* ball;

	// Spring
	SDL_Texture* spring;
	SDL_Texture* springBase;
	SDL_Texture* springParticles;

	// Background
	SDL_Texture* tableroBG;
	SDL_Texture* tableroNoBG;
	SDL_Texture* tableroParticles;

	float scrollerBG[2];

	// Fonts
	char textScore[100];

	p2List<char[100]> textDialogs;

	// -------- Animation --------

	// Ball
	Animation ballLightAnim;

	int N;	// Ball Sprite width.

	// Spring
	Animation springAnim;
	Animation springExplosionAnim;

	bool expl = false;

	// -------- FX --------
	uint collision1Fx;
	uint collision2Fx;
	uint collision3Fx;
	uint collision4Fx;
	uint collision5Fx;

	int music;

	// Spring
	uint springChargeFx;
	uint springReleaseFx;


	// -------- Raycast --------
	p2Point<int> ray;
	bool ray_on;
};

/*

 ,-.       _,---._ __  / \
 /  )    .-'       `./ /   \
(  (   ,'            `/    /|
 \  `-"             \'\   / |
  `.              ,  \ \ /  |
   /`.          ,'-`----Y   |
  (            ;        |   '
  |  ,-.    ,-'         |  /
  |  | (   |        hjw | /
  )  |  \  `.___________|/
  `--'   `--'

  Meow 

  */