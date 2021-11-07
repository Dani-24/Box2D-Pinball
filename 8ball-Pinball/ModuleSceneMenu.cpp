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
	menuBg = App->textures->Load("pinball/sprites/background/menuBG.png");

	// Load Sprite Animations


	// Load Audio

	gameOverFx = App->audio->LoadFx("pinball/audio/fx/gameOverFx.wav");
	playGOFx = true;

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
		App->renderer->Blit(menuBg,65,315);

		App->qfonts->drawText("GAME PAUSED", 160, 350);
		App->qfonts->drawText("Press Space to continue", 80, 400);

		break;
	case GAMEOVER:
		// --- Update ---
		App->audio->ChangeVolume(10);
		if (playGOFx == true) {
			//App->audio->PlayFx(gameOverFx);	// it doesn't fit there without animations :c
			playGOFx = false;
		}

		App->renderer->Blit(menuBg, 50, 320);

		App->qfonts->drawText("GAME OVER", 165, 350);
		App->qfonts->drawText("Press R to play again", 82, 400);

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
		playGOFx = true;
		// Do nothing bc this module is """""Disabled""""
		break;
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneMenu::CleanUp()
{
	LOG("Unloading menu scene");

	gameOverFx = 0;

	return true;
}
