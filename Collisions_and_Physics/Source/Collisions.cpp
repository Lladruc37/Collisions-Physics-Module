#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

Collisions::Collisions() : Module()
{
	memset(name, 0, TEXT_LEN);
	strcpy_s(name, TEXT_LEN, "collisions");
}

Collisions::~Collisions()
{
}

void Collisions::Init()
{
	active = false;
}
bool Collisions::Start()
{
	bool ret = true;

	return ret;
}
bool Collisions::PreUpdate()
{
	bool ret = true;

	return ret;
}
bool Collisions::Update()
{
	bool ret = true;

	return ret;
}
bool Collisions::PostUpdate()
{
	bool ret = true;

	return ret;
}
bool Collisions::CleanUp()
{
	bool ret = true;

	return ret;
}