#include "DataPrinter.h"
#include "Data.h"

replayFrame DataPrinter::rpyframe[M_SAVEINPUTMAX];
partInfo DataPrinter::partinfo[RPYPARTMAX];
replayInfo DataPrinter::rpyinfo;
string DataPrinter::str;

DataPrinter::DataPrinter()
{
}

DataPrinter::~DataPrinter()
{
}

void DataPrinter::getHeader()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	char buffer[M_STRITOAMAX];

	str = DPS_SECTION_HEADER;
	str += DPS_COMMENT;
	str += GAME_TITLE;
	str += DPS_SPACE;
	str += GAME_VERSION_STR;
	str += DPS_SPACE;
	str += DPS_HEAD_TITLE;
	str += DPS_RETURN;

	str += DPS_COMMENT;
	str += DPS_RETURN;

	str += DPS_COMMENT;
	str += DPS_HEAD_EXPLAIN_1;
	str += DPS_RETURN;

	str += DPS_COMMENT;
	str += DPS_HEAD_EXPLAIN_2;
	str += DPS_RETURN;

	str += DPS_COMMENT;
	str += DPS_HEAD_EXPLAIN_3;
	str += DPS_RETURN;

	str += DPS_COMMENT;
	str += DPS_TABLE_1;
	str += DPS_HEAD_TIMESTAMPSIGN;
	str += DPS_LEFTANGLEBRACKET;

	sprintf(buffer, "%04d%s%02d%s%02d%s%02d%s%02d%s%02d",
		systime.wYear, DPS_SLASH,
		systime.wMonth, DPS_SLASH,
		systime.wDay, DPS_SPACE,
		systime.wHour, DPS_COLON,
		systime.wMinute, DPS_COLON,
		systime.wSecond);
	str += buffer;
/*
	str += itoa(systime.wYear, buffer, 10);
	str += DPS_SLASH;
	str += itoa(systime.wMonth, buffer, 10);
	str += DPS_SLASH;
	str += itoa(systime.wDay, buffer, 10);
	str += DPS_SPACE;
	str += itoa(systime.wHour, buffer, 10);
	str += DPS_COLON;
	str += itoa(systime.wMinute, buffer, 10);
	str += DPS_COLON;
	str += itoa(systime.wSecond, buffer, 10);
*/
	str += DPS_RIGHTANGLEBRACKET;
	str += DPS_RETURN;

	str += DPS_RETURN;

	str += DPS_RETURN;
}

bool DataPrinter::PrintScore()
{
	char filename[M_PATHMAX];
	strcpy(filename, DPS_SCOREFILENAME);
	strcat(filename, ".");
	strcat(filename, DPS_EXTENSION);
	HANDLE hFile = CreateFile(hge->Resource_MakePath(filename), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		HGELOG("Can't create Score File %s!", filename);
		return false;
	}

	DWORD bytestowrite = 0;
	DWORD byteswritten = 0;

	char buffer[M_STRMAX];

	//Header
	getHeader();

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	//Top10
	str = DPS_SECTION_TOP10;
	str += DPS_COMMENT;
	str += DPS_TITLE_TOP;
	str += DPS_RETURN;

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	DWORD sec = 0;
	DWORD name = 0;

	sec = data.sLinkType(DATAS_TOP);

	for (int i=0; i<M_NSTAGEDIFFI; i++)
	{
		str = DPS_RETURN;
		str += DPS_COMMENT;
		str += DPS_COMMENTDOUBLELINE;
		str += DPS_RETURN;
		str += RPYSTR_DIFFLV;
		str += DPS_SPACE;
		switch (i)
		{
		case M_DIFFI_EASY:
			str += M_DIFFISTR_EASY;
			break;
		case M_DIFFI_NORMAL:
			str += M_DIFFISTR_NORMAL;
			break;
		case M_DIFFI_HARD:
			str += M_DIFFISTR_HARD;
			break;
		case M_DIFFI_EXTRA:
			str += M_DIFFISTR_EXTRA;
			break;
		case M_DIFFI_PHANTASM:
			str += M_DIFFISTR_PHANTASM;
			break;
		}

		str += DPS_RETURN;

		bytestowrite = strlen(str.data());
		WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

		sec = data.sLinkDiff(sec, i);

		for (int j=0; j<M_NCHARA; j++)
		{
			str = DPS_COMMENT;
			str += DPS_COMMENTLINE;
			str += DPS_RETURN;
			str += RPYSTR_USINGCHARA;
			str += DPS_TABLE_1;
			switch (j)
			{
			case M_CHARA_SHI_LINGYE:
				str += M_CHARASTR_FULL_SPACED_SHI_LINGYE;
				break;
			case M_CHARA_NUSIKUIMA_GEJI:
				str += M_CHARASTR_FULL_SPACED_NUSIKUIMA_GEJI;
				break;
			case M_CHARA_CILAN_ZANGYUE:
				str += M_CHARASTR_FULL_SPACED_CILAN_ZANGYUE;
				break;
			case M_CHARA_WUSHUANG:
				str += M_CHARASTR_FULL_SPACED_WUSHUANG;
				break;
			}
			str += DPS_RETURN;

			bytestowrite = strlen(str.data());
			WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

			sec = data.sLinkChara(sec, j);

			for (int k=0; k<DATA_NTOP10FILESAVE; k++)
			{
				sec = data.sLinkNum(sec, k+1);

				LONGLONG _score = data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_SCORE), 0);
				if (!_score)
				{
					continue;
				}
				str = DPS_TABLE_1;
				str += DPS_TAG_TOPNUM;
				str += itoa(k+1, buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_SCORE;
				str += DPS_TABLE_1;
				str += _i64toa(_score, buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_USERNAME;
				str += DPS_TABLE_1;
				str += data.sRead(DATA_BINFILE, sec, data.nLinkType(DATAN_USERNAME), RESCONFIGDEFAULT_USERNAME);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_LASTSTAGE;
				str += DPS_TABLE_1;
				switch(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_LASTSTAGE), 0))
				{
				case 1:
					str += M_LASTSTAGESTR_1;
					break;
				case 2:
					str += M_LASTSTAGESTR_2;
					break;
				case 3:
					str += M_LASTSTAGESTR_3A;
					break;
				case 4:
					str += M_LASTSTAGESTR_3B;
					break;
				case 5:
					str += M_LASTSTAGESTR_4;
					break;
				case 6:
					str += M_LASTSTAGESTR_5;
					break;
				case 7:
					str += M_LASTSTAGESTR_6A;
					break;
				case 8:
					str += M_LASTSTAGESTR_6B;
					break;
				default:
					str += M_LASTSTAGESTR_C;
					break;
				}
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_DATE;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_YEAR), 0), buffer, 10);
				str += DPS_SLASH;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MONTH), 0), buffer, 10);
				str += DPS_SLASH;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_DAY), 0), buffer, 10);
				str += DPS_SPACE;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_HOUR), 0), buffer, 10);
				str += DPS_COLON;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MINUTE), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_LOST;
				str += DPS_TABLE_1;
				sprintf(buffer, "%f", data.fRead(DATA_BINFILE, sec, data.nLinkType(DATAN_LOST), 0));
				str += buffer;
				str += DPS_PERCENT;
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_ALLTIME;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_ALLTIME), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_MAXPLAYER;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_MAXPLAYER), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_POINT;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_POINT), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_ALIVENESS;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_ALIVENESS), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_MISS;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_MISS), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_BOMB;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_BOMB), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_GET;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_RANGEGET), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_PAUSE;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_PAUSE), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_CONTINUE;
				str += DPS_TABLE_1;
				str += itoa(data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_CONTINUE), 0), buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_CIRCLERATE;
				str += DPS_TABLE_1;
				sprintf(buffer, "%f", data.fRead(DATA_BINFILE, sec, data.nLinkType(DATAN_CIRCLERATE), 0) * 100);
				str += buffer;
				str += DPS_PERCENT;
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += RPYSTR_FASTRATE;
				str += DPS_TABLE_1;
				sprintf(buffer, "%f", data.fRead(DATA_BINFILE, sec, data.nLinkType(DATAN_FASTRATE), 0) * 100);
				str += buffer;
				str += DPS_PERCENT;
				str += DPS_RETURN;

				str += DPS_TABLE_2;
				str += DPS_TOP_TAGSTR_RANGELIST;
				str += DPS_RETURN;

				bytestowrite = strlen(str.data());
				WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

				name = data.nLinkType(DATAN_GETRANGE);
				for (int l=0; l<M_GETRANGEMAX; l++)
				{
					name = data.nLinkNum(name, l+1);
					int _sno = data.iRead(DATA_BINFILE, sec, name, 0);
					if (!_sno)
					{
						continue;
					}

					str = DPS_TABLE_2;
					str += DPS_TAG_RANGEPRIFIX;
					str += itoa(_sno, buffer, 10);
					str += data.getRangeName(_sno);
					while (strlen(str.data()) < DPS_RANGENAME_NFORMAT)
					{
						str += DPS_SPACE;
					}
					str += DPS_LEFTBRACKET;
					str += itoa(data.nGet(_sno), buffer, 10);
					str += DPS_SLASH;
					str += itoa(data.nMeet(_sno), buffer, 10);
					str += DPS_RIGHTBRACKET;
					str += DPS_RETURN;

					bytestowrite = strlen(str.data());
					WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);
				}
			}
		}
	}

	str = DPS_RETURN;

	//Range
	str += DPS_SECTION_RANGE;
	str += DPS_COMMENT;
	str += DPS_COMMENTDOUBLELINE;
	str += DPS_RETURN;
	str += DPS_TITLE_RANGE;
	str += DPS_RETURN;

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	for (int i=S0; i<S1; i++)
	{
		if (data.getRangeNumber(i) && data.raGetIndi(i))
		{
			str = DPS_TAG_RANGEPRIFIX;
			str += itoa(i, buffer, 10);
			str += data.getRangeName(i);
			while (strlen(str.data()) < DPS_RANGENAME_NFORMAT)
			{
				str += DPS_SPACE;
			}
			str += itoa(data.nGet(i), buffer, 10);
			str += DPS_SLASH;
			str += DPS_TABLE_1;
			str += itoa(data.nMeet(i), buffer, 10);
			str += DPS_SLASH;
			str += DPS_TABLE_1;
			str += itoa(data.nGet(i, true), buffer, 10);
			str += DPS_SLASH;
			str += DPS_TABLE_1;
			str += itoa(data.nMeet(i, true), buffer, 10);
			str += DPS_TABLE_1;
			str += DPS_LEFTBRACKET;
			switch (data.getDiff(i))
			{
			case M_DIFFI_EASY:
				str += M_DIFFISTR_EASY;
				break;
			case M_DIFFI_NORMAL:
				str += M_DIFFISTR_NORMAL;
				break;
			case M_DIFFI_HARD:
				str += M_DIFFISTR_HARD;
				break;
			case M_DIFFI_EXTRA:
				str += M_DIFFISTR_EXTRA;
				break;
			case M_DIFFI_PHANTASM:
				str += M_DIFFISTR_PHANTASM;
				break;
			case M_DIFFI_DESTINY:
				str += M_DIFFISTR_DESTINY;
				break;
			}
			str += DPS_SPACE;
			str += _i64toa(data.nHighScore(i, 0, true), buffer, 10);
			str += DPS_RIGHTBRACKET;
			str += DPS_RETURN;

			bytestowrite = strlen(str.data());
			WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);
		}
	}

	str = DPS_RETURN;
	//total
	str += DPS_SECTION_TOTAL;
	str += DPS_COMMENT;
	str += DPS_COMMENTDOUBLELINE;
	str += DPS_RETURN;
	str += DPS_TITLE_TOTAL;
	str += DPS_RETURN;

	str += DATASTR_TOTAL_RUNTIME;
	str += DPS_TABLE_1;
	LONGLONG tldiffruntime = data.getTotalRunTime();
	int trunhour = tldiffruntime / 3600;
	int trunminute = (tldiffruntime / 60) % 60;
	int trunsecond = tldiffruntime % 60;
	str += itoa(trunhour, buffer, 10);
	str += DPS_COLON;
	str += itoa(trunminute, buffer, 10);
	str += DPS_COLON;
	str += itoa(trunsecond, buffer, 10);
	str += DPS_RETURN;

	str += DATASTR_TOTAL_GAMETIME;
	str += DPS_TABLE_1;
	LONGLONG tltotalplaytime = data.lRead(DATA_BINFILE, data.sLinkType(DATAS_TOTAL), data.nLinkType(DATAN_TOTALPLAYTIME), 0) / 10000000;
	int tplayhour = tltotalplaytime / 3600;
	int tplayminute = (tltotalplaytime / 60) % 60;
	int tplaysecond = tltotalplaytime % 60;
	str += itoa(tplayhour, buffer, 10);
	str += DPS_COLON;
	str += itoa(tplayminute, buffer, 10);
	str += DPS_COLON;
	str += itoa(tplaysecond, buffer, 10);
	str += DPS_RETURN;

	str += DATASTR_TOTAL_PLAYTIME;
	str += DPS_TABLE_2;
	str += M_DIFFISTR_EASY;
	str += DPS_TABLE_1;
	str += M_DIFFISTR_NORMAL;
	str += DPS_TABLE_1;
	str += M_DIFFISTR_HARD;
	str += DPS_TABLE_1;
	str += M_DIFFISTR_EXTRA;
	str += DPS_TABLE_1;
	str += M_DIFFISTR_PHANTASM;
	str += DPS_TABLE_1;
	str += DATASTR_TOTAL_PLAYTIME_ALL;
	str += DPS_RETURN;

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
	for(int i=0;i<M_NSTAGEDIFFI;i++)
	{
		tcleartime[M_NSTAGEDIFFI] += tcleartime[i];
		tpracticetime[M_NSTAGEDIFFI] += tpracticetime[i];
	}

	for (int i=0; i<M_NCHARA+1; i++)
	{
		switch (i)
		{
		case M_CHARA_SHI_LINGYE:
			str += M_CHARASTR_FULL_SPACED_SHI_LINGYE;
			break;
		case M_CHARA_NUSIKUIMA_GEJI:
			str += M_CHARASTR_FULL_SPACED_NUSIKUIMA_GEJI;
			break;
		case M_CHARA_CILAN_ZANGYUE:
			str += M_CHARASTR_FULL_SPACED_CILAN_ZANGYUE;
			break;
		case M_CHARA_WUSHUANG:
			str += M_CHARASTR_FULL_SPACED_WUSHUANG;
			break;
		default:
			str += DATASTR_TOTAL_SPACED_ALLCHARA;
			break;
		}
		for (int j=0; j<M_NSTAGEDIFFI+1; j++)
		{
			str += DPS_TABLE_1;
			str += itoa(tplaytime[i][j], buffer, 10);
		}
		str += DPS_RETURN;
	}
	
	str += DATASTR_TOTAL_SPACED_CLEARTIME;
	for (int i=0; i<M_NSTAGEDIFFI+1; i++)
	{
		str += DPS_TABLE_1;
		str += itoa(tcleartime[i], buffer, 10);
	}
	str += DPS_RETURN;

	str += DATASTR_TOTAL_SPACED_PRACTICETIME;
	for (int i=0; i<M_NSTAGEDIFFI+1; i++)
	{
		str += DPS_TABLE_1;
		str += itoa(tpracticetime[i], buffer, 10);
	}

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	CloseHandle(hFile);
	return true;
}

bool DataPrinter::PrintReplayData(const char * foldername, const char * filename)
{
	hge->Resource_SetPath(foldername);
	if (!filename || !Export::rpyLoad(filename, &rpyinfo, partinfo, rpyframe))
	{
		hge->Resource_SetPath("../");
		return false;
	}

	char ofilename[M_PATHMAX];
	strcpy(ofilename, filename);
	while (ofilename[strlen(ofilename)-1] != '.')
	{
		ofilename[strlen(ofilename)-1] = 0;
	}
	strcat(ofilename, DPS_EXTENSION);
	HANDLE hFile = CreateFile(hge->Resource_MakePath(ofilename), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		HGELOG("Can't create Replay Info File %s!", ofilename);
		return false;
	}

	DWORD bytestowrite = 0;
	DWORD byteswritten = 0;

	char buffer[M_STRMAX];

	//Header
	getHeader();

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	//replayinfo
	str = DPS_SECTION_REPLAYINFO;
	str += DPS_COMMENT;
	str += DPS_COMMENTDOUBLELINE;
	str += DPS_RETURN;
	str += DPS_TITLE_REPLAYINFO;
	str += DPS_RETURN;
	str += DPS_RETURN;

	str += RPYSTR_FILENAME;
	str += DPS_TABLE_1;
	str += filename;
	str += DPS_RETURN;

	str += RPYSTR_USERNAME;
	str += DPS_TABLE_1;
	str += rpyinfo.username;
	str += DPS_RETURN;

	str += RPYSTR_USINGCHARA;
	str += DPS_TABLE_1;
	switch (rpyinfo.usingchara)
	{
	case M_CHARA_SHI_LINGYE:
		str += M_CHARASTR_FULL_SPACED_SHI_LINGYE;
		break;
	case M_CHARA_NUSIKUIMA_GEJI:
		str += M_CHARASTR_FULL_SPACED_NUSIKUIMA_GEJI;
		break;
	case M_CHARA_CILAN_ZANGYUE:
		str += M_CHARASTR_FULL_SPACED_CILAN_ZANGYUE;
		break;
	case M_CHARA_WUSHUANG:
		str += M_CHARASTR_FULL_SPACED_WUSHUANG;
		break;
	}
	str += DPS_RETURN;

	str += RPYSTR_LASTSTAGE;
	str += DPS_TABLE_1;
	if (rpyinfo.modeflag & RPYMODE_RANGE)
	{
		str += DPS_TAG_RANGEPRIFIX;
		str += itoa(rpyinfo.startscene, buffer, 10);
		str += data.getRangeName(rpyinfo.startscene);
	}
	else
	{
		switch (rpyinfo.laststage)
		{
		case 1:
			str += M_LASTSTAGESTR_1;
			break;
		case 2:
			str += M_LASTSTAGESTR_2;
			break;
		case 3:
			str += M_LASTSTAGESTR_3A;
			break;
		case 4:
			str += M_LASTSTAGESTR_3B;
			break;
		case 5:
			str += M_LASTSTAGESTR_4;
			break;
		case 6:
			str += M_LASTSTAGESTR_5;
			break;
		case 7:
			str += M_LASTSTAGESTR_6A;
			break;
		case 8:
			str += M_LASTSTAGESTR_6B;
			break;
		}
	}
	str += DPS_RETURN;

	str += RPYSTR_SCORE;
	str += DPS_TABLE_1;
	str += _i64toa(rpyinfo.score, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_POINT;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.point, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_ALIVENESS;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.aliveness, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_ALLTIME;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.alltime, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_DATE;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.year, buffer, 10);
	str += DPS_SLASH;
	str += itoa(rpyinfo.month, buffer, 10);
	str += DPS_SLASH;
	str += itoa(rpyinfo.day, buffer, 10);
	str += DPS_SPACE;
	str += itoa(rpyinfo.hour, buffer, 10);
	str += DPS_COLON;
	str += itoa(rpyinfo.minute, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_LOST;
	str += DPS_TABLE_1;
	sprintf(buffer, "%f", rpyinfo.lost);
	str += buffer;
	str += DPS_PERCENT;
	str += DPS_RETURN;

	str += RPYSTR_CIRCLERATE;
	str += DPS_TABLE_1;
	sprintf(buffer, "%f", rpyinfo.circlerate * 100);
	str += buffer;
	str += DPS_PERCENT;
	str += DPS_RETURN;

	str += RPYSTR_FASTRATE;
	str += DPS_TABLE_1;
	sprintf(buffer, "%f", rpyinfo.fastrate * 100);
	str += buffer;
	str += DPS_PERCENT;
	str += DPS_RETURN;

	str += RPYSTR_MODEFLAG;
	str += DPS_TABLE_1;
	if (rpyinfo.modeflag & RPYMODE_RANGE)
	{
		str += RPYSTR_MODE_RANGE;
	}
	else if (rpyinfo.modeflag & RPYMODE_PRACTICE)
	{
		str += RPYSTR_MODE_PRACTICE;
	}
	else
	{
		str += RPYSTR_MODE_NONE;
	}
	str += DPS_RETURN;

	str += RPYSTR_MAXPLAYER;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.maxplayer, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_MISS;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.miss, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_SHORT_BOMB;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.bomb, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_SHORT_GET;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.get, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_CONTINUE;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.cont, buffer, 10);
	str += DPS_RETURN;

	str += RPYSTR_PAUSE;
	str += DPS_TABLE_1;
	str += itoa(rpyinfo.pause, buffer, 10);
	str += DPS_RETURN;
	str += DPS_RETURN;

	str += DPS_RPYINFO_TAGSTR_RANGELIST;
	str += DPS_RETURN;
	for (int i=0; i<M_GETRANGEMAX; i++)
	{
		if (rpyinfo.getrange[i])
		{
			str += DPS_TABLE_1;
			str += data.getRangeName(rpyinfo.getrange[i]);
			str += DPS_RETURN;
		}
	}
	str += DPS_RETURN;

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	if (!rpyinfo.modeflag)
	{
		//partinfo
		str = DPS_SECTION_PARTNFO;
		str += DPS_COMMENT;
		str += DPS_COMMENTDOUBLELINE;
		str += DPS_RETURN;
		str += DPS_TITLE_PARTINFO;
		str += DPS_RETURN;
		for (int i=0; i<RPYPARTMAX; i++)
		{
			int index = 0;
			if (partinfo[i].offset)
			{
				for (int j=i+1; j<RPYPARTMAX; j++)
				{
					if (partinfo[j].offset)
					{
						index = j;
						break;
					}
				}
				str += DPS_COMMENTLINE;
				str += DPS_RETURN;
				str += DPS_TAG_PART;
				str += DPS_TABLE_1;
				switch (i)
				{
				case 0:
					str += M_LASTSTAGESTR_1;
					break;
				case 1:
					str += M_LASTSTAGESTR_2;
					break;
				case 2:
					str += M_LASTSTAGESTR_3A;
					break;
				case 3:
					str += M_LASTSTAGESTR_3B;
					break;
				case 4:
					str += M_LASTSTAGESTR_4;
					break;
				case 5:
					str += M_LASTSTAGESTR_5;
					break;
				case 6:
					str += M_LASTSTAGESTR_6A;
					break;
				case 7:
					str += M_LASTSTAGESTR_6B;
					break;
				}
				str += DPS_RETURN;

				str += DPS_TABLE_1;
				str += RPYSTR_SCORE;
				str += DPS_TABLE_1;
				str += _i64toa(partinfo[index].nowscore, buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_1;
				str += RPYSTR_POINT;
				str += DPS_TABLE_1;
				str += itoa(partinfo[index].nowpoint, buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_1;
				str += RPYSTR_ALIVENESS;
				str += DPS_TABLE_1;
				str += itoa(partinfo[index].nowaliveness, buffer, 10);
				str += DPS_RETURN;

				str += DPS_TABLE_1;
				str += RPYSTR_GRAZE;
				str += DPS_TABLE_1;
				str += itoa(partinfo[index].nowgraze, buffer, 10);
				str += DPS_RETURN;
			}
		}
		str += DPS_RETURN;

		bytestowrite = strlen(str.data());
		WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);
	}

	//fps
	str = DPS_SECTION_REPLAYFRAME;
	str += DPS_COMMENT;
	str += DPS_COMMENTDOUBLELINE;
	str += DPS_RETURN;
	str += DPS_TITLE_REPLAYFRAME;
	str += DPS_RETURN;

	int index=0;
	double sum = 0;
	double sumsub = 0;
	while (rpyframe[index].input != 0xff)
	{
		float tfps = Export::rpyGetReplayFPS(rpyframe[index]);
		sumsub += tfps;
		sum += tfps;
		index++;
		if (index % DPS_REPLAYFRAME_AVERAGE_INTERVAL == 0)
		{
			str += DPS_REPLAYFRAME_TAGSTR_TOTAL_AVERAGE;
			str += DPS_COLON;
			str += itoa(index / DPS_REPLAYFRAME_AVERAGE_INTERVAL, buffer, 10);
			str += DPS_TABLE_1;
			sumsub /= DPS_REPLAYFRAME_AVERAGE_INTERVAL;
			sprintf(buffer, "%f", (float)sumsub);
			str += buffer;
			str += DPS_RETURN;
			sumsub = 0;
		}
	}
	if (index)
	{
		sum /= index;
	}
	str += DPS_REPLAYFRAME_TAGSTR_AVERAGE;
	str += DPS_TABLE_1;
	sprintf(buffer, "%f", (float)sum);
	str += buffer;

	bytestowrite = strlen(str.data());
	WriteFile(hFile, str.data(), bytestowrite, &byteswritten, NULL);

	CloseHandle(hFile);

	hge->Resource_SetPath("../");
	return true;
}