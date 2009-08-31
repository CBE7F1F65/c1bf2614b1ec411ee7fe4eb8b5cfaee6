#include "Process.h"
#include "Bullet.h"
#include "Player.h"
#include "Scripter.h"
#include "BGLayer.h"
#include "Selector.h"
#include "Replay.h"

int Process::processContinue()
{
	if(Player::p.ncCont >= M_PL_CONTINUEMAX || practicemode)
		goto exit;

	if(!CINT(scr.d[SCR_RESERVEBEGIN].value))
	{
		fgpause.exist = true;
		fgpause.changetimer = 0;
		fgpause.flag = FG_PAUSEIN;

		hge->Channel_Pause(channel);
	}

	scr.controlExecute(STATE_CONTINUE, 1);
	//60 sec
	int tsec = CINT(scr.d[SCR_RESERVEBEGIN].value);

	if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		fgpause.changetimer = 0;
		fgpause.flag = FG_PAUSEOUT;
	}
	else if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
	{
		Selector::select = 1;
	}

	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if(i->ID == 0x10)
		{
			i->sprite->SetTextureRect((5-Player::p.ncCont)*32, 64, 32, 32);
		}
	}

	if(tsec == 0x10)
	{
		fgpause.exist = false;
		if(!rangemode && scene < S900)
		{
			Player::p.valueSet(luchara<<1, maxplayer, PL_DEFAULTNBOMB, true);
			Player::ncCont++;

			IzeZone _ize;
			_ize.x = 0;
			_ize.y = 0;
			_ize.r = BULLET_IZEOVERZONE;
			Bullet::izel[BULLETIZE_FADEOUT].push_back(_ize);

			hge->Channel_Resume(channel);
			hge->Channel_SetVolume(channel, 0);
			hge->Channel_SlideTo(channel, 1.5f, bgmvol);
			state = STATE_START;
			return PTURN;
		}
		else
		{
			scene = startscene;
			startPrep();
			if(rangemode)
			{
				hge->Channel_Resume(channel);
				hge->Channel_SetVolume(channel, 0);
				hge->Channel_SlideTo(channel, 1.5f, bgmvol);
			}
			state = STATE_START;
			return PTURN;
		}
	}
	else if(tsec == 0x11)
	{
exit:
		rpy.partFill(0xff);
		fgpause.exist = false;
		hge->Stream_Free(stream);
		time = 0;
		state = STATE_OVER;
		return PTURN;
	}

	CINT(scr.d[SCR_RESERVEBEGIN].value) = tsec;

	return PGO;
}