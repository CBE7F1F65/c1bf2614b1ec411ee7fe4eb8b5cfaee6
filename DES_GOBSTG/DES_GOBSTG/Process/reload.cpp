#include "processPrep.h"

bool Process::reload()
{
	SetCurrentDirectory(hge->Resource_MakePath(""));

	hge->frameSkip = 0;

	strcpy(rpyfilename, "");
	replayIndex = 0;

	Selector::Clear();
	Player::p.exist = false;
	for(int i=0; i<BGLAYERSETMAX; i++)
	{
		BGLayer::set[i].sID = 0;
	}
	for(int i=0;i<BGLAYERMAX;i++)
	{
		bg[i].exist = false;
	}
	for(int i=0;i<FGLAYERMAX;i++)
	{
		fg[i].exist = false;
	}
	bgmask.exist = false;
	fgpause.exist = false;
	fgpanel.exist = false;
	fgdiff.exist = false;
	for(int i=0;i<ENEMYMAX;i++)
	{
		en[i].exist = false;
		en[i].able = false;
	}
	for(int i=0;i<GHOSTMAX;i++)
	{
		gh[i].exist = false;
		gh[i].able = false;
	}	
	for(int i=0;i<EFFECTSPMAX;i++)
	{
		es[i].exist = false;
	}
	for(int i=0;i<TARGETMAX;i++)
	{
		tar[i].x = 0;
		tar[i].y = 0;
	}
	infoselect.clear();
	Chat::chatting = false;
	Chat::timer = 0;
	Chat::chati = 0;
	Chat::chatinit = false;
	BossInfo::empty();
	Player::ncCont = 0;
	Player::ncGet = 0;
	Player::ncBomb = 0;
	Player::ncMiss = 0;
	Player::ncPause = 0;
	pauseinit = false;

	practicemode = false;
	rangemode = false;
	replaymode = false;

	hge->Channel_Stop(channel);
	if(stream)
		hge->Stream_Free(stream);
	channel = NULL;
	stream = NULL;
	musicpos = 0;
	replayFPS = 0;

	scene = S100;

	worldx = 0;
	worldy = 0;
	worldz = 0;

	Bullet::Init(tex[TEX_BULLET]);
	Enemy::Init(tex[TEX_ENEMYBEGIN]);
	EffectSp::tex = tex[TEX_OTHER];

	Ghost::index = 0;
	PlayerGhost::tex = tex[TEX_GHOST];
	
	if(!Effectsys::Init(tex, res.resdata.effectsysfoldername, res.resdata.effectsysfilename))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in Initializing Effectsys.", HGELOG_ERRSTR);
#endif
		return false;
	}

	if(!Player::Init(&tex[TEX_PL0], tex[TEX_PLAYERS]))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in Initializing Player.", HGELOG_ERRSTR);
#endif
		return false;
	}

	Item::Init(tex[TEX_OTHER], res.resdata.normalfontfilename);
	Beam::Init();
	Chat::Init(tex[TEX_PLAYERS], tex[TEX_ENEMIES], tex[TEX_TEXT], tex[TEX_WORD]);

	BossInfo::Init(res.resdata.normalfontfilename);
	InfoQuad::tex = tex[TEX_WHITE];

#ifdef __DEBUG
	HGELOG("\nCleared up.\n");
#endif

	SetCurrentDirectory(hge->Resource_MakePath(""));

	return true;
}