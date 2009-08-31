#include "Item.h"
#include "Player.h"
#include "Bullet.h"
#include "SE.h"

hgeSprite * Item::spItem[ITEMSPRITEMAX];

VectorList<Item> mi;
VectorList<infoFont>Item::infofont;

HTEXTURE Item::tex;
hgeFont * Item::item_font = NULL;


Item::Item()
{
}

Item::~Item()
{
}

void Item::Init(HTEXTURE _tex, char * fontname)
{
	tex = _tex;
	Release();
	mi.init(ITEMMAX);
	infofont.init(ITEMINFOFONTMAX);
	for(int i=0;i<ITEMTYPEMAX;i++)
	{
		spItem[i] = new hgeSprite(tex, (float)(i*16), 80, 16, 16);
	}
	for(int i=ITEMTYPEMAX;i<ITEMSPRITEMAX;i++)
	{
		spItem[i] = new hgeSprite(tex, (float)((i-ITEMTYPEMAX)*16), 96, 16, 16);
	}
	item_font = new hgeFont(fontname);
	item_font->SetColor(0xffffffff);
}

void Item::valueSet(WORD type, float _x, float _y, bool _bDrained, int _angle, float _speed)
{
	ID			=	type;
	x			=	_x;
	y			=	_y;
	bDrained	=	_bDrained;

	bFast	=	true;

	timer	=	0;
	speed	=	_speed;
	angle	=	_angle;
	headangle =	0;
	hscale	=	1.0f;
	vscale	=	1.0f;
	alpha	=	0xff;
	exist	=	true;
}

void Item::Build(WORD type, float _x, float _y, bool _bDrained /* = false */, int _angle, float _speed)
{
	mi.push_back()->valueSet(type, _x, _y, _bDrained, _angle, _speed);
}

void Item::Release()
{
	for(int i=0;i<ITEMSPRITEMAX;i++)
	{
		if(spItem[i])
			delete spItem[i];
		spItem[i] = NULL;
	}
	mi.clear();
	infofont.clear();
	if(item_font)
	{
		delete item_font;
		item_font = NULL;
	}
}

void Item::Render()
{
	if(y < 0)
	{
		spItem[ID + ITEMTYPEMAX]->SetColor((BYTE)((int)y + 0xff)<<24 | 0xffffff);
		spItem[ID + ITEMTYPEMAX]->Render(x, 24);
	}
	spItem[ID]->Render(x,y);
}

void Item::drainAll()
{
	if (mi.size)
	{
		DWORD _index = mi.index;

		DWORD i = 0;
		DWORD size = mi.size;
		for (mi.toBegin(); i<size; mi.toNext(), i++)
		{
			if (!mi.isValid())
			{
				continue;
			}
			Item * _i = &(*mi);
			if (_i->exist)
			{
				_i->bDrained = true;
			}
		}
		mi.index = _index;
	}
}

void Item::undrainAll()
{
	if (mi.size)
	{
		DWORD _index = mi.index;

		DWORD i = 0;
		DWORD size = mi.size;
		for (mi.toBegin(); i<size; mi.toNext(), i++)
		{
			if (!mi.isValid())
			{
				continue;
			}
			Item * _i = &(*mi);
			if (_i->exist && _i->bDrained)
			{
				_i->bDrained = false;
				_i->bFast = true;
				_i->speed = ITEM_RETHROWSPEED;
				_i->angle = 9000;
			}
		}
		mi.index = _index;
	}
}

void Item::powerFull()
{
	if (mi.size)
	{
		DWORD _index = mi.index;

		DWORD i = 0;
		DWORD size = mi.size;
		for (mi.toBegin(); i<size; mi.toNext(), i++)
		{
			if (!mi.isValid())
			{
				continue;
			}
			Item * _i = &(*mi);
			if (_i->exist)
			{
				if (_i->ID == ITEM_POWER || _i->ID == ITEM_BIGPOWER)
				{
					_i->ID = ITEM_POINT;
					_i->speed = ITEM_RETHROWSPEED;
				}
			}
		}
		mi.index = _index;
	}

	IzeZone _ize;
	_ize.x = 0;
	_ize.y = 0;
	_ize.r = BULLET_IZEOVERZONE;
	Bullet::izel[BULLETIZE_ITEM].push_front(_ize);

	Player::p.nPower = PL_POWERMAX;
	Player::p.bFull = true;
}

void Item::action()
{
	if(!bDrained && !(Player::p.flag & PLAYER_COLLAPSE || Player::p.flag & PLAYER_SHOT))
	{
		float rdrain = ITEM_DRAINR(Player::p.bSlow);
//		if(fabsf(x-Player::p.x) < rdrain && fabsf(y-Player::p.y) < rdrain)
		if (checkCollisionSquare(Player::p, rdrain))
		{
			bDrained = true;
			bFast = false;
		}
		if(Player::p.y < ITEM_DRAINY)
		{
			bDrained = true;
		}
	}
	if(bDrained)
	{
		if(ID != ITEM_SMALLCIRCLE && ID != ITEM_CIRCLE || timer > ITEM_DRAINDELAY)
		{
//			angle = aMainAngle(Player::p.x, Player::p.y, 0);
			if(bFast)
				speed = ITEM_DRAINFASTSPEED;
			else
				speed = ITEM_DRAINSLOWSPEED;
			float dist = DIST(x, y, Player::p.x, Player::p.y);
			x += speed * (Player::p.x - x) / dist;
			y += speed * (Player::p.y - y) / dist;
		}
		else if(ID == ITEM_SMALLCIRCLE || ID == ITEM_CIRCLE)
		{
			speed = ITEM_CIRCLEUPSPEED;
			y += speed;
		}
	}
	else if(speed > 0)
		angle = 9000;

	timer++;
	if(speed < ITEM_DROPSPEEDMAX)
		speed += ITEM_DROPSPEEDACC;
	if(!bDrained)
	{
		if(angle != 9000)
		{
			x += speed * cost(angle);
			y += speed * sint(angle);
		}
		else
			y += speed;
	}

//	arc = 0;

	if(Player::p.bFull && (ID == ITEM_POWER || ID == ITEM_BIGPOWER))
		ID = ITEM_POINT;

//	if(fabsf(x-Player::p.x) < ITEM_GETR && fabsf(y-Player::p.y) < ITEM_GETR
	if (checkCollisionSquare(Player::p, ITEM_GETR)
		&& !(Player::p.flag & PLAYER_COLLAPSE)
		&& !((ID == ITEM_SMALLCIRCLE || ID == ITEM_CIRCLE) && timer <= ITEM_DRAINDELAY))
	{
		DWORD score;

		if(ID != ITEM_BOMB && ID != ITEM_EXTEND)
			SE::push(SE_ITEM_GET, x);
		else
			SE::push(SE_ITEM_EXTEND, x);

		switch(ID)
		{
		case ITEM_POINT:
			Player::p.nPoint++;
			score = PL_ITEMBONUS_POINT(Player::p.nAliveness);
			break;
		case ITEM_POWER:
			Player::p.nPower++;
			if(Player::p.nPower == PL_POWERLEVEL_1 || Player::p.nPower == PL_POWERLEVEL_2 || Player::p.nPower == PL_POWERLEVEL_3 || Player::p.nPower == PL_POWERLEVEL_4 || Player::p.nPower == PL_POWERLEVEL_5)
			{
				SE::push(SE_ITEM_POWERUP, x);
			}
			if(Player::p.nPower >= PL_POWERMAX)
			{
				powerFull();
			}
			score = PL_ITEMBONUS_POWER;
			break;
		case ITEM_BIGPOWER:
			Player::p.nPower += ITEM_BIGPOWERADD;
			if(	Player::p.nPower >= PL_POWERLEVEL_1 && Player::p.nPower < PL_POWERLEVEL_1 + ITEM_BIGPOWERADD || 
				Player::p.nPower >= PL_POWERLEVEL_2 && Player::p.nPower < PL_POWERLEVEL_2 + ITEM_BIGPOWERADD || 
				Player::p.nPower >= PL_POWERLEVEL_3 && Player::p.nPower < PL_POWERLEVEL_3 + ITEM_BIGPOWERADD || 
				Player::p.nPower >= PL_POWERLEVEL_4 && Player::p.nPower < PL_POWERLEVEL_4 + ITEM_BIGPOWERADD || 
				Player::p.nPower >= PL_POWERMAX)
			{
				SE::push(SE_ITEM_POWERUP, x);
			}
			if(Player::p.nPower >= PL_POWERMAX)
			{
				powerFull();
			}
			score = PL_ITEMBONUS_BIGPOWER;
			break;
		case ITEM_FULL:
			if(Player::p.nPower != PL_POWERMAX)
			{
				SE::push(SE_ITEM_POWERUP, x);
				powerFull();
			}
			score = PL_ITEMBONUS_FULL;
			break;
		case ITEM_BOMB:
			SE::push(SE_ITEM_EXTEND, x);
			Player::p.nBomb++;
			if(Player::p.nBomb > M_PL_BOMBMAX)
				Player::p.nBomb = M_PL_BOMBMAX;
			score = PL_ITEMBONUS_BOMB;
			break;
		case ITEM_EXTEND:
			SE::push(SE_ITEM_EXTEND, x);
			Player::p.nLife++;
			if(Player::p.nLife > M_PL_PLAYERMAX)
			{
				Player::p.nLife = M_PL_PLAYERMAX;
				Player::p.nBomb++;
			}
			if(Player::p.nBomb > M_PL_BOMBMAX)
				Player::p.nBomb = M_PL_BOMBMAX;
			score = PL_ITEMBONUS_PLAYER;
			break;
		case ITEM_CIRCLE:
			Player::p.nAliveness += PL_ITEMBONUS_CIRCLEALIVENESS;
			Player::p.nPop += PL_ITEMBONUS_CIRCLENPOP;
			score = PL_ITEMBONUS_CIRCLE;
			break;
		case ITEM_SMALLCIRCLE:
			Player::p.nAliveness += PL_ITEMBONUS_SMALLCIRCLEALIVENESS;
			Player::p.nPop += PL_ITEMBONUS_SMALLCIRCLENPOP;
			score = PL_ITEMBONUS_SMALLCIRCLE;
			break;
		}
		exist = false;
		Player::p.nScore += score;

		struct infoFont info;
		itoa(score, info.cScore, 10);
		for(int i=0;i<(int)strlen(info.cScore);i++)
		{
			info.cScore[i] -= ('0' - ITEMINFO_TURNVALUEKEY);
		}
		info.timer = 0;
		info.x = x;
		info.y = y;
		if(ID == ITEM_POINT || ID == ITEM_CIRCLE)
			info.yellow = true;
		else
			info.yellow = false;
		infofont.push_back(info);
	}
	if(y > M_DELETECLIENT_BOTTOM)
		exist = false;
}