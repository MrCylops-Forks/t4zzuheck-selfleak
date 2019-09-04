#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\Utils/Math.h"
#include <deque>

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
	Vector Angles;
	Vector Origin;
};
struct backtrackData
{
	float simtime;
	Vector hitboxPos;
};

class BackTrack
{
	int latest_tick;
	bool IsTickValid(int tick);
	void UpdateRecord(int i);
public:
	lbyRecords records[64];
	bool RunLBYBackTrack(int i, CUserCmd* cmd, Vector& aimPoint);
	void Update(int tick_count);
	void LegitBackTrack(CUserCmd* cmd, C_BaseEntity* pLocal);
};

extern backtrackData HeadPositions[64][12];

extern BackTrack* Backtracking;


class LegitAimbot
{
public:
	void Aim_CreateMove(CUserCmd* cmd);
	
private:
	void FindTarget();
	void GoToTarget();
	void DropTarget();
	bool EntityIsValid(int i);
	void RCS();
};
extern LegitAimbot g_LegitAimbot;