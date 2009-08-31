#ifndef _SELECTOR_H
#define _SELECTOR_H

#include "MainDependency.h"
#include "Const.h"

#define SEL_STATEMAX	0x04

#define	SEL_NONE		0x00
#define	SEL_OVER		0x01
#define	SEL_ENTER		0x02
#define	SEL_LEAVE		0x04

#define SELINFO_NONE	0
#define SELINFO_OVER	1
#define SELINFO_ENTER	2
#define SELINFO_LEAVE	3

#define SEL_NONACTIVE	0x10
#define SEL_GRAY		0x20

struct selinfo
{
	float xadj;
	float yadj;
};

class Selector
{
public:
	Selector();
	~Selector();

	void valueSet(BYTE ID, HTEXTURE tex, float cenx, float ceny, float hscale, float vscale, float column, float row, float nColumn, float nRow, BYTE maxtime);
	void actionSet(BYTE setflag, float xadj, float yadj);

	static void Build(BYTE ID, HTEXTURE tex, float cenx, float ceny, float hscale, float vscale, float column, float row, float nColumn, float nRow, BYTE maxtime,
		float xadj0, float yadj0,
		float xadj1, float yadj1,
		float xadj2, float yadj2,
		float xadj3, float yadj3,
		bool nonactive = false, bool gray = false);

	static bool confirm(HTEXTURE tex);
	static void Clear();

	void action();

public:
	selinfo	info[SEL_STATEMAX];

	float	x;
	float	y;
	float	hscale;
	float	vscale;

	hgeSprite * sprite;

	BYTE	ID;
	BYTE	flag;
	BYTE	timer;
	BYTE	maxtime;
	BYTE	alpha;
	BYTE	pushtimer;

	static int nselect;
	static int select;
	static int sellock;
	static bool updown;
	static bool complete;
	static bool plus;

	static bool confirminit;

	static bool avoid;
};

extern list<Selector> sel;

#endif