#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	// Initialise all the internal class variables, at least to NULL pointer
	ball = box = rick = NULL;
	ray_on = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	// You should do some memory cleaning here, if required
}

bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets :D");
	bool ret = true;

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Load textures
	BallManager();
	
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	CreateSpring();

	CreateBG();

	return ret;
}

void ModuleSceneIntro::BallManager() {
	ball = App->textures->Load("pinball/8ball.png");

	/*
	On :

	ballLightAnim.PushBack({0,0,N,N});

	Off :

	ballLightAnim.PushBack({N,0,N,N});

	*/
	
	// Order 1

	for (int i = 0; i < 21; i++) {
		if (i == 0) {
			ballLightAnim.PushBack({ 0,0,N,N });
			ballLightAnim.PushBack({ N+3,0,N,N });
			ballLightAnim.PushBack({ 0,0,N,N });
			ballLightAnim.PushBack({ N+3,0,N,N });
			ballLightAnim.PushBack({ 0,0,N,N });
		}
		ballLightAnim.PushBack({ 0,0,N,N });
	}

	ballLightAnim.loop = true;
	ballLightAnim.speed = 0.15f;
}

void ModuleSceneIntro::CreateSpring()
{
	springTop = App->physics->CreateRectangle(512, 700, 37, 10);
	springBot = App->physics->CreateRectangle(512, 755, 38, 10);
	springBot->body->SetType(b2_staticBody);
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
	tablero = App->textures->Load("pinball/tablero.png");

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
	int tableroTrianguloIz[8] = {
	118, 576,
	118, 630,
	152, 652,
	119, 578
	};
	int tableroTrianguloDer[8] = {
	329, 577,
	329, 629,
	294, 652,
	328, 579
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
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroExterno, 96);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroTrianguloIz, 8);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroTrianguloDer, 8);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroColchonetaDer, 20);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroEsponja, 30);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroInterruptoresTop, 26);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroCarrilDer, 16);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroCarrilIz, 16);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroBloqueDer, 24);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroCurvaDer, 36);
	tableroColliders = App->physics->CreateSolidChain(0, 0, tableroInicioRailes, 48);

}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (springForce < 300) {
			springForce += 10;
		}
		springTop->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		springForce = 0;
	}


	// If user presses SPACE, enable RayCast
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Enable raycast mode
		ray_on = !ray_on;

		// Origin point of the raycast is be the mouse current position now (will not change)
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// If user presses 1, create a new circle object
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), (N/2)));

		// Add this module (ModuleSceneIntro) as a "listener" interested in collisions with circles.
		// If Box2D detects a collision with this last generated circle, it will automatically callback the function ModulePhysics::BeginContact()
		balls.getLast()->data->listener = this;
	}

	// If user presses 2, create a new box object
	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	// If user presses 3, create a new RickHead object
	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	// The target point of the raycast is the mouse current position (will change over game time)
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Total distance of the raycast reference segment
	int ray_hit = ray.DistanceTo(mouse);

	// Declare a vector. We will draw the normal to the hit surface (if we hit something)
	fVector normal(0.0f, 0.0f);

	ballLightAnim.Update();
	SDL_Rect rect = ballLightAnim.GetCurrentFrame();

	// All draw functions ------------------------------------------------------

	App->renderer->Blit(tablero, 0, 0);

	// Balls
	p2List_item<PhysBody*>* c = balls.getFirst();
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(ball, x, y, &rect, 1.0f, c->data->GetRotation());

		c = c->next;
	}

	// Boxes
	c = boxes.getFirst();
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		// Always paint boxes texture
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());

		// Are we hitting this box with the raycast?
		if(ray_on)
		{
			// Test raycast over the box, return fraction and normal vector
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	// Rick Heads
	c = ricks.getFirst();
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// Raycasts -----------------
	if(ray_on == true)
	{
		// Compute the vector from the raycast origin up to the contact point (if we're hitting anything; otherwise this is the reference length)
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		// Draw a line from origin to the hit point (or reference length if we are not hitting anything)
		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		// If we are hitting something with the raycast, draw the normal vector to the contact point
		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	// Keep playing
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Play Audio FX on every collision, regardless of who is colliding
	App->audio->PlayFx(bonus_fx);

	// Do something else. You can also check which bodies are colliding (sensor? ball? player?)
}
