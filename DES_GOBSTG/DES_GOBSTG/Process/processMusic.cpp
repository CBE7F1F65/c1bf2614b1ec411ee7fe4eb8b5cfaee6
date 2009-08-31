#include "Process.h"
#include "InfoSelect.h"
#include "Scripter.h"
#include "SE.h"
#include "BResource.h"

int Process::processMusic()
{
	time++;
	if(time == 1)
	{
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = -1;
		CINT(scr.d[SCR_RESERVEBEGIN+3].value) = 0;

		int i = 1;
		while(true)
		{
			if(!strlen(res.musdata[i-1].musicfilename))
			{
				CINT(scr.d[SCR_RESERVEBEGIN+2].value) = i-1;
				break;
			}
			i++;
		}

		InfoSelect::select = 0;
	}
	scr.controlExecute(STATE_MUSIC, time);

	if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_CANCEL);
		time = 0;
		state = STATE_TITLE;
		return PTURN;
	}
	//pushtimer sel maxnum page
	int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
	int tmusicsel = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
	int tmaxmusic = CINT(scr.d[SCR_RESERVEBEGIN+2].value);
	int tpage = CINT(scr.d[SCR_RESERVEBEGIN+3].value);

	if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_OK);
		musicChange(tpage * 10 + tmusicsel + 1, true);
	}
	else if(hge->Input_GetDIKey(KS_PAUSE, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_CANCEL);
		hge->Channel_Stop(channel);
	}
	else if(hge->Input_GetDIKey(KS_CIRCLE, DIKEY_DOWN) || hge->Input_GetDIKey(KS_SLOW, DIKEY_DOWN))
	{
		if(hge->Channel_IsPlaying(channel))
		{
			SE::push(SE_SYSTEM_OK);
			hge->Channel_SetPos(channel, 0);
		}
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

	if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_SELECT);
		if(tpage)
			tpage--;
		else
			tpage = (tmaxmusic - 1) / 10;
		infoselect.clear();
	}
	else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_SELECT);
		if(tpage < (tmaxmusic - 1) / 10)
			tpage++;
		else
			tpage = 0;
		infoselect.clear();
	}

	int tthismax = tmaxmusic-1 - tpage * 10;
	if(tthismax >= 10)
		tthismax = 10;
	else
		tthismax++;
	if(!infoselect.size())
	{
		InfoSelect _ifs[11];
		for(int i=0;i<tthismax;i++)
		{
			strcpy(_ifs[i].info, "No.");
			_ifs[i].linki("|10305", tpage * 10 + i + 1);
			strcat(_ifs[i].info, "|008");	
			strcat(_ifs[i].info, res.musdata[tpage * 10 + i].musicname);

			_ifs[i].valueSet(i, _ifs[i].info, 120, i*22+110, INFO_GREEN);
			infoselect.push_back(_ifs[i]);
		}
		_ifs[10].valueSet(10, "  Ðò|012ÇúÃû", 120, 80, INFO_RED, true);
		infoselect.push_back(_ifs[10]);

		InfoSelect::updown = true;
		InfoSelect::nselect = tthismax;
		if(InfoSelect::select >= InfoSelect::nselect)
			InfoSelect::select = InfoSelect::nselect-1;

		tmusicsel = -1;
	}

	if(InfoSelect::select != tmusicsel)
	{
		tmusicsel = InfoSelect::select;

		if((int)(infoselect.size()) > tthismax+1)
		{
			infoselect.pop_back();
			infoselect.pop_back();
			infoselect.pop_back();
			infoselect.pop_back();
		}
		InfoSelect _ifs[4];
		strcpy(_ifs[0].info, res.musdata[tpage * 10 + tmusicsel].explain_1);
		strcpy(_ifs[1].info, res.musdata[tpage * 10 + tmusicsel].explain_2);
		strcpy(_ifs[2].info, res.musdata[tpage * 10 + tmusicsel].explain_3);
		strcpy(_ifs[3].info, res.musdata[tpage * 10 + tmusicsel].explain_4);

		for(int i=0;i<4;i++)
		{
			_ifs[i].valueSet(i+11, _ifs[i].info, 120+i*15, 340+i*25, INFO_RED, true);
			infoselect.push_back(_ifs[i]);
		}
	}

	CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tmusicsel;
	CINT(scr.d[SCR_RESERVEBEGIN+3].value) = tpage;

	return PGO;
}