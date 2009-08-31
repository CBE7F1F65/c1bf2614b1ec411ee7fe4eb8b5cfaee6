#ifndef _PLAYERBULLET_H
#define _PLAYERBULLET_H

#define PLAYERBULLETSPRITEMAX	0x08

#define PBLOCK_ACCSPEED		1.0f
#define PBLOCK_MINSPEED		2.0f
#define PBLOCK_TURNMAX		600
#define PBLOCK_LOCKTIMERMAX	0x80

#define PBLOCK_LOST		0xffff
#define PBLOCK_GHOST	0x8000

#define PB_GHOSTCOLLISIONR			24
#define PB_GHOSTCOLLISIONR_CIRCLE	32
#define PB_ENEMYCOLLISIONR			32

#define PBGHOST_ENEMYCOST(MINUS)	((MINUS) / 4)
#define PBENEMY_COSTSCALE			0.8f

#define PB_DELETEBOLDER			40
#define PB_DELETE_LEFT			M_DELETECLIENT_LEFT - PB_DELETEBOLDER
#define PB_DELETE_RIGHT			M_DELETECLIENT_RIGHT + PB_DELETEBOLDER
#define PB_DELETE_TOP			M_DELETECLIENT_TOP - PB_DELETEBOLDER
#define PB_DELETE_BOTTOM		M_DELETECLIENT_BOTTOM + PB_DELETEBOLDER

#define PB_HEADARCPLUS			10


#include "BObject.h"

class PlayerBullet : public BObject
{
public:
	PlayerBullet();
	virtual ~PlayerBullet();

	static void Init_Player(HTEXTURE tex);

	void valueSet(WORD ID, BYTE arrange, int angle, float speed, float power);

	virtual void action();
	void Render();

	void hitOn();
	bool isInRange(float aimx, float aimy, bool hiton = true);

	static void RenderBeam();

	void Lock();

	static void Release();

public:
	float	oldspeed;
	float	xplus;
	float	yplus;

	DWORD	color;
	bool	fadeout;
	bool	able;
	float	power;

	int		locktimer;

	BYTE	arrange;

	static hgeSprite * spPlayerBullet[PLAYERBULLETSPRITEMAX];
	static DWORD bcol0, bcol1, bcol2, bcol3;
	static int locked;
	static HTEXTURE tex;
	static WORD beams;
};

extern VectorList<PlayerBullet>pb;

#endif