#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneTitle.h"
#include "ModuleFade.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModuleSceneTitle::ModuleSceneTitle(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleSceneTitle::~ModuleSceneTitle()
{
	// You should do some memory cleaning here, if required
}

bool ModuleSceneTitle::Start()
{
	LOG("Loading Title assets uwu");
	bool ret = true;

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


	return ret;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	return true;
}

update_status ModuleSceneTitle::Update()
{
	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		App->fade->FadeToBlack(this, (Module*)App->scene_intro, 60);
	//}

	// - - Printing "thangs" - -

	octoAnim.Update();
	SDL_Rect rect = octoAnim.GetCurrentFrame();

	App->renderer->Blit(octoling, 0, 230, &rect);

	return UPDATE_CONTINUE;
}
