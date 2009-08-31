#include "InfoSelect.h"
#include "SE.h"
#include "FontSys.h"
#include "Main.h"
#include "Process.h"

list<InfoSelect>infoselect;

int InfoSelect::nselect = 0;
int InfoSelect::select = 0;
bool InfoSelect::updown = true;
bool InfoSelect::complete = false;
bool InfoSelect::plus = true;

bool InfoSelect::avoid = false;

InfoSelect::InfoSelect()
{
}

InfoSelect::~InfoSelect()
{
}

void InfoSelect::linki(const char * linkcode, int num)
{
	if(!info)
		return;

	char buffer[M_STRITOAMAX];
	itoa(num, buffer, 10);
	if(!linkcode)
	{
		strcat(info, buffer);
		return;
	}
	strcat(info, linkcode);
	strcat(info, buffer);
	if(strlen(linkcode) == 6)
		strcat(info, FONTSYS_CONTROLSTR);
}

void InfoSelect::linkl(const char * linkcode, LONGLONG num)
{
	if(!info)
		return;

	char buffer[M_STRMAX];
	_i64toa(num, buffer, 10);
	if(!linkcode)
	{
		strcat(info, buffer);
		return;
	}
	strcat(info, linkcode);
	strcat(info, buffer);
	if(strlen(linkcode) == 6)
		strcat(info, FONTSYS_CONTROLSTR);
}

void InfoSelect::linkf(const char * linkcode, BYTE tail, float num)
{
	if(!info)
		return;

	char buffer[M_STRMAX];
	if(!tail)
	{
		sprintf(buffer, "%f", num);
	}
	else
	{
		char buff[M_STRMAX];
		char tbuf[M_STRITOAMAX];
		strcpy(buff, "%.");
		strcat(buff, itoa(tail, tbuf, 10));
		strcat(buff, "f");
		sprintf(buffer, buff, num);
	}
	if(!linkcode)
	{
		strcat(info, buffer);
		return;
	}
	strcat(info, linkcode);
	strcat(info, buffer);
	if(strlen(linkcode) == 6)
		strcat(info, FONTSYS_CONTROLSTR);
}

void InfoSelect::valueSet(BYTE _ID, const char * _info, float _x, float _y, BYTE _coltype, bool _nonactive, bool _gray)
{
	ID = _ID;
	strcpy(info, _info);
	Fontsys::SignUp(ID, info);
	x = _x;
	y = _y;
	coltype = _coltype;
	nonactive = _nonactive;
	gray = _gray;
	complete = false;
	plus = true;
	pushtimer = 0;
	ucol = 0xceffffff;

	avoid = false;
}

void InfoSelect::Build(BYTE ID, const char * info, float x, float y, BYTE coltype, bool nonactive, bool gray)
{
	InfoSelect _infoselect;

	_infoselect.valueSet(ID, info, x, y, coltype, nonactive, gray);
	infoselect.push_back(_infoselect);
}

void InfoSelect::action()
{
	if(nonactive)
	{
		if(select == ID)
		{
			for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
			{
				if(i->ID == ID)
				{
					list<InfoSelect>::iterator j = i;
					do
					{
						if(plus)
						{
							j++;
							if(j == infoselect.end())
								j++;
						}
						else
						{
							j--;
							if(j == infoselect.end())
								j--;
						}
						if(!j->nonactive)
						{
							select = j->ID;
							break;
						}
					}while(j!=i);
					break;
				}
			}
		}
		if(gray)
			dcol = 0xce808080;
//		return;
	}

	int fvID = -1;
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
	{
		if(!(i->nonactive))
		{
			fvID = i->ID;
			break;
		}
	}

	if(ID == fvID)
	{
		if(updown)
		{
			if(hge->Input_GetDIKey(KS_UP_MP) || hge->Input_GetDIKey(KS_DOWN_MP))
			{
				pushtimer++;
			}
			else
				pushtimer = 0;
			if(pushtimer == M_PUSH_FIRST)
				pushtimer = M_PUSH_ROLLTO;
			if(pushtimer == M_PUSH_ROLLTO)
			{
				hge->Input_SetDIKey(KS_UP_MP, false);
				hge->Input_SetDIKey(KS_DOWN_MP, false);
			}
			if(hge->Input_GetDIKey(KS_DOWN_MP, DIKEY_DOWN))
			{
				plus = true;
				SE::push(SE_SYSTEM_SELECT);
				select++;
				if(select == nselect)
					select = 0;
			}
			else if(hge->Input_GetDIKey(KS_UP_MP, DIKEY_DOWN))
			{
				plus = false;
				SE::push(SE_SYSTEM_SELECT);
				select--;
				if(select == -1)
					select = nselect - 1;
			}
		}
		else
		{
			if(hge->Input_GetDIKey(KS_LEFT_MP) || hge->Input_GetDIKey(KS_RIGHT_MP))
			{
				pushtimer++;
			}
			else
				pushtimer = 0;
			if(pushtimer == M_PUSH_FIRST)
				pushtimer = M_PUSH_ROLLTO;
			if(pushtimer == M_PUSH_ROLLTO)
			{
				hge->Input_SetDIKey(KS_LEFT_MP, false);
				hge->Input_SetDIKey(KS_RIGHT_MP, false);
			}
			if(hge->Input_GetDIKey(KS_RIGHT_MP, DIKEY_DOWN))
			{
				plus = true;
				SE::push(SE_SYSTEM_SELECT);
				select++;
				if(select == nselect)
					select = 0;
			}
			else if(hge->Input_GetDIKey(KS_LEFT_MP, DIKEY_DOWN))
			{
				plus = false;
				SE::push(SE_SYSTEM_SELECT);
				select--;
				if(select == -1)
					select = nselect - 1;
			}
		}
		if(hge->Input_GetDIKey(KS_FIRE_MP, DIKEY_DOWN) && !avoid)
		{
			bool benter = false;
			for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
			{
				if(i->ID == select)
				{
					if(!i->nonactive)
					{
						benter = true;
						break;
					}
				}
			}
			if(benter)
			{
				SE::push(SE_SYSTEM_OK);
				complete = true;
			}
			else
			{
				SE::push(SE_SYSTEM_CANCEL);
			}
		}
		avoid = false;
	}

	if(select == ID && fvID >= 0)
	{
		dcol = 0xff4000ff;
	}
	else
	{ 
		if (!gray)
		{
			switch(coltype)
			{
			case INFO_GREEN:
				dcol = 0xce00ff00;
				break;
			case INFO_YELLOW:
				dcol = 0xced0d000;
				break;
			case INFO_GRAY:
				dcol = 0xce808080;
				break;
			case INFO_RED:
				dcol = 0xffff2000;
				break;
			case INFO_BLUE:
				dcol = 0xce4000ff;
			}
		}
	}
}