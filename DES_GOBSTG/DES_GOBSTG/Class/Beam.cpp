#include "Beam.h"
#include "Bullet.h"
#include "Player.h"
#include "SE.h"
#include "Chat.h"
#include "BossInfo.h"
#include "Item.h"
#include "Main.h"

VectorList<Beam> be;

WORD Beam::index;

Beam::Beam()
{
	able	= false;
}

Beam::~Beam()
{
}

void Beam::Init()
{
	be.init(BEAMMAX);
	index = 0;
}

bool Beam::Build(float x, float y, int angle, float speed, BYTE type, BYTE color, WORD lenth, bool stop, bool horizon, int fadeouttime, BYTE tarID)
{
	Beam * _tbe;
	if(type != 41)
	{
		_tbe = be.push_back();
		index = be.getEndIndex();
	}
	else
	{
		_tbe = be.push_front();
		index = be.getBeginIndex();
	}
	_tbe->valueSet(index, x, y, angle, speed, type, color, lenth, stop, horizon, fadeouttime, tarID);
	return true;
}

void Beam::Render()
{
	int i = type<<3|color;
	Bullet::sp[i]->SetBlendMode(BLEND_ALPHAADD);
	Bullet::sp[i]->SetColor(alpha<<24|diffuse);
	if(type == 41)
		Bullet::sp[i]->SetHotSpot(0, 32);
	Bullet::sp[i]->RenderEx(x, y, ARC(angle+headangle), hscale, vscale);
	if(type == 41)
		Bullet::sp[i]->SetHotSpot(128, 32);
	Bullet::sp[i]->SetBlendMode(BLEND_DEFAULT);
}

void Beam::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, BYTE _color, WORD _lenth, bool _stop, bool _horizon, int _fadeouttime, BYTE _tarID)
{
	ID			=	_ID;
	x			=	_x;
	y			=	_y;
	angle		=	_angle;
	speed		=	_speed;
	type		=	_type;
	color		=	_color;
	lenth		=	_lenth;
	stop		=	_stop;
	horizon		=	_horizon;
	fadeouttime	=	_fadeouttime;

	tarID	=	_tarID;
	holdtar	=	0xff;
	pintar	=	0xff;

	timer			=	0;
	grazetimer		=	0;
	exist			=	true;
	fadeout			=	false;
	able			=	true;
	headangle		=	0;
	alpha			=	0xff;

	lastangle = angle;

	xplus = cost(angle);
	yplus = sint(angle);
	if(horizon)
	{
		hscale = lenth / 16.0f;
		vscale = 0.1f;
		x += lenth * cost(angle) / 2;
		y += lenth * sint(angle) / 2;
		speed /= 100;
	}
	else
	{
		hscale = 0;
		vscale = 1;
		xplus *= speed;
		yplus *= speed;
	}
}

void Beam::SetVector(const BObject &ori, const BObject &aim)
{
	SetVector(ori.x, ori.y, aim.x, aim.y);
}

void Beam::SetVector(float orix, float oriy, float aimx, float aimy)
{
	lenth = DIST(orix, oriy, aimx, aimy);
	if (type == 41)
	{
		x = orix;
		y = oriy;
		hscale = lenth / 128;
	}
	else
	{
		x = (orix + aimx) / 2;
		y = (oriy + aimy) / 2;
		hscale = lenth / 16;
	}
	angle = aMainAngle(aimx, aimy);
}

void Beam::SetHold(BYTE _holdtar)
{
	holdtar = _holdtar;
}

void Beam::SetHold(BYTE _holdtar, BYTE _pintar)
{
	SetHold(_holdtar);
	pintar = _pintar;
}

void Beam::action()
{
	if(angle != lastangle)
	{
		xplus = speed * cost(angle);
		yplus = speed * sint(angle);
		lastangle = angle;
		lastspeed = speed;
	}
	timer++;

	if(!fadeout)
	{
		if(BossInfo::flag >> 2)
		{
			fadeout = true;
			timer = 0;
		}
		if(timer == 1)
			SE::push(SE_BEAM_1, x);
		if(!horizon)
		{
			if(type == 41)
			{
				if(hscale * 128 < lenth)
				{
					hscale += speed / 16;
				}
			}
			else if(hscale * 16 < lenth)
			{
				x += xplus / 2;
				y += yplus / 2;
				hscale += speed / 16;
			}
			else if(!stop)
			{
				x += xplus;
				y += yplus;
			}
		}
		else
		{
			if(vscale < 1)
				vscale += speed;
			if(timer > (DWORD)fadeouttime)
			{
				fadeout = true;
				timer = 0;
			}
		}
		if (holdtar != 0xff)
		{
			if (pintar != 0xff)
			{
				SetVector(tar[holdtar].x, tar[holdtar].y, tar[pintar].x, tar[pintar].y);
			}
			else
			{
				float factor = hscale * 16 / speed;
				SetVector(tar[holdtar].x, tar[holdtar].y, tar[holdtar].x + factor * xplus, tar[holdtar].y + factor * yplus);
			}
		}

		if(tarID != 0xff)
		{
			tar[tarID].x = x;
			tar[tarID].y = y;
		}

		if(Chat::chatting)
		{
			fadeout = true;
			timer = 0;
		}

		if(isInRect(Player::p.r, Player::p.x, Player::p.y))
		{
//			if(type != 41)
//			{
//				fadeout = true;
//				timer = 0;
//			}
			if(!Player::p.bInfi && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
			{	
				if(Player::p.nBomb)
					Player::p.flag |= PLAYER_SHOT;
				else
					Player::p.flag |= PLAYER_COLLAPSE;
			}
		}
		if(grazetimer)
		{
			grazetimer++;
			if(grazetimer == 12)
				grazetimer = 0;
		}
		else
		{
			grazetimer++;
			if(isInRect(Player::p.rgraze, Player::p.x, Player::p.y))
			{
				if(!Player::p.bInfi && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
				{
					Player::p.nGraze += PL_GRAZEADD(Player::p.bSlow);
					Player::p.nScore += PL_GRAZEBONUS_SCORE(Player::p.bSlow);

					if(Player::p.bCircle)
					{
						float itemx;
						float itemy;
						float tk = 0;
						if (xplus || yplus)
						{
							tk = - ((x - Player::p.x) * xplus + (y - Player::p.y) * yplus) / (xplus * xplus + yplus * yplus);
							itemx = x + xplus * tk;
							itemy = y + yplus * tk;
						}
						else
						{
							itemx = Player::p.x;
							itemy = Player::p.y;
						}
//						itemx = Player::p.rgraze * 0.75f * cost(tangle) + Player::p.x;
//						itemy = Player::p.rgraze * 0.75f * sint(tangle) + Player::p.y;
						Item::Build(ITEM_SMALLCIRCLE, itemx, itemy, true);

						Player::p.nPop += PL_GRAZEBONUS_NPOP;
					}

					if(BossInfo::range && !bossinfo.failed)
					{
						bossinfo.bonus += PL_GRAZEBONUS_BOSSBONUS(bossinfo.maxbonus, bossinfo.limit);
					}
					Player::p.flag |= PLAYER_GRAZE;
				}
			}
		}

		if(type != 41)
		{
			if(	x - hscale * 16 > M_DELETECLIENT_RIGHT ||
				x + hscale * 16 < M_DELETECLIENT_LEFT ||
				y - hscale * 16 > M_DELETECLIENT_BOTTOM ||
				y + hscale * 16 < M_DELETECLIENT_TOP)
				exist = false;
		}
	}
	else
	{
		if(timer == 1)
		{
			SE::push(SE_BEAM_FADEOUT, x);
		}
		if(timer == 32)
			exist = false;
		else
		{
			vscale -= 0.03125;
			alpha = (BYTE)(32-timer) * 8 - 1;
		}
	}
	able = exist && !fadeout;
}

bool Beam::isInRect(float r, float aimx, float aimy)
{
	if(vscale < BEAM_INVALIDSCALE)
		return false;
	float nowlenth = hscale * 16;
	if(type == 41)
	{
		nowlenth *= 8;
	}
	float cx;
	float cy;
	if(type == 41)
	{
		cx = x+xplus/speed*nowlenth*5/8;
		cy = y+yplus/speed*nowlenth*5/8;
	}
	else if(type == 0)
	{
		cx = x-xplus/speed*nowlenth*1/8;
		cy = y-yplus/speed*nowlenth*1/8;
	}
	else
	{
		cx = x;
		cy = y;
	}
	/*
	if(fabsf(aimx - cx) > nowlenth / 2 + r || fabsf(aimy - cy) > nowlenth / 2 + r)
	{
		return false;
	}
	*/

	if(type == 13)
		nowlenth *= 0.4f;
	else
		nowlenth *= 0.3125f;

	float rotCos;
	float rotSin;
	if (!horizon && speed != 0)
	{
		rotCos = xplus / speed;
		rotSin = yplus / speed;
	}
	else
	{
		rotCos = cost(angle);
		rotSin = sint(angle);
	}
	return checkCollisionEllipse(aimx, aimy, 4.5f*vscale, nowlenth, rotCos, rotSin, r);

	/*
	float newx;
	float newy;
	if(!horizon && speed != 0)
	{
		newx = (float)fabsf((xplus*(aimx-cx)+yplus*(aimy-cy))/speed);
		newy = (float)fabsf((yplus*(aimx-cx)-xplus*(aimy-cy))/speed);
	}
	else
	{
		newx = (float)fabsf(cost(angle)*(aimx-cx)+sint(angle)*(aimy-cy));
		newy = (float)fabsf(sint(angle)*(aimx-cx)-cost(angle)*(aimy-cy));
	}

	if(newy*newy*(nowlenth+r)*(nowlenth+r)+newx*newx*(4.5f*vscale+r)*(4.5f*vscale+r) <= (4.5f*vscale+r)*(4.5f*vscale+r)*(nowlenth+r)*(nowlenth+r))
	{
		return true;
	}
	return false;
	*/
}