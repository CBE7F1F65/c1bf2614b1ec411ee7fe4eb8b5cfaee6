#include "Process.h"
#include "Replay.h"
#include "Scripter.h"
#include "InfoSelect.h"
#include "BResource.h"
#include "SE.h"
#include "Data.h"
#include "DataPrinter.h"
#include "Export.h"

int Process::processReplay()
{
	time++;

	static Replay _rpy[RPYENUMMAX];
	static char _rpyfilename[RPYENUMMAX][M_PATHMAX];

	if(time == 1)
	{
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+2].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+3].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+4].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+5].value) = 0;

		SetCurrentDirectory(hge->Resource_MakePath(res.resdata.replayfoldername));
		char * buffer;
		char enumfile[M_STRMAX];
		strcpy(enumfile, res.resdata.replayfoldername);
		strcat(enumfile, "*.");
		strcat(enumfile, res.resdata.replayextensionname7);
		buffer = hge->Resource_EnumFiles(enumfile);

		int tnrpys = 0;
		while(buffer != NULL && tnrpys < RPYENUMMAX)
		{
			if(strlen(buffer) < 4+RPYFILENAME_CONTENTMAX+1+strlen(res.resdata.replayextensionname7)+1)
			{
				strcpy(_rpyfilename[tnrpys], buffer);

				if(_rpy[tnrpys].Load(_rpyfilename[tnrpys]))
				{
					tnrpys++;
				}
			}
			if(!hge->Resource_EnumFiles())
				break;
		}
		CINT(scr.d[SCR_RESERVEBEGIN+4].value) = tnrpys;

		InfoSelect::select = 0;
	}

	scr.controlExecute(STATE_REPLAY, time);

	//-> pushtimer page depth index nrpys rpydifflv

	int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
	int tpage = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
	int tdepth = CINT(scr.d[SCR_RESERVEBEGIN+2].value);
	int tindex = CINT(scr.d[SCR_RESERVEBEGIN+3].value);
	int tnrpys = CINT(scr.d[SCR_RESERVEBEGIN+4].value);
	int trpydifflv = CINT(scr.d[SCR_RESERVEBEGIN+5].value);

	if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN) && tdepth < 2)
	{
		SE::push(SE_SYSTEM_CANCEL);
		SetCurrentDirectory(hge->Resource_MakePath(""));
		time = 0;
		state = STATE_TITLE;
		return PTURN;
	}

	if(!tdepth)
	{
		infoselect.clear();
		InfoSelect _ifs[13];
		_ifs[12].valueSet(15, "名号|009时刻|015主役|021舞台|033得点|043抄记", 30, 80, INFO_RED, true);
		infoselect.push_back(_ifs[12]);
		int i;
		for(i=0;i<tnrpys-tpage*12 && i<12;i++)
		{
			tindex = i+tpage*12;
			strcpy(_ifs[i].info, _rpy[tindex].rpyinfo.username);
			_ifs[i].linki("|20911", _rpy[tindex].rpyinfo.month);

			strcat(_ifs[i].info, "/");
			_ifs[i].linki("|21214", _rpy[tindex].rpyinfo.day);

			strcat(_ifs[i].info, "|015");

			switch(_rpy[tindex].rpyinfo.usingchara)
			{
			case M_CHARA_SHI_LINGYE:
				strcat(_ifs[i].info, M_CHARASTR_SHI_LINGYE);
				break;
			case M_CHARA_NUSIKUIMA_GEJI:
				strcat(_ifs[i].info, M_CHARASTR_NUSIKUIMA_GEJI);
				break;
			case M_CHARA_CILAN_ZANGYUE:
				strcat(_ifs[i].info, M_CHARASTR_CILAN_ZANGYUE);
				break;
			case M_CHARA_WUSHUANG:
				strcat(_ifs[i].info, M_CHARASTR_WUSHUANG);
				break;
			}
			strcat(_ifs[i].info, "|021");
			if(_rpy[tindex].rpyinfo.modeflag & RPYMODE_PRACTICE)
			{
				if(_rpy[tindex].rpyinfo.modeflag & RPYMODE_RANGE)
				{
					strcat(_ifs[i].info, "R-");
					_ifs[i].linki("|11519", _rpy[tindex].rpyinfo.startscene);
					_ifs[i].coltype = INFO_YELLOW;
				}
				else
				{
					switch(_rpy[tindex].rpyinfo.difflv)
					{
					case M_DIFFI_EASY:
						strcat(_ifs[i].info, M_DIFFISTR_EASY);
						break;
					case M_DIFFI_NORMAL:
						strcat(_ifs[i].info, M_DIFFISTR_NORMAL);
						break;
					case M_DIFFI_HARD:
						strcat(_ifs[i].info, M_DIFFISTR_HARD);
						break;
					}
					strcat(_ifs[i].info, "-");
					switch(_rpy[tindex].rpyinfo.startscene / M_STAGENSCENE)
					{
					case 1:
						strcat(_ifs[i].info, M_LASTSTAGESTR_1);
						break;
					case 2:
						strcat(_ifs[i].info, M_LASTSTAGESTR_2);
						break;
					case 3:
						strcat(_ifs[i].info, M_LASTSTAGESTR_3A);
						break;
					case 4:
						strcat(_ifs[i].info, M_LASTSTAGESTR_3B);
						break;
					case 5:
						strcat(_ifs[i].info, M_LASTSTAGESTR_4);
						break;
					case 6:
						strcat(_ifs[i].info, M_LASTSTAGESTR_5);
						break;
					case 7:
						strcat(_ifs[i].info, M_LASTSTAGESTR_6A);
						break;
					case 8:
						strcat(_ifs[i].info, M_LASTSTAGESTR_6B);
						break;
					}
					_ifs[i].coltype = INFO_GREEN;
				}
			}
			else
			{
				switch(_rpy[tindex].rpyinfo.difflv)
				{
				case M_DIFFI_EASY:
					strcat(_ifs[i].info, M_DIFFISTR_EASY);
					break;
				case M_DIFFI_NORMAL:
					strcat(_ifs[i].info, M_DIFFISTR_NORMAL);
					break;
				case M_DIFFI_HARD:
					strcat(_ifs[i].info, M_DIFFISTR_HARD);
					break;
				case M_DIFFI_EXTRA:
					strcat(_ifs[i].info, M_DIFFISTR_EXTRA);
					break;
				case M_DIFFI_PHANTASM:
					strcat(_ifs[i].info, M_DIFFISTR_PHANTASM);
					break;
				}
				_ifs[i].coltype = INFO_GREEN;
			}
			_ifs[i].linkl("|12840", _rpy[tindex].rpyinfo.score);

			strcat(_ifs[i].info, "|041");
			strcat(_ifs[i].info, _rpyfilename[tindex]);
			_ifs[i].info[strlen(_ifs[i].info) - 4] = 0;

			_ifs[i].valueSet(i, _ifs[i].info, 30, 120+i*22, _ifs[i].coltype);
			infoselect.push_back(_ifs[i]);
		}

		InfoSelect::updown = true;
		InfoSelect::nselect = i;
		if(InfoSelect::select >= i)
			InfoSelect::select = i - 1;

		tdepth = 1;
	}
	if(tdepth == 1)
	{
		tindex = InfoSelect::select + tpage*12;
		trpydifflv = _rpy[tindex].rpyinfo.difflv;

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
				tpage = (tnrpys-1) / 12;
			tdepth = 0;
		}
		else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_SELECT);
			if(tpage >= (tnrpys-1) / 12)
				tpage = 0;
			else
				tpage++;
			tdepth = 0;
		}

		if(InfoSelect::complete)
		{
			tdepth = 2;
		}
	}
	if(tdepth == 2)
	{
		infoselect.clear();
		InfoSelect _ifs[20];
		if(!(_rpy[tindex].rpyinfo.modeflag & RPYMODE_RANGE))
		{
			for (int i=0; i<10; i++)
			{
				strcpy(_ifs[i].info, "Stage ");
			}
			strcat(_ifs[0].info, M_LASTSTAGESTR_1);
			strcat(_ifs[1].info, M_LASTSTAGESTR_2);
			strcat(_ifs[2].info, M_LASTSTAGESTR_3A);
			strcat(_ifs[3].info, M_LASTSTAGESTR_3B);
			strcat(_ifs[4].info, M_LASTSTAGESTR_4);
			strcat(_ifs[5].info, M_LASTSTAGESTR_5);
			strcat(_ifs[6].info, M_LASTSTAGESTR_6A);
			strcat(_ifs[7].info, M_LASTSTAGESTR_6B);
			strcat(_ifs[8].info, M_LASTSTAGESTR_EX);
			strcat(_ifs[9].info, M_LASTSTAGESTR_PH);
			for(int i=0;i<10;i++)
			{
				_ifs[i].nonactive = true;
				_ifs[i].gray = true;
				_ifs[i].coltype = INFO_GRAY;
			}
			if(_rpy[tindex].rpyinfo.difflv < 3)
			{
				for(int i=0;i<RPYPARTMAX;i++)
				{
					if(_rpy[tindex].partinfo[i].offset)
					{
						int j;
						for(j=i+1;j<RPYPARTMAX;j++)
						{
							if(_rpy[tindex].partinfo[j].offset)
							{
								_ifs[i].linkl("|21020", _rpy[tindex].partinfo[j].nowscore);
								break;
							}
						}
						if(j == RPYPARTMAX)
							_ifs[i].linkl("|21020", _rpy[tindex].rpyinfo.score);
						_ifs[i].nonactive = false;
						_ifs[i].gray = false;
						_ifs[i].coltype = INFO_GREEN;
					}
				}
			}
			else
			{
				int i = 8+_rpy[tindex].rpyinfo.difflv-3;
				_ifs[i].linkl("|21020", _rpy[tindex].rpyinfo.score);
				_ifs[i].nonactive = false;
				_ifs[i].gray = false;
				_ifs[i].coltype = INFO_GREEN;
			}
			for(int i=0;i<10;i++)
			{
				_ifs[i].valueSet(i, _ifs[i].info, 360, 120+22*i, _ifs[i].coltype, _ifs[i].nonactive, _ifs[i].gray);
				infoselect.push_back(_ifs[i]);
			}
			InfoSelect::updown = true;
			InfoSelect::nselect = 10;
			InfoSelect::select = 0;
		}
		else
		{
			strcpy(_ifs[0].info, "R-");
			_ifs[0].linki("|10206", _rpy[tindex].rpyinfo.startscene);

			strcat(_ifs[0].info, data.getRangeName((int)(_rpy[tindex].rpyinfo.startscene)));
			_ifs[0].valueSet(0, _ifs[0].info, 360, 180, INFO_RED);
			infoselect.push_back(_ifs[0]);

			strcpy(_ifs[1].info, RPYSTR_SCORE);
			_ifs[1].linkl("|10616", _rpy[tindex].rpyinfo.score);

			_ifs[1].valueSet(1, _ifs[1].info, 400, 220, INFO_RED, true);
			infoselect.push_back(_ifs[1]);

			InfoSelect::updown = true;
			InfoSelect::nselect = 1;
			InfoSelect::select = 0;
		}

		strcpy(_ifs[10].info, RPYSTR_USERNAME);
		strcat(_ifs[10].info, "|010");
		strcat(_ifs[10].info, _rpy[tindex].rpyinfo.username);

		strcpy(_ifs[11].info, RPYSTR_USINGCHARA);
		strcat(_ifs[11].info, "|010");
		switch(_rpy[tindex].rpyinfo.usingchara)
		{
		case M_CHARA_SHI_LINGYE:
			strcat(_ifs[11].info, M_CHARASTR_FULL_SPACED_SHI_LINGYE);
			break;
		case M_CHARA_NUSIKUIMA_GEJI:
			strcat(_ifs[11].info, M_CHARASTR_FULL_SPACED_NUSIKUIMA_GEJI);
			break;
		case M_CHARA_CILAN_ZANGYUE:
			strcat(_ifs[11].info, M_CHARASTR_FULL_SPACED_CILAN_ZANGYUE);
			break;
		case M_CHARA_WUSHUANG:
			strcat(_ifs[11].info, M_CHARASTR_FULL_SPACED_WUSHUANG);
			break;
		}

		strcpy(_ifs[12].info, RPYSTR_DATE);
		_ifs[12].linki("|21014", _rpy[tindex].rpyinfo.year);
		strcat(_ifs[12].info, "/");
		_ifs[12].linki("|21517", _rpy[tindex].rpyinfo.month);
		strcat(_ifs[12].info, "/");
		_ifs[12].linki("|21820", _rpy[tindex].rpyinfo.day);
		strcat(_ifs[12].info, " ");
		_ifs[12].linki("|22123", _rpy[tindex].rpyinfo.hour);
		strcat(_ifs[12].info, ":");
		_ifs[12].linki("|22426", _rpy[tindex].rpyinfo.minute);

		strcpy(_ifs[13].info, RPYSTR_ALIVENESS);
		_ifs[13].linki("|010", _rpy[tindex].rpyinfo.aliveness);

		strcpy(_ifs[14].info, RPYSTR_LOST);
		_ifs[14].linkf("|010", 0, _rpy[tindex].rpyinfo.lost);
		strcat(_ifs[14].info, "%");		

		strcpy(_ifs[15].info, RPYSTR_MISS);
		_ifs[15].linki("|010", _rpy[tindex].rpyinfo.miss);

		strcpy(_ifs[16].info, RPYSTR_SHORT_BOMB);
		_ifs[16].linki("|010", _rpy[tindex].rpyinfo.bomb);

		strcpy(_ifs[17].info, RPYSTR_SHORT_GET);
		_ifs[17].linki("|010", _rpy[tindex].rpyinfo.get);

		strcpy(_ifs[18].info, RPYSTR_FILENAME);
		strcat(_ifs[18].info, "|010");
		strcat(_ifs[18].info, _rpyfilename[tindex]);

		strcpy(_ifs[19].info, RPYSTR_MODEFLAG);
		strcat(_ifs[19].info, "|010");
		if(_rpy[tindex].rpyinfo.modeflag & RPYMODE_RANGE)
		{
			strcat(_ifs[19].info, RPYSTR_MODE_RANGE);
		}
		else if(_rpy[tindex].rpyinfo.modeflag & RPYMODE_PRACTICE)
		{
			strcat(_ifs[19].info, RPYSTR_MODE_PRACTICE);
		}
		else
		{
			strcat(_ifs[19].info, RPYSTR_MODE_RANGE);
		}

		for(int i=10;i<20;i++)
		{
			_ifs[i].valueSet(i, _ifs[i].info, 40, 90+(i-10)*25, INFO_GREEN, true);
			infoselect.push_back(_ifs[i]);
		}

		tdepth = 3;
	}
	if(tdepth == 3)
	{
		if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_CANCEL);
			tdepth = 0;
			infoselect.clear();
			InfoSelect::select = tindex % 12;
		}
		else if (hge->Input_GetDIKey(DIK_D, DIKEY_DOWN))
		{
			DataPrinter::PrintReplayData(res.resdata.replayfoldername, _rpyfilename[tindex]);
		}

		if(InfoSelect::complete)
		{
			replaymode = true;
			scene = (InfoSelect::select+1) * M_STAGENSCENE;
			strcpy(rpyfilename, _rpyfilename[tindex]);
			state = STATE_START;
			startPrep();
			return PTURN;
		}
	}

	CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tpage;
	CINT(scr.d[SCR_RESERVEBEGIN+2].value) = tdepth;
	CINT(scr.d[SCR_RESERVEBEGIN+3].value) = tindex;
	CINT(scr.d[SCR_RESERVEBEGIN+5].value) = trpydifflv;

	return PGO;
}