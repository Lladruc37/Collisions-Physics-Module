#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(Input* input, Render* render, Collisions* collisions) : Module()
{
	memset(name, 0, TEXT_LEN);
	strcpy_s(name, TEXT_LEN, "scenemanager");
	this->input = input;
	this->render = render;
	this->collisions = collisions;
}

// Destructor
Scene::~Scene()
{}

// Called before the first frame
bool Scene::Start()
{
	LOG("Loading Scene Manager");
	char tmp[TEXT_LEN] = { 0 };

	Collider* box;
	box = collisions->AddCollider({ 150, 200, 100, 100 }, Collider::Type::SOLID, this);
	boxes.Add(box);
	box = collisions->AddCollider({ 500, 300, 100, 100 }, Collider::Type::SOLID, this);
	boxes.Add(box);
	box = collisions->AddCollider({ 0, 0, 50, 500 }, Collider::Type::SOLID, this);
	boxes.Add(box);
	box = collisions->AddCollider({ 50, 0, 300, 50 }, Collider::Type::SOLID, this);
	boxes.Add(box);

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
	nextPos = { (float)boxes.At(0)->data->rect.x, (float)boxes.At(0)->data->rect.y };

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;
	if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		nextPos.x -= 2;
	}
	if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		nextPos.x += 2;
	}
	if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		nextPos.y -= 2;
	}
	if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		nextPos.y += 2;
	}

	//-----------------------RESOLVE COLLISIONS---------------------------//

	intersect = false;
	Collider* otherBox = new Collider({ 0,0,0,0 }, Collider::Type::SOLID);
	for (uint i = 0; i < boxes.Count(); ++i)
	{
		for (uint j = i + 1; j < boxes.Count(); ++j)
		{
			if (boxes.At(i)->data->Intersects(boxes.At(j)->data->rect))
			{
				otherBox = boxes.At(j)->data;
				intersect = true;
				LOG("INTERSECT");
			}
		}
	}

	if (intersect)
	{
		fPoint tmp = { nextPos.x, nextPos.y };
		int i = 0;
		fPoint dif = { tmp.x - (float)boxes.At(0)->data->rect.x, tmp.y - (float)boxes.At(0)->data->rect.y };
		fPoint increment = { dif.x / 60.0f , dif.y / 60.0f };
		LOG("%f , %f = %f , %f - %d , %d", dif.x, dif.y, tmp.x, tmp.y, boxes.At(0)->data->rect.x, boxes.At(0)->data->rect.y);
		while (otherBox->Intersects({ (int)tmp.x,(int)tmp.y, 100,100 }))
		{
			/*if (dif.x == 0)
			{
				if (dif.y > 0)
				{
					tmp.y -= increment.y;
				}
				else if (dif.y < 0)
				{
					tmp.y += increment.y;
				}
			}
			else if (dif.y == 0)
			{
				if (dif.x > 0)
				{
					tmp.x += increment.x;
				}
				else if (dif.x < 0)
				{
					tmp.x -= increment.x;
				}
			}
			else if (abs(dif.x) > abs(dif.y))
			{
				if (dif.y > 0)
				{
					tmp.y -= increment.y;
				}
				else if (dif.y < 0)
				{
					tmp.y += increment.y;
				}
			}
			else if (abs(dif.x) < abs(dif.y))
			{
				if (dif.x > 0)
				{
					tmp.x += increment.x;
				}
				else if (dif.x < 0)
				{
					tmp.x -= increment.x;
				}
			}*/

			if (dif.x > 0)
			{
				if (dif.x < 0.05f)
				{
					tmp.x = (int)tmp.x;
				}
				else
				{
					tmp.x -= increment.x;
				}
			}
			else if (dif.x < 0)
			{
				if (dif.x > -0.05f)
				{
					tmp.x = (int)tmp.x;
				}
				else
				{
					tmp.x += increment.x;
				}
			}
			if (dif.y > 0)
			{
				if (dif.y < 0.05f)
				{
					tmp.y = (int)tmp.y;
				}
				else
				{
					tmp.y -= increment.y;
				}
			}
			else if (dif.y < 0)
			{
				if (dif.y > -0.05f)
				{
					tmp.y = (int)tmp.y;
				}
				else
				{
					tmp.y += increment.y;
				}
			}
			i++;
			/*if (abs(dif.x) < abs(dif.y))
			{
				if (dif.x > 0)
				{
					tmp.x++;
				}
				else if (dif.x < 0)
				{
					tmp.x--;
				}
			}
			else if(abs(dif.x) > abs(dif.y))
			{
				if (dif.y > 0)
				{
					tmp.y--;
				}
				else if (dif.y < 0)
				{
					tmp.y++;
				}
			}*/

			dif = { tmp.x - (float)boxes.At(0)->data->rect.x, tmp.y - (float)boxes.At(0)->data->rect.y };
			LOG("%f , %f = %f , %f - %d , %d", dif.x, dif.y, tmp.x, tmp.y, boxes.At(0)->data->rect.x, boxes.At(0)->data->rect.y);
			if (!otherBox->Intersects({ (int)tmp.x,(int)tmp.y, 100,100 })/* || i == 60 || dif.x == 0 || dif.y == 0*/)
			{
				LOG("--------------------YEY--------------");
				nextPos = tmp;
				intersect = false;
			}
		}
	}
	

	/*if (intersect)
	{
		iPoint tmp = { nextPos.x, nextPos.y };

		iPoint dif = { tmp.x - boxes.At(0)->data->rect.x, tmp.y - boxes.At(0)->data->rect.y };
		LOG("%d , %d = %d , %d - %d , %d", dif.x, dif.y, tmp.x, tmp.y, boxes.At(0)->data->rect.x, boxes.At(0)->data->rect.y);
		while (intersect)
		{
			if (dif.x > 0)
			{
				tmp.x--;
			}
			else if (dif.x < 0)
			{
				tmp.x++;
			}

			if (dif.y > 0)
			{
				tmp.y--;
			}
			else if (dif.y < 0)
			{
				tmp.y++;
			}
			dif = { tmp.x - boxes.At(0)->data->rect.x, tmp.y - boxes.At(0)->data->rect.y };
			LOG("%d , %d = %d , %d - %d , %d", dif.x, dif.y, tmp.x, tmp.y, boxes.At(0)->data->rect.x, boxes.At(0)->data->rect.y);
			if (dif.x == 0 && dif.y == 0)
			{
				if (!otherBox->Intersects({ tmp.x,tmp.y, 100,100 }))
				{
					LOG("NOT INTERSECTING");
				}
				LOG("---------------YEY--------------");
				nextPos = tmp;
				intersect = false;
				break;
			}
		}
	}*/

	boxes.At(0)->data->SetPos(nextPos.x, nextPos.y, boxes.At(0)->data->rect.w, boxes.At(0)->data->rect.h);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	//render->camera.x = -(boxes.At(0)->data->rect.x - (1280 / 2 + boxes.At(0)->data->rect.w / 2));
	//render->camera.y = -(boxes.At(0)->data->rect.y - (720 / 2 + boxes.At(0)->data->rect.h / 2));

	Draw();
	return ret;
}

bool Scene::Draw()
{
	bool ret = true;

	for (uint i = 0; i < boxes.Count(); ++i)
	{
		if (boxes.At(i)->data == nullptr)
		{
			break;
		}
		else
		{
			if (intersect)
			{
				render->DrawRectangle(boxes.At(i)->data->rect, 0, 255, 0, 255);
			}
			else
			{
				render->DrawRectangle(boxes.At(i)->data->rect, 255, 0, 0, 255);
			}
		}
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}