#pragma once
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ISurface.h"
#include "Menu.h"

class SpecList
{
public:
	void Draw();

private:
	bool IsObserving(C_BaseEntity* pEnt);
	struct
	{
		float x = 5.f, y = 5.f;
	}Pos;
};

extern SpecList g_SpecList;