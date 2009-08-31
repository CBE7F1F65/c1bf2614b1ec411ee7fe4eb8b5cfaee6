#ifndef _DATA_H
#define _DATA_H

#include "MainDependency.h"
#include "Const.h"

#define DATASTR_TOTAL_RUNTIME			"总启动时间"
#define DATASTR_TOTAL_GAMETIME			"总游戏时间"
#define DATASTR_TOTAL_PLAYTIME			"总游戏次数"
#define DATASTR_TOTAL_PLAYTIME_ALL		"总计"
#define DATASTR_TOTAL_SPACED_ALLCHARA		"全人物合计　　"
#define DATASTR_TOTAL_SPACED_CLEARTIME		"通关次数　　　"
#define DATASTR_TOTAL_SPACED_PRACTICETIME	"练习次数　　　"

#define DATA_BINHEADER			0x10
#define DATA_BINFILE			0x11

#define DATA_MEMORYHEADER		0x20
#define DATA_RANGEACCESSFILE	0x21
#define DATA_SCRIPTFILE			0x22

#define DATA_TEXTHEADER			0x40
#define DATA_CUSTOMCONSTFILE	0x41
#define DATA_RANGEDEFINEFILE	0x42
#define DATA_MUSICDEFINEFILE	0x44

#define DATA_BOTHHEADER			0x80
#define DATA_RESOURCEFILE		0x81

#define DATA_HEADEROFFSET		0x40

#define DATA_NTOP10FILESAVE		10

#define DATABIN_TYPE_INT		0x01
#define DATABIN_TYPE_FLOAT		0x02
#define DATABIN_TYPE_LONGLONG	0x04
#define DATABIN_TYPE_STRING		0x08

#define DATARA_INDI_SIZE		0x02
#define DATARA_INDIMAX			(S1-S0)

/*
Section Align
Type[2]_Diffi[1]_Chara[1]_Num[4]
*/

#define DATASBINMASK_TYPE	0x00ffffff
#define DATASBINMASK_DIFF	0xff0fffff
#define DATASBINMASK_CHARA	0xfff0ffff
#define DATASBINMASK_NUM	0xffff0000

#define DATANBINMASK_TYPE	0x0000ffff
#define DATANBINMASK_NUM	0xffff0000

enum{
	//FFFF GP MP GG MG for ABCD
	DATAMAPENUM_DATASTART,

	DATATDIFF_E,
	DATATDIFF_N,
	DATATDIFF_H,
	DATATDIFF_X,
	DATATDIFF_P,
	DATATDIFF_D,

	DATATCHARA_A,
	DATATCHARA_B,
	DATATCHARA_C,
	DATATCHARA_D,

	DATAS_HEADER,
	DATAN_GAMEVERSION,
	DATAN_SIGNATURE,
	DATAN_FILETYPE,

	DATAS_TOP,
	//Top_E_A_1

	DATAN_SCORE,
	DATAN_LASTSTAGE,
	DATAN_USERNAME,
	DATAN_TIME_YEAR,
	DATAN_TIME_MONTH,
	DATAN_TIME_DAY,
	DATAN_TIME_HOUR,
	DATAN_TIME_MINUTE,
	DATAN_LOST,
	DATAN_CIRCLERATE,
	DATAN_FASTRATE,
	DATAN_ALLTIME,
	DATAN_RANGEGET,
	DATAN_MAXPLAYER,
	DATAN_POINT,
	DATAN_ALIVENESS,
	DATAN_MISS,
	DATAN_BOMB,
	DATAN_CONTINUE,
	DATAN_PAUSE,
	DATAN_GETRANGE,

	DATAS_RANGE,
	//Range_A_100

	DATAN_GETGAME,
	DATAN_MEETGAME,
	DATAN_GETPRACTICE,
	DATAN_MEETPRACTICE,
	DATAN_TOPBONUS,

	DATAS_STAGEPRACTICE,
	//StagePractice_E_A_1

	DATAN_TRYTIME,
	DATAN_TOPSCORE,

	DATAS_TOTAL,
	//Total_E_A

	DATAN_PLAYTIME,
	DATAN_CLEARTIME,
	DATAN_PRACTICETIME,

	//Total

	DATAN_FIRSTRUNTIME,
	DATAN_TOTALPLAYTIME,

	//Range Define
	//Range_100

	DATAN_ISRANGE,
	DATAN_RANGENUMBER,
	DATAN_RANGENAME,
	DATAN_TIMELIMIT,
	DATAN_REMAIN,
	DATAN_ENEMYNAME,
	DATAN_ENEMYPINYIN,
	DATAN_BONUS,
	DATAN_TURNTOSCENE,
	DATAN_ISWAIT,
	DATAN_AUTORANK,

	DATAN_EXPLAIN,

	//Music Define
	//Music_1
	DATAS_MUSIC,

	DATAN_MUSICNAME,
	DATAN_MUSICFILENAME,
	DATAN_STARTPOS,
	DATAN_ENDPOS,
	DATAN_LOOPPOS,

	//
	DATAS_CUSTOMCONST,

	//
	DATAS_STRINGDESC,

	DATAMAPENUM_RESOURCESTART,
	////////////RESOURCE/////////////

	RESDATAN_FILENAME,
	RESDATAN_NAME,
	RESDATAN_TYPE,

	RESDATAS_FONT,
	RESDATAN_FONT,

	RESDATAS_DATAFILE,

	RESDATAN_BINFILE,
	RESDATAN_RANGEACCESSFILE,
	RESDATAN_SCRIPTFILE,

	RESDATAN_STRINGDESCFILE,
	RESDATAN_CUSTOMCONSTFILE,
	RESDATAN_RANGEDEFINEFILE,
	RESDATAN_MUSICDEFINEFILE,

	RESDATAS_PACKAGE,
	RESDATAS_TEXTURE,
	RESDATAS_SE,
	RESDATAS_GHOST,
	RESDATAS_EFFECTSYS,

	RESDATAS_FOLDER,

	RESDATAN_SCRIPTFOLDER,
	RESDATAN_SNAPSHOTFOLDER,
	RESDATAN_REPLAYFOLDER,
	RESDATAN_DATAFOLDER,
	RESDATAN_EFFECTSYSFOLDER,

	RESDATAS_EXTENSION,
	RESDATAN_SCRIPTEXT7,
	RESDATAN_REPLAYEXT7,

	RESDATAS_REPLAYHEADER,
	RESDATAN_RPYSIGNATURE11,
	RESDATAN_RPYTEMPSIGN3,
	RESDATAN_RPYCOMPLETESIGN3,
	RESDATAN_RPYTAG3,

	DATAMAPENUM_END
};

struct dataMapTable{
	char * textname;
	DWORD binname;
};

struct dataName{
	LONGLONG value;
	DWORD name;
};

struct dataSection{
	vector<dataName> data;
	DWORD section;
};

struct dataBin{
	LONGLONG value;
	DWORD section;
	DWORD name;
//	BYTE type;
};

extern dataMapTable dataMap[];

class Data
{
public:
	Data();
	~Data();

	bool Init(BYTE type);
	void GetIni();
	bool SetFile(const char * filename, BYTE type);

	bool SaveBin();

	bool iWrite(BYTE type, DWORD section, DWORD name, int value);
	int iRead(BYTE type, DWORD section, DWORD name, int def_val);
	bool lWrite(BYTE type, DWORD section, DWORD name, LONGLONG value);
	LONGLONG lRead(BYTE type, DWORD section, DWORD name, LONGLONG def_val);
	bool fWrite(BYTE type, DWORD section, DWORD name, float value);
	float fRead(BYTE type, DWORD section, DWORD name, float def_val);
	bool sWrite(BYTE type, DWORD section, DWORD name, const char * value);
	char * sRead(BYTE type, DWORD section, DWORD name, const char * def_val);

	DWORD sLinkType(DWORD type);
	DWORD sLinkDiff(DWORD sec, BYTE diff);
	DWORD sLinkChara(DWORD sec, BYTE chara);
	DWORD sLinkNum(DWORD sec, DWORD num);

	DWORD nLinkType(DWORD type);
	DWORD nLinkNum(DWORD name, DWORD num);

	char * translateSection(DWORD sec);
	char * translateName(DWORD name);

	bool MemToList(BYTE * memcontent, DWORD size);
	BYTE * ListToMem(DWORD * size);

	LONGLONG * AttachValue(DWORD section, DWORD name, LONGLONG def_val = 0);

	void getFile(BYTE type);

	BYTE * CreateMemHeader(BYTE type);
	bool CheckMemHeader(const BYTE * memdata, DWORD size, BYTE type);
	bool CheckHeader(BYTE type);

	int getRangeNumber(int sno);
	char * getRangeName(int sno);
	char * getRangeUserName(int sno);
	char * getRangeExplain(int sno, BYTE expi);

	void MoveDown(DWORD sec, BYTE i);

	WORD raGetIndi(int sno);
	void raSetIndi(int sno, WORD indi);

	int nMeet(int sno, bool bRange = false, BYTE chara = 0xff);
	int nGet(int sno, bool bRange = false, BYTE chara = 0xff);
	LONGLONG nHighScore(int sno, BYTE difflv, bool bRange = false, bool bPractice = false, BYTE chara = 0xff);
	int nTryStageTime(int stage, BYTE difflv, BYTE chara = 0xff);

	BYTE getDiff(int sno);

	LONGLONG getTotalRunTime();
	//
	
	bool SetEffectSystemResourceName(int effi, const char * filename);
	bool GetEffectSystemResourceName(int effi, char * filename);


public:
	WORD indi[DATARA_INDIMAX];
	char buf[M_STRMAX];
	char transbufs[M_STRMAX];
	char transbufn[M_STRMAX];

	vector<dataSection> bin;

	bool binmode;

	int password;

	char * filename;
	char * binfilename;
	char * binname;
	char * rangeaccessfilename;
	char * rabinname;
	char * scriptfilename;
	char * scrbinname;
	char * resourcefilename;
	char * resbinname;

	char * customconstfilename;
	char * rangedefinefilename;
	char * musicdefinefilename;
};

extern Data data;
extern HGE * hge;

#endif