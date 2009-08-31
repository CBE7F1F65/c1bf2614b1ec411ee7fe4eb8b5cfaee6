#include "Process.h"
#include "Scripter.h"
#include "BResource.h"
#include "Data.h"
#include "DataConnector.h"
#include "InfoSelect.h"
#include "Selector.h"
#include "SE.h"
#include "DataPrinter.h"

int Process::processResult()
{
	static char maxpage[7];
	static int pageindex[7][10];
	static char tnowpage;

	time++;
	if(time == 1)
	{
		Selector::Clear();

		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = - 0xf;
		CINT(scr.d[SCR_RESERVEBEGIN+2].value) = - 0xf;
		CINT(scr.d[SCR_RESERVEBEGIN+3].value) = - 0xf;
		CINT(scr.d[SCR_RESERVEBEGIN+4].value) = 0;

		int roller[7];
		for(int i=0;i<7;i++)
		{
			roller[i] = 0;
			maxpage[i] = 0;
		}

		for(vector<rangeData>::iterator i=res.rdata.begin();i!=res.rdata.end();i++)
		{
			if(!i->isrange)
				continue;

			BYTE thisdiff = data.getDiff(i->sno);

			if(roller[thisdiff] == 0)
			{
				pageindex[thisdiff][maxpage[thisdiff]] = i->sno;
				maxpage[thisdiff]++;
			}
			if(roller[6] == 0)
			{
				pageindex[6][maxpage[6]] = i->sno;
				maxpage[6]++;
			}

			roller[thisdiff]++;
			if(roller[thisdiff] == 10)
			{
				roller[thisdiff] = 0;
			}
			roller[6]++;
			if(roller[6] == 10)
			{
				roller[6] = 0;
			}
		}
	}
	if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_CANCEL);
		infoselect.clear();
		int d62 = CINT(scr.d[SCR_RESERVEBEGIN+3].value);
		int d61 = CINT(scr.d[SCR_RESERVEBEGIN+2].value);
		int d60 = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
		if(d61 >= 0)
		{
			CINT(scr.d[SCR_RESERVEBEGIN+2].value) = d61 - 0xf;
			if(d62 >= 0)
				CINT(scr.d[SCR_RESERVEBEGIN+3].value) = d62 - 0xf;
			else
				CINT(scr.d[SCR_RESERVEBEGIN+3].value) = -0xf;
		}
		else if(d60 >= 0)
		{
			CINT(scr.d[SCR_RESERVEBEGIN+1].value) = d60 - 0xf;
			if(d61 >= 0)
				CINT(scr.d[SCR_RESERVEBEGIN+2].value) = d61 - 0xf;
			else
				CINT(scr.d[SCR_RESERVEBEGIN+2].value) = -0xf;
			if(d62 >= 0)
				CINT(scr.d[SCR_RESERVEBEGIN+3].value) = d62 - 0xf;
			else
				CINT(scr.d[SCR_RESERVEBEGIN+3].value) = -0xf;
		}
		else
		{
			time = 0;
			state = STATE_TITLE;
			return PTURN;
		}
	}
	else if (hge->Input_GetDIKey(DIK_D, DIKEY_DOWN))
	{
		DataPrinter::PrintScore();
	}
	scr.controlExecute(STATE_RESAULT, time);
	//-> pushtimer sec diffi chara nowpage

	if(retvalue == POK)
	{
		int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
		int tsec = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
		int tdiff = CINT(scr.d[SCR_RESERVEBEGIN+2].value);
		int tchara = CINT(scr.d[SCR_RESERVEBEGIN+3].value);
		int tnowpage = CINT(scr.d[SCR_RESERVEBEGIN+4].value);

		bool update = false;
		hge->Input_SetDIKey(KS_FIRE, false);
		if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
		{
			tchara = -1;
			update = true;
		}
		if(tsec != 2)
		{
			if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				if(tchara)
					tchara--;
				else
					tchara = tsec + 3;
				update = true;
			}
			else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				if(tchara < tsec + 3)
					tchara++;
				else
					tchara = 0;
				update = true;
			}
			if(tsec)
			{
				if(hge->Input_GetDIKey(KS_UP) || hge->Input_GetDIKey(KS_DOWN))
				{
					tpushtimer++;
					if(tpushtimer == M_PUSH_FIRST)
						tpushtimer = M_PUSH_ROLLTO;
					if(tpushtimer == M_PUSH_ROLLTO)
					{
						hge->Input_SetDIKey(KS_UP, false);
						hge->Input_SetDIKey(KS_DOWN, false);
					}
				}
				else
				{
					tpushtimer = 0;
				}

				if(hge->Input_GetDIKey(KS_UP, DIKEY_DOWN))
				{
					SE::push(SE_SYSTEM_SELECT);
					if(tnowpage)
						tnowpage--;
					else
						tnowpage = maxpage[tdiff]-1;
					update = true;
				}
				else if(hge->Input_GetDIKey(KS_DOWN, DIKEY_DOWN))
				{
					SE::push(SE_SYSTEM_SELECT);
					if(tnowpage < maxpage[tdiff]-1)
						tnowpage++;
					else
						tnowpage = 0;
					update = true;
				}
			}
		}

		if(!infoselect.size())
		{
			if(!tsec)
			{
				InfoSelect _ifs[11];
				for(int i=0;i<10;i++)
				{
					DWORD sec;
					sec = data.sLinkType(DATAS_TOP);
					sec = data.sLinkDiff(sec, tdiff);
					sec = data.sLinkChara(sec, tchara);
					sec = data.sLinkNum(sec, i+1);

					strcpy(_ifs[i].info, "");
					_ifs[i].linki("|10002", i+1);
					strcat(_ifs[i].info, ".|004");
					strcat(_ifs[i].info, data.sRead(DATA_BINFILE, sec, data.nLinkType(DATAN_USERNAME), RESCONFIGDEFAULT_USERNAME));
					_ifs[i].linkl("|11325", data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_SCORE), 0));
					BYTE tlaststage = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_LASTSTAGE), 0);
					strcat(_ifs[i].info, "(");
					switch(tlaststage)
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
					default:
						strcat(_ifs[i].info, M_LASTSTAGESTR_C);
						break;
					}
					strcat(_ifs[i].info, ")");
					_ifs[i].linki("|13036", data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_ALIVENESS), 0));
					_ifs[i].linki("|23840", data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MONTH), 0));
					strcat(_ifs[i].info, "/");
					_ifs[i].linki("|24143", data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_DAY), 0));
					_ifs[i].linkf("|24550", 2, data.fRead(DATA_BINFILE, sec, data.nLinkType(DATAN_LOST), 0));
					strcat(_ifs[i].info, "%");

					_ifs[i].valueSet(i, _ifs[i].info, 30, 150+20*i, INFO_GREEN, true);
					infoselect.push_back(_ifs[i]);
				}
				_ifs[10].valueSet(10, " 序   名号|016得分（结束关）|032生符   日期  处理落率", 30, 130, INFO_RED, true);
				infoselect.push_back(_ifs[10]);

				InfoSelect::nselect = 0;
				InfoSelect::select = 0;
			}
			else if(tsec == 1)
			{
				int sno[10];
				int maxindex = 0;
				bool save = false;
				for(vector<rangeData>::iterator i=res.rdata.begin();i!=res.rdata.end();i++)
				{
					if(!i->isrange)
						continue;
					if(i->sno == pageindex[tdiff][tnowpage])
					{
						save = true;
					}
					if(save && (tdiff == M_NDIFFI || data.getDiff(i->sno) == tdiff))
					{
						sno[maxindex] = i->sno;
						maxindex++;
						if(maxindex == 10)
							break;
					}
				}
				InfoSelect _ifs[11];
				for(int i=0;i<maxindex;i++)
				{
					int tnget = 0;
					int tnmeet = 0;
					LONGLONG tmaxbonus = 0;

					tnget = data.nGet(sno[i], true, tchara);
					tnmeet = data.nMeet(sno[i], true, tchara);
					tmaxbonus = data.nHighScore(sno[i], 0, true, false, tchara);

					strcpy(_ifs[i].info, "R-");
					_ifs[i].linki("|20205", data.getRangeNumber(sno[i]));
					strcat(_ifs[i].info, "|005");
					if(data.raGetIndi(sno[i]))
					{
						strcat(_ifs[i].info, data.getRangeName(sno[i]));
						if(tnget)
							_ifs[i].coltype = INFO_GREEN;
						else
							_ifs[i].coltype = INFO_YELLOW;
					}
					else
					{
						strcat(_ifs[i].info, DATA_DEFAULTSTR_WIDE);
						_ifs[i].coltype = INFO_GRAY;
					}

					_ifs[i].linki("|13135", tnget);
					strcat(_ifs[i].info, "/");
					_ifs[i].linki("|13640", tnmeet);
					strcat(_ifs[i].info, "(");
					switch(data.getDiff(sno[i]))
					{
					case M_DIFFI_EASY:
						strcat(_ifs[i].info, "E");
						break;
					case M_DIFFI_NORMAL:
						strcat(_ifs[i].info, "N");
						break;
					case M_DIFFI_HARD:
						strcat(_ifs[i].info, "H");
						break;
					case M_DIFFI_EXTRA:
						strcat(_ifs[i].info, "X");
						break;
					case M_DIFFI_PHANTASM:
						strcat(_ifs[i].info, "P");
						break;
					case M_DIFFI_DESTINY:
						strcat(_ifs[i].info, "D");
						break;
					}
					strcat(_ifs[i].info, ")");
					_ifs[i].linkl("|14252", tmaxbonus);

					_ifs[i].valueSet(i, _ifs[i].info, 30, i*20+150, _ifs[i].coltype, true);
					infoselect.push_back(_ifs[i]);
				}
				_ifs[10].valueSet(10, "  序|008领域界名|020取得数/挑战数（难易度）|044最高得点", 30, 130, INFO_RED, true);
				infoselect.push_back(_ifs[10]);
				
				InfoSelect::nselect = 0;
				InfoSelect::select = 0;
			}
			else
			{
				InfoSelect _ifs[9];

				char buff[M_STRITOAMAX];
				strcpy(_ifs[0].info, DATASTR_TOTAL_GAMETIME);
				LONGLONG tltotalplaytime = data.lRead(DATA_BINFILE, data.sLinkType(DATAS_TOTAL), data.nLinkType(DATAN_TOTALPLAYTIME), 0) / 10000000;
				int tplayhour = tltotalplaytime / 3600;
				int tplayminute = (tltotalplaytime / 60) % 60;
				int tplaysecond = tltotalplaytime % 60;
				_ifs[0].linki("|014", tplayhour);
				strcat(_ifs[0].info, ":");
				if(tplayminute < 10)
					strcat(_ifs[0].info, "0");
				strcat(_ifs[0].info, itoa(tplayminute, buff, 10));
				strcat(_ifs[0].info, ":");
				if(tplaysecond < 10)
					strcat(_ifs[0].info, "0");
				strcat(_ifs[0].info, itoa(tplaysecond, buff, 10));
				_ifs[0].valueSet(0, _ifs[0].info, 40, 120, INFO_BLUE, true);
				
				char buffer[M_STRMAX];
				strcpy(buffer, DATASTR_TOTAL_PLAYTIME);
				strcat(buffer, "|017");
				strcat(buffer, M_DIFFISTR_EASY);
				strcat(buffer, "|022");
				strcat(buffer, M_DIFFISTR_NORMAL);
				strcat(buffer, "|027");
				strcat(buffer, M_DIFFISTR_HARD);
				strcat(buffer, "|032");
				strcat(buffer, M_DIFFISTR_EXTRA);
				strcat(buffer, "|037");
				strcat(buffer, M_DIFFISTR_PHANTASM);
				strcat(buffer, "|042");
				strcat(buffer, DATASTR_TOTAL_PLAYTIME_ALL);

				_ifs[1].valueSet(1, buffer, 40, 160, INFO_GREEN, true);

				int tplaytime[M_NCHARA+1][M_NSTAGEDIFFI+1];
				int tcleartime[M_NSTAGEDIFFI+1];
				int tpracticetime[M_NSTAGEDIFFI+1];
				for(int i=0;i<M_NCHARA+1;i++)
					tplaytime[i][M_NSTAGEDIFFI] = 0;
				for(int i=0;i<M_NSTAGEDIFFI+1;i++)
				{
					tplaytime[M_NCHARA][i] = 0;
					tcleartime[i] = 0;
					tpracticetime[i] = 0;
				}
				for(int i=0;i<M_NCHARA;i++)
				{
					for(int j=0;j<M_NSTAGEDIFFI;j++)
					{
						DWORD sec;
						sec = data.sLinkType(DATAS_TOTAL);
						sec = data.sLinkDiff(sec, j);
						sec = data.sLinkChara(sec, i);

						tplaytime[i][j] = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_PLAYTIME), 0);
						tplaytime[i][M_NSTAGEDIFFI] += tplaytime[i][j];
						tplaytime[M_NCHARA][j] += tplaytime[i][j];
						tcleartime[j] += data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_CLEARTIME), 0);
						tpracticetime[j] += data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_PRACTICETIME), 0);
					}
					tplaytime[M_NCHARA][M_NSTAGEDIFFI] += tplaytime[i][M_NSTAGEDIFFI];
				}
				for (int i=0; i<M_NSTAGEDIFFI; i++)
				{
					tcleartime[M_NSTAGEDIFFI] += tcleartime[i];
					tpracticetime[M_NSTAGEDIFFI] += tpracticetime[i];
				}
				strcpy(_ifs[2].info, M_CHARASTR_FULL_SPACED_SHI_LINGYE);
				strcpy(_ifs[3].info, M_CHARASTR_FULL_SPACED_NUSIKUIMA_GEJI);
				strcpy(_ifs[4].info, M_CHARASTR_FULL_SPACED_CILAN_ZANGYUE);
				strcpy(_ifs[5].info, M_CHARASTR_FULL_SPACED_WUSHUANG);
				strcpy(_ifs[6].info, DATASTR_TOTAL_SPACED_ALLCHARA);
				for(int i=0;i<M_NCHARA+1;i++)
				{
					_ifs[i+2].linki("|11419", tplaytime[i][0]);
					_ifs[i+2].linki("|11924", tplaytime[i][1]);
					_ifs[i+2].linki("|12429", tplaytime[i][2]);
					_ifs[i+2].linki("|12934", tplaytime[i][3]);
					_ifs[i+2].linki("|13439", tplaytime[i][4]);
					_ifs[i+2].linki("|13945", tplaytime[i][5]);

					_ifs[i+2].valueSet(i+2, _ifs[i+2].info, 40, i*25+192+i/4*12, INFO_BLUE, true);
				}

				strcpy(_ifs[7].info, DATASTR_TOTAL_SPACED_CLEARTIME);
				_ifs[7].linki("|11419", tcleartime[0]);
				_ifs[7].linki("|11924", tcleartime[1]);
				_ifs[7].linki("|12429", tcleartime[2]);
				_ifs[7].linki("|12934", tcleartime[3]);
				_ifs[7].linki("|13439", tcleartime[4]);
				_ifs[7].linki("|13945", tcleartime[5]);

				_ifs[7].valueSet(7, _ifs[7].info, 40, 340, INFO_YELLOW, true);

				strcpy(_ifs[8].info, DATASTR_TOTAL_SPACED_PRACTICETIME);
				_ifs[8].linki("|11419", tpracticetime[0]);
				_ifs[8].linki("|11924", tpracticetime[1]);
				_ifs[8].linki("|12429", tpracticetime[2]);
				_ifs[8].linki("|12934", tpracticetime[3]);
				_ifs[8].linki("|13439", tpracticetime[4]);
				_ifs[8].linki("|13945", tpracticetime[5]);

				_ifs[8].valueSet(8, _ifs[8].info, 40, 370, INFO_YELLOW, true);

				for(int i=0;i<9;i++)
				{
					infoselect.push_back(_ifs[i]);
				}

				InfoSelect::nselect = 0;
				InfoSelect::select = 0;
			}
		}

		if(tsec == 2)
		{
			if(infoselect.size() > 9)
				infoselect.pop_back();

			InfoSelect _ifs;

			char buff[M_STRITOAMAX];
			strcpy(_ifs.info, DATASTR_TOTAL_RUNTIME);
			LONGLONG tldiffruntime = data.getTotalRunTime();
			int trunhour = tldiffruntime / 3600;
			int trunminute = (tldiffruntime / 60) % 60;
			int trunsecond = tldiffruntime % 60;
			_ifs.linki("|014", trunhour);
			strcat(_ifs.info, ":");
			if(trunminute < 10)
				strcat(_ifs.info, "0");
			strcat(_ifs.info, itoa(trunminute, buff, 10));
			strcat(_ifs.info, ":");
			if(trunsecond < 10)
				strcat(_ifs.info, "0");
			strcat(_ifs.info, itoa(trunsecond, buff, 10));
			_ifs.valueSet(9, _ifs.info, 40, 100, INFO_RED, true);

			infoselect.push_back(_ifs);
		}

		if(update)
		{
			CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tsec;
			CINT(scr.d[SCR_RESERVEBEGIN+2].value) = tdiff;
			CINT(scr.d[SCR_RESERVEBEGIN+3].value) = tchara;
			CINT(scr.d[SCR_RESERVEBEGIN+4].value) = tnowpage;
			infoselect.clear();
		}
		CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	}
	retvalue = PGO;
	return PGO;
}