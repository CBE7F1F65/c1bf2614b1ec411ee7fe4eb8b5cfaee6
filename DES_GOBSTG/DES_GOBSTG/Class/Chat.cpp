#include "Chat.h"
#include "BGLayer.h"
#include "Player.h"
#include "Bullet.h"
#include "Fontsys.h"
#include "Main.h"
#include "Process.h"

Chat chat;
HTEXTURE Chat::tex[CHATTEXMAX];
BGLayer chatter[CHATTERMAX];

bool Chat::chatting;
bool Chat::chatinit;
BYTE Chat::timer;
BYTE Chat::chati;

Chat::Chat()
{
	col = 0x00000000;
	chatting = false;
	chatinit = false;
	timer = 0;
	pushtimer = 0xff;
}

Chat::~Chat()
{
}

void Chat::Init(HTEXTURE texl, HTEXTURE texr, HTEXTURE textext, HTEXTURE texname)
{
//	Fontsys::Release();
	tex[CHAT_TEXLEFT] = texl;
	tex[CHAT_TEXRIGHT] = texr;
	tex[CHAT_TEXTEXT] = textext;
	tex[CHAT_TEXTEXNAME] = texname;
	chati = 0;
	timer = 0;
	chatinit = false;
	chatting = false;
}


bool Chat::chat(BYTE ID, bool left, const char * _text)
{
	timer++;
	if(!chatting)
	{
		timer = 0;
		return true;
	}
	Player::p.bInfi = true;

	if(hge->Input_GetDIKey(KS_FIRE_MP))
	{
		if(pushtimer < M_PUSH_FIRST)
			pushtimer++;
		else if(pushtimer == M_PUSH_FIRST)
			pushtimer = M_PUSH_FIRST - M_PUSH_SKIP;
	}
	else
	{
		pushtimer = 0;
	}

	if(hge->Input_GetDIKey(KS_FIRE_MP, DIKEY_DOWN) || pushtimer == M_PUSH_FIRST || timer == M_NOPUSH_SKIP)
	{
		chatinit = false;
		timer = 0;
		return true;
	}

	if(chatinit)
	{
		return false;
	}
	if(left)
	{
		chatter[CHATTER_RIGHT].colorSet(0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff);
		chatter[CHATTER_LEFT].valueSet(tex[CHAT_TEXLEFT], 90, 290, 360, 360, ID%4+1, ID/4+1, 4, 4);
		match(CHATTER_LEFTNAME, ID);
	}
	else
	{
		chatter[CHATTER_LEFT].colorSet(0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff);
		chatter[CHATTER_RIGHT].valueSet(tex[CHAT_TEXRIGHT], 350, 290, 360, 360, ID%4+1, ID/4+1, 4, 4);
		match(CHATTER_RIGHTNAME, ID);
	}
	int line = 0;
	for(WORD i=0;i<strlen(_text)+1;i++)
	{
		if(i!=0 && !(i%28))
		{
			text[i+line] = '\n';
			line++;
		}
		text[i+line] = _text[i];
	}
	Fontsys::SignUp(FONTSYS_CHATUSE, text);
	if(left)
	{
		switch(ID)
		{
		case M_CHARAENUM_SHI:
			col = 0xff999999;
			break;
		case M_CHARAENUM_LINGYE:
			col = 0xff33cccc;
			break;
		case M_CHARAENUM_NUSIKUIMA:
			col = 0xff663300;
			break;
		case M_CHARAENUM_GEJI:
			col = 0xffff3399;
			break;
		case M_CHARAENUM_CILAN:
			col = 0xff000099;
			break;
		case M_CHARAENUM_ZANGYUE:
			col = 0xff003366;
			break;
		case M_CHARAENUM_WUSHUANG:
		case M_CHARAENUM_BUSINIAO:
			col = 0xffff3300;
			break;
		default:
			col = 0x00000000;
		}
	}
	else
	{
		switch(ID)
		{
		case CHATCHAR_EN_CHIWEN:
			col = 0xffcccc33;
			break;
		case CHATCHAR_EN_FUXI:
			col = 0xffff9966;
			break;
		case CHATCHAR_EN_BIAN:
			col = 0xff660000;
			break;
		case CHATCHAR_EN_BAXIA:
			col = 0xffcc6633;
			break;
		case CHATCHAR_EN_PULAO:
			col = 0xff996633;
			break;
		case CHATCHAR_EN_CHAOFENG:
			col = 0xff9966cc;
			break;
		case CHATCHAR_EN_YAZI:
			col = 0xff996666;
			break;
		case CHATCHAR_EN_QIUNIU:
			col = 0xffff6633;
			break;
		case CHATCHAR_EN_LINGYE:
			col = 0xff33cccc;
			break;
		case CHATCHAR_EN_SUANNI:
			col = 0xff33ff33;
			break;
		case CHATCHAR_EN_MEIMEI:
			col = 0xff00ff33;
			break;
		case CHATCHAR_EN_MING:
			col = 0xff666666;
			break;
		case CHATCHAR_EN_YOUJI:
			col = 0xff669999;
			break;
		case CHATCHAR_EN_KEER:
			col = 0xff666666;
			break;
		case CHATCHAR_EN_THOR:
			col = 0xff33cc00;
			break;
		case CHATCHAR_EN_NANAYA:
			col = 0xff006666;
			break;
		default:
			col = 0x00000000;
		}
	}
	chatinit = true;
	return false;
}

void Chat::match(BYTE index, BYTE ID)
{
	if(index == CHATTER_LEFTNAME)
	{
		if(ID < M_CHARAENUM_NUSIKUIMA)
			chatter[CHATTER_LEFTNAME].valueSet(tex[CHAT_TEXTEXNAME], 90, 320, 64, 32, ID+1, 9, 16, 32);
		else if(ID == M_CHARAENUM_NUSIKUIMA)
			chatter[CHATTER_LEFTNAME].valueSet(tex[CHAT_TEXTEXNAME], 90, 320, 128, 32, 2, 9, 8, 32);
		else if(ID <= M_CHARAENUM_BUSINIAO)
			chatter[CHATTER_LEFTNAME].valueSet(tex[CHAT_TEXTEXNAME], 90, 320, 64, 32, ID+2, 9, 16, 32);
		else
			chatter[CHATTER_LEFTNAME].valueSet(tex[CHAT_TEXTEXNAME], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
	}
	else
	{
		if(ID < CHATCHAR_EN_LINGYE)
			chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 350, 320, 64, 32, ID+9, 9, 16, 32);
		else if(ID == CHATCHAR_EN_LINGYE)
			chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 350, 320, 64, 32, 2, 9, 16, 32);
		else if(ID < CHATCHAR_EN_KEER)
			chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 350, 320, 64, 32, ID+4, 8, 16, 32);
		else if(ID == CHATCHAR_EN_KEER)
			chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 350, 320, 128, 64, 4, 6, 8, 16);
		else if(ID <= CHATCHAR_EN_NANAYA)
			chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 350, 320, 128, 32, ID-7, 7, 8, 32);
		else
			chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
	}
}

bool Chat::chatOn(BYTE left, BYTE right, bool lbegin)
{
	if(Player::p.flag & PLAYER_BOMB || Player::p.flag & PLAYER_DEADBOMB)
		return false;
	timer++;

	Player::p.bInfi = true;
	chatinit = false;
	
	if(timer == 1)
	{
		pushtimer = 0xff;

		strcpy(text, "");
		Fontsys::SignUp(FONTSYS_CHATUSE, text);
		
		IzeZone _ize;
		_ize.x = 0;
		_ize.y = 0;
		_ize.r = BULLET_IZEOVERZONE;
		Bullet::izel[BULLETIZE_ITEM].push_front(_ize);

		chatting = true;
		chatter[CHATTER_LEFTNAME].valueSet(tex[CHAT_TEXTEXNAME], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
		chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
	}
	if(timer <= 36)
	{
		chatter[CHATTER_LEFT].valueSet(tex[CHAT_TEXLEFT], -270+timer*10, 290, 360, 360, left%4+1, left/4+1, 4, 4);
		chatter[CHATTER_RIGHT].valueSet(tex[CHAT_TEXRIGHT], 710-timer*10, 290, 360, 360, right%4+1, right/4+1, 4, 4);
		if(lbegin)
		{
			chatter[CHATTER_RIGHT].colorSet(0x80ffffff, 0x80ffffff, 0x80ffffff, 0x80ffffff);
		}
		else
		{
			chatter[CHATTER_LEFT].colorSet(0x80ffffff, 0x80ffffff, 0x80ffffff, 0x80ffffff);
		}
		chatter[CHATTER_TEXT].valueSet(tex[CHAT_TEXTEXT], M_ACTIVECLIENT_CENTER_X, 524-timer*4, FONTSYS_CHATTEXTWIDTH + CHATTEXT_BORDER, FONTSYS_CHATTEXTHEIGHT + CHATTEXT_BORDER, 1, 1, 1, 1);

		if(left == 0xff)
			chatter[CHATTER_LEFT].valueSet(tex[CHAT_TEXLEFT], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
		if(right == 0xff)
			chatter[CHATTER_RIGHT].valueSet(tex[CHAT_TEXRIGHT], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
	}
	else
	{
		match(CHATTER_LEFTNAME, left);
		match(CHATTER_RIGHTNAME, right);
		timer = 0;
		return true;
	}
	return false;
}

bool Chat::chatOff()
{
	timer++;

	Player::p.bInfi = true;

	if(timer == 1)
	{
		strcpy(text, "");
		Fontsys::SignUp(FONTSYS_CHATUSE, text);
		chatter[CHATTER_LEFTNAME].valueSet(tex[CHAT_TEXTEXNAME], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
		chatter[CHATTER_RIGHTNAME].valueSet(tex[CHAT_TEXTEXNAME], 0, 0, 0, 0, 0, 0, 0xff, 0xff);
	}

	if(timer <= 32)
	{
		for(int i=0;i<4;i++)
		{
			chatter[CHATTER_LEFT].quad.v[i].x -= timer * 15;
			chatter[CHATTER_RIGHT].quad.v[i].x += timer * 15;
		}
		chatter[CHATTER_TEXT].valueSet(tex[CHAT_TEXTEXT], 220, 380+timer*4, 320, 100, 1, 1, 1, 1);
	}
	else
	{
		Player::p.bInfi = false;
		timer = 0;
		chatting = false;
		return true;
	}
	return false;
}