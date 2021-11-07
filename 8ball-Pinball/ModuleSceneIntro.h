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
	
	void PauseGame();
	void UnPauseGame();

	void PlayPtsFx();
	void insertTextBcVisualBug();

	void ChangeMusic();

public:

	// -------- HUD -----------------------------

	int score;
	int lives;
	bool pause;
	int lastFrameScore;

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

	PhysBody* loseSensor;

	// Bumpers
	PhysBody* bumperTop;

	float bumperTopX;
	float bumperTopY;
	int count;
	bool dir;

	float bumperVel = 0.4f;

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

	// Bounce Pads
	SDL_Texture* bouncePad;
	SDL_Texture* bouncePadB;

	// Flipper
	SDL_Texture* flipper;
	SDL_Texture* flipper2;

	// Bumper
	SDL_Texture* bumperTexture;

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

	// Bumper

	Animation bumperAnim;

	// BouncePads

	Animation bounceAnim;
	Animation bounceAnim2;
	Animation bounceAnimB;
	Animation bounceAnimB2;
	bool pd1 = false, pd2 = false, pd3 = false, pd4 = false;

	// -------- FX --------
	uint collision1Fx;
	uint collision2Fx;
	uint collision3Fx;
	uint collision4Fx;
	uint collision5Fx;

	uint kickerInitFx;
	uint kickerBurstFx;

	uint ptsFx2;
	uint ptsFx3;
	uint bounceFx;

	uint bumperfx;
	uint bumperMovefx;
	uint bumperStopfx;

	uint pausefx;
	uint flipperfx;

	int music;
	bool lastMusic;
	bool lastMusicLastFrame;

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