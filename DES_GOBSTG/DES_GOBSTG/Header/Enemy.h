#ifndef _ENEMY_H
#define _ENEMY_H

#include "BObject.h"
#include "EffectSp.h"
#include "Effectsys.h"

#define ENEMY_INDEXSTART	4
#define ENEMY_MAINBOSSINDEX	0

#define ENEMY_TEXMAX		4
#define ENEMY_PARAMAX		4

#define ENEMY_BOSSMAX		ENEMY_TEXMAX

#define ENEMY_BOSSTYPEBEGIN	40

#define ENEMY_BOSSINFITIMER		240
#define ENEMY_BOSSMOVELIMIT		0.01f
#define ENEMY_ENEMYMOVELIMIT	0.2f

#define ENEMY_COLLISIONR		8

#define ENEMY_BOSSX_FADERANGE	32

#define	BOSS_ATTACK		0x1
#define BOSS_RANGEUP	0x2

#define	BOSS_STORE		0x11
#define BOSS_BREAK		0x12
#define BOSS_COLLAPSE	0x13

class Enemy : public BObject
{
public:
	Enemy();
	virtual ~Enemy();

	static bool Build(WORD eID, BYTE tarID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer, DWORD take);

	static void Init(HTEXTURE tex0, HTEXTURE tex1=0, HTEXTURE tex2=0, HTEXTURE tex3=0);
	static void texAssign(HTEXTURE tex0, HTEXTURE tex1=0, HTEXTURE tex2=0, HTEXTURE tex3=0);

	void valueSet(WORD ID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer, DWORD take,
		WORD ac=0, float para0 = 0, float para1 = 0, float para2 = 0, float para3 = 0);

	void setMove(float para0, float para1, float para2, float para3 = 0, WORD ac = 42);

	virtual void action();
	void matchAction();
	void bossAction();

	void giveItem();

	void Render();
	void RenderEffect();

public:
	Effectsys	effCollapse;
	Effectsys	effShot;

	EffectSp	esCircle;

	hgeSprite * sprite;

	float	para[ENEMY_PARAMAX];
	Target	aim;
	float	lastx;
	float	life;
	float	defrate;
	DWORD	take;

	bool	fadeout;
	bool	able;
	bool	damage;
	int		infitimer;
	float	maxlife;

	WORD	eID;
	WORD	ac;
	BYTE	type;
	BYTE	damagetimer;

	static HTEXTURE tex[ENEMY_TEXMAX];
	static BYTE bossflag[ENEMY_BOSSMAX];
	static BYTE rangeuptimer[ENEMY_BOSSMAX];
	static BYTE storetimer[ENEMY_BOSSMAX];
	static WORD index;
};

extern Enemy en[ENEMYMAX];

#endif