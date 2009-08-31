#include "PlayerGhost.h"
#include "Player.h"
#include "Main.h"
#include "Process.h"

HTEXTURE PlayerGhost::tex;

PlayerGhost::PlayerGhost()
{
	sprite	= NULL;
}

PlayerGhost::~PlayerGhost()
{
	if(sprite)
		delete sprite;
	sprite = NULL;
}

void PlayerGhost::valueSet(WORD _ID)
{
	ID		=	_ID;

	timer	=	0;
	x		=	PL_MERGEPOS_X;
	y		=	PL_MERGEPOS_Y;
	exist	=	true;
	speed	=	1;
	angle	=	0;
	headangle =	0;
	alpha	=	0xce;
	scale	=	1;

	if(!sprite)
		sprite = new hgeSprite(tex, 0, 0, 32, 32);
	sprite->SetTexture(tex);
	sprite->SetTextureRect(64, 64, 32, 32);

	sprite->SetBlendMode(BLEND_ALPHAADD);

	matchColor();
}

void PlayerGhost::Render()
{
	sprite->SetColor((alpha<<24)|0xffffff);
	sprite->RenderEx(x, y, timer, scale/2.5f);
	sprite->SetColor((alpha<<24)|diffuse);
	sprite->RenderEx(x, y, timer, scale);
}

void PlayerGhost::action()
{
	timer++;

	float aimx = x;
	float aimy = y;

	switch(ID)
	{
	case 0:
	case 4:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX]-40;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX];
		break;
	case 1:
	case 5:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 40;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX];
		break;
	case 2:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 20;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 20;
		break;
	case 3:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 20;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 20;
		break;
	case 6:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 20;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 20;
		break;
	case 7:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 20;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 20;
		break;
		//
	case 8:
		aimx = Player::p.lastmx[7];
		aimy = Player::p.lastmy[7];
		break;
	case 9:
		aimx = Player::p.lastmx[15];
		aimy = Player::p.lastmy[15];
		break;
	case 10:
		aimx = Player::p.lastmx[23];
		aimy = Player::p.lastmy[23];
		break;
	case 11:
		aimx = Player::p.lastmx[31];
		aimy = Player::p.lastmy[31];
		break;
	case 12:
		if(timer < PLAYERGHOST_TRACETIME)
		{
			aimx = Player::p.x - 50;
			aimy = Player::p.y;
		}
		break;
	case 13:
		if(timer < PLAYERGHOST_TRACETIME)
		{
			aimx = Player::p.x + 50;
			aimy = Player::p.y;
		}
		break;
	case 14:
		if(timer < PLAYERGHOST_TRACETIME)
		{
			aimx = Player::p.x - 25;
			aimy = Player::p.y - 30;
		}
		break;
	case 15:
		if(timer < PLAYERGHOST_TRACETIME)
		{
			aimx = Player::p.x + 25;
			aimy = Player::p.y - 30;
		}
		break;
		//
	case 16:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 60;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 10;
		break;
	case 17:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 60;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 10;
		break;
	case 18:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 30;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 20;
		break;
	case 19:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 30;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 20;
		break;
	case 20:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 10;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 5;
		break;
	case 21:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 10;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 5;
		break;
	case 22:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 20;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 20;
		break;
	case 23:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 20;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 20;
		break;
	case 24:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 40;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 40;
		break;
	case 25:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 40;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 40;
		break;
	case 26:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 40;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 40;
		break;
	case 27:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 40;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] + 40;
		break;
	case 28:
	case 30:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] - 15;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 20;
		break;
	case 29:
	case 31:
		aimx = Player::p.lastx[PLAYERGHOST_LASTINDEX] + 15;
		aimy = Player::p.lasty[PLAYERGHOST_LASTINDEX] - 20;
	}
	if(ID == 10 || ID == 11)
	{
		while(angle >= 27000)
			angle -=36000;
		while(angle < -9000)
			angle += 36000;
		if(aimx == x)
		{
			if(hge->Input_GetDIKey(KS_UP_MP) && angle != -9000)
			{
				if(angle > 9000)
				{
					angle += 200;
					if(angle >= 27000)
						angle = -9000;
				}
				else if(angle < 9000)
				{
					angle -= 200;
					if(angle < -9000)
						angle = -9000;
				}
			}
			if(hge->Input_GetDIKey(KS_DOWN_MP) && angle != 9000)
			{
				if(angle < 9000)
				{
					angle += 200;
					if(angle > 9000)
						angle = 9000;
				}
				else if(angle > 9000)
				{
					angle -= 200;
					if(angle < 9000)
						angle = 9000;
				}
			}
		}
		while(angle >= 36000)
			angle -= 36000;
		while(angle < 0)
			angle += 36000;
		if(aimy == y)
		{
			if(hge->Input_GetDIKey(KS_LEFT_MP) && angle != 18000)
			{
				if(angle < 18000)
				{
					angle += 200;
					if(angle > 18000)
						angle = 18000;
				}
				else if(angle > 18000)
				{
					angle -= 200;
					if(angle < 18000)
						angle = 18000;
				}
			}
			if(hge->Input_GetDIKey(KS_RIGHT_MP) && angle != 0)
			{
				if(angle > 18000)
				{
					angle += 200;
					if(angle >= 36000)
						angle = 0;
				}
				else if(angle < 18000)
				{
					angle -= 200;
					if(angle < 0)
						angle = 0;
				}
			}
		}
		if((aimx != x || aimy != y) &&timer >= PLAYERGHOST_TRACETIME)
			angle = aMainAngle(Player::p);
	}
	if(timer < PLAYERGHOST_TRACETIME)
	{
		chaseAim(aimx, aimy, PLAYERGHOST_TRACETIME-timer);
		/*
		angle = aMainAngle(aimx, aimy);
		speed = 2 * DIST(x, y, aimx, aimy) / (PLAYERGHOST_TRACETIME-timer);
		*/
		x += speed * cost(angle);
		y += speed * sint(angle);
	}
	else
	{
		x = aimx;
		y = aimy;
	}
	scale = ROLL(timer, 10)/20.0f+0.8f;
}

void PlayerGhost::matchColor()
{
	switch(ID>>2)
	{
	case M_CHARAENUM_SHI:
		diffuse = 0xffffff;
		break;
	case M_CHARAENUM_LINGYE:
		diffuse = 0x33cccc;
		break;
	case M_CHARAENUM_NUSIKUIMA:
		diffuse = 0x663300;
		break;
	case M_CHARAENUM_GEJI:
		diffuse = 0xff3399;
		break;
	case M_CHARAENUM_CILAN:
		diffuse = 0x000099;
		break;
	case M_CHARAENUM_ZANGYUE:
		diffuse = 0x003366;
		break;
	case M_CHARAENUM_WUSHUANG:
	case M_CHARAENUM_BUSINIAO:
		diffuse = 0xff3300;
		break;
	default:
		diffuse = 0;
	}
}