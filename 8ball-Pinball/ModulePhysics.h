#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody();
	~PhysBody();

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	
	// Constructors & Destructors
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	// Main module steps
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// Create main ground
	void CreateScenarioGround();

	// Create basic physics objects
	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	// Debug mode
	bool debug;

	// Box2D World
	b2World* world;

	// Main ground
	b2Body* ground;

	// Mouse joint
	b2MouseJoint* mouse_joint;
	b2Body* mouse_body;
};