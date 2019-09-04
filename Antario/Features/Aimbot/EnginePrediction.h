#pragma once

class EnginePrediction // singleton ftw
{
private:

	int iTickBaseBackup;
	int iFlagsBackup;
	int iButtonsBackup;
	int* m_pPredictionRandomSeed;
	int* m_pSetPredictionPlayer;

public:
	void Start();
	void End();
};

extern EnginePrediction g_EnginePrediction;