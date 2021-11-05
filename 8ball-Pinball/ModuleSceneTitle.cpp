#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneTitle.h"
#include "ModuleFade.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleQFonts.h"

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

	bg = App->textures->Load("pinball/sprites/background/tableroBG.png");
	bgPart = App->textures->Load("pinball/sprites/background/particulasBG.png");

	octoling = App->textures->Load("pinball/sprites/octoling.png");
	cursor = App->textures->Load("pinball/sprites/cursor.png");
	cursorX = 350;
	cursorY = 500;

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

	// Assign Text

	strcpy_s(textPlay, "Play");
	strcpy_s(textSettings, "Settings");
	strcpy_s(textScores, "Scores");

	strcpy_s(githubLink, "https://github.com/Dani-24/Box2D-Pinball");

	return true;
}

update_status ModuleSceneTitle::Update()
{
	// Print BG
	App->renderer->Blit(bg, 0, 0);
	App->renderer->Blit(bgPart, 0, 0);

	switch (currentState)
	{
	case MENU:
		// --- UPDATE ---

		// Cursor Movement
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && cursorY != 650) {
			cursorY += 75;
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && cursorY != 500) {
			cursorY -= 75;
		}

		// State and Scene management
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 575) {
			currentState = SETTINGS;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 650) {
			currentState = SCORES;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 500) {
			App->fade->FadeToBlack(this, (Module*)App->scene_intro, 60);
		}
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			return UPDATE_STOP;
		}

		// Open website
		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
			LOG("Opening Link : %s", githubLink);
			SDL_OpenURL(githubLink);
		}

		// --- PRINT ---

		// Cursor
		App->renderer->Blit(cursor, cursorX, cursorY);

		// Fonts
		App->qfonts->RenderText(textPlay, 242, 500);
		App->qfonts->RenderText(textScores, 225, 575);
		App->qfonts->RenderText(textSettings, 210, 650);

		break;
	case SETTINGS:
		// Return back to menu
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			currentState = MENU;
		}

		// --- UPDATE ---


		// --- PRINT ---


		break;
	case SCORES:
		// Return
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			currentState = MENU;
		}

		// --- UPDATE ---

		
		// --- PRINT ---

		octoAnim.Update();
		SDL_Rect rect = octoAnim.GetCurrentFrame();

		App->renderer->Blit(octoling, 0, 230, &rect);

		break;
	}

	return UPDATE_CONTINUE;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	App->textures->Unload(octoling);
	octoling = nullptr;

	octoAnim.DeleteAnim();
	return true;
}
