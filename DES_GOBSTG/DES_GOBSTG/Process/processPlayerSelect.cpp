#include "Process.h"
#include "Scripter.h"
#include "Player.h"
#include "InfoSelect.h"
#include "Selector.h"
#include "Data.h"
#include "DataConnector.h"
#include "SE.h"
#include "BResource.h"

int Process::processPlayerSelect()
{
	static int nowindex[10][10];

	time++;
	if(time == 1)
	{
		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+2].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+3].value) = 0;
		nowindex[0][0] = S0;
	}
	scr.controlExecute(STATE_PLAYER_SELECT, time);

	//pushtimer maxpage page depth
	int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
	int tmaxpage = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
	int tpage = CINT(scr.d[SCR_RESERVEBEGIN+2].value);
	int tdepth = CINT(scr.d[SCR_RESERVEBEGIN+3].value);

	if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN) && !tdepth)
	{
		infoselect.clear();
		SE::push(SE_SYSTEM_CANCEL);
		time = 0;
		if(!rangemode)
			state = STATE_DIFFICULT_SELECT;
		else
			state = STATE_RANGE;
		return PTURN;
	}

	if(practicemode && !tdepth && (time == 1 || luchara != Selector::select))
	{
		infoselect.clear();
		luchara = Selector::select;

		if(rangemode)
		{
			InfoSelect _ifs[14];

			for (int i=0; i<11; i++)
			{
				strcpy(_ifs[i].info, "Stage  ");
			}
			strcat(_ifs[0].info, M_LASTSTAGESTR_1);
			strcat(_ifs[1].info, M_LASTSTAGESTR_2);
			strcat(_ifs[2].info, M_LASTSTAGESTR_3A);
			strcat(_ifs[3].info, M_LASTSTAGESTR_3B);
			strcat(_ifs[4].info, M_LASTSTAGESTR_4);
			strcat(_ifs[5].info, M_LASTSTAGESTR_5);
			strcat(_ifs[6].info, M_LASTSTAGESTR_6A);
			strcat(_ifs[7].info, M_LASTSTAGESTR_6B);
			strcat(_ifs[8].info, M_LASTSTAGESTR_FULL_EX);
			strcat(_ifs[9].info, M_LASTSTAGESTR_FULL_PH);
			strcat(_ifs[10].info, M_LASTSTAGESTR_FULL_DES);
			strcpy(_ifs[11].info, "    Total");

			int nopened[11];
			int nallopened[11];
			int ngot[11];
			int nallgot[11];
			int nrange[11];

			int totalopened = 0;
			int totalgot = 0;
			int totalrange = 0;

			for(int i=0;i<11;i++)
			{
				nopened[i] = 0;
				nallopened[i] = 0;
				ngot[i] = 0;
				nallgot[i] = 0;
				nrange[i] = 0;
				for(vector<rangeData>::iterator it=res.rdata.begin(); it!=res.rdata.end(); it++)
				{
					if(it->isrange && it->sno>=i*M_STAGENSCENE+S0 && it->sno<(i+1)*M_STAGENSCENE+S0)
					{
						totalrange++;
						nrange[i]++;

						WORD indi;
						indi = data.raGetIndi(it->sno);

						if(indi)
						{
							totalopened++;
							nallopened[i]++;

							if(indi & 0xf000)
							{
								totalgot++;
								nallgot[i]++;

								if(indi & ((1<<luchara)<<12))
								{
									ngot[i]++;
								}
							}
							if(indi & 0x000f)
							{
								if(indi & (1<<luchara))
								{
									nopened[i]++;
								}
							}
						}
					}
				}
			}
			
			char buffer[M_STRITOAMAX];
			for(int i=0;i<11;i++)
			{
				_ifs[i].linki("|11620", ngot[i]);
				strcat(_ifs[i].info, "|12028(");
				strcat(_ifs[i].info, itoa(nallgot[i], buffer, 10));
				strcat(_ifs[i].info, ") /|E");
				_ifs[i].linki("|12832", nallopened[i]);
				strcat(_ifs[i].info, " /");
				_ifs[i].linki("|13438", nrange[i]);
				if(nallopened[i])
				{
					if(ngot[i] == nrange[i] && ngot[i] != 0)
						_ifs[i].coltype = INFO_GREEN;
					else
						_ifs[i].coltype = INFO_YELLOW;
					_ifs[i].nonactive = false;
					_ifs[i].gray = false;
				}
				else
				{
					_ifs[i].coltype = INFO_GRAY;
					_ifs[i].nonactive = true;
					_ifs[i].gray = true;
				}

				_ifs[i].valueSet(i, _ifs[i].info, 160, 180+20*i, _ifs[i].coltype, _ifs[i].nonactive, _ifs[i].gray);
				infoselect.push_back(_ifs[i]);
			}
			_ifs[i].linki("|12125", totalgot);
			strcat(_ifs[11].info, "  /");
			_ifs[11].linki("|12832", totalopened);
			strcat(_ifs[11].info, " /");
			_ifs[11].linki("|13438", totalrange);
			_ifs[11].valueSet(11, _ifs[11].info, 160, 400, INFO_RED, true);
			infoselect.push_back(_ifs[11]);

			_ifs[12].valueSet(12, "关卡   领域界取得数（全人物取得数）/", 160, 140, INFO_RED, true);
			_ifs[13].valueSet(13, "         领域界开启数 / 领域界总数", 160, 160, INFO_RED, true);
			infoselect.push_back(_ifs[12]);
			infoselect.push_back(_ifs[13]);

			InfoSelect::updown = true;
			InfoSelect::nselect = 11;
			if(InfoSelect::select < 0 || InfoSelect::select > 10 || time == 1)
				InfoSelect::select = 0;
		}
		else
		{
			InfoSelect _ifs[9];
			for(int i=0;i<8;i++)
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
			for(int i=0;i<8;i++)
			{
				strcat(_ifs[i].info, "|009");
				scene = (i+1)*M_STAGENSCENE;
				strcat(_ifs[i].info, "(");
				int ntrystagetime = data.nTryStageTime(i+1, nowdifflv, luchara);
				_ifs[i].linki(NULL, ntrystagetime > 0 ? ntrystagetime-1 : 0);
				strcat(_ifs[i].info, ")");

				_ifs[i].nonactive = false;
				_ifs[i].gray = false;

				LONGLONG nhiscore = data.nHighScore(i, nowdifflv, false, true, luchara);
				if(nhiscore)
				{
					_ifs[i].coltype = INFO_GREEN;
				}
				else if(ntrystagetime)
				{
					_ifs[i].coltype = INFO_YELLOW;
				}
				else
				{
					_ifs[i].nonactive = true;
					_ifs[i].gray = true;
				}
				_ifs[i].linkl("|11425", nhiscore);
				_ifs[i].valueSet(i, _ifs[i].info, 30, 200+i*20, _ifs[i].coltype, _ifs[i].nonactive, _ifs[i].gray);
				infoselect.push_back(_ifs[i]);
			}
			_ifs[8].valueSet(8, "关卡 （挑战次数） 最高得分", 30, 180, INFO_RED, true);
			infoselect.push_back(_ifs[8]);

			InfoSelect::updown = true;
			InfoSelect::nselect = 8;
			InfoSelect::select = 0;
		}
	}

	if(practicemode && rangemode)
	{
		if(tdepth == 0)
		{
			if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				bool nonactive = false;
				for(list<InfoSelect>::iterator i=infoselect.begin(); i!=infoselect.end(); i++)
				{
					if(i->ID == InfoSelect::select && i->nonactive)
					{
						nonactive = true;
						break;
					}
				}
				if(!nonactive)
				{
					for(list<Selector>::iterator it=sel.begin();it!=sel.end();it++)
					{
						it->flag |= SEL_NONACTIVE;
					}
					tdepth = 1;
					tpage = 0;

					int i = 0;
					for(vector<rangeData>::iterator it=res.rdata.begin(); it!=res.rdata.end(); it++)
					{
						if(it->isrange && it->sno >= (InfoSelect::select+1) * M_STAGENSCENE && it->sno < (InfoSelect::select+2) * M_STAGENSCENE)
						{
							nowindex[tpage][i] = it->sno;
							i++;
							if(i == 10)
							{
								i = 0;
								tpage++;
							}
						}
					}

					nowindex[tpage][i] = 0;
					if(i)
						tmaxpage = tpage;
					else
						tmaxpage = tpage - 1;
					tpage = 0;
					InfoSelect::select = 0;
					infoselect.clear();
					SE::push(SE_SYSTEM_OK);
				}
			}
		}
		else if(tdepth == 1)
		{
			infoselect.clear();

			InfoSelect _ifs[11];
			int i;
			for(i=0;i<10;i++)
			{
				scene = nowindex[tpage][i];
				if(!scene)
					break;

				strcpy(_ifs[i].info, "R-");
				_ifs[i].linki("|20205", data.getRangeNumber(scene));
				strcat(_ifs[i].info, "|006");
				WORD indi;
				indi = data.raGetIndi(scene);
				if(indi)
					strcat(_ifs[i].info, data.getRangeName(scene));
				else
					strcat(_ifs[i].info, DATA_DEFAULTSTR_WIDE);
				_ifs[i].linki("|13236", DataConnector::nGet());
				strcat(_ifs[i].info, "/");
				_ifs[i].linki("|13741", DataConnector::nMeet());

				practicemode = false;
				strcat(_ifs[i].info, "(");
				_ifs[i].linki("|14246", DataConnector::nGet());
				strcat(_ifs[i].info, "/");
				_ifs[i].linki("|14751", DataConnector::nMeet());
				strcat(_ifs[i].info, ")");

				practicemode = true;

				if(indi)
				{
					if(indi & ((1<<luchara)<<12))
						_ifs[i].coltype = INFO_GREEN;
					else
						_ifs[i].coltype = INFO_YELLOW;
					_ifs[i].nonactive = false;
					_ifs[i].gray = false;
				}
				else
				{
					_ifs[i].coltype = INFO_GRAY;
					_ifs[i].nonactive = true;
					_ifs[i].gray = true;
				}

				_ifs[i].valueSet(i, _ifs[i].info, 40, 125+22*i, _ifs[i].coltype, _ifs[i].nonactive, _ifs[i].gray);
				infoselect.push_back(_ifs[i]);

				scene++;
			}
			InfoSelect::updown = true;
			InfoSelect::nselect = i;
			if(InfoSelect::select >= i)
				InfoSelect::select = i-1;

			_ifs[10].valueSet(10, "序号  领域界名称  获得数 / 挑战数 （括号中为本篇总计）", 40, 90, INFO_RED, true);
			infoselect.push_back(_ifs[10]);

			tdepth = 2;
		}
		if(tdepth == 2)
		{
			scene = nowindex[tpage][InfoSelect::select];

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
				tpage--;
				if(tpage < 0)
					tpage = tmaxpage;
				tdepth = 1;
			}
			else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				tpage++;
				if(tpage > tmaxpage)
					tpage = 0;
				tdepth = 1;
			}
			if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_CANCEL);
				tdepth = 0;
				luchara = M_NCHARA;
				for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
				{
					i->flag &= ~SEL_NONACTIVE;
				}
				InfoSelect::select = nowindex[0][0] / M_STAGENSCENE - 1;
			}
			if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				tdepth = 3;
				for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
				{
					i->flag &= ~SEL_NONACTIVE;
				}
				hge->Input_SetDIKey(KS_FIRE, false);
			}

			if(tdepth)
			{
				if((int)(infoselect.size()) > InfoSelect::nselect+4 && InfoSelect::select != infoselect.back().info[sizeof(infoselect.back().info) - 1])
				{
					infoselect.pop_back();
					infoselect.pop_back();
					infoselect.pop_back();
					infoselect.pop_back();
				}
				if((int)(infoselect.size()) <= InfoSelect::nselect+4)
				{
					InfoSelect _ifs[4];
					
					if(data.raGetIndi(scene) & ((1<<luchara)<<12))
					{
						_ifs[2].valueSet(13, data.getRangeExplain(scene, 1), 40, 410, INFO_RED, true);
						_ifs[3].valueSet(14, data.getRangeExplain(scene, 2), 60, 430, INFO_RED, true);
					}
					else
					{
						_ifs[2].valueSet(13, DATA_DEFAULTSTR_WIDE, 60, 410, INFO_RED, true);
						_ifs[3].valueSet(14, DATA_DEFAULTSTR_WIDE, 80, 430, INFO_RED, true);
					}
					infoselect.push_back(_ifs[2]);
					infoselect.push_back(_ifs[3]);

					strcpy(_ifs[0].info, "使用者  ");
					strcat(_ifs[0].info, data.getRangeUserName(scene));
					strcat(_ifs[0].info, "|018");
					switch(data.getDiff(scene))
					{
					case M_DIFFI_EASY:
						strcat(_ifs[0].info, M_DIFFISTR_EASY);
						break;
					case M_DIFFI_NORMAL:
						strcat(_ifs[0].info, M_DIFFISTR_NORMAL);
						break;
					case M_DIFFI_HARD:
						strcat(_ifs[0].info, M_DIFFISTR_HARD);
						break;
					case M_DIFFI_EXTRA:
						strcat(_ifs[0].info, M_DIFFISTR_EXTRA);
						break;
					case M_DIFFI_PHANTASM:
						strcat(_ifs[0].info, M_DIFFISTR_PHANTASM);
						break;
					case M_DIFFI_DESTINY:
						strcat(_ifs[0].info, M_DIFFISTR_DESTINY);
						break;
					default:
						strcat(_ifs[0].info, "??");
					}
					strcat(_ifs[0].info, "|022最高得分");
					_ifs[0].linkl("|23242", DataConnector::nHiScore());

					_ifs[0].valueSet(11, _ifs[0].info, 80, 370, INFO_RED, true);
					infoselect.push_back(_ifs[0]);

					int totalgetpractice = data.nGet(scene, true);
					int totalmeetpractice = data.nMeet(scene, true);
					int totalgetgame = data.nGet(scene);
					int totalmeetgame = data.nMeet(scene);
					LONGLONG totalmaxbonus = data.nHighScore(scene, 0, true);

					strcpy(_ifs[1].info, "全人物战况");
					_ifs[1].linki("|11216", totalgetpractice);
					strcat(_ifs[1].info, "/");
					_ifs[1].linki("|11721", totalmeetpractice);
					strcat(_ifs[1].info, "(");
					_ifs[1].linki("|12226", totalgetgame);
					strcat(_ifs[1].info, "/");
					_ifs[1].linki("|12731", totalmeetgame);
					strcat(_ifs[1].info, ")");
					_ifs[1].linki("|23444", totalmaxbonus);

					_ifs[1].info[sizeof(_ifs[1].info) - 1] = InfoSelect::select;
					_ifs[1].valueSet(12, _ifs[1].info, 100, 390, INFO_RED, true);
					infoselect.push_back(_ifs[1]);
				}
			}
		}
		else if(tdepth == 3)
		{
			hge->Input_SetDIKey(KS_FIRE);
		}
	}

	if(Selector::complete)
	{
		Selector::Clear();
		if(!practicemode || InfoSelect::complete)
		{
			startPrep();
		}
		else
		{
			scene /= M_STAGENSCENE;
			scene *= M_STAGENSCENE;
			time = 0;
			state = STATE_PLAYER_SELECT;
		}
	}
	CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tmaxpage;
	CINT(scr.d[SCR_RESERVEBEGIN+2].value) = tpage;
	CINT(scr.d[SCR_RESERVEBEGIN+3].value) = tdepth;

	return PGO;
}
