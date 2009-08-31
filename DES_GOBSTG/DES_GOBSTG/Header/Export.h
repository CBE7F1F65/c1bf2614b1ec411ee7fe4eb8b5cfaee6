#ifndef _EXPORT_H
#define _EXPORT_H

#include "MainDependency.h"
#include "Const.h"

#define	RPYMODE_RANGE		1
#define	RPYMODE_PRACTICE	2

#define RPYPARTMAX		8
#define RPYPREFIXMAX	8
#define RPYENUMMAX		0x40

#define RPYFILENAME_CONTENTMAX	7

#define RPYINFO_USERNAMEMAX		0x08
#define RPYINFO_RANGENAMEMAX	0x40

#define RPYOFFSET_SIGNATURE		0x00
#define RPYSIZE_SIGNATURE		0x0C
#define RPYOFFSET_VERSION		(RPYOFFSET_SIGNATURE + RPYSIZE_SIGNATURE)
#define RPYSIZE_VERSION			0x04
#define RPYOFFSET_COMPLETESIGN	(RPYOFFSET_VERSION + RPYSIZE_VERSION)
#define RPYSIZE_COMPLETESIGN	0x04
#define RPYOFFSET_TAG			(RPYOFFSET_COMPLETESIGN + RPYSIZE_COMPLETESIGN)
#define RPYSIZE_TAG				0x04
#define RPYOFFSET_INFOOFFSET	(RPYOFFSET_TAG + RPYSIZE_TAG)
#define RPYSIZE_INFOOFFSET		0x04
#define RPYOFFSET_APPEND		(RPYOFFSET_INFOOFFSET + RPYSIZE_INFOOFFSET)
#define RPYSIZE_APPEND			0x04
#define RPYOFFSET_RPYINFO		(RPYOFFSET_APPEND + RPYOFFSET_APPEND)
#define RPYSIZE_RPYINFO			sizeof(replayInfo)
#define RPYOFFSET_PARTINFO		(RPYOFFSET_RPYINFO + RPYSIZE_RPYINFO)
#define RPYSIZE_PARTINFO		sizeof(partInfo)
#define RPYOFFSET_INPUTDATA		(RPYOFFSET_PARTINFO + (RPYPARTMAX * RPYSIZE_PARTINFO))
#define RPYSIZE_FRAME			sizeof(replayFrame)

#define REPLAYPASSWORD_XORMAGICNUM	0x45A61920

#define RPYSTR_FILENAME		"抄录名"
#define RPYSTR_USERNAME		"名号"
#define RPYSTR_SCORE		"得点"
#define RPYSTR_POINT		"点道具"
#define RPYSTR_ALIVENESS	"生符数"
#define RPYSTR_ALLTIME		"游戏时长"
#define RPYSTR_DATE			"时刻"
#define RPYSTR_LOST			"处理落率"
#define RPYSTR_CIRCLERATE	"光环率"
#define RPYSTR_FASTRATE		"物理率"
#define RPYSTR_MODEFLAG		"模式"
#define RPYSTR_MODE_NONE		"正常"
#define RPYSTR_MODE_PRACTICE	"关卡练习"
#define RPYSTR_MODE_RANGE		"领域界练习"
#define RPYSTR_USINGCHARA	"主役"
#define RPYSTR_MAXPLAYER	"初期自机数"
#define RPYSTR_MISS			"失误回数"
#define RPYSTR_SHORT_BOMB	"领域释放"
#define RPYSTR_BOMB			"领域界释放回数"
#define RPYSTR_CONTINUE		"续关回数"
#define RPYSTR_SHORT_GET	"领域取得"
#define RPYSTR_GET			"领域界取得回数"
#define RPYSTR_PAUSE		"暂停回数"
#define RPYSTR_DIFFLV		"难易度"
#define RPYSTR_LASTSTAGE	"最终面"
#define RPYSTR_GRAZE		"擦弹数"

struct replayInfo
{
	char username[RPYINFO_USERNAMEMAX];
	WORD getrange[M_GETRANGEMAX];

	LONGLONG score;	

	DWORD startscene;
	DWORD endscene;
	DWORD point;
	DWORD aliveness;
	DWORD alltime;

	WORD year;
	WORD month;
	WORD day;
	WORD hour;
	WORD minute;

	float lost;
	float circlerate;
	float fastrate;

	BYTE modeflag;

	BYTE usingchara;
	BYTE maxplayer;

	BYTE miss;
	BYTE bomb;
	BYTE cont;
	BYTE get;
	BYTE pause;

	BYTE difflv;
	BYTE laststage;
};

struct partInfo
{
	LONGLONG nowscore;
	DWORD scene;
	DWORD offset;
	DWORD seed;
	DWORD nowpoint;
	DWORD nowaliveness;
	DWORD nowgraze;
	BYTE nowplayer;
	BYTE nowbomb;
	BYTE nowpower;
};

struct replayFrame{
	BYTE input;
	BYTE bias;
};

class Export
{
public:
	Export();
	~Export();

	static void clientInitial(bool usesound = true);
	static void clientSetMatrix(float worldx = 0, float worldy = 0, float worldz = 0);
	static void clientAdjustWindow();
	static void Release();
	static void SetIni();
	static bool GetResourceFile(bool readbin = false);
	static int GetPassword();
	static bool rpyLoad(const char * filename, replayInfo * _rpyinfo = NULL, partInfo * _partinfo = NULL, replayFrame * _replayframe = NULL);
	static bool rpySetBias(replayFrame * _replayframe);
	static float rpyGetReplayFPS(replayFrame _replayframe);
	static bool packFile(const char * zipname, const char * filename);
	static bool packFolder(const char * zipname, const char * foldername);
	static bool effSave(const char * filename, hgeEffectSystem * eff, int texnum);
	static int effLoad(const char * filename, hgeEffectSystem * eff, HTEXTURE * tex);

public:
	static char resourcefilename[M_PATHMAX];
	static char resbinname[M_PATHMAX];
	static partInfo partinfo[RPYPARTMAX];
	static replayInfo rpyinfo;
	static int password;
};

#ifndef _MAIN_H
extern HGE * hge;
#endif

#endif