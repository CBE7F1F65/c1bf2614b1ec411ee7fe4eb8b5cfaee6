#include "DataConnector.h"
#include "Process.h"
#include "Player.h"
#include "Data.h"
#include "Replay.h"

int DataConnector::Insert()
{
	if(mp.replaymode)
		return -1;

	DWORD sec;
	if(mp.rangemode)
	{
		sec = data.sLinkType(DATAS_RANGE);
		sec = data.sLinkChara(sec, rpy.rpyinfo.usingchara);
		sec = data.sLinkNum(sec, rpy.rpyinfo.startscene);

		if(Player::p.nScore > data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPBONUS), 0))
		{
			data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPBONUS), Player::p.nScore);
		}
		return -1;
	}

	if(mp.practicemode)
	{
		sec = data.sLinkType(DATAS_STAGEPRACTICE);
		sec = data.sLinkDiff(sec, rpy.rpyinfo.difflv);
		sec = data.sLinkChara(sec, rpy.rpyinfo.usingchara);
		sec = data.sLinkNum(sec, rpy.rpyinfo.startscene / M_STAGENSCENE);
		if(rpy.rpyinfo.score > data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPSCORE), 0))
		{
			data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPSCORE), rpy.rpyinfo.score);
		}
		return -1;
	}

	sec = data.sLinkType(DATAS_TOP);
	sec = data.sLinkDiff(sec, rpy.rpyinfo.difflv);
	sec = data.sLinkChara(sec, rpy.rpyinfo.usingchara);

	for(int i=DATA_NTOP10FILESAVE;i>=0;i--)
	{
		DWORD secbuf = sec;
		secbuf = data.sLinkNum(secbuf, i);
		if(i == 0 || data.lRead(DATA_BINFILE, secbuf, data.nLinkType(DATAN_SCORE), 0) > rpy.rpyinfo.score)
		{
			if(i == DATA_NTOP10FILESAVE)
				return 0;

			for(int j=DATA_NTOP10FILESAVE-1;j>i;j--)
			{
				data.MoveDown(sec, j);
			}
			//
			sec = data.sLinkNum(sec, i+1);

			data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_SCORE), rpy.rpyinfo.score);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_LASTSTAGE), rpy.rpyinfo.laststage);
			data.sWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_USERNAME), rpy.rpyinfo.username);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_YEAR), rpy.rpyinfo.year);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MONTH), rpy.rpyinfo.month);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_DAY), rpy.rpyinfo.day);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_HOUR), rpy.rpyinfo.hour);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MINUTE), rpy.rpyinfo.minute);
			data.fWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_LOST), rpy.rpyinfo.lost);
			data.fWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_CIRCLERATE), rpy.rpyinfo.circlerate);
			data.fWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_FASTRATE), rpy.rpyinfo.fastrate);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_ALLTIME), rpy.rpyinfo.alltime);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_RANGEGET), rpy.rpyinfo.get);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MAXPLAYER), rpy.rpyinfo.maxplayer);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_POINT), rpy.rpyinfo.point);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_ALIVENESS), rpy.rpyinfo.aliveness);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MISS), rpy.rpyinfo.miss);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_BOMB), rpy.rpyinfo.bomb);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_CONTINUE), rpy.rpyinfo.cont);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_PAUSE), rpy.rpyinfo.pause);
			DWORD name;
			name = data.nLinkType(DATAN_GETRANGE);
			for (int j=0; j<M_GETRANGEMAX; j++)
			{
				data.iWrite(DATA_BINFILE, sec, data.nLinkNum(name, j+1), rpy.rpyinfo.getrange[j]);
			}

			return i+1;
		}
	}
	return 0;
}


//////////////////////////////
//replaymode, p.ID, scene, rangemode
void DataConnector::Meet()
{
	if(mp.replaymode)
		return;

	DWORD sec;
	int t;
	sec = data.sLinkType(DATAS_RANGE);
	sec = data.sLinkChara(sec, Player::p.ID>>1);
	sec = data.sLinkNum(sec, mp.scene);
	if(mp.rangemode)
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETPRACTICE), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETPRACTICE), t+1);

		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << (Player::p.ID>>1) << 8;
		data.raSetIndi(mp.scene, indi);
	}
	else
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETGAME), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETGAME), t+1);

		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << (Player::p.ID>>1);
		data.raSetIndi(mp.scene, indi);
	}
}

///////////////////////////
//replaymode, p.ID, scene, rangemode
void DataConnector::Get()
{
	if(mp.replaymode)
		return;

	DWORD sec;
	int t;
	sec = data.sLinkType(DATAS_RANGE);
	sec = data.sLinkChara(sec, Player::p.ID>>1);
	sec = data.sLinkNum(sec, mp.scene);
	if(mp.rangemode)
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_GETPRACTICE), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_GETPRACTICE), t+1);
	
		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << (Player::p.ID>>1) << 12;
		data.raSetIndi(mp.scene, indi);
	}
	else
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_GETGAME), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_GETGAME), t+1);
		
		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << (Player::p.ID>>1) << 4;
		data.raSetIndi(mp.scene, indi);
	}
}

//luchara, scene, rangemode
int DataConnector::nMeet()
{
	return data.nMeet(mp.scene, mp.rangemode, mp.luchara);
}

//luchara, scene, rangemode
int DataConnector::nGet()
{
	return data.nGet(mp.scene, mp.rangemode, mp.luchara);
}

//replaymode, practicemode, nowdifflv, luchara
void DataConnector::Clear()
{
	if(mp.replaymode || mp.practicemode)
		return;

	DWORD sec;
	sec = data.sLinkType(DATAS_TOTAL);
	sec = data.sLinkDiff(sec, mp.nowdifflv);
	sec = data.sLinkChara(sec, mp.luchara);

	data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_CLEARTIME), data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_CLEARTIME), 0)+1);
}

//playtimeStart
void DataConnector::addPlayTime()
{
	DWORD sec;

	SYSTEMTIME tsystime;
	FILETIME tfiletime;
	GetLocalTime(&tsystime);
	SystemTimeToFileTime(&tsystime, &tfiletime);
	LONGLONG playtimeEnd = (((LONGLONG)tfiletime.dwHighDateTime) << 32) | tfiletime.dwLowDateTime;

	sec = data.sLinkType(DATAS_TOTAL);
	if(playtimeEnd > mp.playtimeStart)
		data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TOTALPLAYTIME), data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TOTALPLAYTIME), 0) + (playtimeEnd - mp.playtimeStart));

	mp.playtimeStart = 0;
}

//rangemode, luchara, scene, nowdifflv, practicemode
LONGLONG DataConnector::nHiScore()
{
	return data.nHighScore(mp.scene, mp.nowdifflv, mp.rangemode, mp.practicemode, mp.luchara);
}

//nowdifflv, luchara, scene
int DataConnector::nTryStageTime()
{
	return data.nTryStageTime(mp.scene/M_STAGENSCENE, mp.nowdifflv, mp.luchara);
}

//rangemode, replaymode, practicemode, nowdifflv, luchara, scene
void DataConnector::Try(bool first)
{
	if(mp.rangemode || mp.replaymode)
		return;
	
	DWORD sec;
	if(mp.practicemode || !nTryStageTime())
	{
		sec = data.sLinkType(DATAS_STAGEPRACTICE);
		sec = data.sLinkDiff(sec, mp.nowdifflv);
		sec = data.sLinkChara(sec, mp.luchara);
		sec = data.sLinkNum(sec, mp.scene/M_STAGENSCENE);

		if(mp.practicemode)
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TRYTIME), data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TRYTIME), 0)+1);
		else
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TRYTIME), 1);
	}

	sec = data.sLinkType(DATAS_TOTAL);
	sec = data.sLinkDiff(sec, mp.nowdifflv);
	sec = data.sLinkChara(sec, mp.luchara);

	if(mp.practicemode)
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_PRACTICETIME), data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_PRACTICETIME), 0)+1);
	else if (first)
	{
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_PLAYTIME), data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_PLAYTIME), 0)+1);
	}
}