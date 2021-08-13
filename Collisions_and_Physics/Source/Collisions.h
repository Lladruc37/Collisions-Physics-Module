#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "Module.h"

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

private:

};

#endif //!__COLLISIONS_H__