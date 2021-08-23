#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "Module.h"
#include "SDL.h"

#define MAX_COLLIDERS 250

class Collider
{
public:
	// Collision Type enum
	enum class Type
	{
		NONE = -1,
		AIR,
		SOLID,
		DOOR,
		PUZZLE,
		OTHER,
		EVENT,
		ENEMY_SPAWN,
		INTERACTABLE,
		DEBUG,
		PLAYER,
		BUTTON,
		MOVEABLE,
		SOLID_ROCK,
		MAX
	};

	// Constructor
	Collider(SDL_Rect rect, Type type, Module* listener = nullptr) : rect(rect), type(type), listener(listener)
	{}

	// Sets the position of the construct
	void SetPos(int x, int y, int w, int h);

	// Checks if two rects are intersecting
	bool Intersects(const SDL_Rect& r) const;

public:
	SDL_Rect rect;
	Type type;
	Module* listener = nullptr;
	bool pendingToDelete = false;
};

class Collisions : public Module
{
public:
	Collisions();
	~Collisions();
	bool CleanUp();
	void Init();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

private:
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
};

#endif //!__COLLISIONS_H__