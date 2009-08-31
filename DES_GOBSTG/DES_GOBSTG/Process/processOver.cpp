#include "Process.h"
#include "Scripter.h"
#include "BGLayer.h"
#include "Player.h"
#include "DataConnector.h"
#include "Selector.h"
#include "InfoSelect.h"
#include "SE.h"
#include "Data.h"
#include "Fontsys.h"
#include "Replay.h"
#include "BResource.h"
#include "Export.h"

int Process::processOver()
{
	static char savefilename[M_STRMAX];

	time++;
	if(time == 1)
	{
		endscene = scene;

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

		strcpy(savefilename, "");

		CINT(scr.d[SCR_RESERVEBEGIN].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+1].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+2].value) = 0;
		CINT(scr.d[SCR_RESERVEBEGIN+3].value) = 0;

		rpy.Fill();

		nowdifflv = data.getDiff(rpy.rpyinfo.startscene);

		CINT(scr.d[SCR_RESERVEBEGIN+4].value) = DataConnector::Insert();
	}
	retvalue = PGO;
	retvalue = scr.controlExecute(STATE_OVER, time);
	//pushtimer depth sec nowchar insert
	int tpushtimer = CINT(scr.d[SCR_RESERVEBEGIN].value);
	int tdepth = CINT(scr.d[SCR_RESERVEBEGIN+1].value);
	int tsec = CINT(scr.d[SCR_RESERVEBEGIN+2].value);
	int tnowchar = CINT(scr.d[SCR_RESERVEBEGIN+3].value);
	int tinsert = CINT(scr.d[SCR_RESERVEBEGIN+4].value);

	if(tdepth == 0)
	{
		char scorebuffer[M_STRITOAMAX];
		_i64toa(rpy.rpyinfo.score, scorebuffer, 10);
		char alivenessbuffer[M_STRITOAMAX];
		_i64toa(rpy.rpyinfo.aliveness, alivenessbuffer, 10);
		char lostbuffer[M_STRITOAMAX];
		sprintf(lostbuffer, "%.2f", rpy.rpyinfo.lost);
		strcat(lostbuffer, "%");

		for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
		{
			if(i->ID < 0x10)
			{
				if(rpy.rpyinfo.cont)
				{
					if(i->ID == 2)
					{
						i->sprite->SetTextureRect(512, 608, 256, 32);
						Selector::nselect = 1;
						Selector::select = 1;
					}
					else
					{
						if(i->ID == 0)
							i->flag |= SEL_NONACTIVE;
						i->sprite->SetTextureRect(0, 0, 0, 0);
					}
				}
			}
			else if((i->ID & 0xf0) == 0x20)
			{
				if(i->ID < strlen(scorebuffer) + 0x20)
				{
					i->sprite->SetTextureRect((scorebuffer[i->ID-0x20]-'0')*32, 64, 32, 32);
				}
				else
				{
					i->sprite->SetTextureRect(0, 0, 0, 0);
				}
			}
			else if((i->ID & 0xf0) == 0x30)
			{
				if(i->ID < strlen(alivenessbuffer) + 0x30)
				{
					i->sprite->SetTextureRect((alivenessbuffer[i->ID-0x30]-'0')*32, 64, 32, 32);
				}
				else
				{
					i->sprite->SetTextureRect(0, 0, 0, 0);
				}
			}
			else if((i->ID & 0xf0) == 0x40)
			{
				if(i->ID & 1)
					i->sprite->SetTextureRect((rpy.rpyinfo.miss%10)*32, 64, 32, 32);
				else
					i->sprite->SetTextureRect((rpy.rpyinfo.miss/10)*32, 64, 32, 32);
			}
			else if((i->ID & 0xf0) == 0x50)
			{
				if(i->ID & 1)
					i->sprite->SetTextureRect((rpy.rpyinfo.cont%10)*32, 64, 32, 32);
				else
					i->sprite->SetTextureRect((rpy.rpyinfo.cont/10)*32, 64, 32, 32);
			}
			else if((i->ID & 0xf0) == 0x60)
			{
				if(i->ID < strlen(lostbuffer) + 0x60)
				{
					if(lostbuffer[i->ID-0x60] == '.')
						lostbuffer[i->ID-0x60] = '0' + 11;
					else if(lostbuffer[i->ID-0x60] == '%')
						lostbuffer[i->ID-0x60] = '0' + 10;
					i->sprite->SetTextureRect((lostbuffer[i->ID-0x60]-'0')*32, 64, 32, 32);
				}
				else
				{
					i->sprite->SetTextureRect(0, 0, 0, 0);
				}
			}
			else if((i->ID & 0xf0) == 0x70)
			{
				if(i->ID & 1)
					i->sprite->SetTextureRect((rpy.rpyinfo.bomb%10)*32, 64, 32, 32);
				else
					i->sprite->SetTextureRect((rpy.rpyinfo.bomb/10)*32, 64, 32, 32);
			}
			else if((i->ID & 0xf0) == 0x80)
			{
				if(i->ID & 1)
					i->sprite->SetTextureRect((rpy.rpyinfo.get%10)*32, 64, 32, 32);
				else
					i->sprite->SetTextureRect((rpy.rpyinfo.get/10)*32, 64, 32, 32);
			}
		}
		tdepth = 1;
	}
	else if(tdepth == 2)
	{
		infoselect.clear();
		InfoSelect _ifs[10];

		strcpy(_ifs[0].info, RPYSTR_SCORE);
		strcat(_ifs[0].info, "|010");
		_ifs[0].linkl(NULL, rpy.rpyinfo.score);

		strcpy(_ifs[1].info, RPYSTR_USINGCHARA);
		strcat(_ifs[1].info, "|010");
		switch(rpy.rpyinfo.usingchara)
		{
		case M_CHARA_SHI_LINGYE:
			strcat(_ifs[1].info, M_CHARASTR_FULL_SPACED_SHI_LINGYE);
			break;
		case M_CHARA_NUSIKUIMA_GEJI:
			strcat(_ifs[1].info, M_CHARASTR_FULL_SPACED_NUSIKUIMA_GEJI);
			break;
		case M_CHARA_CILAN_ZANGYUE:
			strcat(_ifs[1].info, M_CHARASTR_FULL_SPACED_CILAN_ZANGYUE);
			break;
		case M_CHARA_WUSHUANG:
			strcat(_ifs[1].info, M_CHARASTR_FULL_SPACED_WUSHUANG);
			break;
		}

		strcpy(_ifs[2].info, RPYSTR_DATE);
		_ifs[2].linki("|21014", rpy.rpyinfo.year);
		strcat(_ifs[2].info, "/");
		_ifs[2].linki("|21517", rpy.rpyinfo.month);
		strcat(_ifs[2].info, "/");
		_ifs[2].linki("|21820", rpy.rpyinfo.day);
		strcat(_ifs[2].info, " ");
		_ifs[2].linki("|22123", rpy.rpyinfo.hour);
		strcat(_ifs[2].info, ":");
		_ifs[2].linki("|22426", rpy.rpyinfo.minute);

		strcpy(_ifs[3].info, RPYSTR_ALIVENESS);
		_ifs[3].linki("|010", rpy.rpyinfo.aliveness);

		strcpy(_ifs[4].info, RPYSTR_LOST);
		_ifs[4].linkf("|010", 0, rpy.rpyinfo.lost);
		strcat(_ifs[4].info, "%");		

		strcpy(_ifs[5].info, RPYSTR_MISS);
		_ifs[5].linki("|010", rpy.rpyinfo.miss);

		strcpy(_ifs[6].info, RPYSTR_SHORT_BOMB);
		_ifs[6].linki("|010", rpy.rpyinfo.bomb);

		strcpy(_ifs[7].info, RPYSTR_SHORT_GET);
		_ifs[7].linki("|010", rpy.rpyinfo.get);

		strcpy(_ifs[8].info, RPYSTR_MODEFLAG);
		strcat(_ifs[8].info, "|010");
		if(rpy.rpyinfo.modeflag & RPYMODE_RANGE)
		{
			strcat(_ifs[8].info, RPYSTR_MODE_RANGE);
		}
		else if(rpy.rpyinfo.modeflag & RPYMODE_PRACTICE)
		{
			strcat(_ifs[8].info, RPYSTR_MODE_PRACTICE);
		}
		else
		{
			strcat(_ifs[8].info, RPYSTR_MODE_NONE);
		}

		for(int i=0;i<RPYPARTMAX;i++)
		{
			_ifs[i].valueSet(i+10, _ifs[i].info, 180, 90+i*25, INFO_GREEN, true);
			infoselect.push_back(_ifs[i]);
		}

		if(!strcmp(savefilename, ""))
		{
			do{
				strcat(savefilename, "DES_");
				char buffer[M_STRITOAMAX];
				for(int i=0;i<RPYFILENAME_CONTENTMAX;i++)
				{
					buffer[0] = rand()%26 + 'A';
					buffer[1] = 0;
					strcat(savefilename, buffer);
				}
				strcat(savefilename, ".");
				strcat(savefilename, res.resdata.replayextensionname7);
			}while(_access(hge->Resource_MakePath(savefilename), 00) != -1);
		}

		_ifs[9].valueSet(2, "确定", 320, 370, INFO_GREEN);
		infoselect.push_back(_ifs[9]);

		InfoSelect::updown = true;
		InfoSelect::nselect = 3;
		InfoSelect::select = 0;

		for(int i=0;i<RPYINFO_USERNAMEMAX-1;i++)
		{
			if(!username[i])
			{
				for(int j=i;j<RPYINFO_USERNAMEMAX-1;j++)
					username[j] = ' ';
				break;
			}
		}
		username[RPYINFO_USERNAMEMAX-1] = 0;

		tdepth = 3;
	}
	else if(tdepth == 3)
	{
		if(infoselect.size() > 9)
		{
			infoselect.pop_back();
			infoselect.pop_back();
		}

		if(hge->Input_GetDIKey(KS_UP, DIKEY_DOWN) || hge->Input_GetDIKey(KS_DOWN, DIKEY_DOWN))
		{
			tnowchar = 0;
		}

		tsec = InfoSelect::select;
		if(tsec == 2)
			goto skip1;

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
			if(tnowchar)
				tnowchar--;
		}
		else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_SELECT);
			if(tnowchar < RPYFILENAME_CONTENTMAX-1)
				tnowchar++;
		}
		if(hge->Input_GetDIKey(KS_ENTER, DIKEY_DOWN) || hge->Input_GetDIJoy(joyFire, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_OK);
			tsec++;
			InfoSelect::select++;
			tnowchar = 0;
			InfoSelect::avoid = true;
			goto skip3;
		}
		if(hge->Input_GetDIKey(DIK_BACKSPACE, DIKEY_DOWN))
		{
			if(tnowchar)
			{
				SE::push(SE_SYSTEM_CANCEL);
				tnowchar--;
				if(!tsec)
					username[tnowchar] = ' ';
				else
					savefilename[tnowchar+11] = ' ';
			}
		}
		if(hge->Input_GetDIKey(DIK_DELETE, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_CANCEL);
			if(!tsec)
				username[tnowchar] = ' ';
			else
				savefilename[tnowchar+11] = ' ';
		}

		char nowchar = getInputNowChar(!tsec);
		if(nowchar > 0)
		{
			SE::push(SE_SYSTEM_OK);
			if(!tsec)
				username[tnowchar] = nowchar;
			else
				savefilename[tnowchar+4] = nowchar;
			if(tnowchar < RPYFILENAME_CONTENTMAX-1)
				tnowchar++;
		}
		for(int i=RPYFILENAME_CONTENTMAX-1;i>=0;i--)
		{
			if(username[i] == ' ')
				username[i] = 0;
			else
				break;
		}
skip1:
		InfoSelect _ifs[2];

		strcpy(_ifs[0].info, RPYSTR_USERNAME);
		strcat(_ifs[0].info, "|010");
		strcat(_ifs[0].info, username);
		_ifs[0].valueSet(0, _ifs[0].info, 220, 310, INFO_RED);
		infoselect.push_back(_ifs[0]);

		strcpy(_ifs[1].info, RPYSTR_FILENAME);
		strcat(_ifs[1].info, "|010");
		strcat(_ifs[1].info, savefilename);
		_ifs[1].valueSet(1, _ifs[1].info, 220, 340, INFO_RED);
		infoselect.push_back(_ifs[1]);

		if(tsec == 2)
		{
			if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_OK);

				SetCurrentDirectory(hge->Resource_MakePath(res.resdata.replayfoldername));
				char * buffer;
				char enumfile[M_STRMAX];
				strcpy(enumfile, res.resdata.replayfoldername);
				strcat(enumfile, "*.");
				strcat(enumfile, res.resdata.replayextensionname7);
				buffer = hge->Resource_EnumFiles(enumfile);
				while(buffer != NULL)
				{
					if(!strcmp(buffer, savefilename))
					{
						tdepth = 4;
						break;
					}
					if(!hge->Resource_EnumFiles())
						break;
				}
				SetCurrentDirectory(hge->Resource_MakePath(""));

				if(tdepth != 4)
					tdepth = 6;
				else
					InfoSelect::avoid = true;

			}
			else if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_CANCEL);
				tdepth = 7;
			}
		}
		else
		{
			InfoSelect::avoid = true;
		}
	}
	else if(tdepth == 4 || tdepth == 5)
	{
		InfoSelect::avoid = true;
		hge->Input_SetDIKey(KS_UP, false);
		hge->Input_SetDIKey(KS_DOWN, false);
	}
	else if(tdepth == 6)
	{
//		rpy.Fill();
		strcpy(rpy.rpyinfo.username, username);
		rpy.Save(savefilename);

		tdepth = 7;
	}
	else if(tdepth == 7)
	{
		infoselect.clear();
		if(tinsert < 0)
		{
			time = 0;
			if(practicemode)
				state = STATE_PLAYER_SELECT;
			else
				state = STATE_TITLE;
			return PTURN;
		}

		InfoSelect _ifs[12];
		for(int i=0;i<10;i++)
		{
			DWORD sec;
			sec = data.sLinkType(DATAS_TOP);
			sec = data.sLinkDiff(sec, nowdifflv);
			sec = data.sLinkChara(sec, luchara);
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

			_ifs[i].valueSet(i == tinsert-1 ? 0 : i+2, _ifs[i].info, 30, 150+20*i, INFO_GREEN, i == tinsert-1 ? false : true);
			infoselect.push_back(_ifs[i]);
		}
		_ifs[10].valueSet(12, " 序   名号|016得分（结束关）|032生符   日期  处理落率", 30, 130, INFO_RED, true);
		infoselect.push_back(_ifs[10]);

		if(!tinsert)
			InfoSelect::nselect = 0;
		else
		{
			_ifs[11].valueSet(1, "确定", 300, 380, INFO_GREEN);
			infoselect.push_back(_ifs[11]);
			InfoSelect::nselect = 2;
		}

		InfoSelect::updown = true;
		InfoSelect::select = 0;

		tdepth = 8;
	}
	else if(tdepth == 8)
	{
		if(!tinsert)
		{
			tsec = 1;
			goto skip2;
		}

		for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
		{
			if(i->ID == 0)
			{
				InfoSelect _ifs;
				strcpy(_ifs.info, Fontsys::text[i->ID]);
				for(int j=0;j<RPYINFO_USERNAMEMAX-1;j++)
				{
					if(username[j])
						_ifs.info[j+4] = username[j];
					else
					{
						for(int k=j;k<RPYINFO_USERNAMEMAX-1;k++)
							_ifs.info[k+4] = ' ';
						break;
					}
				}
				_ifs.info[11] = ' ';
				_ifs.valueSet(i->ID, _ifs.info, i->x, i->y, i->coltype, i->nonactive, i->gray);
				infoselect.push_back(_ifs);
				i = infoselect.erase(i);
				break;
			}
		}

		if(hge->Input_GetDIKey(KS_UP, DIKEY_DOWN) || hge->Input_GetDIKey(KS_DOWN, DIKEY_DOWN))
		{
			tnowchar = 0;
		}
		tsec = InfoSelect::select;
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

		if(tsec == 1)
		{
			if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_CANCEL);
				tsec = 0;
				InfoSelect::select = 0;
				goto skip3;
			}
			goto skip2;
		}

		if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_SELECT);
			if(tnowchar)
				tnowchar--;
		}
		else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_SELECT);
			if(tnowchar <= RPYINFO_USERNAMEMAX-1)
				tnowchar++;
		}
		if(hge->Input_GetDIKey(KS_ENTER, DIKEY_DOWN) || hge->Input_GetDIJoy(joyFire, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_OK);
			tsec++;
			InfoSelect::select++;
			InfoSelect::avoid = true;
			goto skip3;
		}
		if(hge->Input_GetDIKey(DIK_BACKSPACE, DIKEY_DOWN))
		{
			if(tnowchar)
			{
				SE::push(SE_SYSTEM_CANCEL);
				tnowchar--;
				username[tnowchar] = ' ';
			}
		}
		if(hge->Input_GetDIKey(DIK_DELETE, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_CANCEL);
			username[tnowchar] = ' ';
		}

		char nowchar = getInputNowChar(!tsec);
		if(nowchar > 0)
		{
			SE::push(SE_SYSTEM_OK);
			username[tnowchar] = nowchar;
			if(tnowchar <= RPYINFO_USERNAMEMAX-1)
				tnowchar++;
		}
		for(int i=RPYINFO_USERNAMEMAX-2;i>=0;i--)
		{
			if(username[i] == ' ')
				username[i] = 0;
			else
				break;
		}
skip2:
		if((tsec == 1 || !tinsert) && hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN) ||
			tsec == 0 && hge->Input_GetDIJoy(joyFire, DIKEY_DOWN))
		{
			DWORD sec;
			sec = data.sLinkType(DATAS_TOP);
			sec = data.sLinkDiff(sec, nowdifflv);
			sec = data.sLinkChara(sec, luchara);
			sec = data.sLinkNum(sec, tinsert);

			data.sWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_USERNAME), username);

			SE::push(SE_SYSTEM_OK);
			time = 0;
			state = STATE_TITLE;
			return PTURN;
		}
		if(!tsec)
			InfoSelect::avoid = true;
	}
skip3:
	CINT(scr.d[SCR_RESERVEBEGIN].value) = tpushtimer;
	CINT(scr.d[SCR_RESERVEBEGIN+1].value) = tdepth;
	CINT(scr.d[SCR_RESERVEBEGIN+2].value) = tsec;
	CINT(scr.d[SCR_RESERVEBEGIN+3].value) = tnowchar;
	return retvalue;
}