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
		App->audio->ChangeVolume(20);


		// unpause
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			App->scene_intro->UnPauseGame();
			currentState = DISABLED;
		}
		
		// --- Draw the stuff ---

		//App->qfonts->drawText("GAME PAUSED", 242, 500);

		break;
	case GAMEOVER:
		// --- Update ---
		App->audio->ChangeVolume(20);

		// Reset scene
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			LOG("Reset scene");
			// Quit Scene menu
			currentState = DISABLED;

			// "Save" score if reset
			App->scene_title->scores.add(App->scene_intro->score);
			LOG("Saving Score %d", App->scene_intro->score);

			// Reset Scene intro
			App->fade->FadeToBlack((Module*)App->scene_intro, (Module*)App->scene_intro, 1);

		}

		// --- Draw ---

		
		break;
	case DISABLED:
		App->audio->ChangeVolume();

		// Do nothing bc this module is """""Disabled""""
		break;
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneMenu::CleanUp()
{
	LOG("Unloading menu scene");


	return true;
}
