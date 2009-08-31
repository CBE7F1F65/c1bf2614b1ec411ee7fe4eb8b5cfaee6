#ifndef _ITEM_H
#define _ITEM_H

#include "BObject.h"

class Player;

#define ITEMINFOFONTMAX		0x40

#define ITEMTYPEMAX			0x08
#define ITEMSPRITEMAX		ITEMTYPEMAX * 2

#define ITEM_STARTSPEED		-5.0f

#define ITEM_GETR				32
#define ITEM_DRAINR(BSLOW)		((BSLOW)?64:48)
#define ITEM_DRAINY				144
#define ITEM_DRAINDELAY			24
#define ITEM_DRAINFASTSPEED		10.8f
#define ITEM_DRAINSLOWSPEED		5.0f
#define ITEM_CIRCLEUPSPEED		-1.0f
#define ITEM_DROPSPEEDMAX		2.8f
#define ITEM_DROPSPEEDACC		0.1f
#define ITEM_RETHROWSPEED		-3.0f

#define ITEM_BIGPOWERADD		8

#define ITEMINFO_TURNVALUEKEY	0x10

#define	ITEM_POWER			0x0
#define	ITEM_POINT			0x1
#define	ITEM_BIGPOWER		0x2
#define	ITEM_BOMB			0x3
#define	ITEM_FULL			0x4
#define	ITEM_EXTEND			0x5
#define	ITEM_CIRCLE			0x6
#define	ITEM_SMALLCIRCLE	0x7

struct infoFont
{
	char	cScore[M_STRITOAMAX];
	float	x;
	float	y;
	bool	yellow;
	BYTE	timer;
};

class Item : public BObject
{
public:
	Item();
	virtual ~Item();

	static void Init(HTEXTURE tex, char * fontname);

	static void Build(WORD type, float x, float y, bool bDrained = false, int angle = 9000, float speed = ITEM_STARTSPEED);

	void valueSet(WORD type, float x, float y, bool bDrained = false, int angle = 9000, float speed = ITEM_STARTSPEED);

	static void Release();

	void Render();

	virtual void action();

	void powerFull();
	static void undrainAll();
	static void drainAll();

public:
	bool bDrained;
	bool bFast;

	static hgeSprite *spItem[ITEMSPRITEMAX];
	static VectorList<infoFont>infofont;
	static HTEXTURE tex;
	static hgeFont * item_font;
};

extern VectorList<Item>mi;

#endif