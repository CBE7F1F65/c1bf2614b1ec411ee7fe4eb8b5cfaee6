#include "processPrep.h"

void Process::frameEnd()
{
	if(active)
	{
		framecounter++;
		float lost = (hge->Timer_GetDelta() - 1/60.0f) * 100 * 60.0f;
		if(lost < 0)
			lost = 0;
		if(lost > 100)
			lost = 100;
		Player::p.lostStack += lost;

		if(Player::p.exist && state != STATE_CLEAR)
		{
			alltime++;
			Player::p.action();
		}
		PlayerBullet::locked = PBLOCK_LOST;
		for(int i=0;i<ENEMYMAX;i++)
		{
			if(en[i].exist)
			{
				en[i].action();
				if(PlayerBullet::locked == PBLOCK_LOST && en[i].able)
				{
					if (en[i].x <= M_CLIENT_RIGHT && en[i].x >= M_CLIENT_LEFT
						&& en[i].y <= M_CLIENT_BOTTOM && en[i].y >= M_CLIENT_TOP)
					{
						PlayerBullet::locked = i;
					}
				}
			}
		}
		Ghost::totalghost = 0;
		Ghost::totalhalfghost = 0;
		for(int i=0;i<GHOSTMAX;i++)
		{
			if(gh[i].exist)
			{
				gh[i].action();
				if(Player::p.bCircle && PlayerBullet::locked == PBLOCK_LOST && gh[i].able && !gh[i].half)
				{
					if (gh[i].x <= M_CLIENT_RIGHT && gh[i].x >= M_CLIENT_LEFT
						&& gh[i].y <= M_CLIENT_BOTTOM && gh[i].y >= M_CLIENT_TOP)
					{
						PlayerBullet::locked = i + PBLOCK_GHOST;
					}
				}
			}
		}
		if (bu.size)
		{
			ZeroMemory(Bullet::renderDepth, sizeof(RenderDepth) * BULLETTYPEMAX);
			DWORD i = 0;
			DWORD size = bu.size;
			for (bu.toBegin(); i<size; bu.toNext(), i++)
			{
				if (!bu.isValid())
				{
					continue;
				}
				if ((*bu).exist)
				{
					(*bu).action();
				}
				else
				{
					bu.pop();
				}
			}
		}
		if (be.size)
		{
			DWORD i = 0;
			DWORD size = be.size;
			for (be.toBegin(); i<size; be.toNext(), i++)
			{
				if (!be.isValid())
				{
					continue;
				}
				if ((*be).exist)
				{
					(*be).action();
				}
				else
				{
					be.pop();
				}
			}
		}
		for(int i=0;i<BULLETIZELISTMAX;i++)
			Bullet::izel[i].clear();
		if (pb.size)
		{
			DWORD i = 0;
			DWORD size = pb.size;
			for (pb.toBegin(); i<size; pb.toNext(), i++)
			{
				if (!pb.isValid())
				{
					continue;
				}
				if ((*pb).exist)
				{
					(*pb).action();
				}
				else
				{
					pb.pop();
				}
			}
		}
		/*
		for(list<PlayerBullet>::iterator i=pb.begin();i!=pb.end();i++)
		{
			if(i->exist)
				i->action();
			else
			{
				i = pb.erase(i);
				--i;
			}
		}
		*/
		if (mi.size)
		{
			DWORD i = 0;
			DWORD size = mi.size;
			for (mi.toBegin(); i<size; mi.toNext(), i++)
			{
				if (!mi.isValid())
				{
					continue;
				}
				if ((*mi).exist)
				{
					(*mi).action();
				}
				else
				{
					mi.pop();
				}
			}
		}
		/*
		for(VectorList<Item>::iterator i=mi.begin();i!=mi.end();i++)
		{
			if(i->exist)
				i->action();
			else
			{
				i = mi.erase(i);
				--i;
			}
		}
		*/

		Scripter::stopEdefScript = false;
		if(BossInfo::flag)
		{
			if(bossinfo.action())
			{
				time = 0;
				if(!rangemode)
				{
					scene = BossInfo::turntoscene;
					time = 0;
				}
				else
					Player::p.exist = false;
			}
			if(BossInfo::flag >= BOSSINFO_COLLAPSE)
				Scripter::stopEdefScript = true;
		}
	}
	if(active || !Player::p.exist && state != STATE_CONTINUE)
	{
		if(active)
		{
			for(int i=0; i<BGLAYERSETMAX; i++)
			{
				if(BGLayer::set[i].sID != 0)
				{
					BGLayer::set[i].timer++;
					BGLayer::setindex = i;
					scr.sceneExecute(BGLayer::set[i].sID, BGLayer::set[i].timer);
				}
			}
		}

		for(int i=0; i<EFFECTSYSMAX; i++)
		{
			if(effsys[i].exist)
				effsys[i].action();
		}

		for(int i=0; i<BGLAYERMAX; i++)
		{
			if(bg[i].exist)
				bg[i].action();
		}
		for(int i=0;i<FGLAYERMAX;i++)
		{
			if(fg[i].exist)
				fg[i].action();
		}
	}
	if(fgpause.exist)
		fgpause.action();
	if(bgmask.exist)
		bgmask.action();
	/*
	if(fgpanel.exist)
		fgpanel.action();
		*/
	for(list<Selector>::iterator i = sel.begin();i != sel.end(); i++)
	{
		i->action();
	}
	Fontsys::changeID = -1;
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
	{
		if(!InfoSelect::complete)
		{
			i->action();
			if (Fontsys::changeID < i->ID)
			{
				Fontsys::changeID = i->ID;
			}
		}
		else
		{
			infoselect.clear();
			break;
		}
	}
	SE::play();
	active = false;
}