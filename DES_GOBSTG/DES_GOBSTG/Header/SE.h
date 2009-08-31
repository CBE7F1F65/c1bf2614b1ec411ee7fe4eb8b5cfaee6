#ifndef _SE_H
#define _SE_H

#include "MainDependency.h"
#include "Const.h"

#define SE_DEFAULT				0

#define	SE_PLAYER_SHOOT_1		1
#define	SE_PLAYER_SHOOT_2		2
#define	SE_PLAYER_SHOOT_3		3
#define	SE_PLAYER_SHOOT_4		4
#define	SE_PLAYER_SHOOT_5		5
#define	SE_PLAYER_SHOOT_6		6
#define	SE_PLAYER_SHOOT_7		7
#define	SE_PLAYER_SHOOT_8		8
#define	SE_PLAYER_BOMB_1		9
#define	SE_PLAYER_BOMB_2		10
#define	SE_PLAYER_BOMB_3		11
#define	SE_PLAYER_BOMB_4		12
#define	SE_PLAYER_GRAZE			13
#define	SE_PLAYER_SHOT			14
#define	SE_PLAYER_DEAD			15

#define	SE_PLAYER_SLOWON		17
#define SE_PLAYER_SLOWOFF		18
#define	SE_PLAYER_CIRCLEON		19
#define	SE_PLAYER_CIRCLEOFF		20

#define	SE_BULLET_1				22
#define	SE_BULLET_2				23
#define	SE_BULLET_3				24
#define	SE_BULLET_4				25
#define	SE_BULLET_CHANGE_1		26
#define	SE_BULLET_CHANGE_2		27
#define	SE_BULLET_BOUND			28
#define	SE_BULLET_FADEOUT		29

#define	SE_BEAM_1				31
#define	SE_BEAM_2				32
#define SE_BEAM_REFLECT			33
#define SE_BEAM_FADEOUT			34

#define	SE_BOSS_UP				36
#define	SE_BOSS_DEAD			37
#define	SE_BOSS_TIMEOUT			38
#define SE_BOSS_TIMEUP			39
#define	SE_BOSS_KIRA			40
#define	SE_BOSS_POWER_1			41
#define	SE_BOSS_POWER_2			42
#define SE_BOSS_BONUS_1			43
#define SE_BOSS_BONUS_2			44

#define	SE_ENEMY_DAMAGE_1		46
#define	SE_ENEMY_DAMAGE_2		47
#define	SE_ENEMY_DEAD			48

#define	SE_GHOST_DEAD			50
#define SE_GHOST_MERGE			51
#define SE_GHOST_HALF			52

#define	SE_ITEM_GET				54
#define	SE_ITEM_EXTEND			55
#define	SE_ITEM_POWERUP			56

#define	SE_SYSTEM_OK			58
#define	SE_SYSTEM_CANCEL		59
#define	SE_SYSTEM_SELECT		60
#define	SE_SYSTEM_PAUSE			61

struct seStruct
{
	HEFFECT eff;
	HCHANNEL chn;
	float sum;
	WORD counter;
};

class SE
{
public:
	static bool Initial();
	static bool have(BYTE type);
	static void play();
	static void push(BYTE type, float x = M_CLIENT_CENTER_X);
	static void stop(BYTE type);

public:
	static seStruct se[SEMAX];
	static BYTE vol;
};

#endif