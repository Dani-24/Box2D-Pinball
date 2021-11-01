#include "ModuleFade.h"
#include "ModuleRender.h"
#include "Application.h"

#include "SDL/include/SDL_render.h"

ModuleFade::ModuleFade(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	screenRect = { 0,0,SCREEN_WIDTH * SCREEN_SIZE,SCREEN_HEIGHT * SCREEN_SIZE };
}
ModuleFade::~ModuleFade()
{

}
bool ModuleFade::Start()
{
	LOG("Preparing Fade Screen");

	SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

update_status ModuleFade::Update()
{
	if (currentStep == Fade_Step::NONE)
	{
		return update_status::UPDATE_CONTINUE;
	}

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleFade::PostUpdate()
{
	if (currentStep == Fade_Step::NONE)
	{
		return update_status::UPDATE_CONTINUE;
	}
	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(App->renderer->renderer, &screenRect);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleFade::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
		ret = true;
	}
	return ret;
}