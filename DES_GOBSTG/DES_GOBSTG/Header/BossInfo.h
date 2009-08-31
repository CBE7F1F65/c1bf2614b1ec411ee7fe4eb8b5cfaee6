#ifndef _BOSSINFO_H
#define _BOSSINFO_H

#include "MainDependency.h"
#include "InfoQuad.h"
#include "Const.h"
#include "Effectsys.h"

#define BOSSINFO_ENABLE		1
#define BOSSINFO_UP			2
#define BOSSINFO_COLLAPSE	4
#define BOSSINFO_TIMEOVER	8

#define BIAUTORANK_NONE		0
#define BIAUTORANK_SHOT		1
#define BIAUTORANK_BOMB		2

class BossInfo
{
public:
	BossInfo(){};
	~BossInfo(){};

	void bossUp();
	void bossCollapse();

	bool action();
	void quit();
	void Render();
	void RenderTimeCircle();

	bool Fill(int sno);

	static void empty();
	static bool Init(const char * fontname);
	static void Release();

public:
	char enemyname[M_STRMAX];
	char enemypinyin[M_STRMAX];
	char rangename[M_STRMAX];
	InfoQuad blood;
	DWORD bonus;
	DWORD maxbonus;
	DWORD lastgraze;
	int get;
	int meet;
	int itemstack;
	bool exist;
	bool wait;
	WORD timer;
	BYTE limit;
	BYTE remain;

	static Effectsys effUp;
	static Effectsys effCollapse;
	static Effectsys effItem;
	static Effectsys effStore;

	static bool range;
	static bool failed;
	static bool uncircled;
	static int sno;
	static int turntoscene;

	static hgeSprite * cutin;
	static hgeSprite * timecircle;
	static hgeSprite * sprange;
	static hgeSprite * spbossx;
	static hgeFont * font;

	static BYTE flag;
	static BYTE autorank;
};

extern BossInfo bossinfo;

#endif