#include "Process.h"
#include "Scripter.h"
#include "Selector.h"
#include "InfoSelect.h"
#include "SE.h"

int Process::processOption()
{
	time++;
	if(time == 1)
	{
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = -1;
		CINT(scr.d[SCR_RESERVEBEGIN+2].value) = 0;
	}
	scr.controlExecute(STATE_OPTION, time);
	//-> pushtimer sel depth
	int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
	int tsel = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
	int tdepth = CINT(scr.d[SCR_RESERVEBEGIN+2].value);

	tsel = Selector::select;
	if(!tdepth)
	{
		if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_CANCEL);
			Selector::select = Selector::nselect-1;
		}
		if((hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN) || hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN)) && tsel == Selector::nselect-1)
		{
			SE::push(SE_SYSTEM_CANCEL);
			Selector::Clear();
			time = 0;
			state = STATE_TITLE;
			return PTURN;
		}

		if(hge->Input_GetDIKey(KS_LEFT) || hge->Input_GetDIKey(KS_RIGHT))
		{
			tpushtimer++;
			if(tpushtimer == M_PUSH_FIRST)
				tpushtimer = M_PUSH_ROLLTO;
			if(tpushtimer == M_PUSH_ROLLTO)
			{
				hge->Input_SetDIKey(KS_LEFT, false);
				hge->Input_SetDIKey(KS_RIGHT, false);
			}
		}
		else
		{
			tpushtimer = 0;
		}

		if(hge->Input_HaveJoy() &&
			(hge->Input_GetDIKey(DIK_TAB, DIKEY_DOWN) ||
			!tsel && (hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN) || hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))))
		{
			SE::push(SE_SYSTEM_OK);
			tdepth = 1;

			for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
			{
				if(i->ID < 0x10)
				{
					i->flag |= SEL_NONACTIVE;
					i->ID += 0x10;
				}
				else if(i->ID < 0x20)
				{
					i->flag &= ~SEL_NONACTIVE;
					i->ID -= 0x10;
				}
			}
			Selector::updown = true;
			Selector::nselect = 5;
			Selector::select = 0;

			InfoSelect _ifs;
			_ifs.valueSet(0, "按Enter、左方向键或TAB键确定", 240, 400, INFO_RED, true);
			infoselect.push_back(_ifs);

			InfoSelect::nselect = 0;
			InfoSelect::updown = true;
			InfoSelect::select = 0;
		}
		else if(tsel == 1)
		{
			if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN) ||
				hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN) ||
				hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_OK);
				screenmode = 1 - screenmode;
				hge->System_SetState(HGE_WINDOWED, !(bool)screenmode);
				hge->System_SetState(HGE_HIDEMOUSE, (bool)screenmode);
			}
		}
		else if(tsel == 2)
		{
			if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_OK);
				maxplayer++;
				if(maxplayer > M_PL_PLAYERMAX-1)
					maxplayer = 0;
			}
			else if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				maxplayer--;
				if(maxplayer < 0)
					maxplayer = 0;
			}
			else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				maxplayer++;
				if(maxplayer > M_PL_PLAYERMAX-1)
					maxplayer = M_PL_PLAYERMAX-1;
			}
		}
		else if(tsel == 3 || tsel == 4)
		{
			int tvol;
			if(tsel == 3)
			{
				tvol = bgmvol;
			}
			else
			{
				tvol = sevol;
				if(!hge->Channel_IsPlaying(SE::se[SE_BOSS_TIMEOUT].chn))
					SE::push(SE_BOSS_TIMEOUT);
			}

			if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				tvol += 5;
				if(tvol > 100)
					tvol = 0;
			}
			else if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				tvol -= 5;
				if(tvol < 0)
					tvol = 0;
			}
			else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				tvol += 5;
				if(tvol > 100)
					tvol = 100;
			}

			if(tsel == 3)
			{
				bgmvol = tvol;
			}
			else
			{
				sevol = tvol;
			}
		}
		else if(tsel == 5 && hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
		{
			joyFire = RESCONFIGDEFAULT_JOYFIRE;
			joyBomb = RESCONFIGDEFAULT_JOYBOMB;
			joySlow = RESCONFIGDEFAULT_JOYSLOW;
			joyCircle = RESCONFIGDEFAULT_JOYCIRCLE;
			joyPause = RESCONFIGDEFAULT_JOYPAUSE;
			screenmode = RESCONFIGDEFAULT_SCREENMODE;
			maxplayer = RESCONFIGDEFAULT_MAXPLAYER;
			bgmvol = RESCONFIGDEFAULT_VOLMUSIC;
			sevol = RESCONFIGDEFAULT_VOLSE;
		}
	}
	else
	{
		for(int i=0;i<M_JOYKEYMAX;i++)
		{
			if(hge->Input_GetDIJoy(i, DIKEY_DOWN) &&
				!hge->Input_GetDIJoy(JOY_LEFT, DIKEY_DOWN) &&
				!hge->Input_GetDIJoy(JOY_RIGHT, DIKEY_DOWN) &&
				!hge->Input_GetDIJoy(JOY_UP, DIKEY_DOWN) &&
				!hge->Input_GetDIJoy(JOY_DOWN, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_OK);
				joyKey[tsel] = i;
				
				for(int j = 0;j < 5;j++)
				{
					if(j == tsel)
						continue;
					if(joyKey[j] == i)
					{
						for(int k = 0;k < M_JOYKEYMAX;k++)
						{
							if(joyKey[0]!=k && 
								joyKey[1]!=k && 
								joyKey[2]!=k && 
								joyKey[3]!=k && 
								joyKey[4]!=k)
							{
								joyKey[j] = k;
								break;
							}
						}
					}
				}
			}
		}

		if(hge->Input_GetDIKey(KS_ENTER, DIKEY_DOWN) ||
			hge->Input_GetDIKey(DIK_TAB, DIKEY_DOWN) ||
			hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_OK);
			tdepth = 0;
			for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
			{
				if(i->ID < 0x10)
				{
					i->flag |= SEL_NONACTIVE;
					i->ID += 0x10;
				}
				else if(i->ID < 0x20)
				{
					i->flag &= ~SEL_NONACTIVE;
					i->ID -= 0x10;
				}
			}
			Selector::updown = true;
			Selector::nselect = 7;
			Selector::select = 0;

			infoselect.clear();
		}
	}

	SE::vol = sevol;
	hge->Channel_SetVolume(channel, bgmvol);

	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if((i->ID & 0xf0) == 0x80)
		{
			int tjk = joyKey[(i->ID & 0x0f) >> 1];
			if(i->ID & 1)
				i->sprite->SetTextureRect((tjk % 10) * 32, 64, 32, 32);
			else
				i->sprite->SetTextureRect((tjk / 10) * 32, 64, 32, 32);
		}
		else if(i->ID == 0x90)
		{
			i->sprite->SetTextureRect(512, (1 - screenmode) * 32 + 192, 128, 32);
		}
		else if(i->ID == 0xA0)
		{
			i->sprite->SetTextureRect((maxplayer + 1) * 32, 64, 32, 32);
		}
		else if((i->ID & 0xf0) == 0xB0 || (i->ID & 0xf0) == 0xC0)
		{
			int tvol;
			if((i->ID & 0xf0) == 0xB0)
			{
				tvol = bgmvol;
			}
			else
			{
				tvol = sevol;
			}
			switch(i->ID & 0x0f)
			{
			case 0x00:
				if(tvol < 100)
					i->sprite->SetTextureRect(0, 0, 0, 0);
				else
					i->sprite->SetTextureRect(32, 64, 32, 32);
				break;
			case 0x01:
				if(tvol < 10)
					i->sprite->SetTextureRect(0, 0, 0, 0);
				else
					i->sprite->SetTextureRect(((tvol / 10) % 10) * 32, 64, 32, 32);
				break;
			case 0x02:
				i->sprite->SetTextureRect((tvol % 10) * 32, 64, 32, 32);
				break;
			case 0x03:
				i->sprite->SetTextureRect(320, 64, 32, 32);
				break;
			}
		}

		if(i->ID < 0x10)
		{
			if(i->flag & SEL_ENTER)
			{
				i->flag &= ~SEL_ENTER;
				i->flag |= SEL_OVER;
			}
			else if(i->flag & SEL_LEAVE)
			{
				i->flag &= ~SEL_LEAVE;
				i->flag |= SEL_NONE;
			}
		}
		else
		{
			i->flag = SEL_NONACTIVE;
		}
	}
	CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tsel;
	CINT(scr.d[SCR_RESERVEBEGIN+2].value) = tdepth;
	return PGO;
}