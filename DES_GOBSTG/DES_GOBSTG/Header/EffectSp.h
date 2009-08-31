#ifndef _EFFECTSP_H
#define _EFFECTSP_H

#include "BObject.h"

#define	EFFECT_PLAYERCHANGE		0x01
#define	EFFECT_PLAYERGRAZE		0x02
#define	EFFECT_PLAYERINFI		0x03
#define	EFFECT_PLAYERSHOT		0x04
#define	EFFECT_PLAYERCIRCLE		0x05
#define EFFECT_PLAYERCIRCLEBOX	0x06
#define	EFFECT_PLAYERPOINT		0x07
#define	EFFECT_PLAYERCOLLAPSE	0x08

#define EFFECT_HOLYDARK			0x11

#define EFFECT_ENEMYCOLLAPSE	0x21
#define EFFECT_ENEMYCIRCLE0		0x22
#define EFFECT_ENEMYCIRCLE1		0x23

#define EFFECT_BOMB_0			0x31
#define EFFECT_BOMB_1			0x32
#define	EFFECT_BOMB_2			0x33
#define	EFFECT_BOMB_3			0x34
#define	EFFECT_BOMB_CUTIN		0x35
#define	EFFECT_BOMB_NAME		0x36

#define EFFECT_OVERALL			0x41

#define EFFECT_BOSSSTORE		0x81
/*
#define	EFFECT_ENVIRONMENT		0x91
#define EFFECT_ENVIRONMENT_EX	0xA1
#define EFFECT_ENVIRONMENT_EX_2	0xA2
*/

class EffectSp : public BObject
{
public:
	EffectSp();
	~EffectSp();

	void valueSet(BYTE type, float x, float y, int angle, float speed, bool onplayer = true, WORD ID = 0);
	void colorSet(DWORD color);

	void action();

//	static void environmentSet(BYTE evtype, BYTE num);

	void Render();

public:
	hgeSprite * sprite;

	bool onplayer;
	BYTE type;

	static HTEXTURE tex;
	static BYTE evtype;
};

extern EffectSp es[EFFECTSPMAX];

#endif