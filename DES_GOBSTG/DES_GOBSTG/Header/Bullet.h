#ifndef _BULLET_H
#define _BULLET_H

#include "BObject.h"


#define BULLETACTIONMAX		0x20
#define BULLETIZELISTMAX	0x03
#define BULLETTYPEMAX		0x40
#define BULLETCOLORMAX		0x08
#define BULLETTYPECOLORMAX	BULLETTYPEMAX * BULLETCOLORMAX

#define BULLET_BIGBEGIN		40
#define BULLET_BONUSTYPE	48
#define BULLET_FADEINTYPE	49
#define BULLET_FADEOUTTYPE	50

#define BULLET_TYPECHANGETIME 16

#define BULLETIZE_FADEOUT	0
#define BULLETIZE_ITEM		1
#define BULLETIZE_POINT		2
#define BULLET_IZEFULLZONE	800
#define BULLET_IZEOVERZONE	1000

#define BULLETACT_FLOATSCALE	100.0f
#define BULLETACT_ANGLECHANGESE	3000

#define THEN -1000000

#define	SECTIONEND		0x0

#define	EVERY			0x1
#define	EVERYMOD		0x2

#define	TIMERGREAT		0x11
#define	TIMEREQUAL		0x12
#define	TIMERLESS		0x13
#define	TIMERRANGE		0x14

#define	TYPEEQUAL		0x21
#define	COLOREQUAL		0x31

#define	ANGLEGREAT		0x41
#define	ANGLEEQUAL		0x42
#define	ANGLELESS		0x43
#define	ANGLERANGE		0x44

#define	XGREAT			0x51
#define	XLESS			0x52
#define	XRANGE			0x53
#define	YGREAT			0x58
#define	YLESS			0x59
#define	YRANGE			0x5A

#define VALGREAT		0x61
#define VALEQUAL		0x62
#define VALLESS			0x63
#define VALRANGE		0x64

#define	SPEEDGREAT		0x71
#define	SPEEDEQUAL		0x72
#define	SPEEDLESS		0x73
#define	SPEEDRANGE		0x74

#define	TYPESET			0x1100
#define	COLORSET		0x2100

#define	ANGLESET		0x3100
#define ANGLESETADD		0x3200
#define	ANGLESETADDMOD	0x3300
#define	ANGLEADJUST		0x3400

#define	HEADANGLESET	0x4100
#define	HEADANGLESETADD	0x4200
#define	ANGLESETRMA		0x4400
#define	ANGLESETRMAP	0x4500
#define ANGLESETRMAT	0x4600
#define	ANGLESETAMA		0x4700
#define	ANGLESETAMAP	0x4800
#define ANGLESETAMAT	0x4900

#define	XSET			0x5100
#define	YSET			0x5200
#define XSETADD			0x5300
#define	YSETADD			0x5400
#define	XSETACCADD		0x5500
#define	YSETACCADD		0x5600

#define VALSET			0x6100
#define VALSETADD		0x6200

#define	SPEEDSET		0x7100
#define	SPEEDSETADD		0x7200

#define CALLSCRIPT		0x8000
#define CHASE			0x8100

#define	AND				-0x1
#define	OR				-0x2
#define	NOT				-0x3
#define	ANDSET			-0x10

#define	REMAIN			-0x20
#define FADEOUT			-0x30
#define ITEMLIZE		-0x31
#define POINTLIZE		-0x32
#define BOUNCE			-0x40

typedef struct tagIzeZone{
	float x;
	float y;
	float r;
}IzeZone;

typedef struct tagRenderDepth{
	int startIndex;
	int endIndex;
	bool haveType;
}RenderDepth;

class Bullet : public BObject
{
public:
	Bullet();
	virtual ~Bullet();

	static void Init(HTEXTURE tex);
	static void Release();
	void Render();

	virtual void action();
	bool isInRect(float r,float aimx,float aimy);

	bool valueSet(WORD ID, float x, float y, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid = 0, BYTE tarID = 0xff);
	inline void valueSet(WORD ID, const BObject & from, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid = 0, BYTE tarID = 0xff)
	{
		valueSet(ID, from.x, from.y, absolute, angle, speed, type, color, fadeinTime, avoid, tarID);
	}

	static bool Build(float x, float y, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid, BYTE tarID);

	void itemlize();
	void pointlize();

	void ChangeAction();

public:
	int		actionList[BULLETACTIONMAX];

	float	xplus;
	float	yplus;
	float	lastx;
	float	lasty;
	float	lastspeed;
	int		lastangle;
	int		fadeinTime;

	bool	fadeout;
	bool	able;
	bool	grazed;
	bool	remain;
	bool	cancelable;

	BYTE	type;
	BYTE	oldtype;
	BYTE	color;
	BYTE	toafter;
	BYTE	typechangetimer;

	static RenderDepth renderDepth[BULLETTYPEMAX];
	static Bullet _bu;
	static hgeSprite * sp[BULLETTYPECOLORMAX];
	static list<IzeZone> izel[BULLETIZELISTMAX];
	static HTEXTURE tex;
	static WORD index;
};
extern VectorList<Bullet>bu;

#endif