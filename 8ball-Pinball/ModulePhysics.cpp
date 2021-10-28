#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

// Tell the compiler to reference the compiled Box2D libraries
#ifdef _DEBUG
	#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
	#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// Initialise all the internal class variables, at least to NULL pointer
	world = NULL;
	ground = NULL;
	mouse_joint = NULL;
	mouse_body = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
	// You should do some memory cleaning here, if required
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// Create a new World
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// Set this module as a listener for contacts
	world->SetContactListener(this);

	// Create the main static ground of the scenario: a big circle in the middle of the screen
	CreateScenarioGround();

	// Create a static, shapeless ground body
	// This will be used to create joints like a mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd); // Add the static ground body to the World

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	// Step (update) the World
	// WARNING: WE ARE STEPPING BY CONSTANT 1/60 SECONDS!
	world->Step(1.0f / 60.0f, 6, 2);

	// Because Box2D does not automatically broadcast collisions/contacts with sensors, 
	// we have to manually search for collisions and "call" the equivalent to the ModulePhysics::BeginContact() ourselves...
	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		// For each contact detected by Box2D, see if the first one colliding is a sensor
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			// If so, we call the OnCollision listener function (only of the sensor), passing as inputs our custom PhysBody classes
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	// Keep playing
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	// Activate or deactivate debug mode
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	// If we are not in debug mode, do nothing else here and Keep playing
	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				// test if the current body contains mouse position
				b2Vec2 p = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
				if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
				{

					// If a body was selected we will attach a mouse joint to it
					// so we can pull it around

					// TODO 2: If a body was selected, create a mouse joint
					// using mouse_joint class property

					// NOTE: you do TODO2 here or also in the original handout's location. 
					// It doesn't matter unless you are triggering several objects at once;
					// I leave it to you to add safety checks to avoid re-defining several mouse joints.

					// The variable "b2Body* mouse_body;" is defined in the header ModulePhysics.h 
					// We need to keep this body throughout several game frames; you cannot define it as a local variable here. 
					mouse_body = b;

					// Get current mouse position
					b2Vec2 mousePosition;
					mousePosition.x = p.x;
					mousePosition.y = p.y;

					// Define new mouse joint
					b2MouseJointDef def;
					def.bodyA = ground; // First body must be a static ground
					def.bodyB = mouse_body; // Second body will be the body to attach to the mouse
					def.target = mousePosition; // The second body will be pulled towards this location
					def.dampingRatio = 0.5f; // Play with this value
					def.frequencyHz = 2.0f; // Play with this value
					def.maxForce = 200.0f * mouse_body->GetMass(); // Play with this value

					// Add the new mouse joint into the World
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}
		}
	}



	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (mouse_body != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			// Get new mouse position and re-target mouse_joint there
			b2Vec2 mousePosition;
			mousePosition.x = PIXEL_TO_METERS(App->input->GetMouseX());
			mousePosition.y = PIXEL_TO_METERS(App->input->GetMouseY());
			mouse_joint->SetTarget(mousePosition);

			// Draw a red line between both anchor points
			App->renderer->DrawLine(METERS_TO_PIXELS(mouse_body->GetPosition().x), METERS_TO_PIXELS(mouse_body->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
		}
	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (mouse_body != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		{
			// Tell Box2D to destroy the mouse_joint
			world->DestroyJoint(mouse_joint);

			// DO NOT FORGET THIS! We need it for the "if (mouse_body != nullptr && mouse_joint != nullptr)"
			mouse_joint = nullptr; 
			mouse_body = nullptr;
		}
	}

	// Keep playing
	return UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void ModulePhysics::CreateScenarioGround()
{
	// Get coordinates of the screen center and radius
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	// Create a static body in the middle of the screen
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add this static body to the World
	b2Body* big_ball = world->CreateBody(&body);

	// Create a big circle shape
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	// Create a fixture and associate the circle to it
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add the ficture (plus shape) to the static body
	big_ball->CreateFixture(&fixture);
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	// Return our PhysBody class
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	// Return our PhysBody class
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE (small "box" rectangle is ok; otherwise create whatever you need)
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true; // Set this fixture as SENSOR type

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	// Return our PhysBody class
	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];
	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.CreateLoop(p, size / 2);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Clean-up temp array
	delete p;

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	// Return our PhysBody class
	return pbody;
}

// Callback function to collisions with Box2D
void ModulePhysics::BeginContact(b2Contact* contact)
{
	// Call the OnCollision listener function to bodies A and B, passing as inputs our custom PhysBody classes
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();
	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);
	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}


// PHYS BODY FUNCTIONS -------------------------------------------------------------------------------

PhysBody::PhysBody() : listener(NULL), body(NULL)
{
	// Initialize all internal class variables
	width = height = 0;
	body = NULL;
	listener = NULL;
}

PhysBody::~PhysBody()
{
	// Destroy the associated Box2D body
	body->GetWorld()->DestroyBody(body);
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}
