#include "Process.h"
#include "InfoSelect.h"
#include "Player.h"
#include "DataConnector.h"
#include "BGLayer.h"
#include "Replay.h"

int Process::processClear()
{
	processStart();
	if(time == 1)
	{
		hge->frameSkip = 0;

		fgpause.exist = true;
		fgpause.changetimer = 0;
		fgpause.flag = FG_PAUSEIN;

		InfoSelect _ifs[10];
		LONGLONG tscore;
		LONGLONG ttotalscore = 0;

		BYTE tstage;
		if(scene - SCLEAR > S1)
			tstage = 0xff;
		else
			tstage = (scene - SCLEAR) / M_STAGENSCENE;

		hge->Channel_SlideTo(channel, 1.5f, 0);
		if(tstage == 0xff)
		{
			DataConnector::Clear();
			strcpy(_ifs[0].info, "All Clear !");
		}
		else
			strcpy(_ifs[0].info, "Stage Clear !");
		_ifs[0].valueSet(0, _ifs[0].info, 120, 80, INFO_RED, true);
		infoselect.push_back(_ifs[0]);

		strcpy(_ifs[1].info, "Clear|012=");
		switch(tstage)
		{
		case 2:
			tscore = PL_STAGEBONUS_STAGE_1;
			break;
		case 3:
		case 4:
			tscore = PL_STAGEBONUS_STAGE_2;
			break;
		case 5:
			tscore = PL_STAGEBONUS_STAGE_3;
			break;
		case 6:
		case 7:
			tscore = PL_STAGEBONUS_STAGE_4;
			break;
		case 8:
			tscore = PL_STAGEBONUS_STAGE_5;
			break;
		case 0xff:
			tscore = PL_STAGEBONUS_STAGE_6;
			break;
		}
		if(nowdifflv >= M_DIFFI_EXTRA)
			tscore = PL_STAGEBONUS_STAGE_OTHER;

		_ifs[1].linkl("|11323", tscore);
		ttotalscore += tscore;

		strcpy(_ifs[2].info, "Aliveness|012=");
		tscore = Player::p.getClearBonusAliveness();
		_ifs[2].linkl("|11323", tscore);
		ttotalscore += tscore;

		strcpy(_ifs[3].info, "Point|012=");
		tscore = Player::p.getClearBonusPoint();
		_ifs[3].linkl("|11323", tscore);
		ttotalscore += tscore;

		strcpy(_ifs[4].info, "Graze|012=");
		tscore = Player::p.getClearBonusGraze();
		_ifs[4].linkl("|11323", tscore);
		ttotalscore += tscore;

		for(int i=1;i<5;i++)
		{
			_ifs[i].valueSet(i, _ifs[i].info, 100, i*20+120, INFO_GRAY, true);
			infoselect.push_back(_ifs[i]);
		}

		if(tstage == 0xff)
		{
			strcpy(_ifs[5].info, "Player|012=");
			tscore = PL_ALLCLEARBONUS_LIFEBONUS(Player::p.nLife);
			_ifs[5].linkl("|11323", tscore);

			strcpy(_ifs[6].info, "Bomb|012=");
			tscore = PL_ALLCLEARBONUS_BOMBBONUS(Player::p.nBomb);
			_ifs[6].linkl("|11323", tscore);

			for(int i=5;i<7;i++)
			{
				_ifs[i].valueSet(i, _ifs[i].info, 100, i*20+120, INFO_YELLOW, true);
				infoselect.push_back(_ifs[i]);
			}
		}

		switch(nowdifflv)
		{
		case M_DIFFI_EASY:
			strcpy(_ifs[7].info, M_CLEARGREETINGSTR_EASY);
			strcat(_ifs[7].info, "|013* 0.5");
			ttotalscore *= 0.5f;
			break;
		case M_DIFFI_NORMAL:
			strcpy(_ifs[7].info, M_CLEARGREETINGSTR_NORMAL);
			strcat(_ifs[7].info, "|013* 1.0");
			break;
		case M_DIFFI_HARD:
			strcpy(_ifs[7].info, M_CLEARGREETINGSTR_HARD);
			strcat(_ifs[7].info, "|013* 1.5");
			ttotalscore *= 1.5f;
			break;
		case M_DIFFI_EXTRA:
			strcpy(_ifs[7].info, M_CLEARGREETINGSTR_EXTRA);
			strcat(_ifs[7].info, "|013* 2.0");
			ttotalscore *= 2.0f;
			break;
		case M_DIFFI_PHANTASM:
			strcpy(_ifs[7].info, M_CLEARGREETINGSTR_PHANTASM);
			strcat(_ifs[7].info, "|013* 3.0");
			ttotalscore *= 3.0f;
			break;
		}
		_ifs[7].valueSet(7, _ifs[7].info, 100, 280, INFO_RED, true);
		infoselect.push_back(_ifs[7]);

		if(maxplayer > PL_DEFAULTNPLAYER && nowdifflv < M_DIFFI_EXTRA)
		{
			strcpy(_ifs[8].info, M_CLEARPLAYERPENALTYSTR);
			strcat(_ifs[8].info, "|013* 0.");
			_ifs[8].linki(NULL, (M_PL_PLAYERMAX-maxplayer)*2);
			ttotalscore *= (M_PL_PLAYERMAX-maxplayer)*2 / 10.0f;
			_ifs[8].valueSet(8, _ifs[8].info, 100, 300, INFO_RED, true);
			infoselect.push_back(_ifs[8]);
		}

		strcpy(_ifs[9].info, "Total|008=");
		_ifs[9].linkl("|11323", ttotalscore);
		_ifs[9].valueSet(9, _ifs[9].info, 100, 340, INFO_GREEN, true);
		infoselect.push_back(_ifs[9]);

		InfoSelect::updown = true;
		InfoSelect::nselect = 0;
		InfoSelect::select = 0;

		Player::p.nScore += ttotalscore;
	}

	if(time > 300 && !replaymode || time > 60 && hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		fgpause.exist = false;

		scene -= SCLEAR;
		infoselect.clear();
		BYTE tpart;
		bool tclear = false;

		if(scene > S1)
		{
			tpart = 0xff;
			tclear = true;
		}
		else
		{
			tpart = scene / M_STAGENSCENE - 1;
		}

		if(!replaymode)
		{
			seed = timeGetTime();
			rpy.partFill(tpart);
		}
		time = 0;
		if (!replaymode && practicemode)
		{
			state = STATE_OVER;
			return PTURN;
		}
		else if (tclear)
		{
			state = STATE_ENDING;
			return PTURN;
		}
		state = STATE_START;
		if(tclear)
		{
			scene = S1;
		}
		else
			DataConnector::Try();

		clearPrep();
		if(replaymode && tpart != 0xff)
		{
			seed = rpy.partinfo[tpart].seed;
		}

		srand(seed);

		return PTURN;
	}

	return PGO;
}