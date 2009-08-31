#ifndef _INFOSELECT_H
#define _INFOSELECT_H

#include "MainDependency.h"
#include "Const.h"

//infoselect colortype
#define	INFO_GREEN		0
#define INFO_YELLOW		1
#define INFO_GRAY		2
#define INFO_RED		3
#define INFO_BLUE		4

class InfoSelect
{
public:
	InfoSelect();
	~InfoSelect();

	void valueSet(BYTE ID, const char * info, float x, float y, BYTE coltype, bool nonactive = false, bool gray = false);

	static void Build(BYTE ID, const char * info, float x, float y, BYTE coltype, bool nonactive, bool gray);

	void action();

	void linki(const char * linkcode, int num);
	void linkl(const char * linkcode, LONGLONG num);
	void linkf(const char * linkcode, BYTE tail, float num);

public:
	char info[M_STRMAX];
	float x;
	float y;
	bool nonactive;
	bool gray;
	DWORD ucol;
	DWORD dcol;

	BYTE coltype;
	BYTE ID;
	BYTE pushtimer;

	static int nselect;
	static int select;
	static bool updown;
	static bool complete;
	static bool plus;

	static bool avoid;
};

extern list<InfoSelect>infoselect;

#endif