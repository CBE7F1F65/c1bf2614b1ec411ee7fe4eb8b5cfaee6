#include "Process.h"
#include "Scripter.h"
#include "BGLayer.h"
#include "Selector.h"
#include "BGLayer.h"
#include "InfoSelect.h"
#include "Player.h"
#include "SE.h"

int Process::processStart()
{
	if (replaymode && (!Player::p.exist || scene == S1))
	{
		replayend = true;
		replaymode = false;
		scene = S1;
		time = 0;
		hge->Channel_Stop(channel);
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
		getInput();
		state = STATE_REPLAY;
		return PTURN;
	}
	if(!Player::p.exist && !replaymode)
	{
		if(rangemode && time != 0)
		{
			time = 0;
			clearPrep(false);
			Player::p.exist = false;
			hge->Input_SetDIKey(KS_PAUSE);
		}
		else
		{
			CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
			state = STATE_CONTINUE;
			return PTURN;
		}
	}
	else
		frameStart();

	if(hge->Input_GetDIKey(KS_PAUSE, DIKEY_DOWN) && !(Player::p.flag & PLAYER_SHOT) && state != STATE_CLEAR && hge->frameSkip <= 0)
	{
		saveInput = nowInput;
		SE::push(SE_SYSTEM_PAUSE);

		fgpause.exist = true;
		fgpause.changetimer = 0;
		fgpause.flag = FG_PAUSEIN;

		if(state == STATE_CLEAR)
			pauseret = true;

		state = STATE_PAUSE;
	}

	if(scene > S1 && state != STATE_CLEAR)
	{
		time = 0;
		state = STATE_CLEAR;
		return PTURN;
	}

	if(time != lasttime && state != STATE_CLEAR)
	{
		int rv = 0xffffffff;

		rv = scr.stageExecute(scene, time);
	}
	return PGO;
}
