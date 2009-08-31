#ifndef _BGLAYER_H
#define _BGLAYER_H

#include "MainDependency.h"
#include "Const.h"

#define BG_NONE			0
#define BG_WHITEFLASH	1
#define BG_REDFLASH		2
#define BG_YELLOWFLASH	3

#define BG_WHITEOUT		11
#define BG_REDOUT		12

#define BG_FADEOUT		41

#define BG_LIGHTUP		100
#define BG_LIGHTRED		110

#define FG_PAUSEIN		200
#define FG_PAUSEOUT		210

struct BGLayerSet
{
	WORD sID;
	DWORD timer;
};

#define BGLAYERSET_NONE	0

class BGLayer
{
public:
	BGLayer();
	virtual ~BGLayer();

	static void Init();

	void valueSet(HTEXTURE tex, float x, float y, float z, float w, float h, int rotx, int roty, int rotz, float paral, float tx, float ty, float tw, float th, float speed = 0, int angle = 9000, bool move = false, bool rotate = false, DWORD col = 0xffffffff);
	void valueSet(HTEXTURE tex, float cenx, float ceny, float width, float height, float column, float row, float nColumn, float nRow, DWORD col = 0xffffffff);
	void texRectSet(float texx, float texy, float texw, float texh);
	void rectSet(float x, float y, float z, float w, float h, int rotx, int roty, int rotz);
	void zSet(float z0, float z1, float z2, float z3);
	void scaleSet(float hscale, float vscale);
	void colorSet(DWORD col0, DWORD col1, DWORD col2, DWORD col3);
	void moveSet(bool move, bool rotate);
	void parallelogram(float paral);
	void vertexSet(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

	void action();

public:
	hgeQuad	quad;
	DWORD	ocol[4];
	DWORD	acol[4];
	bool	exist;
	bool	move;			//move vertex
	bool	rotate;			//set move to enable
	float	width;
	float	height;
	int		texWidth;
	int		texHeight;
	float	speed;
	int		angle;
	DWORD	timer;
	BYTE	flag;
	BYTE	changetimer;
	BYTE	mtimer;

	static BGLayerSet set[BGLAYERSETMAX];
	static WORD setindex;
};

extern BGLayer bg[BGLAYERMAX];
extern BGLayer fg[FGLAYERMAX];
extern BGLayer bgmask;
extern BGLayer fgpanel;
extern BGLayer fgpause;
extern BGLayer fgdiff;

extern BGLayer * ubg[UBGLAYERMAX];

#endif