#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Scene.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	//init
	memset(title, 0, TEXT_LEN);
	strcpy_s(title, TEXT_LEN, "Collisions and Physics");
	memset(organization, 0, TEXT_LEN);
	strcpy_s(organization, TEXT_LEN, "Sergi Colomer");

	LOG("%s", GetTitle());
	cap = 60;
	if (cap > 0) cappedMs = 1000 / (float)cap;

	PERF_START(pTimer);

	win = new Window(/*title*/);
	input = new Input(win);
	render = new Render(win);
	tex = new Textures(render);
	scene = new Scene(input);
	collisions = new Collisions();

	// Ordered for Start & Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(scene);
	AddModule(collisions);

	// Render last to swap buffer
	AddModule(render);

	PERF_PEEK(pTimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true) ret = item->data->Start();

		item = item->next;
	}

	capRequest = false;
	saveRequest = false;
	loadRequest = false;
	exitRequest = false;

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true) ret = false;

	if (ret == true) ret = PreUpdate();

	if (ret == true) ret = DoUpdate();

	if (ret == true) ret = PostUpdate();

	FinishUpdate();

	if (exitRequest) return false;

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();

	// Start the timer after read because we want to know how much time it took from the last frame to the new one
	PERF_START(frameTime);
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (capRequest)
	{
		capRequest = !capRequest;
		ChangeCap();
	}

	// Framerate calculations------------------------------------------
	// To know how many frames have passed in the last second
	if (lastSecFrameTime.Read() >= 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	// Amount of seconds since startup
	float secondsSinceStartup = 0.0f;
	secondsSinceStartup = startupTime.ReadSec();

	// Amount of time since game start (use a low resolution timer)
	Uint32 lastFrameMs = 0;
	lastFrameMs = frameTime.Read(); // Time from the prepare update until now (whole update method)

	// Average FPS for the whole game life (since start)
	float averageFps = 0.0f;
	averageFps = float(frameCount) / startupTime.ReadSec();

	// Amount of frames during the last update
	uint32 framesOnLastUpdate = 0;
	framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	static char vsyncStr[4];

	if (vsync)
		sprintf_s(vsyncStr, "on");
	else
		sprintf_s(vsyncStr, "off");

	sprintf_s(title, 256, "FPS: %i Av.FPS: %.2f Last Frame Ms: %02u Vsync: %s", framesOnLastUpdate, averageFps, lastFrameMs, vsyncStr);
	if (render->debug) win->SetTitle(title);
	else win->SetTitle("Collisions and Physics");

	// Use SDL_Delay to make sure you get your capped framerate
	PERF_START(pTimer);
	if (cappedMs > lastFrameMs) SDL_Delay((Uint32)(cappedMs - lastFrameMs));

	// Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	PERF_PEEK(pTimer);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) continue;

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) continue;

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) continue;

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title;
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization;
}

void App::LoadRequest()
{
	loadRequest = true;
}

void App::SaveRequest()
{
	saveRequest = true;
}

void App::CapRequest()
{
	capRequest = true;
}

bool App::ChangeCap()
{
	switch (cap)
	{
	case 60:
		cap = 30;
		break;
	case 30:
		cap = 60;
		break;
	default:
		break;
	}

	if (cap > 0) cappedMs = 1000 / (float)cap;

	return true;
}