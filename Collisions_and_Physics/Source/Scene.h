#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Point.h"

class Scene : public Module
{
public:

	Scene(Input* input, Render* render, Collisions* collisions);

	// Destructor
	virtual ~Scene();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Draws scene
	bool Draw();

	// Called before quitting
	bool CleanUp();

private:
	Input* input;
	Render* render;
	Collisions* collisions;

	List <Collider*> boxes;
	fPoint nextPos;
	bool intersect;
};
#endif // __SCENE_H__