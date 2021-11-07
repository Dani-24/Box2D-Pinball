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

	void ChangeMusic();
	void CumberFx();

public:

	// -------- HUD -----------------------------

	int score;
	int lives;
	int lastFrameLives;
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

	// Lives
	SDL_Texture* liveTexture;

	// Spring
	SDL_Texture* kicker;
	SDL_Texture* spring;
	SDL_Texture* springBase;
	SDL_Texture* springParticles;

	// Background
	SDL_Texture* tableroBG;
	SDL_Texture* tableroNoBG;
	SDL_Texture* tableroParticles;
	float scrollerBG[2];
	SDL_Texture* bgRed;

	SDL_Texture* tunel;

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

	// Cumber
	SDL_Texture* cumber;
	SDL_Texture* dialogTexture;

	// -------- Animation --------

	// BG
	Animation redBgAnim;

	// Ball
	Animation ballLightAnim;

	// Lives
	Animation livesAnim1, livesAnim2, livesAnim3;

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

	// CqCumber
	Animation cumberAnim;
	Animation* currentDialog;

	Animation dialog1, dialog2, dialog3, dialog4, dialog5, dialog6, dialog7, dialog8, dialog9, dialog10, dialog11, dialog12;

	int contDialog;
	bool gameInit;
	int wDialog = 0;

	// -------- FX --------
	uint collision1Fx;
	uint collision2Fx;
	uint collision3Fx;
	uint collision4Fx;
	uint collision5Fx;
	uint spawnFx;

	uint kickerInitFx;
	uint kickerBurstFx;

	uint ptsFx2;
	uint ptsFx3;
	uint bounceFx;

	uint bumperfx;
	uint bumperMovefx; // Not used bc i don't have more time
	uint bumperStopfx;

	uint pausefx;
	uint flipperfx;

	int music;
	bool lastMusic;
	bool lastMusicLastFrame;

	uint deathfx;

	// Spring
	uint springChargeFx;
	uint springReleaseFx;

	// Cumber

	uint cumberfx1, cumberfx2, cumberfx3, cumberfx4, cumberfx5; // I was going to call this cumFx but it doesn't sound too much family friendly

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