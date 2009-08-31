#include "Enemy.h"
#include "Player.h"
#include "SE.h"
#include "Item.h"
#include "EffectSp.h"
#include "Scripter.h"
#include "Chat.h"
#include "BossInfo.h"
#include "Process.h"

Enemy en[ENEMYMAX];

HTEXTURE Enemy::tex[ENEMY_TEXMAX];
WORD Enemy::index;
BYTE Enemy::bossflag[ENEMY_BOSSMAX];
BYTE Enemy::rangeuptimer[ENEMY_BOSSMAX];
BYTE Enemy::storetimer[ENEMY_BOSSMAX];

Enemy::Enemy()
{
	exist	= false;
	able	= false;
	sprite	= NULL;
	eID		= 0;
	effCollapse.exist = false;
	effShot.exist = false;
}

Enemy::~Enemy()
{
	if(sprite)
		delete sprite;
	sprite = NULL;
}

void Enemy::Init(HTEXTURE tex0, HTEXTURE tex1, HTEXTURE tex2, HTEXTURE tex3)
{
	texAssign(tex0, tex1, tex2, tex3);
	index = ENEMY_INDEXSTART;
}

void Enemy::texAssign(HTEXTURE tex0, HTEXTURE tex1, HTEXTURE tex2, HTEXTURE tex3)
{
	tex[0] = tex0;
	if(tex1 == 0)
		tex[1] = tex0;
	else
		tex[1] = tex1;
	if(tex2 == 0)
		tex[2] = tex1;
	else
		tex[2] = tex2;
	if(tex3 == 0)
		tex[3] = tex2;
	else
		tex[3] = tex3;
}

bool Enemy::Build(WORD _eID, BYTE _tarID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer, DWORD take)
{
	WORD i;
	bool rv = false;
	if(type < ENEMY_BOSSTYPEBEGIN)
	{
		for(int j=0;j<ENEMYMAX-ENEMY_INDEXSTART;j++)
		{
			index++;
			if(index == ENEMYMAX || index < ENEMY_INDEXSTART)
			{
				index = ENEMY_INDEXSTART;
			}
			if(!en[index].exist)
			{
				rv = true;
				i = index;
				break;
			}
		}
	}
	else
	{
		rv = true;
		i = type - ENEMY_BOSSTYPEBEGIN;
		index = i;
	}
	if(!rv)
		return false;
	en[i].valueSet(i, x, y, angle, speed, type, life, infitimer, take);
	en[i].eID = _eID;
	en[i].tarID = _tarID;
	return true;
}

void Enemy::Render()
{
	sprite->SetColor(alpha<<24|diffuse);
	sprite->RenderEx(x, y, 0, hscale, vscale);

	if((type&1) && !(type>>4))
		esCircle.Render();
	if(fadeout && timer && effCollapse.eff)
	{
		effCollapse.Render();
	}
}

void Enemy::RenderEffect()
{
	effShot.Render();
}

void Enemy::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, float _life, int _infitimer, DWORD _take,
					 WORD _ac, float para0, float para1, float para2, float para3)
{
	ID		=	_ID;
	x		=	_x;
	lastx	=	x;
	y		=	_y;
	speed	=	_speed;
	type	=	_type;
	life	=	_life;
	ac		=	_ac;
	angle	=	_angle;
	take	=	_take;
	infitimer = _infitimer;

	timer	=	0;
	damagetimer = 0;
	maxlife	=	life;
	defrate	=	0;
	exist	=	true;
	fadeout	=	false;
	able	=	true;
	damage	=	false;
	hscale	=	1.0f;
	vscale	=	1.0f;
	alpha	=	0xff;
	diffuse	=	0xffffff;
	if (type >= ENEMY_BOSSTYPEBEGIN)
	{
		BYTE tbtnum = type - ENEMY_BOSSTYPEBEGIN;
		bossflag[tbtnum]		=	0;
		rangeuptimer[tbtnum]	=	0;
		storetimer[tbtnum]		=	0;
	}

	eID		=	0;
	tarID	=	0xff;

	para[0] = para0;
	para[1] = para1;
	para[2] = para2;
	para[3] = para3;

	headangle = -angle;

	if(!sprite)
		sprite = new hgeSprite(tex[0], 0, 0, 32, 32);
	sprite->SetTexture(tex[0]);		
	if(type >= ENEMY_BOSSTYPEBEGIN)
	{
		sprite->SetTexture(tex[type - ENEMY_BOSSTYPEBEGIN]);
		sprite->SetTextureRect(0, 0, 64, 64);
	}
	else
	{
		sprite->SetTextureRect(0, 384 + type/2*32, 32, 32);
	}

	effShot.valueSet(EFF_EN_SHOT, *this);
	effShot.Stop();

	if(type < ENEMY_BOSSTYPEBEGIN)
	{
		esCircle.valueSet(EFFECT_ENEMYCIRCLE0 + type/2, x, y, 0, 0, false);
	}
	else
	{
		esCircle.valueSet(EFFECT_ENEMYCOLLAPSE, x, y, 0, 0, false);
	}
/*
	if(take == 0xffffffff)
	{
		if(maxlife < 10)
		{
			if(ID % 3 < 2)
				take = 0x00000001;
			else
				take = 0x00000010;
		}
		else if(maxlife < 20)
		{
			if(ID & 1)
				take = 0x00000100;
			else
				take = 0x00000010;
		}
		else if(type < ENEMY_BOSSTYPEBEGIN)
		{
			take = 0x00000123;
		}
		else
		{
			take = 0x00000145;
		}
	}
*/
}

void Enemy::matchAction()
{
	switch(ac)
	{
	case 1:
		//x，y，angle，speed
		//直接设置（需要连续设置）
		x = para[0];
		y = para[1];
		angle = para[2];
		speed = para[3];
		break;
	case 4:
		//作用时间，摩擦力，退出角度
		//靠近主角
		if(timer < para[0])
		{
			angle = aMainAngle(Player::p);
			speed *= para[1]/1000.0f;
		}
		else
		{
			angle = (int)para[2];
//			speed /= para[1]/1000.0f;
		}
		break;
	case 7:
		//作用时间，摩擦力，停止时间，退出角度
		//向原方向前进、停止、撤离
		if(timer < para[0])
		{
			speed *= para[1]/1000.0f;
		}
		else if(timer < para[2])
			speed = 0;
		else
		{
			angle = (int)para[3];
			speed += 0.03f;
		}
		break;
	case 10:
		//[+-]起始时间，环绕中心x，环绕中心y，终止时间
		//绕圆周行走
		if(para[0] > 0)
		{
			if(timer > (DWORD)para[0] && timer < (DWORD)para[3])
			{
				float tr = DIST(x, y, para[1], para[2]);
				angle += ANGLE(speed / tr);
			}
		}
		else
		{
			if(timer > (DWORD)(-para[0]) && timer < (DWORD)para[3])
			{
				float tr = DIST(x, y, para[1], para[2]);
				angle -= ANGLE(speed / tr);
			}
		}
		break;
	case 13:
		//作用时间[计数器]，变角范围，更替周期，终止时间
		//折线行走
		if((int)para[0] && timer < para[3])
		{
			speed = speed * ((int)para[0] - 1) / (int)para[0];
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else if(timer < para[3])
		{
			para[0] = para[2];
			speed = para[2] / 30;
			if(para[1] > 0)
			{
				angle = para[1];
				para[1] = 18000 - para[1];
			}
			else
			{
				angle = 18000 + para[1];
				para[1] = -18000 - para[1];
			}
		}
		else
		{
			speed += 0.03f;
			angle = -9000;
		}
		break;
	case 16:
		//作用时间，停止时间，放弃时间
		//减速至0，后冲向Player
		if(timer < para[0])
		{
			speed -= speed / (para[0] - timer);
		}
		else if(timer < para[1])
		{
			speed = 0;
		}
		else if(timer < para[2])
		{
			speed += 0.06f;
			angle = aMainAngle(Player::p);
		}
		break;
	case 40:
		//作用时间[计数器]
		//BOSS出场复位
		if((int)para[0])
		{
			/*
			angle = aMainAngle(220, 120);
			if((int)para[0] != 1)
				speed = 2 * DIST(x, y, 220, 120) / para[0];
			else
				speed = 0.1f;
			*/
			chaseAim(220, 120, para[0]);
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else
		{
			ac = 0;
			speed = 0;
		}
		break;
	case 41:
		//作用时间[计数器]，目标x，目标y，更替周期
		//在主角上方随机
		if((int)para[0])
		{
			/*
			angle = aMainAngle(	para[1], para[2]);
			if((int)para[0] != 1)
				speed = 2 * DIST(x, y, para[1], para[2]) / para[0];
			else
				speed = 0.1f;
			*/
			chaseAim(para[1], para[2], para[0]);
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else if(timer % (int)para[3] == 0)
		{
			para[0] = 2 * para[3] > 120 ? 120 : 2 * para[3];
			if(Player::p.x > x)
				para[1] = Player::p.x + rand() % 60;
			else
				para[1] = Player::p.x - rand() % 60;
			if(para[1] < 60)
			{
				if(x <= 70)
					para[1] = 70 + rand() % 50;
				else
					para[1] = 60;
			}
			else if(para[1] > 380)
			{
				if(x >= 370)
					para[1] = 370 - rand() % 50;
				else
					para[1] = 380;
			}
			para[2] = rand() % 80 + 80;
		}
		break;
	case 42:
		//作用时间[计数器]，目标x，目标y
		//滑步走向点
		if((int)para[0])
		{
			/*
			angle = aMainAngle(para[1], para[2]);
			if((int)para[0] != 1)
				speed = 2 * DIST(x, y, para[1], para[2]) / para[0];
			else
				speed = 0.1f;
			*/
			chaseAim(para[1], para[2], para[0]);
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else
			speed = 0;
		break;
	}
}

void Enemy::setMove(float para0, float para1, float para2, float para3, WORD _ac)
{
	para[0] = para0;
	para[1] = para1;
	para[2] = para2;
	para[3] = para3;
	ac = _ac;
}

void Enemy::bossAction()
{
	BYTE tbtnum = type - ENEMY_BOSSTYPEBEGIN;
	if(timer < ENEMY_BOSSINFITIMER)
		defrate = 1.0f;
	else if(timer == ENEMY_BOSSINFITIMER)
		defrate = 0;
	if(!bossflag[tbtnum])
	{
		static bool first = true;
		if(lastx - x > ENEMY_BOSSMOVELIMIT)
		{
			if(first)
			{
				sprite->SetTextureRect(0, 64, 64, 64);
				first = false;
			}
			else
			{
				switch((timer / 8) % 3)
				{
				case 0:
					sprite->SetTextureRect(64, 64, 64, 64);
					break;
				case 1:
					sprite->SetTextureRect(128, 64, 64, 64);
					break;
				case 2:
					sprite->SetTextureRect(192, 64, 64, 64);
					break;
				}
			}
		}
		else if(x - lastx > ENEMY_BOSSMOVELIMIT)
		{
			if(first)
			{
				sprite->SetTextureRect(0, 128, 64, 64);
				first = false;
			}
			else
			{
				switch((timer / 8) % 3)
				{
				case 0:
					sprite->SetTextureRect(64, 128, 64, 64);
					break;
				case 1:
					sprite->SetTextureRect(128, 128, 64, 64);
					break;
				case 2:
					sprite->SetTextureRect(192, 128, 64, 64);
					break;
				}
			}
		}
		else
		{
			first = false;
			switch((timer / 8) % 4)
			{
			case 0:
				sprite->SetTextureRect(0, 0, 64, 64);
				break;
			case 1:
				sprite->SetTextureRect(64, 0, 64, 64);
				break;
			case 2:
				sprite->SetTextureRect(128, 0, 64, 64);
				break;
			case 3:
				sprite->SetTextureRect(192, 0, 64, 64);
				break;
			}
			sprite->SetHotSpot(32, sint(timer*512)*8+28);
		}
	}
	else if(bossflag[tbtnum] == BOSS_ATTACK)
	{
		switch((timer / 8) % 4)
		{
		case 0:
			sprite->SetTextureRect(0, 192, 64, 64);
			break;
		case 1:
			sprite->SetTextureRect(64, 192, 64, 64);
			break;
		case 2:
			sprite->SetTextureRect(128, 192, 64, 64);
			break;
		case 3:
			sprite->SetTextureRect(192, 192, 64, 64);
			break;
		}
	}
	else if(bossflag[tbtnum] == BOSS_RANGEUP)
	{
		if(rangeuptimer[tbtnum] == 0)
		{
			SE::push(SE_BOSS_POWER_2, x);
			sprite->SetTextureRect(0, 256, 64, 64);
		}
		else if(rangeuptimer[tbtnum] == 8)
			sprite->SetTextureRect(64, 256, 64, 64);
		else if(rangeuptimer[tbtnum] == 16)
			sprite->SetTextureRect(128, 256, 64, 64);
		else if(rangeuptimer[tbtnum] == 24)
			sprite->SetTextureRect(192, 256, 64, 64);
		else if(rangeuptimer[tbtnum] == 180)
		{
			bossflag[tbtnum] &= ~BOSS_RANGEUP;
			rangeuptimer[tbtnum] = 0;
		}
		rangeuptimer[tbtnum]++;
	}
	else if(bossflag[tbtnum] == BOSS_STORE)
	{
		storetimer[tbtnum]++;

		switch((timer / 8) % 4)
		{
		case 0:
			sprite->SetTextureRect(0, 192, 64, 64);
			break;
		case 1:
			sprite->SetTextureRect(64, 192, 64, 64);
			break;
		case 2:
			sprite->SetTextureRect(128, 192, 64, 64);
			break;
		case 3:
			sprite->SetTextureRect(192, 192, 64, 64);
			break;
		}

		if(storetimer[tbtnum] == 1)
		{
			SE::push(SE_BOSS_POWER_1, x);
			bossinfo.effStore.Stop(true);
			bossinfo.effStore.Fire();
			bossinfo.effStore.MoveTo(x, y, 0, true);
		}
		if(storetimer[tbtnum] < 80)
		{
			int xdif;
			int ydif;
			xdif = rand() % 50 + 120;
			ydif = rand() % 50 + 120;
			float rnd = rand();
			xdif *= cost(rnd);
			ydif *= sint(rnd);
			if(rand() & 1)
				xdif = -xdif;
			if(rand() & 1)
				ydif = -ydif;
		}
		else if(storetimer[tbtnum] == 120)
		{
			bossinfo.effStore.Stop();
			storetimer[tbtnum] = 0;
			bossflag[tbtnum] &= ~BOSS_STORE;
		}
		bossinfo.effStore.MoveTo(x, y);
	}
}


void Enemy::action()
{
	timer++;

	if(infitimer)
	{
		infitimer--;
		defrate = 1.0f;
		if(!infitimer)
			defrate = 0;
	}

	effShot.MoveTo(x, y);
	effShot.action();

	if(!fadeout)
	{
		if(Player::p.flag & PLAYER_BOMB || (Player::p.flag & PLAYER_DEADBOMB) && Player::p.ID == M_CHARAENUM_BUSINIAO)
		{
			life -= PLBOMB_ENEMYLIFECOST * (1 - defrate);
			damage = true;
			
			if(BossInfo::flag && BossInfo::range && type >= ENEMY_BOSSTYPEBEGIN)
			{
				life -= PLBOMB_BOSSLIFECOST(maxlife) * (1 - defrate);
			}
		}

		if((Chat::chatting || (BossInfo::flag >= BOSSINFO_COLLAPSE)) && type < ENEMY_BOSSTYPEBEGIN)
		{
//			esCircle.valueSet(EFFECT_ENEMYCOLLAPSE, x, y, 0, 0, false);
			life = 0;
			fadeout = true;
			timer = 0;
		}

		if(eID && !Scripter::stopEdefScript)
		{
			WORD tindex = index;
			index = ID;
			scr.edefExecute(eID, timer);
			index = tindex;
		}
		matchAction();

		x += speed * cost(angle);
		y += speed * sint(angle);

		esCircle.x = x;
		esCircle.y = y;
		int tcollapsehead = ANGLE((maxlife - life) / (maxlife+0.1f) * 0.05f + 0.03f);
		if(x > lastx)
			esCircle.headangle += tcollapsehead;
		else
			esCircle.headangle -= tcollapsehead;

		if(type >= ENEMY_BOSSTYPEBEGIN)
			bossAction();
		else
		{
			if(lastx - x > ENEMY_ENEMYMOVELIMIT)
			{
				switch((timer / 8) & 1)
				{
				case 0:
					sprite->SetTextureRect(128, 384 + type/2*32, 32, 32);
					break;
				case 1:
					sprite->SetTextureRect(160, 384 + type/2*32, 32, 32);
					break;
				}
			}
			else if(x - lastx > ENEMY_ENEMYMOVELIMIT)
			{
				switch((timer / 8) & 1)
				{
				case 0:
					sprite->SetTextureRect(192, 384 + type/2*32, 32, 32);
					break;
				case 1:
					sprite->SetTextureRect(224, 384 + type/2*32, 32, 32);
					break;
				}
			}
			else
			{
				switch((timer / 8) % 4)
				{
				case 0:
					sprite->SetTextureRect(0, 384 + type/2*32, 32, 32);
					break;
				case 1:
					sprite->SetTextureRect(32, 384 + type/2*32, 32, 32);
					break;
				case 2:
					sprite->SetTextureRect(64, 384 + type/2*32, 32, 32);
					break;
				case 3:
					sprite->SetTextureRect(96, 384 + type/2*32, 32, 32);
					break;
				}
			}
		}

		lastx = x;

		if(tarID != 0xff)
		{
			tar[tarID].x = x;
			tar[tarID].y = y;
		}

		if (!(Player::p.ID == M_CHARAENUM_WUSHUANG && !Player::p.bCircle))
		{
//			if(fabsf(x - Player::p.x) < ENEMY_COLLISIONR && fabsf(y - Player::p.y) < ENEMY_COLLISIONR)
			if (checkCollisionSquare(Player::p, ENEMY_COLLISIONR))
			{
				if(!Player::p.bInfi && (!Player::p.bCircle || type >= ENEMY_BOSSTYPEBEGIN) && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
				{
					if(Player::p.nBomb)
						Player::p.flag |= PLAYER_SHOT;
					else
						Player::p.flag |= PLAYER_COLLAPSE;
				}
			}
			if (pb.size)
			{
				DWORD i = 0;
				DWORD size = pb.size;
				for (pb.toBegin(); i<size; pb.toNext(), i++)
				{
					if (pb.isValid() && (*pb).able)
					{
						if ((*pb).isInRange(x, y))
						{
//							if(Player::p.bCircle)
//								(*pb).power *= PBENEMY_COSTSCALE;
							life -= (*pb).power * (1 - defrate);
							damage = true;
							break;
						}
					}
				}
			}
		}

		if(BossInfo::flag)
		{
			int txdiff = fabsf(Player::p.x - x);
			if(txdiff < ENEMY_BOSSX_FADERANGE)
				BossInfo::spbossx->SetColor(((0x40 + txdiff*2) << 24) | 0xffffff);
			else
				BossInfo::spbossx->SetColor(0x80ffffff);
		}

		if(!damage && life < maxlife / 5 && timer%8<4)
		{
			SE::push(SE_ENEMY_DAMAGE_1, x);

			if(BossInfo::flag && type >= ENEMY_BOSSTYPEBEGIN)
				BossInfo::spbossx->SetColor(0xffffffff);
		}

		if(damage && !damagetimer)
		{
			damagetimer++;
		}
		else if(damagetimer > 0)
		{
			if(damagetimer < 8)
				damagetimer++;
			else
				damagetimer = 0;
			if(damagetimer > 0 && damagetimer % 8 < 4)
			{
				alpha = 0x7f;
				diffuse = 0xb40000;
				effShot.Fire();

				if(BossInfo::flag && type >= ENEMY_BOSSTYPEBEGIN)
					BossInfo::spbossx->SetColor(0xc0ffffff);
					
				if(life < maxlife / 5)
				{
					SE::push(SE_ENEMY_DAMAGE_2, x);
				}
				else
					SE::push(SE_ENEMY_DAMAGE_1, x);
			}
			else
			{
				alpha = 0xff;
				diffuse = 0xffffff;
				effShot.Stop();
			}
		}
		if(life < 0)
		{
			Player::p.nScore += PL_ENEMYBONUS_COLLAPSE(maxlife);

			SE::push(SE_ENEMY_DEAD, x);

			fadeout = true;
			timer = 0;
		}

		if(x > M_DELETECLIENT_RIGHT || x < M_DELETECLIENT_LEFT || y > M_DELETECLIENT_BOTTOM || y < M_DELETECLIENT_TOP)
			exist = false;
	}
	else
	{
		if(timer == 1)
		{
			giveItem();
			effShot.Stop();
			effCollapse.valueSet(EFF_EN_COLLAPSE, *this);
		}
		else if(timer == 32)
		{
			effCollapse.Stop();
			exist = false;
		}
		effCollapse.action();

		if(type < ENEMY_BOSSTYPEBEGIN)
		{
			hscale	=	timer * 0.05f + 1.0f;
			vscale	=	(32 - timer) * 0.03225f;
			alpha	=	(BYTE)((32 - timer) * 8);
		}
		if (esCircle.exist && timer <= 16)
		{
			esCircle.alpha = (BYTE)((16 - timer) * 16);
			esCircle.hscale = (16 - timer) * 0.0625f;
		}
	}

	damage = false;
	able = exist && !fadeout;
}

void Enemy::giveItem()
{
	if(mp.rangemode)
		return;

	bool first = true;
	float aimx;
	float aimy;

	float tempx = x;
	float tempy = y;

	y -= rand()%30;
	if(x > PL_MOVABLE_RIGHT)
		x = PL_MOVABLE_RIGHT;
	else if(x < PL_MOVABLE_LEFT)
		x = PL_MOVABLE_LEFT;

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<(int)((take>>(i*4))&0xf);j++)
		{
			if(!first)
			{
				aimx = (float)((x + rand()%80 - 40));
				if(aimx > 417)
					aimx = 417;
				else if(aimx < 23)
					aimx = 23;
				aimy = (float)(rand()%80 - 240 + y);
				Item::Build(i, x, y, false, 18000 + rMainAngle(aimx, aimy), -sqrt(2 * 0.1f * DIST(x, y, aimx, aimy)));
			}
			else
			{
				Item::Build(i, x, y);
			}
			first = false;
		}
	}
	x = tempx;
	y = tempy;
}
