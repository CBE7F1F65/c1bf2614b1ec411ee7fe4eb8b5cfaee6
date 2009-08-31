#include "BossInfo.h"
#include "Enemy.h"
#include "Player.h"
#include "SE.h"
#include "EffectSp.h"
#include "Data.h"
#include "Process.h"
#include "BGLayer.h"
#include "Fontsys.h"
#include "Item.h"
#include "Ghost.h"
#include "BResource.h"
#include "DataConnector.h"
#include "Scripter.h"

BossInfo bossinfo;

Effectsys BossInfo::effCollapse;
Effectsys BossInfo::effItem;
Effectsys BossInfo::effUp;
Effectsys BossInfo::effStore;

hgeSprite * BossInfo::cutin;
hgeSprite * BossInfo::timecircle;
hgeSprite * BossInfo::sprange;
hgeSprite * BossInfo::spbossx;
hgeFont * BossInfo::font;

int BossInfo::sno;
int BossInfo::turntoscene;
bool BossInfo::range;
bool BossInfo::failed;
bool BossInfo::uncircled;
BYTE BossInfo::flag;
BYTE BossInfo::autorank;

bool BossInfo::Init(const char * fontname)
{
	Release();

	font = new hgeFont(fontname);

	sprange = new hgeSprite(mp.tex[TEX_WORD], 0, 704, 256, 80);
	spbossx = new hgeSprite(EffectSp::tex, 128, 96, 64, 32);

	effCollapse.valueSet(EFF_BI_COLLAPSE, 0, 0);
	effCollapse.Stop();
	effItem.valueSet(EFF_BI_ITEM, 0, 0);
	effItem.Stop();
	effUp.valueSet(EFF_BI_BOSSUP, 0, 0);
	effCollapse.Stop();
	effStore.valueSet(EFF_BI_BOSSSTORE, 0, 0);
	effStore.Stop();

	cutin = new hgeSprite(Enemy::tex[0], 256, 0, 256, 512);
	cutin->SetColor(0x40ffffff);

	timecircle = new hgeSprite(EffectSp::tex, 192, 128, 64, 64);
	timecircle->SetColor(0xa0ffffff);
	timecircle->SetBlendMode(BLEND_DEFAULT);

	return true;
}

void BossInfo::empty()
{
	range = false;
	turntoscene = S0;
	autorank = BIAUTORANK_NONE;
	flag = 0;
}

//bossinfo
bool BossInfo::Fill(int _sno)
{
	for(vector<rangeData>::iterator i=res.rdata.begin(); i!=res.rdata.end(); i++)
	{
		if(i->sno == _sno)
		{
			sno = _sno;
			range = i->isrange;
			if(bossinfo.range)
			{
				strcpy(rangename, i->rangename);
				wait = i->iswait;
				autorank = i->autorank;
			}
			strcpy(enemyname, i->enemyname);
			strcpy(enemypinyin, i->enemypinyin);
			limit = i->timelimit;
			remain = i->remain;
			turntoscene = i->turntoscene;
			bonus = i->bonus;
			maxbonus = bonus;
			return true;
		}
	}
	return false;
}

void BossInfo::bossUp()
{
	Fill(mp.scene);

	failed	= false;
	uncircled = false;
	lastgraze = Player::p.nGraze;

	flag = BOSSINFO_UP | BOSSINFO_ENABLE;
	timer = 0;
	itemstack = 0;

	sprange->SetTexture(mp.tex[TEX_WORD]);
	sprange->SetTextureRect(0, 704, 256, 80);
	spbossx->SetTexture(EffectSp::tex);
	spbossx->SetTextureRect(128, 96, 64, 32);

	effStore.Stop(true);
	
	if(range)
	{
		bgmask.flag = BG_WHITEFLASH;
		Enemy::bossflag[ENEMY_MAINBOSSINDEX] = BOSS_RANGEUP;
		Fontsys::SignUp(FONTSYS_RANGENAMEUSE, rangename);
		get = DataConnector::nGet();
		meet = DataConnector::nMeet() - 1;
		if(meet < 0)
			meet = 0;
	}
}

void BossInfo::Release()
{
	if(cutin)
	{
		delete cutin;
		delete timecircle;
	}
	cutin = NULL;

	if(sprange)
	{
		delete sprange;
	}
	sprange = NULL;
	if(spbossx)
	{
		delete spbossx;
	}
	spbossx = NULL;

	delete font;
	font = NULL;

	flag = 0;
	range = false;
	failed = false;
}

void BossInfo::quit()
{
	if (flag & BOSS_COLLAPSE)
	{
		Player::p.bInfi = false;
		Player::p.esInfi.hscale = 0;
	}
	else if (flag & BOSSINFO_TIMEOVER)
	{
		failed = true;
	}
	if(range && mp.rangemode)
	{
		bgmask.flag = BG_WHITEOUT;
	}
	if(remain)
	{
		en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
	}

	scr.stageExecute(sno, 0);

	range = false;
	wait = false;
	autorank = BIAUTORANK_NONE;

	flag = 0;
	exist = false;

	hge->frameSkip = 0;
}

bool BossInfo::action()
{
	exist = true;

	if(flag < BOSSINFO_COLLAPSE)
	{
		if(!en[ENEMY_MAINBOSSINDEX].able)
		{
			flag |= BOSSINFO_COLLAPSE;
			if(range && !failed)
			{
				if(wait)
					timer = limit * 30;
				itemstack = (limit * 60 - timer) / 2;
				if(uncircled)
					itemstack /= 2;
			}
			timer = 0;
		}
		else if(timer == limit * 60)
		{
			flag |= BOSSINFO_TIMEOVER;
			timer = 0;
		}
		if(range && Player::p.bCircle == false)
		{
			uncircled = true;
		}
	}
	else
	{
		effStore.Stop();
		effCollapse.action();
		effItem.action();
	}
	effStore.action();
	timer++;

	if(flag & BOSSINFO_UP)
	{
		if(timer == 1)
		{
			DWORD col1, col2;
			if(range)
			{
				col1 = 0xce7f0000;
				col2 = 0xceff0000;

				cutin->SetTexture(Enemy::tex[en[ENEMY_MAINBOSSINDEX].type-ENEMY_BOSSTYPEBEGIN]);
				cutin->SetTextureRect(256, 0, 256, 512);

				timecircle->SetTexture(EffectSp::tex);
				timecircle->SetTextureRect(192, 128, 64, 64);

				effUp.Stop(true);
				effUp.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, 0, true);
				effUp.Fire();
			}
			else
			{
				col1 = 0xcececebd;
				col2 = 0xceffffbd;
			}
			blood.valueSet(0, 220, 32,
				60, 30, col1,
				60, 30, col1,
				60, 34, col2,
				60, 34, col2);
		}
		if((blood.quad.v[1].x-60) / 320.0f < en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife)
		{
			blood.quad.v[1].x += 2;
			blood.quad.v[2].x += 2;
		}
		else
		{
			blood.quad.v[1].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
			blood.quad.v[2].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
		}
		//
		if(range)
		{
			float _tx = en[ENEMY_MAINBOSSINDEX].x;
			float _ty = en[ENEMY_MAINBOSSINDEX].y;
			_tx += (160 - timer) * cost(timer * 400);
			_ty += (160 - timer) * sint(timer * 400);
			effUp.MoveTo(_tx, _ty);
			effUp.action();
			if(timer == 90)
			{
				SE::push(SE_BOSS_UP, en[ENEMY_MAINBOSSINDEX].x);
				effUp.Stop();
			}
			else if(timer == 120)
			{
				timecircle->SetTexture(EffectSp::tex);
				timecircle->SetTextureRect(0, 128, 128, 128);
			}
		}
		//
		if(timer == 160)
			flag &= ~BOSSINFO_UP;
	}
	else if(flag & BOSSINFO_COLLAPSE)
	{
		Player::p.bInfi = true;
		if(timer == 1)
		{
			scr.stageExecute(sno, 0xffff);
			SE::push(SE_BOSS_DEAD, en[ENEMY_MAINBOSSINDEX].x);

			if(remain)
			{
				en[ENEMY_MAINBOSSINDEX].alpha = 0x7f;
				en[ENEMY_MAINBOSSINDEX].defrate = 1.0f;
				en[ENEMY_MAINBOSSINDEX].ac = 0;
				en[ENEMY_MAINBOSSINDEX].speed = 0;
			}

			effCollapse.Stop(true);
			effCollapse.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, 0, true);
			effCollapse.Fire();
			if(uncircled && !failed)
				bonus /= 2;
			Player::p.nScore += bonus;
			if(range)
			{
				bgmask.flag = BG_REDOUT;
			}
			else
				timer = 100;

			if(!failed)
			{
				DataConnector::Get();
			}

			if(!failed && range)
			{
				Player::p.getrange[Player::p.ncGet] = mp.scene;
				Player::p.ncGet++;
			}

			if(turntoscene - SCLEAR > S1 && !mp.rangemode)
				hge->frameSkip = 2;
		}
		else if(timer < 30)
		{
			float tstep = 0.2f;
			float tascale = timer * tstep;
			if(Player::p.esInfi.hscale < tascale)
			{
				Player::p.esInfi.hscale += tstep;
				if(Player::p.esInfi.hscale > tascale)
					Player::p.esInfi.hscale = tascale;
			}
			else if(Player::p.esInfi.hscale > tascale)
			{
				Player::p.esInfi.hscale -= tstep;
				if(Player::p.esInfi.hscale < tascale)
					Player::p.esInfi.hscale = tascale;
			}

			if (range && timer < 15)
			{
				worldx = sint(timer * 4800) * 10;
				worldy = cost(timer * 4800) * 10;
			}
			else if (timer == 15)
			{
				worldx = 0;
				worldy = 0;
			}
		}
		else if (timer == 75)
		{
			if (range)
			{
				worldz = 0;
				if (!failed)
				{
					SE::push(SE_BOSS_BONUS_1);
				}
				else
				{
					SE::push(SE_BOSS_BONUS_2);
				}
			}
		}
		else if(timer == 120)
		{
			effCollapse.Stop();
		}
		else if(timer > 120 && timer < 160)
		{
			float tstep = 0.15f;
			float tascale = (160-timer) * tstep;
			if(Player::p.esInfi.hscale < tascale)
			{
				Player::p.esInfi.hscale += tstep;
				if(Player::p.esInfi.hscale > tascale)
					Player::p.esInfi.hscale = tascale;
			}
			else if(Player::p.esInfi.hscale > tascale)
			{
				Player::p.esInfi.hscale -= tstep;
				if(Player::p.esInfi.hscale < tascale)
					Player::p.esInfi.hscale = tascale;
			}
		}
		else if(timer == 160)
		{
			quit();
			return true;
		}
		if(en[ENEMY_MAINBOSSINDEX].timer == 16 && en[ENEMY_MAINBOSSINDEX].life < 0 && !mp.rangemode && remain)
		{
			en[ENEMY_MAINBOSSINDEX].exist = true;
			en[ENEMY_MAINBOSSINDEX].fadeout = false;
			en[ENEMY_MAINBOSSINDEX].life = 0;
			en[ENEMY_MAINBOSSINDEX].timer = 0;
			en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
		}
	}
	else if(flag & BOSSINFO_TIMEOVER)
	{
		if(timer == 1)
		{
			scr.stageExecute(sno, 0xffff);
			SE::push(SE_BOSS_TIMEUP);
			if(remain)
			{
				en[ENEMY_MAINBOSSINDEX].defrate = 1;
				en[ENEMY_MAINBOSSINDEX].alpha = 0x7f;
			}
			if(!wait)
				failed = true;
			if(!failed && range && wait)
			{
				Player::p.getrange[Player::p.ncGet] = mp.scene;
				Player::p.ncGet++;
			}
			if(range)
			{
				bgmask.flag = BG_WHITEFLASH;
			}
			else
				timer = 100;
			if(wait || !remain)
			{
				en[ENEMY_MAINBOSSINDEX].life = -1;
			}

			if(turntoscene - SCLEAR > S1 && !mp.rangemode)
				hge->frameSkip = 2;
		}
		if(timer == 160)
		{
			quit();
			return true;
		}
		if(en[ENEMY_MAINBOSSINDEX].timer == 16 && en[ENEMY_MAINBOSSINDEX].life < 0 && !mp.rangemode && remain)
		{
			en[ENEMY_MAINBOSSINDEX].exist = true;
			en[ENEMY_MAINBOSSINDEX].fadeout = false;
			en[ENEMY_MAINBOSSINDEX].life = 0;
			en[ENEMY_MAINBOSSINDEX].timer = 0;
			en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
		}
	}
	else
	{
		if(en[ENEMY_MAINBOSSINDEX].life > 0)
		{
			blood.quad.v[1].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
			blood.quad.v[2].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
		}
		else
		{
			blood.quad.v[1].x = 60;
			blood.quad.v[2].x = 60;
		}
	}
	if(Player::p.y < 80)
	{
		blood.quad.v[0].col &= (0x7fffffff);
		blood.quad.v[1].col &= (0x7fffffff);
		blood.quad.v[2].col &= (0x7fffffff);
		blood.quad.v[3].col &= (0x7fffffff);

		sprange->SetColor(0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff);
		font->SetColor(0x5dffffff, 0x5dffffff, 0x5d7f7f00, 0x5d7f7f00);
	}
	else
	{
		blood.quad.v[0].col |= (0x80<<24);
		blood.quad.v[1].col |= (0x80<<24);
		blood.quad.v[2].col |= (0x80<<24);
		blood.quad.v[3].col |= (0x80<<24);

		sprange->SetColor(0xbfffffff, 0xbfffffff, 0xbfffffff, 0xbfffffff);
		font->SetColor(0xbdffffff, 0xbdffffff, 0xbd7f7f00, 0xbd7f7f00);
	}
	if(range && flag < BOSSINFO_COLLAPSE)
	{
		if(!failed && !wait)
		{
			int minus;
			minus = PL_GRAZEBONUS_BOSSBONUS(maxbonus, limit);
			minus -= PL_GRAZEBONUS_BOSSREGAIN(Player::p.nGraze - lastgraze);
			bonus -= minus;
			if(bonus > maxbonus)
				bonus = maxbonus;
			lastgraze = Player::p.nGraze;
		}
		else if(failed)
			bonus = PL_BOSSBONUS_FAILED(maxbonus);
		else if(timer == limit * 60 - 1)
			en[ENEMY_MAINBOSSINDEX].life = -1;
	}
	if(flag < BOSSINFO_COLLAPSE)
	{
		if(timer >= (limit-10) * 60 && timer % 60 == 0)
			SE::push(SE_BOSS_TIMEOUT);
		if(timer >= (limit-3) * 60 && timer % 60 == 10)
			SE::push(SE_BOSS_TIMEOUT);
	}
	else if(range && !failed)
	{
		if(timer == 1)
		{
			effItem.Stop(true);
			effItem.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, 0, true);
			effItem.Fire();
		}
		else if(timer < 90)
		{
			int tthrow = itemstack / (90-timer);
			float tr;
			float tangle;
			if(timer <= 30)
			{
				tr = timer + 40;
				tangle = timer * 1800;
			}
			else if(timer <= 60)
			{
				tr = timer + 20;
				tangle = -timer * 1800 + 12000;
			}
			else
			{
				tr = timer;
				tangle = timer * 1800 - 12000;
			}
			float tx = en[ENEMY_MAINBOSSINDEX].x + tr * cost(tangle);
			float ty = en[ENEMY_MAINBOSSINDEX].y + tr * sint(tangle);

			effItem.MoveTo(tx, ty);

			for(int i=0;i<tthrow;i++)
			{
				Item::Build(ITEM_SMALLCIRCLE, tx+rand()%10-5, ty+rand()%10-5, true);
			}
			itemstack -= tthrow;
		}
		else if(timer == 90)
		{
			effItem.Stop();
		}
	}
	return false;
}

void BossInfo::RenderTimeCircle()
{
	if (flag < BOSSINFO_COLLAPSE)
	{
		float scale;
		if (timer >= 120)
		{
			scale = (timer-60*limit)*1.0f / (30-15*limit);
			timecircle->RenderEx(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, timer/15.0f, scale);
		}
	}
}

void BossInfo::Render()
{
	exist = false;
	effStore.Render();
	if(flag < BOSSINFO_COLLAPSE)
	{
		if(range)
		{
			float yt;
			if(Enemy::bossflag[ENEMY_MAINBOSSINDEX] & BOSS_RANGEUP)
				cutin->Render(312, Enemy::rangeuptimer[ENEMY_MAINBOSSINDEX]*2.4f);
			if(timer < 30)
			{
				yt = 220;
			}
			else if(timer < 120)
			{
				yt = (120-timer)*2+40;
			}
			else
			{
				yt = 40;
			}
			sprange->Render(296, yt);

			float rangenamew = strlen(rangename)*11+1;
			DWORD rangenamealpha = 0xbd000000;
			if(Player::p.y < 80)
				rangenamealpha = 0x5d000000;
			Fontsys::Render(FONTSYS_RANGENAMEUSE, 400-rangenamew, yt-5, rangenamealpha+0xffffff, rangenamealpha+0xff0000);

			if (flag & BOSSINFO_UP)
			{
				effUp.Render();
			}

			font->SetScale(1.0f);
			font->printf(300, yt+20, HGETEXT_RIGHT, "%d", bonus);
			font->printf(370, yt+20, HGETEXT_RIGHT, "%d", get);
			font->printf(400, yt+20, HGETEXT_RIGHT, "%d", meet);
		}
		font->SetScale(1.2f);
		font->SetColor(0xbdffffff, 0xbdffffff, 0xbd7f7f7f, 0xbd7f7f7f);
		font->printf(50, 20, HGETEXT_RIGHT|HGETEXT_MIDDLE, "%d", remain);

		int ttime = limit-timer/60;
		if(ttime < 4)
			font->SetColor(0xbdff0000, 0xbdff0000, 0xbd7f0000, 0xbd7f0000);
		else if(ttime < 11)
			font->SetColor(0xbdff00ff, 0xbdff00ff, 0xbd7f007f, 0xbd7f007f);
		font->printf(400, 20, HGETEXT_CENTER|HGETEXT_MIDDLE, "%d", ttime);
		
		font->SetColor(0xbdffffff, 0xbdffffff, 0xbd7f7f00, 0xbd7f7f00);
		font->SetScale(0.7f);
		font->printf(45, 40, HGETEXT_LEFT, "%s", enemypinyin);
		hge->Gfx_RenderQuad(&blood.quad);
	}
	else if(flag & BOSSINFO_COLLAPSE)
	{
		if(range && !failed)
		{
			effCollapse.Render();
			effItem.Render();
			font->SetScale(2.0f);
			if(uncircled)
			{
				font->SetColor(0xffff8000, 0xffff8000, 0xff0000, 0xff0000);
				font->printf(220, 80, HGETEXT_CENTER|HGETEXT_MIDDLE, "Range Bonus");
			}
			else
			{
				font->SetColor(0xffff0000, 0xffff0000, 0xff7f0000, 0xff7f0000);
				font->printf(220, 80, HGETEXT_CENTER|HGETEXT_MIDDLE, "Perfect Range Bonus");
			}
			font->SetColor(0xffff00ff, 0xffff00ff, 0xff7f007f, 0xff7f007f);
			font->printf(220, 120, HGETEXT_CENTER|HGETEXT_MIDDLE, "%d", bonus);
		}
		else if(!failed)
		{
			font->SetScale(1.5f);
			font->SetColor(0xffffff00, 0xffffff00, 0xff7f7f00, 0xff7f7f00);
			font->printf(220, 40, HGETEXT_CENTER|HGETEXT_MIDDLE, "Bonus %d", (int)(bonus/* * (1 + Player::p.fHoly)*/));
		}
		else
		{
			font->SetScale(1.2f);
			font->SetColor(0xffffff00, 0xffffff00, 0xff7f7f00, 0xff7f7f00);
			font->printf(220, 40, HGETEXT_CENTER|HGETEXT_MIDDLE, "Range Failed Bonus %d", (int)(bonus/* * (1 + Player::p.fHoly)*/));
		}
	}
	else if(flag & BOSSINFO_TIMEOVER)
	{
		if(range)
		{
			font->SetScale(1.5f);
			font->SetColor(0xffff00ff, 0xffff00ff, 0xff7f007f, 0xff7f007f);
			font->printf(220, 40, HGETEXT_CENTER|HGETEXT_MIDDLE, "Range Failed");
		}
	}
}