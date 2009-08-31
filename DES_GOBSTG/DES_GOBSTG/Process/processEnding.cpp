#include "Process.h"
#include "BGLayer.h"
#include "Player.h"
#include "Scripter.h"
#include "Data.h"
#include "Selector.h"
#include "InfoSelect.h"

int Process::processEnding()
{
	time++;
	if (time == 1)
	{
		for(int i=0; i<BGLAYERMAX; i++)
		{
			bg[i].exist = false;
		}
		for(int i=0; i<FGLAYERMAX; i++)
		{
			fg[i].exist = false;
		}

		Player::p.exist = false;
		bgmask.exist = false;
		fgpanel.exist = false;
		fgdiff.exist = false;
		hge->Channel_Stop(channel);

		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+2].value) = Player::p.ncCont;
		CINT(scr.d[SCR_RESERVEBEGIN+3].value) = 0;
	}
	retvalue = PGO;
	retvalue = scr.controlExecute(STATE_ENDING, time);
	//pushtimer depth nccont
	int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
	int tdepth = CINT(scr.d[SCR_RESERVEBEGIN+1].value);

	if (tdepth == 0)
	{
	}
	else if (tdepth == 0xff)
	{
		sel.clear();
		infoselect.clear();
		time = 0;
		state = STATE_OVER;
		return PTURN;
	}
	if(hge->Input_GetDIKey(KS_FIRE) || hge->Input_GetDIKey(KS_BOMB))
	{
		tpushtimer++;
		if(tpushtimer == 120)
		{
			tdepth = 0xff;
		}
	}
	else
	{
		tpushtimer = 0;
	}
	CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tdepth;

	return retvalue;
}