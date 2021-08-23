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

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			RELEASE(colliders[i]);
		}
	}

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

	LOG("freeing colliders");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		RELEASE(colliders[i]);
	}

	return ret;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

void Collider::SetPos(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	int res = 0;
	if (rect.x < r.x + r.w
		&& rect.x + rect.w > r.x
		&& rect.y < r.y + r.h
		&& rect.h + rect.y > r.y)
	{
		res = 1;
		if (rect.x < r.x + r.w)
		{
			LOG("1 : %d = %d - %d", rect.x - (r.x + r.w), rect.x, r.x + r.w);
		}
		if (rect.x + rect.w > r.x)
		{
			LOG("2 : %d = %d - %d", rect.x + rect.w - r.x, rect.x + rect.w, r.x);
		}
		if (rect.y < r.y + r.h)
		{
			LOG("3 : %d = %d - %d", rect.y - (r.y + r.h), rect.y, r.y + r.h);
		}
		if (rect.y + rect.h > r.y)
		{
			LOG("4 : %d = %d - %d", rect.y + rect.h - r.y, rect.y + rect.h, r.y);
		}
	}

	return res;
}