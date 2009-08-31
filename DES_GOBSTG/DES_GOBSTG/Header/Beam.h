#ifndef _BEAM_H
#define _BEAM_H

#include "BObject.h"

#define BEAM_INVALIDSCALE	0.5f

class Beam : public BObject
{
public:
	Beam();
	virtual ~Beam();

	static void Init();
	static bool Build(float x, float y, int angle, float speed, BYTE type, BYTE color, WORD lenth, bool stop, bool horizon, int fadeouttime, BYTE tarID);

	void valueSet(WORD ID, float x, float y,int angle, float speed, BYTE type, BYTE color, WORD lenth, bool stop = false, bool horizon = false, int fadeouttime = 0, BYTE tarID = 0xff);
	bool isInRect(float r, float aimx, float aimy);

	void SetVector(const BObject &ori, const BObject &aim);
	void SetVector(float orix, float oriy, float aimx, float aimy);

	void SetHold(BYTE holdtar);
	void SetHold(BYTE holdtar, BYTE pintar);

	virtual void action();

	void Render();

public:
	float	xplus;
	float	yplus;
	float	lastspeed;
	int		lastangle;
	int		fadeouttime;

	bool	horizon;
	bool	stop;
	bool	fadeout;
	bool	able;

	WORD	lenth;
	BYTE	type;
	BYTE	color;
	BYTE	grazetimer;
	BYTE	holdtar;
	BYTE	pintar;

	static WORD index;
};

extern VectorList<Beam> be;

#endif