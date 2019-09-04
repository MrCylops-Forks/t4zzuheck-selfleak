#pragma once
#include "..\..\Utils\GlobalVars.h"

struct TestPos
{
	Vector Pos;
	float SimulationTime;
};

class ESP
{
public:
	void set_hitmarker_time(float time);
	void DrawHitmarker();
    void Render();
	void QuakeKillCounter();
	void Ammo(C_BaseEntity* Entity);
private:
	struct
	{
		int left, top, right, bottom;
	}Box;

	Color color;
	Color hcolor;
	Color hbcolor;
	Color weaponcolor;
	Color namecolor;
	Color ypcolor;
	Color moneycolor;
	Color shadow;
	int offset = 0;
	DWORD FONT;
	int offsetY;

	void BoundBox(C_BaseEntity* pEnt);
    void RenderBox(C_BaseEntity* pEnt);
	void RenderWeaponName(C_BaseEntity* pEnt);
	void RenderEntHealth(C_BaseEntity* pEnt);
	void RenderYawPitch(C_BaseEntity* pEnt);
    void RenderName(C_BaseEntity* pEnt, int iterator);
	void RenderMoney(C_BaseEntity* pEnt);
	void RenderOffScreen(C_BaseEntity* pEnt, C_BaseEntity* pLocal);

};
extern ESP g_ESP;
