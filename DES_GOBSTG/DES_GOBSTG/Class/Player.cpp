#include "Player.h"

#include "Process.h"
#include "BGLayer.h"
#include "SE.h"

#include "PlayerBullet.h"
#include "Item.h"
#include "Ghost.h"
#include "Bullet.h"
#include "Chat.h"
#include "BossInfo.h"
#include "InfoQuad.h"

Player Player::p;

PlayerBullet Player::_pb;

HTEXTURE Player::texdesc[M_NCHARA];
HTEXTURE Player::tex;
HTEXTURE Player::texcutin;

BYTE Player::ncCont = 0;
BYTE Player::ncGet = 0;
BYTE Player::ncBomb = 0;
BYTE Player::ncMiss = 0;
BYTE Player::ncPause = 0;

Player::Player()
{
	effGraze.exist = false;
	exist			= false;
	sprite			= NULL;
}

Player::~Player()
{
	if(sprite)
		delete sprite;
	sprite = NULL;
}

bool Player::Init(HTEXTURE _texdesc[M_NCHARA], HTEXTURE _texcutin)
{
	for(int i=0;i<M_NCHARA;i++)
		texdesc[i] = _texdesc[i];
	texcutin = _texcutin;
	return true;
}

LONGLONG Player::getClearBonusPoint()
{
	LONGLONG ret = 0;
	if (nPoint > nLastPoint)
	{
		ret = PL_STAGEBONUS_POINTBONUS(nPoint - nLastPoint);
	}
	nLastPoint = nPoint;
	return ret;
}

LONGLONG Player::getClearBonusGraze()
{
	LONGLONG ret = 0;
	if (nGraze > nLastGraze)
	{
		ret = PL_STAGEBONUS_GRAZEBONUS(nGraze - nLastGraze);
	}
	nLastGraze = nGraze;
	return ret;
}

LONGLONG Player::getClearBonusAliveness()
{
	LONGLONG ret = 0;
	if (nAliveness > nLastAliveness)
	{
		ret = PL_STAGEBONUS_ALIVENESSBONUS(nAliveness - nLastAliveness);
	}
	nLastAliveness = nAliveness;
	return ret;
}

void Player::ClearSet()
{
	x			=	PL_MERGEPOS_X;
	y			=	PL_MERGEPOS_Y;

	for(int i=0;i<PL_SAVELASTMAX;i++)
	{
		lastx[i] = x;
		lasty[i] = y;
		lastmx[i] = x;
		lastmy[i] = y;
	}
	timer		=	0;
	angle		=	0;
	flag		=	PLAYER_MERGE;
	bCircle		=	false;
	bSlow		=	false;
	bInfi		=	true;
	bDrain		=	false;
	hscale		=	1.0f;
	vscale		=	1.0f;
	alpha		=	0xff;

	mergetimer		=	0;
	shottimer		=	0;
	collapsetimer	=	0;
	shoottimer		=	0;
	bombtimer		=	0;
	deadbombtimer	=	0;
	slowtimer		=	0;
	fasttimer		=	0;
	circletimer		=	0;
	uncircletimer	=	0;

	nPop			=	0;

	exist = true;

	for(int i=0;i<PL_ESBOMBMAX;i++)
		esBomb[i].exist = false;

	if (effGraze.exist)
	{
		effGraze.Stop(true);
		effGraze.MoveTo(x, y, 0, true);
	}
	if (effChange.exist)
	{
		effChange.Stop(true);
		effChange.MoveTo(x, y, 0, true);
	}
	if (effInfi.exist)
	{
		effInfi.Stop(true);
		effInfi.MoveTo(x, y, 0, true);
	}
	if (effCollapse.exist)
	{
		effCollapse.Stop(true);
		effCollapse.MoveTo(x, y, 0, true);
	}
	if (effMerge.exist)
	{
		effMerge.Stop(true);
		effMerge.MoveTo(x, y, 0, true);
	}
	if (effCircle.exist)
	{
		effCircle.Stop(true);
		effCircle.MoveTo(x, y, 0, true);
	}
	if (effCircleChange.exist)
	{
		effCircleChange.Stop(true);
		effCircleChange.MoveTo(x, y, 0, true);
	}
	if (effUncircleChange.exist)
	{
		effUncircleChange.Stop(true);
		effUncircleChange.MoveTo(x, y, 0, true);
	}
	for (int i=0; i<2; i++)
	{
		if (effBomb[i].exist)
		{
			effBomb[i].Stop(true);
		}
	}
	
	for(int i=0;i<PL_NPLAYERGHOST;i++)
	{
		pg[i].valueSet((ID<<2) + i);
	}
	
	esChange.valueSet(EFFECT_PLAYERCHANGE, x, y, 0, 0);
	esGraze.valueSet(EFFECT_PLAYERGRAZE, x, y, 160, 0);
	esInfi.valueSet(EFFECT_PLAYERINFI, x, y, 240, 0);
	esInfi.colorSet(0xffff00);
	esShot.valueSet(EFFECT_PLAYERSHOT, x, y, 0, 0);
	esShot.colorSet(0xff0000);
	for(int i=0;i<PL_NESCIRCLE;i++)
		esCircle[i].valueSet(EFFECT_PLAYERCIRCLE, x, y, 240, 0, true, i);
	esCircleBox.valueSet(EFFECT_PLAYERCIRCLEBOX, 90, 450, 0, 0);
	esPoint.valueSet(EFFECT_PLAYERPOINT, x, y, 0, 0);
	esCollapse.valueSet(EFFECT_PLAYERCOLLAPSE, x, y, 160 ,0, false);
	
	iqfPower.valueSet(
		IQF_POWER, 500, 173,
		500, 173, 0xffff0000,
		500, 173, 0xffff0000,
		500, 193, 0xff7f0000,
		500, 193, 0xff7f0000);

}

void Player::valueSet(WORD _ID, BYTE _nLife, BYTE _nBomb, bool bContinue)
{
	ID			=	_ID;
	ClearSet();
	tex			=	texdesc[ID>>1];

	PlayerBullet::Init_Player(tex);

	if(ID)
		r		=	PL_DEFAULTR;
	else
		r		=	PL_DEFAULTR/2;

	rgraze		=	PL_DEFAULTGRAZER;
	nLife		=	_nLife;
	nBomb		=	_nBomb;

	nPower		=	0;
	nGraze		=	0;
	bFull		=	false;
	nPoint		=	0;
	nAliveness	=	0;
	nScore		=	0;

	nNext		=	getnNext();

	for(int i=0;i<M_GETRANGEMAX;i++)
		getrange[i] = 0;

	nLastPoint		= 0;
	nLastGraze		= 0;
	nLastAliveness	= 0;

	if (!bContinue)
	{
		lostStack		=	0;
		fastCounter		=	0;
		circleCounter	=	0;
	}

	if (ID>>1 == M_CHARA_SHI_LINGYE)
	{
		shotdelay = PLSHOTDELAY_LONG;
	}
	else
	{
		shotdelay = PLSHOTDELAY_NORMAL;
	}

	switch(ID>>1)
	{
	case M_CHARA_SHI_LINGYE:
		speed		=	PL_DEFAULTMOVESPEED_0;
		slowspeed	=	PL_DEFAULTMOVESPEED_1;

		break;
	case M_CHARA_NUSIKUIMA_GEJI:
		speed		=	PL_DEFAULTMOVESPEED_2;
		slowspeed	=	PL_DEFAULTMOVESPEED_3;

		break;
	case M_CHARA_CILAN_ZANGYUE:
		speed		=	PL_DEFAULTMOVESPEED_4;
		slowspeed	=	PL_DEFAULTMOVESPEED_5;

		break;
	case M_CHARA_WUSHUANG:
		speed		=	PL_DEFAULTMOVESPEED_6;
		slowspeed	=	PL_DEFAULTMOVESPEED_7;

		break;
	}

	if(!sprite)
		sprite = new hgeSprite(tex, 0, 0, 32, 48);
	sprite->SetTexture(tex);
	sprite->SetTextureRect(0, 0, 32, 48);

	effGraze.valueSet(EFF_PL_GRAZE, *this);
	effGraze.Stop();
	effChange.valueSet(EFF_PL_CHANGE, *this);
	effChange.Stop();
	effInfi.valueSet(EFF_PL_INFI, *this);
	effInfi.Stop();
	effCollapse.valueSet(EFF_PL_COLLAPSE, *this);
	effCollapse.Stop();
	effMerge.valueSet(EFF_PL_MERGE, *this);
	effMerge.Stop();
	effCircle.valueSet(EFF_PL_CIRCLE, *this);
	effCircle.Stop();
	effCircleChange.valueSet(EFF_PL_CIRCLECHANGE, *this);
	effCircleChange.Stop();
	effUncircleChange.valueSet(EFF_PL_UNCIRCLECHANGE, *this);
	effUncircleChange.Stop();
	effBomb[0].valueSet(EFF_PL_BOMBBEGIN + ((ID>>1)<<1), *this);
	effBomb[0].Stop();
	effBomb[1].valueSet(EFF_PL_BOMBBEGIN + ((ID>>1)<<1) + 1, *this);
	effBomb[1].Stop();
}

DWORD Player::getnNext()
{
	if (nPoint < PLEXTEND_1)
	{
		return PLEXTEND_1;
	}
	if (nPoint < PLEXTEND_2)
	{
		return PLEXTEND_2;
	}
	if (nPoint < PLEXTEND_3)
	{
		return PLEXTEND_3;
	}
	if (nPoint < PLEXTEND_4)
	{
		return PLEXTEND_4;
	}
	if (nPoint < PLEXTEND_5)
	{
		return PLEXTEND_5;
	}
	return PLEXTEND_MAX;
}

void Player::action()
{
	float nowspeed = 0;
	timer++;

	/*
	flag &= ~PLAYER_SHOT;
	flag &= ~PLAYER_COLLAPSE;
	//*/
	alpha = 0xff;
	if(timer == 1)
		flag |= PLAYER_MERGE;

	if (nPoint >= nNext)
	{
		SE::push(SE_ITEM_EXTEND, x);
		nLife++;
		if(nLife > M_PL_PLAYERMAX)
		{
			nLife = M_PL_PLAYERMAX;
			nBomb++;
		}
		if(nBomb > M_PL_BOMBMAX)
			nBomb = M_PL_BOMBMAX;
		nNext = getnNext();
	}

	//savelast
	if(lastmx[0] != x || lastmy[0] != y)
	{
		for(int i=PL_SAVELASTMAX-1;i>0;i--)
		{
			lastmx[i] = lastmx[i-1];
			lastmy[i] = lastmy[i-1];
		}
		lastmx[0] = x;
		lastmy[0] = y;
	}
	for(int i=PL_SAVELASTMAX-1;i>0;i--)
	{
		lastx[i] = lastx[i-1];
		lasty[i] = lasty[i-1];
	}
	lastx[0] = x;
	lasty[0] = y;

	//input
	if(!(flag & PLAYER_SHOT || flag & PLAYER_COLLAPSE))
	{
		if(!(flag & PLAYER_BOMB || flag & PLAYER_DEADBOMB))
		{
			if (hge->Input_GetDIKey(KS_SLOW_MP))
			{
				bSlow = true;
				flag &= ~PLAYER_FASTCHANGE;
				if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_DOWN))
				{
					flag |= PLAYER_SLOWCHANGE;
				}
			}
			else
			{
				bSlow = false;
				flag &= ~PLAYER_SLOWCHANGE;
				if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_UP))
				{
					flag |= PLAYER_FASTCHANGE;
				}
			}
			if(bCircle && !chat.chatting && !bInfi)
			{
				Player::p.nScore += PL_NPOPBONUS_SCORE(nPop);
				if(nPop > PL_NPOPMAX)
				{
					nPop = PL_NPOPMAX;
				}
				bool iswushuang = (bool)((ID>>1) == 3);
				if(nPop >= PL_NPOPLOSTPERFRAME(iswushuang))
					nPop -= PL_NPOPLOSTPERFRAME(iswushuang);
				else
				{
					bCircle = false;
					flag |= PLAYER_UNCIRCLECHANGE;
					flag &= ~PLAYER_CIRCLECHANGE;
				}
			}
		}
		sprite->SetTextureRect((float)(ID&1)*128+32*((timer/8)%4), 0, 32, 48);
		if(bSlow)
		{
			nowspeed = slowspeed;
			esInfi.angle = 240;
		}
		else
		{
			nowspeed = speed;
			esInfi.angle = -240;
		}
		if(!(flag & PLAYER_BOMB || flag & PLAYER_DEADBOMB))
		{
			if(hge->Input_GetDIKey(KS_CIRCLE_MP, DIKEY_DOWN) && !mp.rangemode)
			{
				if(!bCircle)
				{
					bCircle = true;
					flag |= PLAYER_CIRCLECHANGE;
					flag &= ~PLAYER_UNCIRCLECHANGE;
				}
				else
				{
					bCircle = false;
					flag |= PLAYER_UNCIRCLECHANGE;
					flag &= ~PLAYER_CIRCLECHANGE;
				}
			}
		}
		if((hge->Input_GetDIKey(KS_UP_MP) ^ hge->Input_GetDIKey(KS_DOWN_MP)) &&
			hge->Input_GetDIKey(KS_LEFT_MP) ^ hge->Input_GetDIKey(KS_RIGHT_MP))
			nowspeed *= M_SQUARE_2;
		if(hge->Input_GetDIKey(KS_UP_MP))
			y -= nowspeed;
		if(hge->Input_GetDIKey(KS_DOWN_MP))
			y += nowspeed;
		if(hge->Input_GetDIKey(KS_LEFT_MP))
		{
			sprite->SetTextureRect(32*((timer/8)%4), (float)((ID&1)+1)*48, 32, 48);
			x -= nowspeed;
		}
		if(hge->Input_GetDIKey(KS_RIGHT_MP))
		{
			sprite->SetTextureRect(128+32*((timer/8)%4), (float)((ID&1)+1)*48, 32, 48);
			x += nowspeed;
			if(hge->Input_GetDIKey(KS_LEFT_MP))
				sprite->SetTextureRect((float)(ID&1)*128+32*((timer/8)%4), 0, 32, 48);
		}
		if(hge->Input_GetDIKey(KS_FIRE_MP) && !Chat::chatting)
			flag |= PLAYER_SHOOT;
	}
	if(hge->Input_GetDIKey(KS_BOMB_MP))
	{
		callBombDeadBomb();
	}
	//flag
	if(flag & PLAYER_MERGE)
		if(Merge())
			flag &= ~PLAYER_MERGE;
	if(flag & PLAYER_SHOT)
	{
		bool shotdelaychange = BossInfo::range && (BossInfo::autorank & BIAUTORANK_SHOT);
		if (shotdelaychange)
		{
			shotdelay += PLSHOTDELAY_ADD;
		}
		if(Shot())
		{
			flag &= ~PLAYER_SHOT;
		}
		if (shotdelaychange)
		{
			shotdelay -= PLSHOTDELAY_ADD;
		}
	}
	if(flag & PLAYER_COLLAPSE)
		if(Collapse())
			flag &= ~PLAYER_COLLAPSE;
	if(flag & PLAYER_SLOWCHANGE)
		if(SlowChange())
			flag &= ~PLAYER_SLOWCHANGE;
	if(flag & PLAYER_FASTCHANGE)
		if(FastChange())
			flag &= ~PLAYER_FASTCHANGE;
	if(flag & PLAYER_CIRCLECHANGE)
		if(CircleChange())
			flag &= ~PLAYER_CIRCLECHANGE;
	if(flag & PLAYER_UNCIRCLECHANGE)
		if(UncircleChange())
			flag &= ~PLAYER_UNCIRCLECHANGE;
	if(flag & PLAYER_SHOOT)
		if(Shoot())
			flag &= ~PLAYER_SHOOT;
	if(flag & PLAYER_BOMB)
		if(Bomb())
			flag &= ~PLAYER_BOMB;
	if(flag & PLAYER_DEADBOMB)
		if(DeadBomb())
			flag &= ~PLAYER_DEADBOMB;
	if(flag & PLAYER_GRAZE)
		if(Graze())
			flag &= ~PLAYER_GRAZE;

	if (!(flag & PLAYER_MERGE) || mergetimer >= 32)
	{
		if(x > PL_MOVABLE_RIGHT)
			x = PL_MOVABLE_RIGHT;
		else if(x < PL_MOVABLE_LEFT)
			x = PL_MOVABLE_LEFT;
		if(y > PL_MOVABLE_BOTTOM)
			y = PL_MOVABLE_BOTTOM;
		else if(y < PL_MOVABLE_TOP)
			y = PL_MOVABLE_TOP;
	}

	if(nScore > nHiScore)
		nHiScore = nScore;

	if (bInfi && timer % 16 < 8)
	{
		diffuse = 0xff4040;
	}
	else
		diffuse = 0xffffff;

	esChange.action();
	esGraze.action();
	esInfi.action();
	esShot.action();
	if(bCircle || (flag & PLAYER_UNCIRCLECHANGE))
	{
		for(int i=0;i<PL_NESCIRCLE;i++)
			esCircle[i].action();
	}
	esPoint.action();
	
	iqfPower.quad.v[1].x = (float)(nPower + iqfPower.x);
	iqfPower.quad.v[2].x = (float)(nPower + iqfPower.x);
	
	effGraze.MoveTo(x, y);
	effGraze.action();
	effBomb[0].action();
	effBomb[1].action();
	effCollapse.action();
	if(!(flag & PLAYER_GRAZE))
		effGraze.Stop();

	for(int i=0;i<PL_NPLAYERGHOST;i++)
		pg[i].action();

	if(!bSlow)
		fastCounter++;
	if(bCircle)
		circleCounter++;
}

void Player::callCollapse()
{
	if (flag & PLAYER_COLLAPSE)
	{
		return;
	}
	flag |= PLAYER_COLLAPSE;
	collapsetimer = 0;
}

bool Player::callBombDeadBomb()
{
	if(nBomb && !(flag & PLAYER_BOMB || flag & PLAYER_DEADBOMB) && !(flag & PLAYER_COLLAPSE) && !Chat::chatting)
	{
		bInfi = true;
		nBomb--;
		ncBomb++;
		SE::push(SE_PLAYER_BOMB_1+(ID>>1), x);
		if(bCircle)
			flag |= PLAYER_DEADBOMB;
		else
			flag |= PLAYER_BOMB;
		return true;
	}
//	if(hge->Input_GetDIKey(KS_BOMB_MP, DIKEY_DOWN))
//		SE::push(SE_SYSTEM_CANCEL);
	return false;
}

void Player::callSlowFastChange(bool toslow)
{
	if (toslow)
	{
		hge->Input_SetDIKey(KS_SLOW_MP);
	}
	else
	{
		hge->Input_SetDIKey(KS_SLOW_MP, false);
	}
}

void Player::callCircleUncircleChange()
{
	if (bCircle)
	{
		if (flag & PLAYER_UNCIRCLECHANGE)
		{
			return;
		}
		flag &= ~PLAYER_CIRCLECHANGE;
		flag |= PLAYER_UNCIRCLECHANGE;
		bCircle = false;
	}
	else
	{
		if (flag & PLAYER_CIRCLECHANGE)
		{
			return;
		}
		flag &= ~PLAYER_UNCIRCLECHANGE;
		flag |= PLAYER_CIRCLECHANGE;
		bCircle = true;
	}
	circletimer = 0;
	uncircletimer = 0;
}

bool Player::Merge()
{
	mergetimer++;
	bInfi = true;
	if(flag & PLAYER_BOMB || flag & PLAYER_DEADBOMB)
	{
		mergetimer = 0;
		vscale = 1.0f;
		return true;
	}
	if(mergetimer == 1)
	{
		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
		if(mp.rangemode)
		{
			flag |= PLAYER_CIRCLECHANGE;
			circletimer = 0;
			flag &= ~PLAYER_UNCIRCLECHANGE;
		}
		esInfi.hscale = 2.0f;
	}
	if(mergetimer < 32)
	{
		flag &= ~PLAYER_SHOOT;

		IzeZone _ize;
		_ize.x = 0;
		_ize.y = 0;
		_ize.r = BULLET_IZEOVERZONE;
		Bullet::izel[BULLETIZE_FADEOUT].push_back(_ize);
		/*
		for(VectorList<Bullet>::iterator i=bu.begin();i!=bu.end();i++)
		{
			if(i->able && i->cancelable)
			{
				i->timer = 0;
				i->fadeout = true;
			}
		}
		*/
		x = PL_MERGEPOS_X;
		y = PL_MERGEPOS_Y - mergetimer * 4.5f;
		sprite->SetTextureRect((float)(ID&1)*128+32*((timer/8)%4), 0, 32, 48);
		alpha = mergetimer * 8;
//		vscale = (float)(32-mergetimer)/10.0f + 1.0f;
	}
	else if(mergetimer == 32 && mp.rangemode)
	{
		mergetimer = 208;
	}
	else if(mergetimer > 208 && mergetimer < 240)
	{
		alpha = 0xff;
		esInfi.hscale = (240-mergetimer) * 0.0625f;
	}
	else if(mergetimer == 240)
	{
		mergetimer = 0;
		bInfi = false;
		return true;
	}
	return false;
}

bool Player::Shot()
{
	shottimer++;
	if(flag & PLAYER_BOMB || flag & PLAYER_DEADBOMB)
	{
		shottimer = 0;
		return true;
	}
	else if(shottimer == 1)
	{
		Item::undrainAll();
		SE::push(SE_PLAYER_SHOT, x);
	}
	else if (shottimer == 8 && BossInfo::range && (BossInfo::autorank & BIAUTORANK_BOMB))
	{
		if (callBombDeadBomb())
		{
			shottimer = 0;
			return true;
		}
	}
	else if(shottimer == shotdelay)
	{
		shottimer = 0;
		flag |= PLAYER_COLLAPSE;
		return true;
	}

	esShot.hscale = (shotdelay - shottimer) * 2.0f / shotdelay;
	return false;
}

bool Player::Collapse()
{
	collapsetimer++;
	if(collapsetimer == 1)
	{
		DWORD tminus = PL_COLLAPSEPENALTY_ALIVENESS(nPower);
		if(nAliveness > tminus)
		{
			nAliveness -= tminus;
		}
		else
			nAliveness = 0;

		if(BossInfo::range)
			BossInfo::failed = true;
		esCollapse.x = x;
		esCollapse.y = y;
		SE::push(SE_PLAYER_DEAD, x);
		bFull = false;

		float aimx;
		float aimy;
		for(int i=0;i<5;i++)
		{
			aimx = (float)(rand()%360 + 40);
			aimy = (float)(rand()%80 - 40 - (480 - y) / 2);
			Item::Build(nLife ? ITEM_POWER : ITEM_FULL, x, y + 36, false, 18000 + rMainAngle(aimx, aimy), -sqrt(2 * 0.1f * DIST(x, y, aimx, aimy)));
		}
		aimx = (float)(rand()%360 + 40);
		aimy = (float)(rand()%80 - 40 - (480 - y) / 2);
		Item::Build(nLife ? ITEM_BIGPOWER : ITEM_FULL, x, y + 32, false, 18000 + rMainAngle(aimx, aimy), -sqrt(2 * 0.1f * DIST(x, y, aimx, aimy)));

		effCollapse.MoveTo(x, y , 0, true);
		effCollapse.Fire();
	}
	else if(collapsetimer == 64)
	{
		x = PL_MERGEPOS_X;
		y = PL_MERGEPOS_Y;
		for(int i=0;i<PL_SAVELASTMAX;i++)
		{
			lastx[i] = x;
			lasty[i] = y;
			lastmx[i] = x;
			lastmy[i] = y;
		}

		timer = 0;
		collapsetimer = 0;
		vscale = 1.0f;
		flag |= PLAYER_MERGE;
		bInfi = true;
		ncMiss++;
		if(nLife)
		{
			nLife--;
			nBomb = PL_DEFAULTNBOMB;
		}
		else
		{
			p.exist = false;
			return true;
		}
		if(nPower > PL_COLLAPSEPENALTY_POWER)
			nPower -= PL_COLLAPSEPENALTY_POWER;
		else
			nPower = 0;

		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
		if(bCircle)
		{
			flag |= PLAYER_UNCIRCLECHANGE;
			uncircletimer = 0;
			flag &= ~PLAYER_CIRCLECHANGE;
		}

		effCollapse.Stop();

		return true;
	}
	
	esCollapse.hscale = collapsetimer / 1.5f;
	esCollapse.alpha = (BYTE)((WORD)(0xff * collapsetimer) / 0x3f);
	esCollapse.colorSet(0xff0000);

	alpha = (0xff - collapsetimer * 4);
	vscale = (float)(collapsetimer)/40.0f + 1.0f;
	return false;
}

bool Player::Bomb()
{
	bombtimer++;
	bombAction();
	if(bombtimer == 1)
	{
		DWORD tminus = PL_BOMBPENALTY_ALIVENESS(nPower);
		if(nAliveness > tminus)
		{
			nAliveness -= tminus;
		}
		else
			nAliveness = 0;

		if(BossInfo::range)
			BossInfo::failed = true;
		
		esInfi.hscale = 2.0f;
		Item::drainAll();
	}
	else if(bombtimer > 208 && bombtimer < 240)
	{
		esInfi.hscale = (240-bombtimer) * 0.0625f;
	}
	else if(bombtimer == 240)
	{
		bombtimer = 0;
		bInfi = false;
		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
		if(hge->Input_GetDIKey(KS_CIRCLE_MP))
		{
			flag |= PLAYER_CIRCLECHANGE;
			circletimer = 0;
			flag &= ~PLAYER_UNCIRCLECHANGE;
		}
		return true;
	}
	return false;
}

bool Player::DeadBomb()
{
	deadbombtimer++;
	bombAction();
	if(deadbombtimer == 1)
	{
		DWORD tminus = PL_DEADBOMBPENALTY_ALIVENESS(nPower);
		if(nAliveness > tminus)
		{
			nAliveness -= tminus;
		}
		else
			nAliveness = 0;

		if(BossInfo::range)
			BossInfo::failed = true;

		esInfi.hscale = 2.0f;
		Item::drainAll();
		esShot.hscale = 2.0f;
	}
	else if(deadbombtimer > 208 && deadbombtimer < 240)
	{
		esInfi.hscale = (240-deadbombtimer) * 0.0625f;
		esShot.hscale = (240-deadbombtimer) * 0.0625f;
	}
	else if(deadbombtimer == 240)
	{
		deadbombtimer = 0;
		bInfi = false;
		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
		flag |= PLAYER_UNCIRCLECHANGE;
		uncircletimer = 0;
		flag &= ~PLAYER_CIRCLECHANGE;

		return true;
	}
	return false;
}

bool Player::SlowChange()
{
	if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_DOWN))
		slowtimer = 0;
	bSlow = true;
	slowtimer++;
	if(slowtimer == 1)
	{
		ID=((ID>>1)<<1) +1;
		SE::push(SE_PLAYER_SLOWON, x);
		for(int i=0;i<PL_NPLAYERGHOST;i++)
		{
			pg[i].ID = (Player::p.ID<<2) + i;
			pg[i].matchColor();
			pg[i].timer = 0;
		}
	}
	else if(slowtimer == 16)
	{
		esPoint.alpha = 0xff;
		slowtimer = 0;
		return true;
	}
	esChange.colorSet(0x3030ff | ((slowtimer * 16)<<16));
	esGraze.colorSet(0x3030ff | ((slowtimer * 16)<<16));

	if(bCircle)
	{
		for(int i=0;i<PL_NESCIRCLE;i++)
		{
			float taimspeed = PL_DEFAULTCIRCLEMOVESPEED;
			esCircle[i].speed += (taimspeed - esCircle[i].speed) / (16 - slowtimer);

		}
	}
	esGraze.angle = (8 - slowtimer) * 20;
	esPoint.alpha = slowtimer * 16;
	return false;
}

bool Player::FastChange()
{
	if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_UP))
		fasttimer = 0;
	bSlow = false;
	fasttimer++;
	if(fasttimer == 1)
	{
		ID=(ID>>1)<<1;
		SE::push(SE_PLAYER_SLOWOFF, x);
		for(int i=0;i<PL_NPLAYERGHOST;i++)
		{
			pg[i].ID = (Player::p.ID<<2) + i;
			pg[i].matchColor();
			pg[i].timer = 0;
		}
	}
	else if(fasttimer == 16)
	{
		fasttimer = 0;
		return true;
	}
	esChange.colorSet(0xff30ff - ((fasttimer * 16)<<16));
	esGraze.colorSet(0xff30ff - ((fasttimer * 16)<<16));

	if(bCircle)
	{
		for(int i=0;i<PL_NESCIRCLE;i++)
		{
			float taimspeed = -PL_DEFAULTCIRCLEMOVESPEED;
			esCircle[i].speed += (taimspeed - esCircle[i].speed) / (16 - fasttimer);
		}
	}
	esGraze.angle = (fasttimer - 8) * 20;
	return false;
}

bool Player::CircleChange()
{
	nPop = PL_NPOPMAX;
	if(hge->Input_GetDIKey(KS_CIRCLE_MP, DIKEY_DOWN))
		circletimer = 0;
	bCircle = true;
	circletimer++;
	if(circletimer == 1)
	{
		bgmask.flag = BG_YELLOWFLASH;
		bgmask.changetimer = 0;

		esCircleBox.sprite->SetTextureRect(128, 192, 128, 32);
		SE::push(SE_PLAYER_CIRCLEON, x);

		DWORD tminus = PL_CIRCLECHANGECOST_ALIVENESS(nPower);
		if(nAliveness > tminus)
			nAliveness -= tminus;
		else
			nAliveness = 0;
	}
	else if(circletimer == 16)
	{
		circletimer = 0;
		return true;
	}
	for(int i=0;i<PL_NESCIRCLE;i++)
	{
		float taimspeed = bSlow ? PL_DEFAULTCIRCLEMOVESPEED : -PL_DEFAULTCIRCLEMOVESPEED;
		esCircle[i].speed += (taimspeed - esCircle[i].speed) / (16 - circletimer);
	}
	return false;
}

bool Player::UncircleChange()
{
	if(hge->Input_GetDIKey(KS_CIRCLE_MP, DIKEY_DOWN))
		uncircletimer = 0;
	bCircle = false;
	uncircletimer++;
	if(uncircletimer == 1)
	{
		bgmask.flag = BG_REDFLASH;
		bgmask.changetimer = 0;

		esCircleBox.sprite->SetTextureRect(128, 224, 128, 32);
		SE::push(SE_PLAYER_CIRCLEOFF, x);

		if(mp.rangemode)
		{
			Player::p.exist = false;
			BossInfo::flag = 0;
		}
		if(!bInfi)
		{
			DWORD tminus = PL_UNCIRCLECHANGECOST_ALIVENESS(nPower);
			if(nAliveness > tminus)
				nAliveness -= tminus;
			else
				nAliveness = 0;
		}
	}
	else if(uncircletimer == 16)
	{
		uncircletimer = 0;
		return true;
	}
	for(int i=0;i<PL_NESCIRCLE;i++)
	{
		float taimspeed = 0;
		esCircle[i].speed += (taimspeed - esCircle[i].speed) / (16 - uncircletimer);
	}
	return false;
}

bool Player::Graze()
{
	effGraze.Fire();
	SE::push(SE_PLAYER_GRAZE, x);
	return true;
}

void Player::Render()
{
	sprite->SetColor(alpha<<24|diffuse);
	sprite->RenderEx(x, y, 0, hscale, vscale);
}

void Player::RenderEffect()
{
	effGraze.Render();

	for(int i=0;i<PL_NPLAYERGHOST;i++)
		pg[i].Render();
	if(flag & PLAYER_SLOWCHANGE || flag & PLAYER_FASTCHANGE || flag & PLAYER_CIRCLECHANGE || flag & PLAYER_UNCIRCLECHANGE)
	{
		esChange.Render();
	}
	esGraze.Render();
	if(bInfi)
		esInfi.Render();
	if(flag & PLAYER_SHOT || flag & PLAYER_DEADBOMB)
		esShot.Render();
	if(mp.circlearound && (bCircle || (flag & PLAYER_UNCIRCLECHANGE)))
	{
		for(int i=0;i<PL_NESCIRCLE;i++)
			esCircle[i].Render();
	}
	if(bSlow)
	{
		esPoint.Render();
		esPoint.Render();

	}
	if(flag & PLAYER_COLLAPSE)
		esCollapse.Render();

	if(flag & PLAYER_BOMB || flag & PLAYER_DEADBOMB)
	{
		for(int i=0;i<PL_ESBOMBMAX;i++)
		{
			if(esBomb[i].exist)
				esBomb[i].Render();
		}
	}
	effBomb[0].Render();
	effBomb[1].Render();
	effCollapse.Render();
}