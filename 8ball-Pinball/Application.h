#pragma once

#include "p2List.h"
#include "Globals.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFade;
class ModulePlayer;
class ModuleSceneIntro;
class ModuleSceneTitle;
class ModulePhysics;
class ModuleFonts;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFade* fade;
	ModulePlayer* player;
	ModuleSceneTitle* scene_title;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
	ModuleFonts* fonts;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};