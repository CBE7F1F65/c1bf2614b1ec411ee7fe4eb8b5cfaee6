#ifndef _PLAYER_H
#define _PLAYER_H

#include "BObject.h"
#include "EffectSp.h"
#include "PlayerGhost.h"
#include "PlayerBullet.h"
#include "Effectsys.h"

#define PL_DEFAULTNBOMB		3
#define PL_DEFAULTNPLAYER	RESCONFIGDEFAULT_MAXPLAYER

#define PL_DEFAULTR			1.0f
#define PL_DEFAULTGRAZER	40.0f

#define PLSHOTDELAY_NORMAL	16
#define PLSHOTDELAY_LONG	21
#define PLSHOTDELAY_ADD		12

/***********************************/
#define PL_DEFAULTEXTEND_0	0
#define PL_DEFAULTEXTEND_1	0
#define PL_DEFAULTEXTEND_2	0
#define PL_DEFAULTEXTEND_3	0
#define PL_DEFAULTEXTEND_4	0
#define PL_DEFAULTEXTEND_5	0
#define PL_DEFAULTEXTEND_6	0
#define PL_DEFAULTEXTEND_7	0
#define PL_DEFAULTEXTEND_8	0
#define PL_DEFAULTEXTEND_9	0
/**********************************/

#define PL_DEFAULTMOVESPEED_0	3.6f
#define PL_DEFAULTMOVESPEED_1	1.8f
#define PL_DEFAULTMOVESPEED_2	3.5f
#define PL_DEFAULTMOVESPEED_3	2.1f
#define PL_DEFAULTMOVESPEED_4	4.5f
#define PL_DEFAULTMOVESPEED_5	1.5f
#define PL_DEFAULTMOVESPEED_6	4.2f
#define PL_DEFAULTMOVESPEED_7	2.4f

#define PL_DEFAULTCIRCLEMOVESPEED	4.8f

#define PL_MOVABLEDGE_X		30
#define PL_MOVABLEDGE_Y		30
#define PL_MOVABLE_LEFT		(M_ACTIVECLIENT_LEFT+PL_MOVABLEDGE_X)
#define PL_MOVABLE_RIGHT	(M_ACTIVECLIENT_RIGHT-PL_MOVABLEDGE_X)
#define PL_MOVABLE_TOP		(M_ACTIVECLIENT_TOP+PL_MOVABLEDGE_Y)
#define PL_MOVABLE_BOTTOM	(M_ACTIVECLIENT_BOTTOM-PL_MOVABLEDGE_Y)

#define PL_MERGEPOS_X		M_ACTIVECLIENT_CENTER_X
#define PL_MERGEPOS_Y		(M_ACTIVECLIENT_BOTTOM + 40)

#define PL_SAVELASTMAX		0x20
#define PL_ESBOMBMAX		0x50
#define PL_NESCIRCLE		6
#define PL_NPLAYERGHOST		4

#define PL_NPOPPERCIRCLE	200
#define PL_NPOPMAX			(PL_NPOPPERCIRCLE*PL_NESCIRCLE)
#define PL_NPOPLOSTPERFRAME(ISWUSHUANG)	((ISWUSHUANG) ? 4 : 6)

#define PL_COLLAPSEPENALTY_POWER		16

#define PL_ALIVENESSPENALTY_POWERSCALE(NPOWER)		((NPOWER) / 64)
#define PL_COLLAPSEPENALTY_ALIVENESS(NPOWER)		(500 * PL_ALIVENESSPENALTY_POWERSCALE(NPOWER))
#define PL_BOMBPENALTY_ALIVENESS(NPOWER)			(250 * PL_ALIVENESSPENALTY_POWERSCALE(NPOWER))
#define PL_DEADBOMBPENALTY_ALIVENESS(NPOWER)		(400 * PL_ALIVENESSPENALTY_POWERSCALE(NPOWER))
#define PL_CIRCLECHANGECOST_ALIVENESS(NPOWER)		(100 * PL_ALIVENESSPENALTY_POWERSCALE(NPOWER))
#define PL_UNCIRCLECHANGECOST_ALIVENESS(NPOWER)		(100 * PL_ALIVENESSPENALTY_POWERSCALE(NPOWER))

#define PL_GRAZEADD(BSLOW)							((BSLOW)?1:2)
#define PL_GRAZEBONUS_SCORE(BSLOW)					(2000 * PL_GRAZEADD(BSLOW))
#define PL_GRAZEBONUS_NPOP							100
#define PL_GRAZEBONUS_BOSSBONUS(MAXBONUS, LIMIT)	((MAXBONUS)/((LIMIT) * 60))
#define PL_GRAZEBONUS_BOSSREGAIN(NGRAZE)			(200 * (NGRAZE))
#define PL_BOSSBONUS_FAILED(MAXBONUS)				((MAXBONUS) / 100)
#define PL_ENEMYBONUS_COLLAPSE(MAXLIFE)				((int)((MAXLIFE) * 5000))
#define PL_GHOSTBONUS_COLLAPSE(MAXLIFE)				((int)((MAXLIFE) * 5000))
#define PL_GHOSTBONUS_HALF(MAXLIFE)					((int)((MAXLIFE) * 3000))
#define PL_SHOOTBONUS_NPOP							20
#define PL_NPOPBONUS_SCORE(NPOP)					((NPOP)/8)

#define PL_ITEMBONUS_POINT(ALIVENESS)				(100000 + (ALIVENESS) * 10)
#define PL_ITEMBONUS_POWER							100
#define PL_ITEMBONUS_BIGPOWER						100
#define PL_ITEMBONUS_FULL							100
#define PL_ITEMBONUS_BOMB							5000
#define PL_ITEMBONUS_PLAYER							5000
#define PL_ITEMBONUS_SMALLCIRCLEALIVENESS			1
#define PL_ITEMBONUS_SMALLCIRCLENPOP				4
#define PL_ITEMBONUS_SMALLCIRCLE					2000
#define PL_ITEMBONUS_CIRCLESCALE					150
#define PL_ITEMBONUS_CIRCLEALIVENESS				(PL_ITEMBONUS_SMALLCIRCLENPOP * PL_ITEMBONUS_CIRCLESCALE)
#define PL_ITEMBONUS_CIRCLENPOP						(PL_ITEMBONUS_SMALLCIRCLENPOP * PL_ITEMBONUS_CIRCLESCALE)
#define PL_ITEMBONUS_CIRCLE							(PL_ITEMBONUS_SMALLCIRCLE * PL_ITEMBONUS_CIRCLESCALE)

#define PL_STAGEBONUS_STAGE_1						10000000
#define PL_STAGEBONUS_STAGE_2						15000000
#define PL_STAGEBONUS_STAGE_3						20000000
#define PL_STAGEBONUS_STAGE_4						25000000
#define PL_STAGEBONUS_STAGE_5						30000000
#define PL_STAGEBONUS_STAGE_6						40000000
#define PL_STAGEBONUS_STAGE_OTHER					50000000
#define PL_STAGEBONUS_ALIVENESSBONUS(ALIVENESS)		((ALIVENESS) * 200)
#define PL_STAGEBONUS_POINTBONUS(POINT)				((POINT) * 200000)
#define PL_STAGEBONUS_GRAZEBONUS(GRAZE)				((GRAZE) * 500)
#define PL_ALLCLEARBONUS_LIFEBONUS(LIFE)			((LIFE) * 20000000)
#define PL_ALLCLEARBONUS_BOMBBONUS(BOMB)			((BOMB) * 5000000)

#define PL_NPOWERLEVEL		6
#define PL_POWERLEVEL_0		0
#define PL_POWERLEVEL_1		8
#define PL_POWERLEVEL_2		24
#define PL_POWERLEVEL_3		48
#define PL_POWERLEVEL_4		80
#define PL_POWERLEVEL_5		128
#define PL_POWERMAX			128

#define PLEXTEND_1		100
#define PLEXTEND_2		250
#define PLEXTEND_3		500
#define PLEXTEND_4		800
#define PLEXTEND_5		1100
#define PLEXTEND_MAX	9999

#define PLBOMB_ENEMYLIFECOST			0.1f
#define PLBOMB_BOSSLIFECOST(MAXLIFE)	((MAXLIFE) / 720.0f)
#define PLDEADBOMB_GHOSTLIFECOST		0.2f

#define	PLAYER_MERGE				0x1
#define	PLAYER_SHOT					0x2
#define	PLAYER_COLLAPSE				0x4
#define	PLAYER_SHOOT				0x8
#define	PLAYER_BOMB					0x10
#define	PLAYER_DEADBOMB				0x20
#define	PLAYER_SLOWCHANGE			0x40
#define	PLAYER_FASTCHANGE			0x80
#define	PLAYER_CIRCLECHANGE			0x100
#define	PLAYER_UNCIRCLECHANGE		0x200
#define	PLAYER_GRAZE				0x400

class Player : public BObject
{
public:
	Player();
	virtual ~Player();

	static bool Init(HTEXTURE texdesc[M_NCHARA], HTEXTURE texcutin);

	void valueSet(WORD ID, BYTE nLife, BYTE nBomb, bool bContinue = false);
	void ClearSet();

	virtual void action();

	void RenderEffect();

	bool Merge();
	bool Shot();
	bool Collapse();
	bool Shoot();
	bool Bomb();
	bool DeadBomb();
	bool SlowChange();
	bool FastChange();
	bool CircleChange();
	bool UncircleChange();
	bool Graze();

	DWORD getnNext();

	void callCollapse();
	bool callBombDeadBomb();
	void callSlowFastChange(bool toslow);
	void callCircleUncircleChange();

	void _Shoot0(BYTE level);
	void _Shoot1(BYTE level);
	void _Shoot2(BYTE level);
	void _Shoot3(BYTE level);
	void _Shoot4(BYTE level);
	void _Shoot5(BYTE level);
	void _Shoot6(BYTE level);
	void _Shoot7(BYTE level);

	void bombAction();

	void Render();

	LONGLONG getClearBonusPoint();
	LONGLONG getClearBonusGraze();
	LONGLONG getClearBonusAliveness();

public:
	int getrange[M_GETRANGEMAX];
	float	lastx[PL_SAVELASTMAX];
	float	lasty[PL_SAVELASTMAX];
	float	lastmx[PL_SAVELASTMAX];
	float	lastmy[PL_SAVELASTMAX];

	PlayerGhost pg[PL_NPLAYERGHOST];

	EffectSp	esBomb[PL_ESBOMBMAX];
	EffectSp	esCircle[PL_NESCIRCLE];
	EffectSp	esChange;
	EffectSp	esGraze;
	EffectSp	esInfi;
	EffectSp	esShot;
	EffectSp	esCircleBox;
	EffectSp	esPoint;
	EffectSp	esCollapse;

	Effectsys	effBomb[2];
	Effectsys	effGraze;
	Effectsys	effChange;
	Effectsys	effInfi;
	Effectsys	effCollapse;
	Effectsys	effMerge;
	Effectsys	effCircle;
	Effectsys	effCircleChange;
	Effectsys	effUncircleChange;

	LONGLONG	nScore;
	LONGLONG	nHiScore;
	DWORD	nPoint;
	DWORD	nGraze;
	DWORD	nAliveness;
	DWORD	nNext;

	DWORD	nLastPoint;
	DWORD	nLastGraze;
	DWORD	nLastAliveness;

	float	lostStack;
	DWORD	circleCounter;
	DWORD	fastCounter;

	int		nPop;

	float	slowspeed;
	float	r;
	float	rgraze;

	bool	bCircle;
	bool	bSlow;
	bool	bInfi;
	bool	bDrain;
	bool	bFull;

	hgeSprite * sprite;

	WORD	flag;

	WORD	mergetimer;
	WORD	shottimer;
	WORD	collapsetimer;
	WORD	shoottimer;
	WORD	bombtimer;
	WORD	deadbombtimer;
	WORD	slowtimer;
	WORD	fasttimer;
	WORD	circletimer;
	WORD	uncircletimer;

	BYTE	nLife;
	BYTE	nBomb;
	BYTE	nPower;
	BYTE	shotdelay;

	static Player p;
	static PlayerBullet _pb;
	static HTEXTURE texdesc[M_NCHARA];
	static HTEXTURE	tex;
	static HTEXTURE texcutin;
	static	BYTE	ncMiss;
	static	BYTE	ncBomb;
	static	BYTE	ncCont;
	static	BYTE	ncGet;
	static	BYTE	ncPause;

};
#endif