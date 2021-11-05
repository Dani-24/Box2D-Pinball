#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFade.h"
#include "ModuleQFonts.h"

#include <sstream>
#include <string.h>
using namespace std;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// Initialise all the internal class variables, at least to NULL pointer
	ball = NULL;
	ray_on = false;

	count = 0;
	dir = true;
	axis = true;
	moveX = true;
	changes = 0;
	flipperforce = -250;
	springForce = 0;

	N = 38;	// Ball Sprite width.
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	// You should do some memory cleaning here, if required
}

bool ModuleSceneIntro::Start()
{
	LOG("Loading Gameplay assets :D");
	bool ret = true;

	// --- Set camera position ---
	App->renderer->camera.x = App->renderer->camera.y = 0;

	// --- Load textures & configure each item with methods bc it looks cool --- 
	BallManager();
	CreateSpring();
	CreateBG();
	CreateFlippers();
	CreateSensors();
	CreateBumpers();

	// --- Load Audio ---

	// Randomize Music
	srand(SDL_GetTicks());
	music = rand() % 3;

	switch (music)
	{
	case 0:
		App->audio->PlayMusic("pinball/audio/music_#8_Regret.ogg");
		break;
	case 1:
		App->audio->PlayMusic("pinball/audio/music_#9_Party.ogg");
		break;
	case 2:
		App->audio->PlayMusic("pinball/audio/music_#11_Above.ogg");
		break;
	}

	// Fx

	collision1Fx = App->audio->LoadFx("pinball/audio/fx/collider1.wav");
	collision2Fx = App->audio->LoadFx("pinball/audio/fx/collider2.wav");
	collision3Fx = App->audio->LoadFx("pinball/audio/fx/collider3.wav");
	collision4Fx = App->audio->LoadFx("pinball/audio/fx/collider4.wav");
	collision5Fx = App->audio->LoadFx("pinball/audio/fx/collider5.wav");

	return ret;
}

void ModuleSceneIntro::BallManager() {

	ball = App->textures->Load("pinball/sprites/8ball.png");

	// Ball light Animation
	
	for (int i = 0; i < 21; i++) {
		if (i == 0) {
			ballLightAnim.PushBack({ 0,0,N,N });
			ballLightAnim.PushBack({ N+4,0,N,N });
			ballLightAnim.PushBack({ 0,0,N,N });
			ballLightAnim.PushBack({ N+4,0,N,N });
			ballLightAnim.PushBack({ 0,0,N,N });
		}
		ballLightAnim.PushBack({ 0,0,N,N });
	}

	ballLightAnim.loop = true;
	ballLightAnim.speed = 0.15f;
}

void ModuleSceneIntro::CreateSpring()
{
	// --- Spring Texture and Animation ---
	spring = App->textures->Load("pinball/sprites/fishSpring.png");
	springBase = App->textures->Load("pinball/sprites/fishBase.png");
	springParticles = App->textures->Load("pinball/sprites/fishSpringParticle.png");

	springAnim.PushBack({ 0,0,N,N });
	springAnim.PushBack({ N,0,N,N });
	springAnim.PushBack({ 2*N,0,N,N });
	springAnim.PushBack({ 3*N,0,N,N });
	springAnim.loop = false;
	springAnim.speed = 0.15f;

	for (int i = 0; i < 9; i++) {
		springExplosionAnim.PushBack({ i*2*N,0,2*N,2*N });
	}
	springExplosionAnim.PushBack({ 0,0,0,0 });
	springExplosionAnim.loop = false;
	springExplosionAnim.speed = 0.5f;

	// --- Spring Physics ---
	springTop = App->physics->CreateRectangle(512, 700, 37, 10);
	springBot = App->physics->CreateRectangle(512, 755, 38, 10);
	springBot->body->SetType(b2_staticBody);

	// Create a Joint to Join the top and the bot from the spring
	b2DistanceJointDef SpringJointDef;

	SpringJointDef.bodyA = springTop->body;
	SpringJointDef.bodyB = springBot->body;

	SpringJointDef.localAnchorA.Set(0, 0);
	SpringJointDef.localAnchorB.Set(0, 0);

	SpringJointDef.length = 1.5f;

	SpringJointDef.collideConnected = true;

	SpringJointDef.frequencyHz = 7.0f;
	SpringJointDef.dampingRatio = 0.05f;

	b2PrismaticJoint* SpringJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&SpringJointDef);
}

void ModuleSceneIntro::CreateBG() {

	// Load BG texture
	tableroBG = App->textures->Load("pinball/sprites/background/tableroBG.png");
	tableroNoBG = App->textures->Load("pinball/sprites/background/tableroSinBG.png");
	tableroParticles = App->textures->Load("pinball/sprites/background/particulasBG.png");

	// BG Collider Chains
	int tableroExterno[96] = {
	492, 760,
	531, 760,
	531, 215,
	529, 193,
	523, 166,
	515, 143,
	502, 117,
	485, 94,
	466, 77,
	445, 67,
	421, 61,
	393, 60,
	157, 60,
	139, 63,
	115, 72,
	93, 85,
	59, 60,
	24, 90,
	56, 119,
	43, 133,
	31, 152,
	24, 169,
	21, 186,
	21, 417,
	22, 428,
	26, 443,
	35, 457,
	62, 482,
	38, 506,
	27, 520,
	22, 531,
	20, 549,
	20, 760,
	67, 760,
	67, 720,
	172, 760,
	172, 848,
	277, 848,
	277, 760,
	381, 720,
	381, 760,
	428, 760,
	428, 506,
	407, 489,
	456, 449,
	456, 296,
	492, 310,
	492, 755
	};
	int tableroTrianguloIz[10] = {
	117, 629,
	117, 622,
	152, 646,
	152, 652,
	119, 630
	};
	int tableroTrianguloDer[10] = {
	294, 646,
	294, 652,
	329, 629,
	329, 623,
	296, 645
	};
	int tableroColchonetaDer[20] = {
	286, 283,
	286, 215,
	289, 212,
	292, 212,
	295, 215,
	295, 288,
	264, 310,
	259, 309,
	258, 304,
	283, 286
	};
	int tableroEsponja[30] = {
	96, 446,
	127, 415,
	125, 413,
	96, 443,
	73, 420,
	66, 410,
	63, 402,
	62, 397,
	62, 377,
	60, 377,
	60, 397,
	62, 407,
	67, 416,
	74, 424,
	93, 443
	};
	int tableroInterruptoresTop[26] = {
	266, 100,
	182, 116,
	182, 146,
	184, 145,
	184, 140,
	223, 133,
	223, 138,
	225, 137,
	225, 132,
	264, 124,
	264, 129,
	266, 128,
	266, 102
	};
	int tableroCarrilDer[16] = {
	294, 698,
	294, 702,
	380, 669,
	380, 589,
	375, 587,
	370, 589,
	370, 669,
	296, 697
	};
	int tableroCarrilIz[16] = {
	67, 669,
	67, 589,
	72, 587,
	77, 589,
	77, 669,
	150, 698,
	150, 702,
	70, 670
	};
	int tableroBloqueDer[24] = {
	323, 417,
	323, 352,
	326, 346,
	332, 342,
	340, 341,
	348, 342,
	361, 348,
	386, 361,
	409, 373,
	409, 420,
	367, 455,
	327, 420
	};
	int tableroCurvaDer[36] = {
	492, 264,
	492, 228,
	487, 198,
	479, 173,
	464, 145,
	446, 125,
	428, 113,
	405, 105,
	386, 104,
	386, 128,
	395, 130,
	412, 140,
	426, 152,
	440, 172,
	450, 196,
	455, 220,
	456, 246,
	488, 262
	};
	int tableroInicioRailes[48] = {
	66, 308,
	66, 214,
	72, 214,
	72, 224,
	76, 234,
	84, 241,
	93, 242,
	102, 241,
	110, 234,
	114, 224,
	115, 214,
	121, 214,
	121, 283,
	147, 304,
	146, 307,
	141, 308,
	117, 289,
	103, 291,
	89, 301,
	82, 313,
	103, 332,
	102, 336,
	97, 337,
	68, 310
	};

	// BG Collider Chains Creation
	tableroColliders[0] = App->physics->CreateSolidChain(0, 0, tableroExterno, 96);
	tableroColliders[1] = App->physics->CreateSolidChain(0, 0, tableroTrianguloIz, 10);
	tableroColliders[2] = App->physics->CreateSolidChain(0, 0, tableroTrianguloDer, 10);
	tableroColliders[3] = App->physics->CreateSolidChain(0, 0, tableroColchonetaDer, 20);
	tableroColliders[4] = App->physics->CreateSolidChain(0, 0, tableroEsponja, 30);
	tableroColliders[5] = App->physics->CreateSolidChain(0, 0, tableroInterruptoresTop, 26);
	tableroColliders[6] = App->physics->CreateSolidChain(0, 0, tableroCarrilDer, 16);
	tableroColliders[7] = App->physics->CreateSolidChain(0, 0, tableroCarrilIz, 16);
	tableroColliders[8] = App->physics->CreateSolidChain(0, 0, tableroBloqueDer, 24);
	tableroColliders[9] = App->physics->CreateSolidChain(0, 0, tableroCurvaDer, 36);
	tableroColliders[10] = App->physics->CreateSolidChain(0, 0, tableroInicioRailes, 48);

	// Set Scroll X distance
	scrollerBG[0] = 0;
	scrollerBG[1] = 552;
}

void ModuleSceneIntro::CreateFlippers() {
	// Flippers (not dolphins) x, y, width and height
	int x1 = 150;
	int y1 = 700;

	int x2 = 295;
	int y2 = 700;

	int w = 53;
	int h = 10;

	// --- Left flipper ---
	flipperLeft = App->physics->CreateRectangle(x1, y1, w, h);
	flipperLeftPoint = App->physics->CreateCircle(x1, y2, 2);
	flipperLeftPoint->body->SetType(b2_staticBody);

	// Flipper Joint (flipper rectangle x flipper circle to give it some movement)
	b2RevoluteJointDef flipperLeftJoint;

	flipperLeftJoint.bodyA = flipperLeft->body;
	flipperLeftJoint.bodyB = flipperLeftPoint->body;
	flipperLeftJoint.referenceAngle = 0 * DEGTORAD;
	flipperLeftJoint.enableLimit = true;
	flipperLeftJoint.lowerAngle = -30 * DEGTORAD;
	flipperLeftJoint.upperAngle = 30 * DEGTORAD;
	flipperLeftJoint.localAnchorA.Set(PIXEL_TO_METERS(-33), 0);
	flipperLeftJoint.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_leftFlipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperLeftJoint);

	// --- Right flipper ---
	flipperRight = App->physics->CreateRectangle(x2, y2, w, h);
	flipperRightPoint = App->physics->CreateCircle(x2, y2, 2);
	flipperRightPoint->body->SetType(b2_staticBody);

	// Flipper Joint
	b2RevoluteJointDef flipperRightJoint;

	flipperRightJoint.bodyA = flipperRight->body;
	flipperRightJoint.bodyB = flipperRightPoint->body;
	flipperRightJoint.referenceAngle = 0 * DEGTORAD;
	flipperRightJoint.enableLimit = true;
	flipperRightJoint.lowerAngle = -30 * DEGTORAD;
	flipperRightJoint.upperAngle = 30 * DEGTORAD;
	flipperRightJoint.localAnchorA.Set(PIXEL_TO_METERS(33), 0);
	flipperRightJoint.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_rightFlipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperRightJoint);

}

void ModuleSceneIntro::CreateSensors() {
	// --- Sensors that move the ball or something like that: ---

	// Side Kickers
	rightSideKicker = App->physics->CreateRectangleSensor(42, 755, 40, 10);
	leftSideKicker = App->physics->CreateRectangleSensor(404, 755, 40, 10);

	// Pads
	leftPad = App->physics->CreateRectangleSensor(0, 0, 34, 5);
	rightPad = App->physics->CreateRectangleSensor(0, 0, 34, 5);

	b2Vec2 posLeftPad(5.4f, 5.85f);	// X, Y, and Angle to aply a rotation.... Idk what i'm doing but it works  || Ahh bueno que son metros y no pixeles y me da palo cambiarlo :D
	rightPad->body->SetTransform(posLeftPad, -0.64f);

	b2Vec2 posRightPad(2.65f, 5.8f);
	leftPad->body->SetTransform(posRightPad, 0.64f);

	// Platforms just above the flippers
	int LplatX = 134;
	int LplatY = 635;
	int RplatX = 312;
	int RplatY = 635;

	leftPlat = App->physics->CreateRectangleSensor(LplatX, LplatY, 40, 5);
	rightPlat = App->physics->CreateRectangleSensor(RplatX, RplatY, 40, 5);
	b2Vec2 posLPlat(PIXEL_TO_METERS(LplatX), PIXEL_TO_METERS(LplatY));
	b2Vec2 posRPlat(PIXEL_TO_METERS(RplatX), PIXEL_TO_METERS(RplatY));

	leftPlat->body->SetTransform(posLPlat, 0.6f); // who uses radiants having degrees... 90 degrees all the word knows what it means... but 90 radiants??? wtf are 90 radiants?
	rightPlat->body->SetTransform(posRPlat, -0.6f);

	// Anti flying-bc-yes-ball sensor	(Idk why the ball gets bugged with Bumpers so i put some of these force generator where it happens and solved :)
	int leftAntiBugPadX = 300 - 4;
	int leftAntiBugPadY = 250 - 7;
	antiBugPad = App->physics->CreateRectangleSensor(leftAntiBugPadX, leftAntiBugPadY, 60, 1);

	b2Vec2 posLAntiB(PIXEL_TO_METERS(leftAntiBugPadX), PIXEL_TO_METERS(leftAntiBugPadY));

	antiBugPad->body->SetTransform(posLAntiB, 1.5708f); // 90 degrees


	// --- Sensors that just do what a sensor do ---

}

void ModuleSceneIntro::CreateBumpers() {
	bumperTopX = 355;
	bumperMidX = 380;
	bumperTopY = 190;
	bumperMidY = 280;
	bumperTop = App->physics->CreateCircularBumper(bumperTopX, bumperTopY, 20);
	bumperMid = App->physics->CreateCircularBumper(bumperMidX, bumperMidY, 20);
}

update_status ModuleSceneIntro::PreUpdate() 
{
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP) {
		App->fade->FadeToBlack(this, (Module*)App->scene_title, 60);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::Update()
{
	// --- Bumpers Movement ---

	// Just move the bumpers and then wait 2 sec (if 60 fps) to move them back
	float vel = 0.4f;

	if ( bumperTopX < 400 && dir == true && moveX == true) {
		bumperTopX += vel;
		bumperMidX -= vel;
	}
	else if (bumperTopX > 340 && dir == false && moveX == true) {
		bumperTopX -= vel;
		bumperMidX += vel;
	}
	else if (bumperTopY < 280 && dir == true &&  moveX == false) {
		bumperTopY += vel;
		bumperMidY -= vel;
	}
	else if (bumperTopY > 190 && dir == false && moveX == false) {
		bumperTopY -= vel;
		bumperMidY += vel;
	}
	else {
		count++;
		if (count > 60) {
			count = 0;
			moveX = !moveX;
			changes++;
		}
		// that's just to make the movement cicle and don't move back by the same path
		if (changes >= 3) {	
			changes = 0;
			dir = !dir;
			axis = !axis;
			moveX = !moveX;
		}
	}

	// Change Bumpers X
	b2Vec2 pos1 = b2Vec2(PIXEL_TO_METERS(bumperTopX), PIXEL_TO_METERS(bumperTopY));
	bumperTop->body->SetTransform(pos1, 0);
	b2Vec2 pos2 = b2Vec2(PIXEL_TO_METERS(bumperMidX), PIXEL_TO_METERS(bumperMidY));
	bumperMid->body->SetTransform(pos2, 0);

	// --- Spring ---
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (springForce < 300) {
			springForce += 10;
		}
		springTop->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);

		springAnim.Update();
		expl = false;
		springExplosionAnim.Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		springForce = 0;
		
		springAnim.Reset();
		expl = true;
	}

	// --- Flippers ---
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		flipperLeft->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		flipperRight->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
	}

	// --- Ball Generator ---
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		LOG("Creating 8ball at: X = %d Y = %d", App->input->GetMouseX(), App->input->GetMouseY());

		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), (N/2)));

		// If Box2D detects a collision with this last generated circle, it will automatically callback the function ModulePhysics::BeginContact()
		balls.getLast()->data->listener = this;
	}

	// --------------------------- All draw functions -------------------------------------

	// --- Background ---
	App->renderer->Blit(tableroBG, 0, 0);

	// BG Scrolling:
	for (int i = 0; i < 2; i++) {
		scrollerBG[i] -= 0.3f;
		if (scrollerBG[i] < -552) {
			scrollerBG[i] = 552;
		}
		App->renderer->Blit(tableroParticles, scrollerBG[i], 0);
	}

	App->renderer->Blit(tableroNoBG, 0, 0);

	// --- Balls ---
	// Update Animations 
	ballLightAnim.Update();
	SDL_Rect rect = ballLightAnim.GetCurrentFrame();

	p2List_item<PhysBody*>* c = balls.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(ball, x, y, &rect, 1.0f, c->data->GetRotation());

		c = c->next;
	}

	// --- Spring ---

	SDL_Rect rect1 = springAnim.GetCurrentFrame();
	SDL_Rect rect2 = springExplosionAnim.GetCurrentFrame();


	App->renderer->Blit(spring, 493, 710, &rect1);
	App->renderer->Blit(springBase, 493, 721);

	if (expl == true) {
		springExplosionAnim.Update();
		App->renderer->Blit(springParticles, 474, 691, &rect2);
	}


	// --- Fonts ---

	// Score int -> const char*
	stringstream strs;
	strs << score;
	string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();

	// Change X position to fit better with the HUD 
	if (score < 10){
		App->qfonts->RenderText(char_type, 195, 15);
	}
	else if (score < 1000) {
		App->qfonts->RenderText(char_type, 180, 15);
	}
	else if (score < 10000) {
		App->qfonts->RenderText(char_type, 170, 15);
	}
	else if (score < 100000) {
		App->qfonts->RenderText(char_type, 165, 15);
	}
	else{
		App->qfonts->RenderText(char_type, 150, 15);
	}


	// --- Raycast ------------------------------------------------------

	// If user presses SPACE, enable RayCast for no reason bc this truly do nothing more than printing a line xD
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Enable raycast mode
		ray_on = !ray_on;

		// Origin point of the raycast is be the mouse current position now (will not change)
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// The target point of the raycast is the mouse current position (will change over game time)
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Total distance of the raycast reference segment
	int ray_hit = ray.DistanceTo(mouse);

	// Declare a vector. We will draw the normal to the hit surface (if we hit something)
	fVector normal(0.0f, 0.0f);

	// Raycasts
	if (ray_on == true)
	{
		// Compute the vector from the raycast origin up to the contact point (if we're hitting anything; otherwise this is the reference length)
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		// Draw a line from origin to the hit point (or reference length if we are not hitting anything)
		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		// If we are hitting something with the raycast, draw the normal vector to the contact point
		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Play Audio FX on every collision, randomized to sound more natural
	int fx = rand() % 5;

	switch (fx)
	{
	case 0:
		App->audio->PlayFx(collision1Fx);
		break;
	case 1:
		App->audio->PlayFx(collision2Fx);
		break;
	case 2:
		App->audio->PlayFx(collision3Fx);
		break;
	case 3:
		App->audio->PlayFx(collision4Fx);
		break;
	case 4:
		App->audio->PlayFx(collision5Fx);
		break;
	}

	// If you want to see Sensor collisions check -> ModulePhysics -> BeginContact()
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading gameplay scene");

	// Clean animations
	ballLightAnim.DeleteAnim();
	springAnim.DeleteAnim();
	springExplosionAnim.DeleteAnim();

	// Clean Textures
	App->textures->Unload(ball);
	App->textures->Unload(spring);
	App->textures->Unload(springBase);
	App->textures->Unload(springParticles);
	App->textures->Unload(tableroBG);
	App->textures->Unload(tableroNoBG);
	App->textures->Unload(tableroParticles);

	// Clean fx:
	collision1Fx = collision2Fx = collision3Fx = collision4Fx = collision5Fx = springChargeFx = springReleaseFx = 0;

	// Clean physics

	for (int i = 0; i < 11; i++) {
		App->physics->world->DestroyBody(tableroColliders[i]->body);
	}

	p2List_item<PhysBody*>* c = balls.getFirst();
	while (c != NULL)
	{
		App->physics->world->DestroyBody(c->data->body);
		c = c->next;
	}
	balls.clear();

	App->physics->world->DestroyBody(springBot->body);
	App->physics->world->DestroyBody(springTop->body);
	App->physics->world->DestroyBody(flipperLeft->body);
	App->physics->world->DestroyBody(flipperRight->body);
	App->physics->world->DestroyBody(flipperLeftPoint->body);
	App->physics->world->DestroyBody(flipperRightPoint->body);
	App->physics->world->DestroyBody(leftSideKicker->body);
	App->physics->world->DestroyBody(rightSideKicker->body);
	App->physics->world->DestroyBody(leftPad->body);
	App->physics->world->DestroyBody(rightPad->body);
	App->physics->world->DestroyBody(leftPlat->body);
	App->physics->world->DestroyBody(rightPlat->body);
	App->physics->world->DestroyBody(antiBugPad->body);
	App->physics->world->DestroyBody(bumperTop->body);
	App->physics->world->DestroyBody(bumperMid->body);

	return true;
}
