#include "processPrep.h"

void Process::clearPrep(bool bclearkey)
{
	Selector::Clear();
	for(int i=0; i<EFFECTSYSMAX; i++)
	{
		effsys[i].exist = false;
	}
	for(int i=0; i<BGLAYERSETMAX; i++)
	{
		BGLayer::set[i].sID = 0;
	}
	BGLayer::setindex = 0;
	for(int i=0;i<BGLAYERMAX;i++)
	{
		bg[i].exist = false;
		bg[i].changetimer = 0;
		bg[i].timer = 0;
		bg[i].flag = 0;
	}
	for(int i=0;i<FGLAYERMAX;i++)
	{
		fg[i].exist = false;
		fg[i].changetimer = 0;
		fg[i].timer = 0;
		fg[i].flag = 0;
	}
	for(int i=0;i<ENEMYMAX;i++)
	{
		en[i].exist = false;
		en[i].able = false;
		en[i].timer = 0;
	}
	Enemy::index = ENEMY_INDEXSTART;
	for(int i=0;i<GHOSTMAX;i++)
	{
		gh[i].exist = false;
		gh[i].able = false;
		gh[i].timer = 0;
	}
	Ghost::index = 0;
	for(int i=0;i<EFFECTSPMAX;i++)
	{
		es[i].exist = false;
		es[i].timer = 0;
	}
	for(int i=0;i<TARGETMAX;i++)
	{
		tar[i].x = 0;
		tar[i].y = 0;
	}
	bu.clear_item();
	Bullet::index = 0;
	ZeroMemory(Bullet::_bu.actionList, sizeof(int) * BULLETACTIONMAX);
//	Bullet::_bu.actionList[0] = 0;
	mi.clear_item();
	pb.clear_item();
	be.clear_item();
	Item::infofont.clear_item();
	infoselect.clear();
	Chat::chatting = false;
	Chat::timer = 0;
	Chat::chati = 0;
	Chat::chatinit = false;
	BossInfo::empty();
	pauseinit = false;

	worldx = 0;
	worldy = 0;
	worldz = 0;

	hge->frameSkip = 0;

	Ghost::totalghost = 0;
	Ghost::totalhalfghost = 0;
	Scripter::stopEdefScript = false;

	bgmask.changetimer = 0;
	bgmask.timer = 0;
	bgmask.flag = 0;
	bgmask.exist = false;
	fgpanel.changetimer = 0;
	fgpanel.timer = 0;
	fgpanel.flag = 0;
	fgpause.changetimer = 0;
	fgpause.timer = 0;
	fgpause.flag = 0;
	fgpause.exist = false;

	Player::p.ClearSet();

	if(!bclearkey)
		return;

	hge->Input_SetDIKey(KS_UP, false);
	hge->Input_SetDIKey(KS_DOWN, false);
	hge->Input_SetDIKey(KS_LEFT, false);
	hge->Input_SetDIKey(KS_RIGHT, false);
	hge->Input_SetDIKey(KS_FIRE, false);
	hge->Input_SetDIKey(KS_BOMB, false);
	hge->Input_SetDIKey(KS_CIRCLE, false);
	hge->Input_SetDIKey(KS_SLOW, false);
}

void Process::startPrep()
{
	BYTE tmaxplayer = maxplayer;
	replayend = false;
	replayIndex = 0;

	SetCurrentDirectory(hge->Resource_MakePath(""));

	if(replaymode)
	{
		rpy.Load(rpyfilename, true);
		BYTE part = 0;
		if(scene < S900)
			part = scene / M_STAGENSCENE - 1;
		seed = rpy.partinfo[part].seed;
		scene = rpy.partinfo[part].scene;
		luchara = rpy.rpyinfo.usingchara;
		tmaxplayer = rpy.rpyinfo.maxplayer;
		rangemode = rpy.rpyinfo.modeflag & RPYMODE_RANGE;
		practicemode = (bool)(rpy.rpyinfo.modeflag & RPYMODE_PRACTICE);

		replayIndex = rpy.partinfo[part].offset - 1;
	}
	else
	{
		seed = timeGetTime();
	}
	clearPrep();

	srand(seed);

	Player::ncCont = 0;
	Player::ncGet = 0;
	Player::ncBomb = 0;
	Player::ncMiss = 0;
	Player::ncPause = 0;

	//set
	fgpanel.valueSet(tex[TEX_BKG], 320, 240, 640, 480, 1, 1, 1, 1);
	fgpause.valueSet(tex[TEX_WHITE], 220, 240, 440, 480, 1, 1, 1, 1);
	fgpause.colorSet(0, 0, 0, 0);
	fgpause.exist = false;

	startscene = scene;
	nowdifflv = data.getDiff(scene);
	if(!practicemode)
	{
		if(scene < S900)
			Player::p.valueSet(luchara<<1, tmaxplayer, PL_DEFAULTNBOMB);
		else
			Player::p.valueSet(luchara<<1, PL_DEFAULTNPLAYER, PL_DEFAULTNBOMB);
	}
	else if(!rangemode)
	{
		Player::p.valueSet(luchara<<1, M_PL_PLAYERMAX, PL_DEFAULTNBOMB);
		Player::p.nPower = PL_POWERMAX;
		Player::p.bFull = true;
	}
	else
	{
		Player::p.valueSet(luchara<<1, 0, 0);
		Player::p.nPower = PL_POWERMAX;
		Player::p.bFull = true;
	}
	Player::p.nHiScore = DataConnector::nHiScore();
	DataConnector::Try(true);

	if(!replaymode)
	{
		ZeroMemory(&rpy.rpyinfo, sizeof(replayInfo));

		//partinfo
		BYTE part = 0;
		if(scene < S900)
			part = scene / M_STAGENSCENE - 1;
		for(int i=0;i<RPYPARTMAX;i++)
		{
			if(i != part)
			{
				ZeroMemory(&rpy.partinfo[i], sizeof(partInfo));
			}
			else
			{
				rpy.partFill(part);
			}
		}
	}
	else
	{
		BYTE part = 0;
		if(scene < S900)
			part = scene / M_STAGENSCENE - 1;
		if (part)
		{
			Player::p.nScore = rpy.partinfo[part].nowscore;
			Player::p.nPoint = Player::p.nLastPoint = rpy.partinfo[part].nowpoint;
			Player::p.nAliveness = Player::p.nLastAliveness = rpy.partinfo[part].nowaliveness;
			Player::p.nGraze = Player::p.nLastGraze = rpy.partinfo[part].nowgraze;
			Player::p.nNext = Player::p.getnNext();
		}
		Player::p.nLife = rpy.partinfo[part].nowplayer;
		Player::p.nBomb = rpy.partinfo[part].nowbomb;
		Player::p.nPower = rpy.partinfo[part].nowpower;
		if (Player::p.nPower >= PL_POWERMAX)
		{
			Player::p.bFull = true;
		}
		if(Player::p.nHiScore < Player::p.nScore)
			Player::p.nHiScore = Player::p.nScore;
	}

	bgmask.valueSet(tex[TEX_WHITE], 220, 240, 440, 480, 1, 1, 1, 1);
	bgmask.colorSet(0, 0, 0, 0);
	fgdiff.valueSet(tex[TEX_WORD], 560, 400, 220, 55, nowdifflv%4+1, nowdifflv/4+3, 4, 16);

	framecounter = 0;
	time = 0;
	alltime = 0;
}