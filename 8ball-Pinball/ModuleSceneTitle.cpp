#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneTitle.h"
#include "ModuleFade.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"

ModuleSceneTitle::ModuleSceneTitle(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	currentState = MENU;
}

ModuleSceneTitle::~ModuleSceneTitle()
{
}

bool ModuleSceneTitle::Start()
{
	LOG("Loading Title assets uwu");

	currentState = MENU;

	// Load Textures
	octoling = App->textures->Load("pinball/sprites/octoling.png");

	// Load Sprite Animations

	int N = 0;
	for (int i = 0; i < 30; i++) {
		octoAnim.PushBack({ N,0,540,540 });
		N += 540;
	}
	octoAnim.speed = 0.25f;
	octoAnim.loop = true;

	// Load Audio

	App->audio->PlayMusic("pinball/audio/music_title.ogg",0.5f);

	// Load Font
	
	char fontTable[] = { "abcdefghijklmnopqrstuvwxyz " };
	char fontTableCaps[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	char fontTableNum[] = { "0123456789" };
	char fontTableEx[] = { " .:,; (*!? }^)#${ % &-+@ " };

	//char testing[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };

	titleFont = App->fonts->Load("pinball/font/font.png", fontTable, 1);

	return true;
}

update_status ModuleSceneTitle::Update()
{
	switch (currentState)
	{
	case MENU:
		// Change Current State
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
			currentState = SETTINGS;
		}
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
			currentState = SCORES;
		}

		// --- UPDATE ---
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			App->fade->FadeToBlack(this, (Module*)App->scene_intro, 60);
		}
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP) {
			return UPDATE_STOP;
		}

		// --- PRINT ---

		octoAnim.Update();
		SDL_Rect rect = octoAnim.GetCurrentFrame();

		App->renderer->Blit(octoling, 0, 230, &rect);

		// Fonts

		App->fonts->BlitText(220, 600, titleFont, "a esta bcdefghijklmnopqrstuvwxyz");

		break;
	case SETTINGS:
		// Change Current State
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
			currentState = MENU;
		}
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
			currentState = SCORES;
		}

		// --- UPDATE ---


		// --- PRINT ---


		break;
	case SCORES:
		// Change Current State
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
			currentState = SETTINGS;
		}
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
			currentState = MENU;
		}

		// --- UPDATE ---


		// --- PRINT ---


		break;
	}

	return UPDATE_CONTINUE;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	App->textures->Unload(octoling);
	octoling = nullptr;

	App->fonts->UnLoad();

	octoAnim.DeleteAnim();
	return true;
}
