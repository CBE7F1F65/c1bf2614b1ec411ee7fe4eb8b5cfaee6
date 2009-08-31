#ifndef _PLAYERGHOST_H
#define _PLAYERGHOST_H

#include "BObject.h"

#define PLAYERGHOST_TRACETIME	0x10
#define PLAYERGHOST_LASTINDEX	1

class PlayerGhost : public BObject
{
public:
	PlayerGhost();
	~PlayerGhost();

	void valueSet(WORD ID);

	void matchColor();

	void Render();

	virtual void action();

public:
	hgeSprite * sprite;
	float scale;

	static HTEXTURE tex;
};

#endif