#include "PlayerBullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Ghost.h"
#include "SE.h"
#include "Main.h"

VectorList<PlayerBullet>pb;

hgeSprite * PlayerBullet::spPlayerBullet[PLAYERBULLETSPRITEMAX];

HTEXTURE PlayerBullet::tex;

int PlayerBullet::locked = PBLOCK_LOST;

DWORD PlayerBullet::bcol0;
DWORD PlayerBullet::bcol1;
DWORD PlayerBullet::bcol2;
DWORD PlayerBullet::bcol3;
WORD PlayerBullet::beams;

PlayerBullet::PlayerBullet()
{
	able	= false;
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Init_Player(HTEXTURE _tex)
{
	tex = _tex;
	Release();
	pb.init(PLAYERBULLETMAX);
	for(int i=0;i<PLAYERBULLETSPRITEMAX;i++)
	{
		spPlayerBullet[i] = new hgeSprite(PlayerBullet::tex, (float)((i%4)*32), (float)(192+(i/4)*32), 32, 32);
		spPlayerBullet[i]->SetBlendMode(BLEND_DEFAULT);
	}
	if (Player::p.ID>>1 == M_CHARA_WUSHUANG)
	{
		PlayerBullet::spPlayerBullet[6]->SetTextureRect(0, 224, 256, 32);
		PlayerBullet::spPlayerBullet[6]->SetBlendMode(BLEND_ALPHAADD);
	}
}

void PlayerBullet::valueSet(WORD _ID, BYTE _arrange, int _angle, float _speed, float _power)
{
	ID		=	_ID;
	angle	=	_angle + 27000;
	speed	=	_speed;
	oldspeed =	speed;
	power	=	_power;
	arrange	=	_arrange;

	timer	=	0;
	exist	=	true;
	able	=	true;
	fadeout	=	false;
	hscale	=	1.0f;
	vscale	=	1.0f;
	alpha	=	time & 1;
	headangle =	0;

	color	=	0x80ffffff;

	xplus	=	speed * cost(angle);
	yplus	=	speed * sint(angle);

	locktimer = 0;

	if(ID == 4 || ID == 5)
		hscale = 1.5f;

	if(arrange)
	{
		x = Player::p.pg[arrange-1].x;
		y = Player::p.pg[arrange-1].y;
	}
	else
	{
		x = Player::p.x;
		y = Player::p.y;
	}
	if (!SE::have(SE_PLAYER_SHOOT_1+ID))
	{
		SE::push(SE_PLAYER_SHOOT_1+ID, x);
	}
}

void PlayerBullet::Release()
{
	for(int i=0;i<PLAYERBULLETSPRITEMAX;i++)
	{
		if(spPlayerBullet[i])
			delete spPlayerBullet[i];
		spPlayerBullet[i] = NULL;
	}
	pb.clear();
}

void PlayerBullet::Render()
{
	spPlayerBullet[ID]->SetColor(color);
	spPlayerBullet[ID]->RenderEx(x, y, ARC(angle+headangle), hscale, vscale);
}

void PlayerBullet::RenderBeam()
{
	//shadow
	spPlayerBullet[6]->SetColor(bcol0 | 0x10000000, bcol1 | 0x10000000, bcol2 | 0x10000000, bcol3 | 0x10000000);
	spPlayerBullet[6]->RenderEx(Player::p.lastx[15], Player::p.lasty[15], -M_PI_2, 2.0f, Player::p.nPower/200.0f+1.0f);
	spPlayerBullet[6]->SetColor(bcol0 | 0x20000000, bcol1 | 0x20000000, bcol2 | 0x20000000, bcol3 | 0x20000000);
	spPlayerBullet[6]->RenderEx(Player::p.lastx[11], Player::p.lasty[11], -M_PI_2, 2.0f, Player::p.nPower/200.0f+1.0f);
	spPlayerBullet[6]->SetColor(bcol0 | 0x40000000, bcol1 | 0x40000000, bcol2 | 0x40000000, bcol3 | 0x40000000);
	spPlayerBullet[6]->RenderEx(Player::p.lastx[6], Player::p.lasty[6], -M_PI_2, 2.0f, Player::p.nPower/200.0f+1.0f);
	//beam
	spPlayerBullet[6]->SetColor(bcol0 | 0x80000000, bcol1 | 0x80000000, bcol2 | 0x80000000, bcol3 | 0x80000000);
	spPlayerBullet[6]->RenderEx(Player::p.x, Player::p.y, -M_PI_2, 2.0f, Player::p.nPower/200.0f+1.0f);
}

void PlayerBullet::Lock()
{
	if(locked == PBLOCK_LOST)
	{
		if(speed < oldspeed)
			speed += PBLOCK_ACCSPEED;
		if(speed > oldspeed)
			speed = oldspeed;
		return;
	}

	BObject * _tobj;
	if (locked < PBLOCK_GHOST)
	{
		_tobj = &en[locked];
	}
	else
	{
		_tobj = &gh[locked-PBLOCK_GHOST];
	}

	int aimangle = aMainAngle(*_tobj);
	bool clockwise = false;

	if(locktimer >= PBLOCK_LOCKTIMERMAX)
	{
		locktimer = PBLOCK_LOCKTIMERMAX;
		angle = aimangle;
	}

	while(angle > 18000)
		angle -= 36000;
	while(angle < -18000)
		angle += 36000;
	while(aimangle > 18000)
		aimangle -= 36000;
	while(aimangle < -18000)
		aimangle += 36000;

	int incangle = aimangle - angle;
	if(incangle > 18000)
		incangle = incangle - 36000;
	else if(incangle < -18000)
		incangle = 36000 + incangle;

	if(incangle > 0 && incangle < 18000)
		clockwise = true;

	incangle = abs(incangle);

	if(timer == 1 && incangle > 9000 && _tobj->y > y)
	{
		if(_tobj->x > x)
			angle = aimangle - 9000;
		else
			angle = aimangle + 9000;
	}

	if(incangle < PBLOCK_TURNMAX)
	{
		angle = aimangle;
		if(speed < oldspeed)
			speed += PBLOCK_ACCSPEED;
		if(speed > oldspeed)
			speed = oldspeed;
	}
	else
	{
		if(speed > PBLOCK_MINSPEED)
			speed -= PBLOCK_ACCSPEED;
		if(speed < PBLOCK_MINSPEED)
			speed = PBLOCK_MINSPEED;
		if(clockwise)
			angle += PBLOCK_TURNMAX;
		else
			angle -= PBLOCK_TURNMAX;
	}
}

void PlayerBullet::hitOn()
{
	fadeout = true;
	able = false;
	ID++;
	timer = 0;
}

bool PlayerBullet::isInRange(float aimx, float aimy, bool hiton)
{
	float r = Player::p.bCircle ? PB_GHOSTCOLLISIONR_CIRCLE : PB_GHOSTCOLLISIONR;
	if (checkCollisionSquare(aimx, aimy, r))
	{
		if (hiton)
		{
			hitOn();
		}
		return true;
	}
	return false;
	
	/*
	if (fabsf(_x - x) < r && fabsf(_y - y) < r)
	{
		if (hiton)
		{
			hitOn();
		}
		return true;
	}
	return false;
	*/
}

void PlayerBullet::action()
{
	timer++;

	if(!fadeout)
	{
		if(Player::p.ID >= M_CHARAENUM_NUSIKUIMA || ID < 6)
		{
			if(Player::p.ID < M_CHARAENUM_NUSIKUIMA && ID == 2)
			{
				if(locked == PBLOCK_LOST)
				{
					if(timer < 32)
						timer = 32;
					if(speed != oldspeed)
					{
						speed = oldspeed;
					}
				}
				else
				{
					BObject * _tobj;
					if (locked < PBLOCK_GHOST)
					{
						_tobj = &en[locked];
					}
					else
					{
						_tobj = &gh[locked-PBLOCK_GHOST];
					}
					if(timer == 1)
					{
						speed = -speed;
					}
					if(timer < 32)
					{
						angle = aMainAngle(*_tobj);
						if(arrange & 1)
							headangle += 1200;
						else
							headangle -= 1200;
						speed *= 0.8f;
					}
					else if(timer == 32)
					{
						angle = aMainAngle(*_tobj);
						headangle = 0;
						speed = oldspeed;
					}
				}
				xplus = speed * cost(angle);
				yplus = speed * sint(angle);
			}
			x += xplus;
			y += yplus;

			locktimer = 0;
		}
		else
		{
			locktimer++;
			Lock();
			x += speed * cost(angle);
			y += speed * sint(angle);
		}

		if(x < PB_DELETE_LEFT || x > PB_DELETE_RIGHT || y < PB_DELETE_TOP || y > PB_DELETE_BOTTOM)
			exist = false;
	}
	else
	{
		if(timer == 1)
		{
			Player::p.nPop += PL_SHOOTBONUS_NPOP;
		}
		else if(timer == 32)
			exist = false;
		y -= 1;
		if(alpha)
			headangle = PB_HEADARCPLUS;
		else
			headangle = -PB_HEADARCPLUS;
		color -= 0x4000000;
		hscale = timer * 0.1f + 1.0f;
		vscale = hscale;
	}
	able = exist && !fadeout;
}