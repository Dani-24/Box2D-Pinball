#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneTitle.h"
#include "ModuleFade.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleQFonts.h"

#include <sstream>
#include <string.h>
using namespace std;

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

	music = false;
	ManageMusic();

	scorefx = App->audio->LoadFx("pinball/audio/fx/openScore.wav");

	acceptfx = App->audio->LoadFx("pinball/audio/fx/fxAccept.wav");
	backfx = App->audio->LoadFx("pinball/audio/fx/fxBack.wav");
	selectfx = App->audio->LoadFx("pinball/audio/fx/fxSelect.wav");

	// Assign Text

	strcpy_s(textPlay, "Play");
	strcpy_s(textSettings, "Settings");
	strcpy_s(textScores, "Scores");

	strcpy_s(githubLink, "https://github.com/Dani-24/Box2D-Pinball");

	return true;
}

void ModuleSceneTitle::ManageMusic() {
	if (music == false) {
		music = true;
		switch (currentState)
		{
		case MENU:
			App->audio->PlayMusic("pinball/audio/music_title.ogg", 0.5f);
			break;
		case SETTINGS:
			App->audio->PlayMusic("pinball/audio/music_settings.ogg", 0.5f);
			break;
		case SCORES:
			break;
		}
	}
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
			App->audio->PlayFx(selectfx);
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && cursorY != 500) {
			cursorY -= 75;
			App->audio->PlayFx(selectfx);
		}

		// State and Scene management
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 575) {
			currentState = SETTINGS;
			App->audio->PlayFx(acceptfx);
			music = false;
			ManageMusic();
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 650) {
			currentState = SCORES;
			App->audio->PlayFx(scorefx);
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 500) {
			App->audio->PlayFx(acceptfx);
			App->fade->FadeToBlack(this, (Module*)App->scene_intro, 30);
		}
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			App->audio->PlayFx(backfx);
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
		App->qfonts->RenderText(textSettings, 210, 575);
		App->qfonts->RenderText(textScores, 225, 650);

		break;
	case SETTINGS:
		// Return back to menu
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			currentState = MENU;

			App->audio->PlayFx(backfx);
			music = false;
			ManageMusic();
		}

		// --- UPDATE ---


		// --- PRINT ---

		break;
	case SCORES:
		// Return
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			currentState = MENU;

			App->audio->PlayFx(backfx);

		}

		// --- UPDATE ---


		// --- PRINT ---

		octoAnim.Update();
		SDL_Rect rect = octoAnim.GetCurrentFrame();

		App->renderer->Blit(octoling, 0, 230, &rect);

		// Score list
		int N = 1, y = 200;

		p2List_item<int>* c = scores.getFirst();
		if (c == NULL) {
			strcpy_s(scorePosition, "There is no Score registed");
			App->qfonts->RenderText(scorePosition, 75, y);
		}
		else {
			strcpy_s(scorePosition, "Score from lastest Games");
			App->qfonts->RenderText(scorePosition, 75, y - 150);
		}
		while (c != NULL)
		{

			// print nums:
			stringstream strs, strs2;
			strs << N;
			string temp_str = strs.str();
			char* numToChar = (char*)temp_str.c_str();

			strcpy_s(scorePosition, "Game ");	// Add "Score "  to scorePosition

			strcat_s(scorePosition, numToChar); // Add "N "  to scorePosition

			strs2 << c->data;
			string temp_str2 = strs2.str();
			char* numToChar2 = (char*)temp_str2.c_str();

			strcat_s(scorePosition, " : "); // Add " : "  to scorePosition

			strcat_s(scorePosition, numToChar2); // Add " c->data "  to scorePosition

			App->qfonts->RenderText(scorePosition, 150, y); // Print scorePosition

			LOG(scorePosition);

			N++;
			y += 50;
			c = c->next;
		}

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

	scorefx = acceptfx = selectfx = backfx = 0;

	return true;
}