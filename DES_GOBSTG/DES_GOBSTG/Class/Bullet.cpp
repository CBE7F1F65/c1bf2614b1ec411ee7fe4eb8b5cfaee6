#include "Bullet.h"
#include "Player.h"
#include "SE.h"
#include "Item.h"
#include "BossInfo.h"
#include "Scripter.h"
#include "Main.h"

RenderDepth Bullet::renderDepth[BULLETTYPEMAX];

Bullet Bullet::_bu;
hgeSprite * Bullet::sp[BULLETTYPECOLORMAX];
list<IzeZone> Bullet::izel[BULLETIZELISTMAX];

VectorList<Bullet>bu;
HTEXTURE Bullet::tex;

WORD Bullet::index;

Bullet::Bullet()
{
	able	= false;
}

Bullet::~Bullet()
{
}

void Bullet::Init(HTEXTURE _tex)
{
	Release();

	ZeroMemory(renderDepth, sizeof(RenderDepth) * BULLETTYPEMAX);
	bu.init(BULLETMAX);
	tex = _tex;
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<BULLETCOLORMAX;j++)
		{
			sp[i<<3|j] = new hgeSprite(tex, j*16+i/16*128, (i%16)*16, 16, 16);
			sp[i<<3|j]->SetBlendMode(BLEND_DEFAULT);
		}
	}
	for(int i=32;i<40;i++)
	{
		for(int j=0;j<BULLETCOLORMAX;j++)
		{
			sp[i<<3|j] = new hgeSprite(tex, j*32, (i%8)*32+256, 32, 32);
			sp[i<<3|j]->SetBlendMode(BLEND_DEFAULT);
		}
	}
	for(int i=0;i<BULLETCOLORMAX;i++)
	{
		sp[40<<3|i] = new hgeSprite(tex, (i%4)*64+256, i/4*64, 64, 64);
		sp[40<<3|i]->SetBlendMode(BLEND_DEFAULT);
	}
	for(int i=0;i<2;i++)
	{
		sp[41<<3|i] = new hgeSprite(tex, i*128+256, 128, 128, 64);
		sp[41<<3|i]->SetBlendMode(BLEND_DEFAULT);
	}
	for(int i=0;i<6;i++)
	{
		sp[48<<3|i] = new hgeSprite(tex, i*40+256, 432, 40, 16);
		sp[48<<3|i]->SetBlendMode(BLEND_DEFAULT);
	}
	for(int i=49;i<51;i++)
	{
		for(int j=0;j<BULLETCOLORMAX;j++)
		{
			sp[i<<3|j] = new hgeSprite(tex, j*32+256, (1-(i&1))*32+448, 32, 32);
			sp[i<<3|j]->SetBlendMode(BLEND_DEFAULT);
		}
	}

	for (int i=0; i<BULLETTYPECOLORMAX; i++)
	{
		if (!sp[i])
		{
			sp[i] = new hgeSprite(tex, 0, 0, 0, 0);
			sp[i]->SetBlendMode(BLEND_DEFAULT);
		}
	}

	for(int i=0;i<BULLETCOLORMAX;i++)
	{
		sp[10<<3|i]->SetBlendMode(BLEND_ALPHAADD);
	}

	for(int i=0;i<BULLETIZELISTMAX;i++)
		izel[i].clear();
	index = 0;
}

bool Bullet::Build(float x, float y, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid, BYTE tarID)
{
	Bullet * _tbu = NULL;
//	if(type != 10)
//	{
		_tbu = bu.push_back();
		index = bu.getEndIndex();
//	}
/*	else
	{
		_tbu = bu.push_front();
		index = bu.getBeginIndex();
	}
*/
	if (!_tbu->valueSet(index, x, y, absolute, angle, speed, type, color, fadeinTime, avoid, tarID))
	{
		bu.pop(index);
		return false;
	}
	memcpy(_tbu->actionList, _bu.actionList, BULLETACTIONMAX*sizeof(int));
	return true;
}

void Bullet::Release()
{
	bu.clear();
	for(int i=0;i<BULLETTYPECOLORMAX;i++)
	{
		if(sp[i])
			delete sp[i];
		sp[i] = NULL;
	}
}

void Bullet::Render()
{
	int i = type<<3 | color;
	sp[i]->SetColor(alpha<<24 | diffuse);
	sp[i]->RenderEx(x, y, ARC(angle+headangle), hscale, vscale);
}

bool Bullet::valueSet(WORD _ID, float _x, float _y, bool absolute, int _angle, float _speed, BYTE _type, BYTE _color, int _fadeinTime, float avoid, BYTE _tarID)
{
	ID			=	_ID;
	x			=	_x;
	y			=	_y;
	if(avoid)
	{
		if(isInRect(avoid, Player::p.x, Player::p.y))
			return false;
	}
	if(absolute)
		angle	=	_angle;
	else
		angle	=	rMainAngle(Player::p, _angle);
	speed		=	_speed;
	type		=	_type;
	oldtype		=	type;
	color		=	_color;
	fadeinTime	=	_fadeinTime;

	tarID	=	_tarID;

	if(type < BULLET_BIGBEGIN && fadeinTime)
		type = BULLET_FADEINTYPE;

	timer			=	0;
	typechangetimer	=	0;
	exist			=	true;
	grazed			=	false;
	fadeout			=	false;
	able			=	true;
	remain			=	false;
	actionList[0]	=	SECTIONEND;
	hscale			=	1.0f;
	vscale			=	0.0f;
	headangle		=	0;
	alpha			=	0xff;
	toafter			=	0;
	cancelable		=	true;

	xplus = speed * cost(angle);
	yplus = speed * sint(angle);
	lastx = x;
	lasty = y;
	lastangle = angle;
	lastspeed = speed;

	return true;
}

void Bullet::action()
{
	index = ID;
	if(angle != lastangle || lastspeed == 0)
	{
		xplus = speed * cost(angle);
		yplus = speed * sint(angle);
	}
	else if(speed != lastspeed)
	{
		xplus *= speed / lastspeed;
		yplus *= speed / lastspeed;
	}
	lastangle = angle;
	lastspeed = speed;
	
	timer++;

	lastx = x;
	lasty = y;

	if(!fadeout && cancelable)
	{
		if(Player::p.flag & PLAYER_BOMB || Player::p.flag & PLAYER_COLLAPSE)
		{
			if(Player::p.bombtimer == 1)
			{
				itemlize();
			}
			else
			{
				fadeout = true;
				timer = 0;
			}
		}
		else if(Player::p.flag & PLAYER_DEADBOMB)
		{
			if(Player::p.deadbombtimer == 1 || isInRect(64, Player::p.x, Player::p.y))
			{
				fadeout = true;
				timer = 0;
			}
		}
	}

	if(!fadeout)
	{
		if(BossInfo::flag >= BOSSINFO_COLLAPSE)
		{
			if(!BossInfo::range)
			{
				itemlize();
				timer = 0;
			}
			else
			{
				pointlize();
				timer = 0;
			}
		}
		if (cancelable)
		{
			if(izel[BULLETIZE_FADEOUT].size())
			{
				for(list<IzeZone>::iterator i=izel[BULLETIZE_FADEOUT].begin();i!=izel[BULLETIZE_FADEOUT].end();i++)
				{
					if(i->r>BULLET_IZEFULLZONE || (x-i->x)*(x-i->x)+(y-i->y)*(y-i->y)<=(i->r)*(i->r))
					{
						fadeout = true;
						timer = 0;
						break;
					}
				}
			}
			if(izel[BULLETIZE_ITEM].size())
			{
				for(list<IzeZone>::iterator i=izel[BULLETIZE_ITEM].begin();i!=izel[BULLETIZE_ITEM].end();i++)
				{
					if(i->r>BULLET_IZEFULLZONE || (x-i->x)*(x-i->x)+(y-i->y)*(y-i->y)<=(i->r)*(i->r))
					{
						itemlize();
						break;
					}
				}
			}
			if(izel[BULLETIZE_POINT].size())
			{
				for(list<IzeZone>::iterator i=izel[BULLETIZE_POINT].begin();i!=izel[BULLETIZE_POINT].end();i++)
				{
					if(i->r>BULLET_IZEFULLZONE || (x-i->x)*(x-i->x)+(y-i->y)*(y-i->y)<=(i->r)*(i->r))
					{
						pointlize();
						break;
					}
				}
			}
		}

		if(timer == 1 && fadeinTime > 1)
		{
			if(oldtype < BULLET_BIGBEGIN)
			{
				hscale *= (fadeinTime + 15.0f) * 0.0625f;
				if(vscale)
					vscale *= (fadeinTime + 15.0f) * 0.0625f;
			}
			else
			{
				hscale /= fadeinTime;
				if(vscale)
					vscale /= fadeinTime;
			}
		}
		else if(timer < (DWORD)fadeinTime)
		{
			if(oldtype < BULLET_BIGBEGIN)
			{
				hscale -= 0.0625f;
				if(vscale)
					vscale -= 0.0625f;
				alpha = 0x80;
			}
			else
			{
				hscale += 1.0f / fadeinTime;
				if(vscale)
					vscale += 1.0f / fadeinTime;
			}
		}
		else if(timer == fadeinTime)
		{
			type = oldtype;
			if(type < BULLET_BIGBEGIN)
			{
				hscale -= 0.0625f;
				if(vscale)
					vscale -= 0.0625f;
			}
			else
			{
				hscale += 1.0f / fadeinTime;
				if(vscale)
					vscale += 1.0f / fadeinTime;
			}
			alpha = 0xff;
			SE::push(SE_BULLET_1+type%4, x);
		}
		else
		{
			x += xplus;
			y += yplus;

			ChangeAction();

			if(typechangetimer)
			{
				if(typechangetimer == 1)
				{
					hscale *= (BULLET_TYPECHANGETIME + 15.0f) * 0.0625f;
					if(vscale)
						vscale *= (BULLET_TYPECHANGETIME + 15.0f) * 0.0625f;
					type = BULLET_FADEINTYPE;
				}
				if(typechangetimer < BULLET_TYPECHANGETIME)
				{
					typechangetimer++;
					hscale -= 0.0625f;
					if(vscale)
						vscale -= 0.0625f;
					alpha = 0x80;
				}
				else
				{
					hscale -= 0.0625f;
					if(vscale)
						vscale -= 0.0625f;
					type = oldtype;
					typechangetimer = 0;
					alpha = 0xff;
				}
			}

			if(isInRect(Player::p.r, Player::p.x, Player::p.y))
			{
				if (cancelable)
				{
					fadeout = true;
					timer = 0;
				}
				if(!Player::p.bInfi && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
				{	
					if(Player::p.nBomb)
						Player::p.flag |= PLAYER_SHOT;
					else
						Player::p.flag |= PLAYER_COLLAPSE;
				}
			}
			if(!grazed)
			{
				if((Player::p.x - x) * (Player::p.x - x) + (Player::p.y - y) * (Player::p.y - y) < Player::p.rgraze * Player::p.rgraze)
				{
					if(!Player::p.bInfi && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
					{
						Player::p.nGraze += PL_GRAZEADD(Player::p.bSlow);
						Player::p.nScore += PL_GRAZEBONUS_SCORE(Player::p.bSlow);

						if(Player::p.bCircle)
						{
							Item::Build(ITEM_SMALLCIRCLE, x, y, true);

							Player::p.nPop += PL_GRAZEBONUS_NPOP;
						}
						
						if(BossInfo::range && !bossinfo.failed)
						{
							bossinfo.bonus += PL_GRAZEBONUS_BOSSBONUS(bossinfo.maxbonus, bossinfo.limit);
						}

						Player::p.flag |= PLAYER_GRAZE;
						grazed = true;
					}
				}
			}
		}

		if(type == 11 || type == 36 || type == 40)
		{
			if(color & 1)
				headangle += 200;
			else
				headangle -= 200;
		}
		if(tarID != 0xff)
		{
			tar[tarID].x = x;
			tar[tarID].y = y;
		}

		if(!remain)
		{
			if(x > M_DELETECLIENT_RIGHT || x < M_DELETECLIENT_LEFT || y > M_DELETECLIENT_BOTTOM || y < M_DELETECLIENT_TOP)
				exist = false;
		}
	}
	else
	{
		/*
		if(timer == 1)
		{
			SE::push(SE_BULLET_FADEOUT, x);
		}
		*/
		if(timer == 16)
		{
			if(toafter == BULLETIZE_ITEM)
			{
				Item::Build(ITEM_SMALLCIRCLE, x, y, true);
				exist = false;
			}
			else if(toafter == BULLETIZE_POINT)
			{
				switch(oldtype/8)
				{
				case 0:
					Player::p.nScore += 500;
					break;
				case 1:
					Player::p.nScore += 1000;
					break;
				case 2:
					Player::p.nScore += 2000;
					break;
				case 3:
					Player::p.nScore += 3000;
					break;
				case 4:
					Player::p.nScore += 5000;
					break;
				case 5:
					Player::p.nScore += 8000;
					break;
				}
				color = oldtype/8;
				type = BULLET_BONUSTYPE;
				alpha = 0xff;
				hscale = vscale = 1;
				angle = 0;
				headangle = 0;
				timer = 32;
			}
		}
		else if(timer == 32)
		{
			exist = false;
		}
		else if(timer == 96)
			exist = false;
		else if(timer < 32)
		{
			diffuse = 0xffffff;
			if(type < BULLET_BIGBEGIN)
			{
				type = BULLET_FADEOUTTYPE;
			}
			hscale = vscale = timer / 60.0f + 0.6f;
			alpha = (BYTE)(32-timer) * 4 - 1;
		}
		else if(timer > 32 && timer < 96)
		{
			if(timer > 64)
				alpha = (BYTE)(96-timer) * 8 - 1;
			y -= 0.2f;
		}
	}
	able = exist && !fadeout;

	if (exist)
	{
		if (!renderDepth[type].haveType)
		{
			renderDepth[type].haveType = true;
			renderDepth[type].startIndex = index;
		}
		renderDepth[type].endIndex = index + 1;
		if (renderDepth[type].endIndex == BULLETMAX)
		{
			renderDepth[type].endIndex = 0;
		}
	}
}

void Bullet::itemlize()
{
	timer = 0;
	fadeout = true;
	able = false;
	toafter = BULLETIZE_ITEM;
}

void Bullet::pointlize()
{
	timer = 0;
	fadeout = true;
	able = false;
	toafter = BULLETIZE_POINT;
}

bool Bullet::isInRect(float r,float aimx,float aimy)
{
	if(type < 16 || type == 35)
	{
		return checkCollisionCircle(aimx, aimy, 3+r);
	}
	else if(type < 24)
	{
		return checkCollisionCircle(aimx, aimy, 5+r);
	} 
	else if(type < 35)
	{
		float rotCos;
		float rotSin;
		if (!speed)
		{
			rotCos = xplus / speed;
			rotSin = yplus / speed;
		}
		else
		{
			rotCos = cost(angle);
			rotSin = sint(angle);
		}
		float rPrep;
		float rParal;
		if (type < 32)
		{
			rPrep = 3;
			rParal = 5;
		}
		else
		{
			rPrep = 5;
			rParal = 9;
		}
		return checkCollisionEllipse(aimx, aimy, rPrep, rParal, rotCos, rotSin, r);
	}
	/*
	else if(type < 35)
	{
		if((fabsf(x-aimx)>=9+r)||(fabsf(y-aimy)>=9+r))
			return false;
		float newx,newy;
		if(!speed)
		{
			newx = (float)fabsf((yplus*(aimx-x)-xplus*(aimy-y))/speed);
			newy = (float)fabsf((xplus*(aimx-x)+yplus*(aimy-y))/speed);
		}
		else
		{
			newx = (float)fabsf(sint(angle)*(aimx-x)-cost(angle)*(aimy-y));
			newy = (float)fabsf(cost(angle)*(aimx-x)+sint(angle)*(aimy-y));
		}
		if(newx*newx*(9+r)*(9+r)+newy*newy*(5+r)*(5+r) <= (5+r)*(5+r)*(9+r)*(9+r))
			return true;
	}
	*/
	else if(type == 36)
	{
		return checkCollisionCircle(aimx, aimy, 7+r);
	}
	else if(type < 40)
	{
		return checkCollisionCircle(aimx, aimy, 10+r);
	}
	else
	{
		return checkCollisionCircle(aimx, aimy, 16+r);
	}
	return false;
}

void Bullet::ChangeAction()
{
	bool doit = false;
	for(int i=0;i<BULLETACTIONMAX;++i)
	{
		switch(actionList[i])
		{
		case SECTIONEND:
			i = BULLETACTIONMAX-1;
			break;

		case EVERY:
			doit = true;
			break;
		case EVERYMOD:
			if(timer % (DWORD)actionList[i+1] == 0)
				doit = true;
			++i;
			break;

		case TIMERGREAT:
			if(timer >= (DWORD)actionList[i+1])
				doit = true;
			++i;
			break;
		case TIMEREQUAL:
			if(timer == (DWORD)actionList[i+1])
				doit = true;
			++i;
			break;
		case TIMERLESS:
			if(timer <= (DWORD)actionList[i+1])
				doit = true;
			++i;
			break;
		case TIMERRANGE:
			if(timer >= (DWORD)actionList[i+1] && timer <= (DWORD)actionList[i+2])
				doit = true;
			i+=2;
			break;

		case TYPEEQUAL:
			if(type == actionList[i+1])
				doit = true;
			++i;
			break;
		case COLOREQUAL:
			if(color == actionList[i+1])
				doit = true;
			++i;
			break;
		case ANGLEGREAT:
			if(angle >= actionList[i+1])
				doit = true;
			++i;
			break;
		case ANGLEEQUAL:
			if(angle == actionList[i+1])
				doit = true;
			++i;
			break;
		case ANGLELESS:
			if(angle <= actionList[i+1])
				doit = true;
			++i;
			break;
		case ANGLERANGE:
			if(angle >= actionList[i+1] && angle <= actionList[i+2])
				doit = true;
			i+=2;
			break;

		case XGREAT:
			if(x >= actionList[i+1])
				doit = true;
			++i;
			break;
		case XLESS:
			if(x <= actionList[i+1])
				doit = true;
			++i;
			break;
		case XRANGE:
			if(x >= actionList[i+1] && x <= actionList[i+2])
				doit = true;
			i+=2;
			break;
		case YGREAT:
			if(y >= actionList[i+1])
				doit = true;
			++i;
			break;
		case YLESS:
			if(y <= actionList[i+1])
				doit = true;
			++i;
			break;
		case YRANGE:
			if(y >= actionList[i+1] && y <= actionList[i+2])
				doit = true;
			i+=2;
			break;

		case VALGREAT:
			if(CAST(scr.d[actionList[i+1]]) >= actionList[i+2])
				doit = true;
			i+=2;
			break;
		case VALEQUAL:
			if(CAST(scr.d[actionList[i+1]]) == actionList[i+2])
				doit = true;
			i+=2;
			break;
		case VALLESS:
			if(CAST(scr.d[actionList[i+1]]) <= actionList[i+2])
				doit = true;
			i+=2;
			break;
		case VALRANGE:
			if(CAST(scr.d[actionList[i+1]]) >= actionList[i+2] && CAST(scr.d[actionList[i+1]]) <= actionList[i+3])
				doit = true;
			i+=3;
			break;

		case SPEEDGREAT:
			if(speed*BULLETACT_FLOATSCALE >= actionList[i+1])
				doit = true;
			++i;
			break;
		case SPEEDEQUAL:
			if(speed*BULLETACT_FLOATSCALE == actionList[i+1])
				doit = true;
			++i;
			break;
		case SPEEDLESS:
			if(speed*BULLETACT_FLOATSCALE <= actionList[i+1])
				doit = true;
			++i;
			break;
		case SPEEDRANGE:
			if(speed*BULLETACT_FLOATSCALE >= actionList[i+1] && speed*BULLETACT_FLOATSCALE <= actionList[i+2])
				doit = true;
			i+=2;
			break;

		case TYPESET:
			if(doit)
			{
				oldtype = actionList[i+1];
				typechangetimer = 1;
				SE::push(SE_BULLET_CHANGE_2, x);
			}
			++i;
			doit = false;
			break;
		case COLORSET:
			if(doit)
			{
				color = actionList[i+1];
				SE::push(SE_BULLET_CHANGE_2, x);
			}
			++i;
			doit = false;
			break;

		case ANGLESET:
			if(doit)
			{
				angle = actionList[i+1];
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			++i;
			doit = false;
			break;
		case ANGLESETADD:
			if(doit)
			{
				angle += actionList[i+1];
				if(actionList[i+1] > BULLETACT_ANGLECHANGESE || actionList[i+1] < -BULLETACT_ANGLECHANGESE)
					SE::push(SE_BULLET_CHANGE_1, x);
			}
			++i;
			doit = false;
			break;
		case HEADANGLESET:
			if(doit)
				headangle = actionList[i+1];
			++i;
			doit = false;
			break;
		case HEADANGLESETADD:
			if(doit)
				headangle += actionList[i+1];
			++i;
			doit = false;
			break;
		case ANGLESETRMA:
			if(doit)
			{
				angle = rMainAngle(actionList[i+1]*1.0f,actionList[i+2]*1.0f,actionList[i+3]*1.0f);
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			i+=3;
			doit = false;
			break;
		case ANGLESETRMAP:
			if(doit)
			{
				angle = rMainAngle(Player::p.x,Player::p.y,actionList[i+1]*1.0f);
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			++i;
			doit = false;
			break;
		case ANGLESETRMAT:
			if(doit)
			{
				angle = rMainAngle(tar[actionList[i+1]].x,tar[actionList[i+1]].y,actionList[i+2]*1.0f);
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			i+=2;
			doit = false;
			break;
		case ANGLESETAMA:
			if(doit)
			{
				angle = aMainAngle(actionList[i+1]*1.0f,actionList[i+2]*1.0f,actionList[i+3]);
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			i+=3;
			doit = false;
			break;
		case ANGLESETAMAP:
			if(doit)
			{
				angle = aMainAngle(Player::p.x,Player::p.y,actionList[i+1]);
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			++i;
			doit = false;
			break;
		case ANGLESETAMAT:
			if(doit)
			{
				angle = aMainAngle(tar[actionList[i+1]].x,tar[actionList[i+1]].y,actionList[i+2]);
				SE::push(SE_BULLET_CHANGE_1, x);
			}
			i+=2;
			doit = false;
			break;
		case ANGLEADJUST:
			if(doit)
				angle = aMainAngle(lastx, lasty) + 18000;
			doit = false;
			break;

		case XSET:
			if(doit)
			{
				x = (float)actionList[i+1];
				SE::push(SE_BULLET_CHANGE_2, x);
			}
			++i;
			doit = false;
			break;
		case YSET:
			if(doit)
			{
				y = (float)actionList[i+1];
				SE::push(SE_BULLET_CHANGE_2, x);
			}
			++i;
			doit = false;
			break;
		case XSETADD:
			if(doit)
				x += actionList[i+1]/BULLETACT_FLOATSCALE;
			++i;
			doit = false;
			break;
		case YSETADD:
			if(doit)
				y += actionList[i+1]/BULLETACT_FLOATSCALE;
			++i;
			doit = false;
			break;
		case XSETACCADD:
			if(doit && timer > (DWORD)actionList[i+1])
				x += (int)(timer - actionList[i+1]) * actionList[i+2]/BULLETACT_FLOATSCALE;
			i+=2;
			doit = false;
			break;
		case YSETACCADD:
			if(doit && timer > (DWORD)actionList[i+1])
				y += (int)(timer - actionList[i+1]) * actionList[i+2]/BULLETACT_FLOATSCALE;
			i+=2;
			doit = false;
			break;
		case SPEEDSET:
			if(doit)
			{
				if (speed == 0)
				{
					SE::push(SE_BULLET_CHANGE_1, x);
				}
				speed = actionList[i+1]/BULLETACT_FLOATSCALE;
			}
			++i;
			doit = false;
			break;
		case SPEEDSETADD:
			if(doit)
			{
				if(speed == 0)
					SE::push(SE_BULLET_CHANGE_2, x);
				speed += actionList[i+1]/BULLETACT_FLOATSCALE;
			}
			++i;
			doit = false;
			break;

		case VALSET:
			if(doit)
			{
				CINT(scr.d[actionList[i+1]].value) = actionList[i+2];
				scr.d[actionList[i+1]].bfloat = false;
			}
			i+=2;
			doit = false;
			break;
		case VALSETADD:
			if(doit)
			{
				CINT(scr.d[actionList[i+1]].value) += actionList[i+2];
				scr.d[actionList[i+1]].bfloat = false;
			}
			i+=2;
			doit = false;
			break;

		case CALLSCRIPT:
			if (doit)
			{
				scr.functionExecute(actionList[i+1], actionList[i+2]);
			}
			i+=2;
			doit = false;
			break;
		case CHASE:
			if (doit)
			{
				actionList[i+2]--;
				chaseAim(tar[actionList[i+1]].x, tar[actionList[i+1]].y, actionList[i+2]);
				/*
				angle = aMainAngle(tar[actionList[i+1]].x, tar[actionList[i+1]].y);
				if (actionList[i+2] > 1)
				{
					speed = 2 * DIST(x, y, tar[actionList[i+1]].x, tar[actionList[i+1]].y) / actionList[i+2];
				}
				else
				{
					speed = 0.1f;
				}
				*/
			}
			i+=2;
			doit = false;
			break;

		case AND:
			if(!doit)
				for(++i;i<BULLETACTIONMAX;++i)
					if(actionList[i] == THEN)
						break;
			doit = false;
			break;
		case OR:
			if(doit)
			{
				for(++i;i<BULLETACTIONMAX;++i)
					if(actionList[i] == THEN)
						break;
			}
			else
				doit = false;
			break;
		case NOT:
			doit = !doit;
			break;
//		case ANDSET:
//			doit = true;
//			break;

		case REMAIN:
			if(doit)
				remain = true;
			break;
		case FADEOUT:
			if(doit)
			{
				fadeout = true;
				timer = 0;
			}
			break;
		case ITEMLIZE:
			if(doit)
			{
				itemlize();
			}
			break;
		case POINTLIZE:
			if(doit)
			{
				pointlize();
			}
			break;
		case BOUNCE:
			if (doit)
			{
				if (actionList[i+2])
				{
					if (x < M_ACTIVECLIENT_LEFT + actionList[i+1] || x > M_ACTIVECLIENT_RIGHT - actionList[i+1])
					{
						actionList[i+2]--;
						SE::push(SE_BULLET_CHANGE_2, x);
						angle = 18000 - angle;
					}
					if (y < M_ACTIVECLIENT_TOP + actionList[i+1] || y > M_ACTIVECLIENT_BOTTOM - actionList[i+1])
					{
						actionList[i+2]--;
						SE::push(SE_BULLET_CHANGE_2, x);
						angle = -angle;
					}
				}
			}
			i+=2;
			doit = false;
			break;
		}
	}
}
