#ifndef _GHOST_H
#define _GHOST_H

#include "BObject.h"
#include "Effectsys.h"

#define GHOST_COLLISIONR	8

#define GHOST_IZER(LIFE, MAXLIFE)	((LIFE) / (MAXLIFE) * 32.0f)

#define GHOST_COLORAFFECTORMAX 4

class Ghost : public BObject
{
public:
	Ghost();
	virtual ~Ghost();

	static bool Build(WORD gID, BYTE tarID, BYTE belong, float x, float y, int angle, float speed, BYTE type, float life, int ac);

	void valueSet(WORD ID, float x, float y, int angle, float speed, BYTE type, float life, int ac);
	void valueSet(WORD ID, BYTE belong, int angle, float speed, BYTE type, float life, int ac);

	virtual void action();
	void Render();

public:

	Effectsys effghost;

	DWORD * startcolmin[GHOST_COLORAFFECTORMAX];
	DWORD * startcolmax[GHOST_COLORAFFECTORMAX];
	DWORD * endcolmin[GHOST_COLORAFFECTORMAX];
	DWORD * endcolmax[GHOST_COLORAFFECTORMAX];

	Target aim;

	float	life;
	float	maxlife;
	float	cenx;
	float	ceny;
	bool	half;
	bool	gave;
	int		ac;
	bool	fadeout;
	bool	able;
	bool	damage;

	WORD	gID;

	BYTE	belong;
	BYTE	type;

	static WORD index;

	static BYTE totalghost;
	static BYTE totalhalfghost;
};

extern Ghost gh[GHOSTMAX];

#endif
