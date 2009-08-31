#include "EffectSp.h"
#include "Player.h"
#include "Enemy.h"

HTEXTURE EffectSp::tex;
BYTE EffectSp::evtype;

EffectSp es[EFFECTSPMAX];

EffectSp::EffectSp()
{
	sprite = NULL;
}

EffectSp::~EffectSp()
{
	if(sprite)
		delete sprite;
	sprite = NULL;
}

void EffectSp::Render()
{
	sprite->SetColor((alpha<<24)|diffuse);
	sprite->RenderEx(x, y, ARC(angle+headangle), hscale, vscale);
}

void EffectSp::valueSet(BYTE _type, float _x, float _y, int _angle, float _speed, bool _onplayer, WORD _ID)
{
	ID			= _ID;
	type		= _type;
	x			= _x;
	y			= _y;
	angle		= _angle;
	speed		= _speed;
	onplayer	= _onplayer;

	timer = 0;
	exist = true;
	hscale = 1;
	vscale = 0;
	headangle = 0;

	if(!sprite)
		sprite = new hgeSprite(tex, 0, 0, 64, 64);
	sprite->SetTexture(tex);

	colorSet(0xffffff);

	switch(type)
	{
	case EFFECT_PLAYERCHANGE:
		sprite->SetTextureRect(0, 0, 64, 64);
		sprite->SetBlendMode(BLEND_ALPHAADD);
		alpha = 0x7f;
		hscale = 2.0f;
		break;
	case EFFECT_PLAYERGRAZE:
		sprite->SetTextureRect(64, 0, 64, 64);
		alpha = 0xff;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERINFI:
		sprite->SetTextureRect(128, 0, 64, 64);
		sprite->SetBlendMode(BLEND_ALPHAADD);
		alpha = 0x66;
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERSHOT:
		sprite->SetTextureRect(192, 0, 64, 64);
		alpha = 0xcc;
		hscale = 1.2f;
		break;
	case EFFECT_PLAYERCIRCLE:
		sprite->SetTextureRect(128, 64, 32, 32);
		alpha = 0xC0;
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERCIRCLEBOX:
		sprite->SetTextureRect(128, 224, 128, 32);
		alpha = 0xff;
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERPOINT:
		sprite->SetTextureRect(160, 64, 32, 32);
		alpha = 0xff;
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERCOLLAPSE:
		sprite->SetTextureRect(192, 64, 64, 64);
		alpha = 0x7f;
		hscale = 1.0f;
		break;
/*
	case EFFECT_HOLYDARK:
		sprite->SetTextureRect(0, 544, 128, 128);
		alpha = 0xbd;
		hscale = 1.0f;
		break;
*/
	case EFFECT_ENEMYCOLLAPSE:
		sprite->SetTextureRect(192, 64, 64, 64);
		alpha = 0xff;
		hscale = 1.0f;
		break;

	case EFFECT_ENEMYCIRCLE0:
		sprite->SetTexture(Enemy::tex[0]);
		sprite->SetTextureRect(0, 448, 64, 64);
		alpha = 0x7f;
		colorSet(0xffffff);
		hscale = 1.0f;
		break;
	case EFFECT_ENEMYCIRCLE1:
		sprite->SetTexture(Enemy::tex[0]);
		sprite->SetTextureRect(64, 448, 64, 64);
		alpha = 0x7f;
		colorSet(0xffffff);
		hscale = 1.0f;
		break;
/*
	case EFFECT_BOSSSTORE:
		sprite->SetTextureRect(64, 0, 64, 64);
		alpha = 0x7f;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		hscale = 1.5f;
		headangle = rand()&1;
		break;

	case EFFECT_ENVIRONMENT_EX:
		sprite->SetTextureRect(128, 128, 64, 64);
		alpha = 0x3f;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		hscale = (rand()%5)/30.0f + 0.1f;
		sprite->quad.v[3].z = sprite->quad.v[2].z = sprite->quad.v[1].z = sprite->quad.v[0].z = -rand()%200;
		colorSet(((rand()&0xff)<<16)|((rand()&0xff)<<8)|(rand()&0xff));
		headangle = (rand()&1)*20-10;
		break;
	case EFFECT_ENVIRONMENT_EX_2:
		sprite->SetTextureRect(192, 128, 64, 64);
		alpha = 0x3f;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		hscale = (rand()%5)/20.0f + 0.1f;
		sprite->quad.v[3].z = sprite->quad.v[2].z = sprite->quad.v[1].z = sprite->quad.v[0].z = rand()%200;
		colorSet(((rand()&0xff)<<16)|((rand()&0xff)<<8)|(rand()&0xff));
		headangle = -10;
		break;
*/
	case EFFECT_BOMB_0:
		sprite->SetTexture(Player::tex);
		sprite->SetTextureRect(0, 148, 128, 40);
		alpha = 0xcf;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		break;
	case EFFECT_BOMB_1:
		sprite->SetTexture(Player::tex);
		sprite->SetTextureRect(128, 148, 128, 40);
		alpha = 0xcf;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		break;
	case EFFECT_BOMB_2:
		sprite->SetTexture(Player::tex);
		sprite->SetTextureRect(128, 192, 64, 64);
		alpha = 0xcf;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		break;
	case EFFECT_BOMB_3:
		sprite->SetTexture(Player::tex);
		sprite->SetTextureRect(192, 192, 64, 64);
		alpha = 0xcf;
		sprite->SetBlendMode(BLEND_ALPHAADD);
		break;
	case EFFECT_BOMB_CUTIN:
		sprite->SetTexture(Player::texcutin);
		sprite->SetTextureRect(Player::p.ID%4*256, Player::p.ID/4*256, 256, 256);
		hscale = vscale = 1.40625f;
		alpha = 0x7f;
		break;
	case EFFECT_BOMB_NAME:
		sprite->SetTexture(Player::texcutin);
		sprite->SetTextureRect(0, Player::p.ID*32+768, 128, 32);
		hscale = vscale = 1.40625f;
		alpha = 0xcf;
		break;

	case EFFECT_OVERALL:
		sprite->SetTextureRect(992, 992, 32, 32);
		alpha = 0x3f;
		break;
	}
}

void EffectSp::colorSet(DWORD color)
{
	diffuse = color;
}

void EffectSp::action()
{
	timer++;

	if(onplayer)
	{
		headangle += angle;
		x = Player::p.x;
		y = Player::p.y;

		if(type == EFFECT_PLAYERCIRCLE)
		{
			float tabsspeed = fabsf(speed);
			float tangle = timer*500*(speed > 0 ? -1 : 1) + ID*6000;
			x += tabsspeed*8*sint(tangle);
			y += tabsspeed*8*cost(tangle);
			if(tabsspeed <= 4)
			{
				alpha = tabsspeed * 48;
			}
			else
				alpha = 0xC0;

			int tdiffer = Player::p.nPop - ID * 200;
			if(tdiffer >= 200)
				hscale = 1;
			else if(tdiffer <= 0)
				hscale = 0;
			else
				hscale = (float)tdiffer / 200;
		}
	}
	else
	{
		x += speed * cost(angle);
		y += speed * sint(angle);

		if(type > 0x80)
		{
			if(headangle > 0)
				headangle += ANGLE(speed / 15);
			else
				headangle -= ANGLE(speed / 15);
		}
/*
		if(type == EFFECT_BOSSSTORE)
		{
			if(!en[ENEMY_MAINBOSSINDEX].able)
				exist = false;
			else if(fabsf(en[ENEMY_MAINBOSSINDEX].x - x) < 8 && fabsf(en[ENEMY_MAINBOSSINDEX].y - y) < 8)
			else if (checkCollisionSquare(en[ENEMY_MAINBOSSINDEX], 8))
				exist = false;
		}
		else if(type == EFFECT_ENVIRONMENT_EX)
		{
			y++;

			sprite->quad.v[0].z += 2*speed;
			sprite->quad.v[1].z += 2*speed;
			sprite->quad.v[2].z += 2*speed;
			sprite->quad.v[3].z += 2*speed;
			if(x < 0 || x > 440 || y < 0 || y > 480)
			{
				valueSet(EFFECT_ENVIRONMENT_EX, rand()%(int)M_ACTIVECLIENT_WIDTH, rand()%(int)M_ACTIVECLIENT_HEIGHT, rand()%6000+6000, (rand()%5)/15.0f+0.5f, false);
			}
		}
		else if(type == EFFECT_ENVIRONMENT_EX_2)
		{
			angle = aMainAngle(Player::p, 6000);
	
			if(sprite->quad.v[0].z > 0)
			{
				sprite->quad.v[0].z -= 2*speed;
				sprite->quad.v[1].z -= 2*speed;
				sprite->quad.v[2].z -= 2*speed;
				sprite->quad.v[3].z -= 2*speed;
			}
			else
			{
				hscale *= 0.995f;
			}
			if(hscale < 0.1f)
			{
				valueSet(EFFECT_ENVIRONMENT_EX_2, rand()%(int)M_ACTIVECLIENT_WIDTH, rand()%(int)M_ACTIVECLIENT_HEIGHT, 0, (rand()%5)/15.0f+0.5f, false);
			}
		}
		else if(type == EFFECT_OVERALL)
		{
			if(timer <= 32)
				hscale += ARC(headangle);
			else
			{
				hscale = 0;
				exist = false;
				timer = 0;
			}
		}
*/
	}
}
/*
void EffectSp::environmentSet(BYTE _evtype, BYTE num)
{
	evtype = _evtype;
	if(evtype == 0)
	{
		for(int i=0;i<EFFECTSPMAX;i++)
			es[i].exist = false;
		return;
	}
	for(int i=0;i<num;i++)
	{
		es[i].valueSet(evtype, rand()%(int)M_ACTIVECLIENT_WIDTH, rand()%(int)M_ACTIVECLIENT_HEIGHT, rand()%6000+6000, (rand()%5)/15.0f+0.5f, false);
	}
	for(int i=num;i<EFFECTSPMAX;i++)
		es[i].exist = false;
}
*/