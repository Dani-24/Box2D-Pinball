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

	scroller[0] = 0;
	scroller[1] = -1500;
	partScroll[0] = 0;
	partScroll[1] = 552;

	// Load Textures

	bg = App->textures->Load("pinball/sprites/background/tableroBG.png");
	bgPart = App->textures->Load("pinball/sprites/background/particulasBG.png");

	octoling = App->textures->Load("pinball/sprites/octoling.png");
	cursor = App->textures->Load("pinball/sprites/cursor.png");
	cursorX = 350;
	cursorY = 425;

	metro = App->textures->Load("pinball/sprites/metro.png");
	scrollerTexture = App->textures->Load("pinball/sprites/background/bgScroller.png");

	// Load Sprite Animations

	int N = 0;
	for (int i = 0; i < 30; i++) {
		octoAnim.PushBack({ N, 0, 540, 540 });
		N += 540;
	}
	octoAnim.speed = 0.25f;
	octoAnim.loop = true;

	N = 0;
	for (int i = 0; i < 24; i++) {
		if (i >= 14) {
			metroAnim.PushBack({ 0, 0, 400, 70 });
		}
		else {
			metroAnim.PushBack({ 0, N, 400, 70 });
			N += 70;
		}
	}
	metroAnim.speed = 0.2f;
	metroAnim.loop = true;

	metroX = 75;
	metroY = 0;
	metroMoving = false;

	// Load Audio
	App->audio->ChangeVolume();
	App->audio->PlayMusic("pinball/audio/music_title.ogg", 0.5f);

	scorefx = App->audio->LoadFx("pinball/audio/fx/openScore.wav");
	acceptfx = App->audio->LoadFx("pinball/audio/fx/fxAccept.wav");
	backfx = App->audio->LoadFx("pinball/audio/fx/fxBack.wav");
	selectfx = App->audio->LoadFx("pinball/audio/fx/fxSelect.wav");
	metroFx = App->audio->LoadFx("pinball/audio/fx/metroFx.wav");

	// Fonts

	App->qfonts->Init();

	App->qfonts->LoadFont("pinball/font/Paintball.ttf");

	strcpy_s(githubLink, "https://github.com/Dani-24/Box2D-Pinball");

	return true;
}

update_status ModuleSceneTitle::Update()
{
	// Print BG
	App->renderer->Blit(bg, 0, 0);

	// BG particle Scrolling:
	for (int i = 0; i < 2; i++) {
		partScroll[i] -= 0.3f;
		if (partScroll[i] < -552) {
			partScroll[i] = 552;
		}
		App->renderer->Blit(bgPart, partScroll[i], 0);
	}
	// BG Scrolling:
	for (int i = 0; i < 2; i++) {
		scroller[i] += 0.4f;
		if (scroller[i] > 1500) {
			scroller[i] = -1500;
		}
		App->renderer->Blit(scrollerTexture, scroller[i], 0);
	}

	switch (currentState)
	{
	case MENU:
		if (metroMoving != true) {
			// --- UPDATE ---

			// Cursor Movement
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && cursorY != 650) {
				cursorY += 75;
				App->audio->PlayFx(selectfx);
			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && cursorY != 425) {
				cursorY -= 75;
				App->audio->PlayFx(selectfx);
			}

			// State and Scene management
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 575) {
				App->audio->PlayFx(acceptfx);
				// Open Website
				LOG("Opening Link : %s", githubLink);
				SDL_OpenURL(githubLink);
			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 500) {
				currentState = SCORES;
				App->audio->PlayFx(scorefx);
			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 425) {
				metroMoving = true;
				App->audio->PlayFx(metroFx);
				App->audio->PlayFx(acceptfx);
				App->fade->FadeToBlack(this, (Module*)App->scene_intro, 102);
			}
			if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && cursorY == 650) {
				App->audio->PlayFx(backfx);
				return UPDATE_STOP;
			}
		}
		// --- PRINT ---

		// Cursor
		App->renderer->Blit(cursor, cursorX, cursorY);

		// Fonts
		App->qfonts->drawText("Play", 242, 425);
		App->qfonts->drawText("Scores", 225, 500);
		App->qfonts->drawText("Github", 225, 575);
		App->qfonts->drawText("Exit", 242, 650);

		break;
	case SCORES:
		// Return
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			currentState = MENU;

			App->audio->PlayFx(backfx);

		}

		// --- UPDATE ---

		// Debug lifehacks

		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) {
			scores.add(69);
			scores.add(420);
			scores.add(777666);
		}

		// --- PRINT ---


		// Score list
		int N = 1, x = 150, y = 200; bool mt11 = false;
		p2List_item<int>* c = scores.getFirst();
		if (c == NULL) {
			strcpy_s(scorePosition, "` There is no Score registed");
			App->qfonts->drawText(scorePosition, 50, 400);
		}
		else {
			strcpy_s(scorePosition, "´ Score from lastest Games ´");
			App->qfonts->drawText(scorePosition, 40, y - 110);
			strcpy_s(scorePosition, "(Sorted from first");
			App->qfonts->drawText(scorePosition, 130, y - 80);
			strcpy_s(scorePosition, " to lastest game)");
			App->qfonts->drawText(scorePosition, 130, y - 60);
		}
		while (c != NULL)
		{
			if (N == 11) {
				strcpy_s(scorePosition, "¡ Stay Fresh !");
				App->qfonts->drawText(scorePosition, 150, y);
			}
			else {
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

				App->qfonts->drawText(scorePosition, x, y); // Print scorePosition

			}
			LOG(scorePosition);

			N++;
			y += 50;
			c = c->next;
		}
		break;
	}

	// Print in all scenes

	if (metroMoving == false) {
		metroAnim.Update();
	}
	else {
		if (metroX < 800) {
			increase += 1;
			metroX += increase/10;
		}
		else {
			increase = 0;
			metroMoving = false;
		}
	}

	SDL_Rect posEso = metroAnim.GetCurrentFrame();
	App->renderer->Blit(metro, metroX, metroY, &posEso);

	return UPDATE_CONTINUE;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	App->textures->Unload(octoling);
	App->textures->Unload(bg);
	App->textures->Unload(bgPart);
	App->textures->Unload(cursor);
	App->textures->Unload(metro);
	App->textures->Unload(scrollerTexture);

	octoling = bg = bgPart = cursor = nullptr;
	octoAnim.DeleteAnim();

	metroAnim.DeleteAnim();

	scorefx = acceptfx = selectfx = backfx = cursorX = cursorY = NULL;

	App->qfonts->UnloadFont();
	App->qfonts->CleanUp();

	for (int j = 0; j < 100; j++) {
		githubLink[j] = scorePosition[j] = NULL;
	}

	increase = 0;
	return true;
}