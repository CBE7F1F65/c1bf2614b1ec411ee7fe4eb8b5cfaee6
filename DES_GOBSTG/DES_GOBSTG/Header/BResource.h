#ifndef _BRESOURCE_H
#define _BRESOURCE_H

#include "MainDependency.h"
#include "Const.h"

struct musicData{
	char musicname[M_STRMAX];
	char musicfilename[M_PATHMAX];
	char explain_1[M_STRMAX];
	char explain_2[M_STRMAX];
	char explain_3[M_STRMAX];
	char explain_4[M_STRMAX];
	LONGLONG startpos;
	LONGLONG endpos;
	LONGLONG looppos;
};

struct rangeData{
	char rangename[M_STRMAX];
	char enemyname[M_STRMAX];
	char enemypinyin[M_STRMAX];
	char explain_1[M_STRMAX];
	char explain_2[M_STRMAX];
	LONGLONG bonus;
	int rangenumber;
	int sno;
	int timelimit;
	int turntoscene;
	bool isrange;
	bool iswait;
	BYTE autorank;
	BYTE remain;
};

struct resourceData
{
	//resource file
	char texfilename[TEXMAX][M_PATHMAX];
	char sefilename[SEMAX][M_PATHMAX];
	char effectsysfilename[EFFECTSYSTYPEMAX][M_PATHMAX];
	char packagefilename[PACKAGEMAX][M_PATHMAX];

	//font
	char normalfontfilename[M_PATHMAX];
	char widefontname[M_STRMAX];

	//datafile

	char binfilename[M_PATHMAX];
	char binname[M_PATHMAX];
	char rangeaccessfilename[M_PATHMAX];
	char rabinname[M_STRMAX];
	char scriptfilename[M_PATHMAX];
	char scrbinname[M_PATHMAX];

	char customconstfilename[M_PATHMAX];
	char rangedefinefilename[M_PATHMAX];
	char musicdefinefilename[M_PATHMAX];

	//folder
	char scriptfoldername[M_PATHMAX];
	char snapshotfoldername[M_PATHMAX];
	char replayfoldername[M_PATHMAX];
	char datafoldername[M_PATHMAX];
	char effectsysfoldername[M_PATHMAX];

	//extension
	char scriptextensionname7[8];
	char replayextensionname7[8];

	//replay header
	char replaysignature11[12];
	char replaytempsign3[4];
	char replaycompletesign3[4];
	char replaytag3[4];

	bool binmode;
};

class Resource
{
public:
	Resource();
	~Resource();

	bool Fill();
	bool Pack(void * pStrdesc, void * pCustomConstName);
	bool Gain(void * pStrdesc = NULL, void * pCustomConstName = NULL);
	void CopyData();
	bool SetDataFile();
	bool LoadPackage();
public:
	musicData musdata[MUSICMAX];
	resourceData resdata;
	vector<rangeData> rdata;
};

extern Resource res;
extern HGE * hge;

#endif