#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneMenu.h"
#include "ModuleFade.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleQFonts.h"
#include "ModuleSceneIntro.h"
#include "ModuleSceneTitle.h"

ModuleSceneMenu::ModuleSceneMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneMenu::~ModuleSceneMenu()
{
}

bool ModuleSceneMenu::Start()
{
	LOG("Loading Scene Menu assets :P");

	currentState = DISABLED;

	// Load Textures

	prueba = App->textures->Load("pinball/sprites/octoling.png");
	gO = App->textures->Load("pinball/sprites/cursor.png");

	// Load Sprite Animations


	// Load Audio


	// Assign Text

	return true;
}

update_status ModuleSceneMenu::Update()
{
	switch (currentState)	
	{
	case PAUSE:
		// --- DO stuff ---
		// unpause
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			App->scene_intro->UnPauseGame();
			currentState = DISABLED;
		}
		
		// --- Draw the stuff ---
		App->renderer->Blit(prueba, 0, 230);

		break;
	case GAMEOVER:
		// --- Update ---

		// Reset scene
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {

			LOG("Reset scene");
			// Quit Scene menu
			currentState = DISABLED;

			// "Save" score if reset
			App->scene_title->scores.add(App->scene_intro->score);
			LOG("Saving Score %d", App->scene_intro->score);

			// Reset Scene intro
			App->fade->FadeToBlack((Module*)App->scene_intro, (Module*)App->scene_intro, 60);

		}

		// --- Draw ---
		App->renderer->Blit(gO, 0, 230);
		
		break;
	case DISABLED:
		// Do nothing bc this module is """""Disabled""""
		break;
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneMenu::CleanUp()
{
	LOG("Unloading menu scene");

	App->textures->Unload(prueba);
	App->textures->Unload(gO);

	return true;
}