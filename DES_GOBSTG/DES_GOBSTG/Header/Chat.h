#ifndef _CHAT_H
#define _CHAT_H

#include "MainDependency.h"
#include "Const.h"
#include "BGLayer.h"

#define CHATTEXMAX	4
#define CHATTERMAX	5

#define CHATTER_LEFT		0
#define CHATTER_RIGHT		1
#define CHATTER_TEXT		2
#define CHATTER_LEFTNAME	3
#define CHATTER_RIGHTNAME	4

#define CHAT_TEXLEFT		0
#define CHAT_TEXRIGHT		1
#define CHAT_TEXTEXT		2
#define CHAT_TEXTEXNAME		3

#define CHATCHAR_EN_CHIWEN		0
#define CHATCHAR_EN_FUXI		1
#define CHATCHAR_EN_BIAN		2
#define CHATCHAR_EN_BAXIA		3
#define CHATCHAR_EN_PULAO		4
#define CHATCHAR_EN_CHAOFENG	5
#define CHATCHAR_EN_YAZI		6
#define CHATCHAR_EN_QIUNIU		7
#define CHATCHAR_EN_LINGYE		8
#define CHATCHAR_EN_SUANNI		9
#define CHATCHAR_EN_MEIMEI		10
#define CHATCHAR_EN_MING		11
#define CHATCHAR_EN_YOUJI		12
#define CHATCHAR_EN_KEER		13
#define CHATCHAR_EN_THOR		14
#define CHATCHAR_EN_NANAYA		15

#define CHATTEXT_BORDER		20

class Chat
{
public:
	Chat();
	~Chat();

	static void Init(HTEXTURE texl, HTEXTURE texr, HTEXTURE textext, HTEXTURE texname);

	void match(BYTE index, BYTE ID);

	bool chatOn(BYTE left, BYTE right, bool lbegin);
	bool chatOff();

	bool chat(BYTE ID, bool left, const char * text);

public:

	char text[M_STRMAX];
	static HTEXTURE tex[CHATTEXMAX];

	DWORD col;

	BYTE pushtimer;

	static bool chatinit;
	static bool chatting;
	static BYTE timer;
	static BYTE chati;
};

extern Chat chat;
extern BGLayer chatter[CHATTERMAX];

#endif