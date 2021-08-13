#include "Input.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(Input* input) : Module()
{
	memset(name, 0, TEXT_LEN);
	strcpy_s(name, TEXT_LEN, "scenemanager");
	this->input = input;
}

// Destructor
Scene::~Scene()
{}

// Called before the first frame
bool Scene::Start()
{
	LOG("Loading Scene Manager");
	char tmp[TEXT_LEN] = { 0 };

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Scene::Draw()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}