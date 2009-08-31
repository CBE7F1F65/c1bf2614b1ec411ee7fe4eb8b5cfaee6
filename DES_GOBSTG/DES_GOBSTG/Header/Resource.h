#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "MainDependency.h"
#include "Const.h"

struct musicData{
	char musicname[M_STRMAX];
	char musicfilename[M_STRMAX];
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
	char texfilename[TEXMAX][M_STRMAX];
	char sefilename[SEMAX][M_STRMAX];
	char effectsysfilename[EFFECTSYSTYPEMAX][M_STRMAX];
	char packagefilename[PACKAGEMAX][M_STRMAX];

	//font
	char normalfontfilename[M_STRMAX];
	char widefontname[M_STRMAX];

	//datafile

	char binfilename[M_STRMAX];
	char binname[M_STRMAX];
	char rangeaccessfilename[M_STRMAX];
	char rabinname[M_STRMAX];
	char scriptfilename[M_STRMAX];
	char scrbinname[M_STRMAX];

	char customconstfilename[M_STRMAX];
	char rangedefinefilename[M_STRMAX];
	char musicdefinefilename[M_STRMAX];

	//folder
	char scriptfoldername[M_STRMAX];
	char snapshotfoldername[M_STRMAX];
	char replayfoldername[M_STRMAX];
	char datafoldername[M_STRMAX];
	char effectsysfoldername[M_STRMAX];

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