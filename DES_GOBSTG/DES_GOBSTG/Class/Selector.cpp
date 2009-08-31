#include "Selector.h"
#include "SE.h"
#include "Main.h"
#include "Process.h"

list<Selector> sel;

int Selector::select = 0;
int Selector::sellock = 0;
int Selector::nselect = 0;
bool Selector::updown = true;
bool Selector::complete = false;
bool Selector::plus = true;

bool Selector::confirminit = false;

bool Selector::avoid = false;

Selector::Selector()
{
	sprite = NULL;
	complete = false;
	flag	= SEL_NONE;
}

Selector::~Selector()
{
}

void Selector::Clear()
{
	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if(i->sprite)
			delete i->sprite;
		i->sprite = NULL;
	}
	sel.clear();
	complete = false;
	confirminit = false;
	plus = true;
}

void Selector::valueSet(BYTE _ID, HTEXTURE tex, float cenx, float ceny, float _hscale, float _vscale, float column, float row, float nColumn, float nRow, BYTE _maxtime)
{
	ID		=	_ID;
	x		=	cenx;
	y		=	ceny;
	maxtime	=	_maxtime;
	hscale	=	_hscale;
	vscale	=	_vscale;

	timer	=	0;
	flag	=	SEL_NONE;

	pushtimer = 0;

	int texWidth = hge->Texture_GetWidth(tex);
	int texHeight = hge->Texture_GetHeight(tex);

	if(sprite)
		delete sprite;
	sprite = new hgeSprite(tex, (column - 1) * texWidth / nColumn, (row - 1) * texHeight / nRow, (float)(texWidth) / nColumn, (float)(texHeight) / nRow);

	alpha = 0xff;

	info[SELINFO_NONE].xadj = x;
	info[SELINFO_NONE].yadj = y;

	for(int i=1;i<4;i++)
	{
		info[i].xadj = 0;
		info[i].yadj = 0;
	}

	avoid = false;
}

void Selector::actionSet(BYTE setflag, float xadj, float yadj)
{
	int i = 1;
	while(setflag = setflag>>1)
		i++;
	info[i].xadj = xadj;
	info[i].yadj = yadj;
}

void Selector::Build(BYTE _ID, HTEXTURE tex, float cenx, float ceny, float _hscale, float _vscale, float column, float row, float nColumn, float nRow, BYTE _maxtime,
		float xadj0, float yadj0,
		float xadj1, float yadj1,
		float xadj2, float yadj2,
		float xadj3, float yadj3,
		bool nonactive, bool gray)
{
	Selector _sel;
	_sel.valueSet(_ID, tex, cenx, ceny, _hscale, _vscale, column, row, nColumn, nRow, _maxtime);
	_sel.actionSet(SEL_NONE, xadj0, yadj0);
	_sel.actionSet(SEL_OVER, xadj1, yadj1);
	_sel.actionSet(SEL_ENTER, xadj2, yadj2);
	_sel.actionSet(SEL_LEAVE, xadj3, yadj3);
	if(nonactive)
		_sel.flag |= SEL_NONACTIVE;
	if(gray)
		_sel.flag |= SEL_GRAY;
	sel.push_back(_sel);
}

bool Selector::confirm(HTEXTURE tex)
{
	if(!confirminit)
	{
		Clear();
		Selector _sel[3];
		_sel[0].valueSet(0, tex, 180, 280, 1, 0, 13, 18, 32, 32, 8);
		_sel[0].actionSet(SEL_OVER, -1, -0.5f);
		_sel[0].actionSet(SEL_ENTER, 0, 0);
		_sel[0].actionSet(SEL_LEAVE, 2, 0.5f);
		sel.push_back(_sel[0]);
		_sel[1].valueSet(1, tex, 260, 280, 1, 0, 14, 18, 32, 32, 8);
		_sel[1].actionSet(SEL_OVER, -1, -0.5f);
		_sel[1].actionSet(SEL_ENTER, 0, 0);
		_sel[1].actionSet(SEL_LEAVE, 2, 0.5f);
		sel.push_back(_sel[1]);

		_sel[2].valueSet(2, tex, 220, 240, 1, 0, 4, 19, 8, 32, 8);
		_sel[2].flag |= SEL_NONACTIVE;
		sel.push_back(_sel[2]);

		select = 1;
		nselect = 2;
		confirminit = true;
		updown = false;
	}

	if(hge->Input_GetDIKey(KS_BOMB_MP, DIKEY_UP) || hge->Input_GetDIKey(KS_PAUSE_MP))
	{
		SE::push(SE_SYSTEM_CANCEL);
		confirminit = false;
	}
	if(complete)
	{
		confirminit = false;
		if(select == 0)
			return true;
	}

	for(list<Selector>::iterator i = sel.begin(); i != sel.end(); i++)
	{
		if(i->ID == 2)
			i->flag = SEL_OVER;
	}
	return false;
}

void Selector::action()
{
	bool sub = false;
	if((flag & SEL_GRAY) && !(flag & SEL_NONACTIVE))
	{
		sub = true;
		flag &= ~SEL_GRAY;
	}

	if(flag & SEL_NONACTIVE)
	{
		if(select == ID)
		{
			for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
			{
				if(i->ID == ID)
				{
					list<Selector>::iterator j = i;
					do
					{
						if(plus)
						{
							j++;
							if(j == sel.end())
								j++;
						}
						else
						{
							j--;
							if(j == sel.end())
								j--;
						}
						if(!((j->flag & SEL_NONACTIVE) || (j->flag & SEL_GRAY)))
						{
							select = j->ID;
							break;
						}
					}while(j!=i);
					break;
				}
			}
		}
	}

	if(!(flag>>1) && ID < nselect)
	{
		if(ID == select)
		{
			flag |= SEL_OVER;
		}
		else
		{
			flag &= ~SEL_OVER;
		}
	}

	int fvID = -1;
	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if(!(i->flag & SEL_NONACTIVE))
		{
			fvID = i->ID;
			break;
		}
	}
	if(ID == fvID && !(flag>>1) && !sub)
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
			for(list<Selector>::iterator i = sel.begin(); i != sel.end(); i++)
			{
				if(i->ID == select)
				{
					if(!(i->flag & SEL_NONACTIVE))
					{
						sellock = select;
						benter = true;
						break;
					}
				}
			}
			if(benter)
			{
				SE::push(SE_SYSTEM_OK);
				for(list<Selector>::iterator i = sel.begin(); i != sel.end(); i++)
				{
					if(i->ID == select)
						i->flag |= SEL_ENTER;
					else
						i->flag |= SEL_LEAVE;
				}
			}
			else
			{
				SE::push(SE_SYSTEM_CANCEL);
			}
		}
		avoid = false;
	}

	if((flag &0xf) == SEL_NONE)
	{
		if(timer > 0)
		{
			timer--;
			x += (info[SELINFO_NONE].xadj - x) / timer;
			y += (info[SELINFO_NONE].yadj - y) / timer;

			alpha = 0x80 * timer / maxtime + 0x7f;
		}
		if(timer == 0)
		{
			x = info[SELINFO_NONE].xadj;
			y = info[SELINFO_NONE].yadj;
			alpha = ROLL(time, 0x3f) + 0x40;
		}
	}
	else
	{
		if(flag & SEL_OVER)
		{
			if(timer < maxtime)
			{
				timer++;
				x += info[SELINFO_OVER].xadj;
				y += info[SELINFO_OVER].yadj;

				alpha = 0x80 * timer / maxtime + 0x7f;
			}
			else if(timer == maxtime)
			{
				alpha = ROLL(time, 0x3f) * 1.5f + 0xA0;
			}
		}
		if(flag & SEL_ENTER)
		{
			select = sellock;
			if(flag & 0xff & ~SEL_ENTER)
			{
				timer = maxtime;
				flag = SEL_ENTER;
				alpha = 0xff;
			}
			if(timer > 0)
			{
				timer--;
				x += info[SELINFO_ENTER].xadj;
				y += info[SELINFO_ENTER].yadj;
			}
			else if(timer == 0)
			{
				complete = true;
			}
		}
		if(flag & SEL_LEAVE)
		{
			if(flag & 0xff & ~SEL_LEAVE)
			{
				timer = 0;
				flag = SEL_LEAVE;
				alpha = 0x80;
			}
			if(timer < maxtime)
			{
				timer++;
				x += info[SELINFO_LEAVE].xadj;
				y += info[SELINFO_LEAVE].yadj;

				alpha = 0x80 * (maxtime - timer) / maxtime;
			}
			if(timer == maxtime)
			{
				alpha = 0;
			}
		}
	}
	
	if(flag & SEL_NONACTIVE)
	{
		if(flag & SEL_GRAY)
			alpha = 0x40;
		else
			alpha = 0xff;
	}
	sprite->SetColor((alpha << 24) | 0xffffff);

	if(sub)
	{
		flag |= SEL_GRAY;
	}
}