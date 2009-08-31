#include "Process.h"
#include "Scripter.h"
#include "Selector.h"
#include "Player.h"
#include "InfoSelect.h"
#include "SE.h"
#include "BGLayer.h"
#include "BossInfo.h"

int Process::processPause()
{
	BYTE tstate = state;
	if(!pauseinit)
	{
		hge->Channel_Pause(channel);

		scr.d[SCR_RESERVEBEGIN].bfloat = false;
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0x00;
		pauseinit = true;

		if(replaymode)
		{
			if (replayend)
			{
				time = 0;
			}
		}
		else if (Player::p.exist && Player::ncPause < 0xff)
		{
			Player::ncPause++;
		}
	}
	if(CINT(scr.d[SCR_RESERVEBEGIN].value) < 0x100)
		scr.controlExecute(STATE_PAUSE, 1);

	if(sel.size() && hge->Input_GetDIKey(KS_BOMB, DIKEY_UP))
	{
		Selector::Clear();
		if(CINT(scr.d[SCR_RESERVEBEGIN].value) == 0x10)
		{
			CINT(scr.d[SCR_RESERVEBEGIN].value) = 0xff;
			if(replaymode && replayend || rangemode && !replaymode && time == 0)
				state = STATE_TITLE;
			else
				state = STATE_START;
		}
		else
			CINT(scr.d[SCR_RESERVEBEGIN].value) = 0x01;
		SE::push(SE_SYSTEM_CANCEL);
//		state = STATE_START;
	}
	if(hge->Input_GetDIKey(KS_PAUSE, DIKEY_DOWN))
	{
		Selector::Clear();
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0xff;
		SE::push(SE_SYSTEM_CANCEL);
		if(replaymode && replayend)
			state = STATE_TITLE;
		else
			state = STATE_START;
	}

	if(CINT(scr.d[SCR_RESERVEBEGIN].value) == 0xff)
	{
		fgpause.flag = FG_PAUSEOUT;
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0x100 | state;
		state = STATE_PAUSE;
	}
	else if(CINT(scr.d[SCR_RESERVEBEGIN].value) >= 0x100 && !fgpause.flag)
	{
		fgpause.exist = false;
		pauseinit = false;
		state = CINT(scr.d[SCR_RESERVEBEGIN].value) & 0xff;
		if(state == STATE_START)
		{
			if(time == 0)
			{
				scene = startscene;
				startPrep();
				return PTURN;
			}
			hge->Input_SetDIKey(KS_LEFT, (bool)(saveInput & 0x1));
			hge->Input_SetDIKey(KS_RIGHT, (bool)(saveInput & 0x2));
			hge->Input_SetDIKey(KS_UP, (bool)(saveInput & 0x4));
			hge->Input_SetDIKey(KS_DOWN, (bool)(saveInput & 0x8));
			hge->Input_SetDIKey(KS_FIRE, (bool)(saveInput & 0x10));
			hge->Input_SetDIKey(KS_BOMB, (bool)(saveInput & 0x20));
			hge->Input_SetDIKey(KS_CIRCLE, (bool)(saveInput & 0x40));
			hge->Input_SetDIKey(KS_SLOW, (bool)(saveInput & 0x80));

			hge->Channel_Resume(channel);
			hge->Channel_SetVolume(channel, 0);
			hge->Channel_SlideTo(channel, 1.5f, bgmvol);

			return PTURN;
		}
		else if(state == STATE_TITLE)
		{
			time = 0;
			if(replaymode)
			{
				for(int i=0;i<BGLAYERMAX;i++)
					bg[i].exist = false;
				for(int i=0;i<FGLAYERMAX;i++)
					fg[i].exist = false;
				Selector::Clear();
				infoselect.clear();
				bgmask.exist = false;
				fgpanel.exist = false;
				fgdiff.exist = false;
				Player::p.exist = false;
				BossInfo::empty();
				getInput();
				replaymode = false;
				state = STATE_REPLAY;
				return PTURN;
			}
			if(practicemode || rangemode)
			{
				for(int i=0;i<BGLAYERMAX;i++)
					bg[i].exist = false;
				for(int i=0;i<FGLAYERMAX;i++)
					fg[i].exist = false;
				Selector::Clear();
				infoselect.clear();
				bgmask.exist = false;
				fgpanel.exist = false;
				fgdiff.exist = false;
				BossInfo::empty();
				if(rangemode && !replaymode && time == 0)
					state = STATE_CONTINUE;
				else
					state = STATE_PLAYER_SELECT;
			}
			scene = startscene;
			Player::p.exist = false;
//			startPrep();
			return PTURN;
		}
	}
	return PGO;
}
