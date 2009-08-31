#include "Ghost.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "SE.h"
#include "Bullet.h"
#include "Scripter.h"
#include "Chat.h"
#include "BossInfo.h"
#include "Main.h"

Ghost gh[GHOSTMAX];

WORD Ghost::index;

BYTE Ghost::totalghost;
BYTE Ghost::totalhalfghost;

Ghost::Ghost()
{
	able			= false;
	effghost.exist	= false;
	gID				= 0;
}

Ghost::~Ghost()
{
}

bool Ghost::Build(WORD _gID, BYTE _tarID, BYTE _belong, float x, float y, int angle, float speed, BYTE type, float life, int ac)
{
	bool rv = false;
	for(int i=0;i<GHOSTMAX;i++)
	{
		index++;
		if(index == GHOSTMAX)
		{
			index = 0;
		}
		if(!gh[index].exist)
		{
			rv = true;
			break;
		}
	}
	if(!rv)
		return false;
	gh[index].gID = _gID;
	gh[index].valueSet(index, x, y, angle, speed, type, life, ac);
	gh[index].belong = _belong;
	gh[index].tarID = _tarID;
	return true;
}

void Ghost::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, float _life, int _ac)
{
	ID		=	_ID;
	x		=	_x;
	y		=	_y;
	speed	=	_speed;
	type	=	_type;
	life	=	_life;
	ac		=	_ac;
	angle	=	_angle;

	tarID	=	0xff;

	timer	=	0;
	cenx	=	x;
	ceny	=	y;
	maxlife	=	life;
	exist	=	true;
	fadeout	=	false;
	able	=	true;
	damage	=	false;
	half	=	false;
	gave	=	false;
	hscale	=	1.0f;
	vscale	=	1.0f;
	headangle =	0;
	alpha	=	0xff;
	belong	=	0xff;

	effghost.valueSet(EFF_GH_TYPEBEGIN + type, *this);

	CEmitterList * emitterItem = effghost.eff->eiList;
	int i = 0;
	while (emitterItem)
	{
		CAffectorList * affectorItem = emitterItem->emitter.eaiList;
		while (affectorItem)
		{
			if (*(char *)(effghost.eff->GetAffectorInfoValuePointer(emitterItem->emitter.ID, affectorItem->affector.ID, HGEEFFECT_AI_TYPE)) == HGEEFFECT_AFFECTORTYPE_COLOR)
			{
				startcolmin[i] = (DWORD *)(effghost.eff->GetAffectorInfoValuePointer(emitterItem->emitter.ID, affectorItem->affector.ID, HGEEFFECT_AI_FSTARTVALUEMIN));
				startcolmax[i] = (DWORD *)(effghost.eff->GetAffectorInfoValuePointer(emitterItem->emitter.ID, affectorItem->affector.ID, HGEEFFECT_AI_FSTARTVALUEMAX));
				endcolmin[i] = (DWORD *)(effghost.eff->GetAffectorInfoValuePointer(emitterItem->emitter.ID, affectorItem->affector.ID, HGEEFFECT_AI_FENDVALUEMIN));
				endcolmax[i] = (DWORD *)(effghost.eff->GetAffectorInfoValuePointer(emitterItem->emitter.ID, affectorItem->affector.ID, HGEEFFECT_AI_FENDVALUEMAX));
			}
			affectorItem = affectorItem->next;
		}
		emitterItem = emitterItem->next;
		if (i < GHOST_COLORAFFECTORMAX)
		{
			i++;
		}
	}
	while (i < GHOST_COLORAFFECTORMAX)
	{
		startcolmin[i] = 0;
		startcolmax[i] = 0;
		endcolmin[i] = 0;
		endcolmax[i] = 0;
		i++;
	}

	effghost.Fire();
}

void Ghost::valueSet(WORD ID, BYTE _belong, int angle, float speed, BYTE type, float life, int ac)
{
	valueSet(ID, en[_belong].x, en[_belong].y,angle, speed, type, life, ac);
	belong = _belong;
}

void Ghost::Render()
{
	effghost.Render();
}

void Ghost::action()
{
	timer++;

	if(life < maxlife / 2 && life > 0)
	{
		if (!half)
		{
			SE::push(SE_GHOST_HALF, x);
			half = true;
		}
		totalhalfghost++;
	}
	totalghost++;

	if(!fadeout && !gave && half && (Player::p.bCircle || Player::p.ID == M_CHARAENUM_WUSHUANG))
	{
		Player::p.nScore += PL_GHOSTBONUS_HALF(maxlife);

		Item::Build(ITEM_CIRCLE, x, y, true);

		gave = true;
	}

	DWORD startcol = 0;
	DWORD endcol = 0;
	if(Player::p.bCircle || Player::p.ID >> 1 == M_CHARA_WUSHUANG)
	{
		gID = (gID >> 1 << 1) + 1;
		if(!half)
		{
			startcol = 0xf0ff0000;
			endcol = 0x00ff8000;
		}
		else
		{
			startcol = 0xf0ff8000;
			endcol = 0x00ffff00;
		}
	}
	else
	{
		gID = (gID >> 1 << 1);
		if(!half)
		{
			startcol = 0xf00000ff;
			endcol = 0x000080ff;
		}
		else
		{
			startcol = 0xf00080ff;
			endcol = 0x0000ffff;
		}
	}
	if (fadeout)
	{
		startcol -= 0xc0000000;
	}
	for (int i=0; i<GHOST_COLORAFFECTORMAX; i++)
	{
		if (startcolmin[i])
		{
			*startcolmin[i] = startcol;
		}
		if (startcolmax[i])
		{
			*startcolmax[i] = startcol;
		}
		if (endcolmin[i])
		{
			*endcolmin[i] = endcol;
		}
		if (endcolmax[i])
		{
			*endcolmax[i] = endcol;
		}
	}

	if(!fadeout)
	{
		if(timer == 1)
		{
			SE::push(SE_GHOST_MERGE, x);
		}
		if(!half && Player::p.ID != M_CHARAENUM_BUSINIAO && (Player::p.flag & PLAYER_DEADBOMB))
		{
			life -= PLDEADBOMB_GHOSTLIFECOST;
			damage = true;
			gave = true;
		}

		if(Chat::chatting || BossInfo::flag >= BOSSINFO_COLLAPSE)
		{
			life = 0;
			fadeout = true;
			timer = 0;
		}

		if(gID && !Scripter::stopEdefScript)
		{
			WORD tindex = index;
			index = ID;
			scr.edefExecute(gID, timer);
			index = tindex;
		}

		if(!half && Player::p.ID != M_CHARAENUM_BUSINIAO)
		{
//			if (fabsf(x - Player::p.x) < GHOST_COLLISIONR && fabsf(y - Player::p.y) < GHOST_COLLISIONR)
			if (checkCollisionSquare(Player::p, GHOST_COLLISIONR))
			{
				if(!Player::p.bInfi && Player::p.bCircle && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
				{
					if(Player::p.nBomb)
						Player::p.flag |= PLAYER_SHOT;
					else
						Player::p.flag |= PLAYER_COLLAPSE;
				}
			}
			if (pb.size && (Player::p.bCircle || Player::p.ID == M_CHARAENUM_WUSHUANG))
			{
				DWORD i = 0;
				DWORD size = pb.size;
				for (pb.toBegin(); i<size; pb.toNext(), i++)
				{
					if (pb.isValid() && (*pb).able)
					{
						if ((*pb).isInRange(x, y))
						{
							float power = (*pb).power;
							life -= power;
							if (life < 0)
							{
								life = maxlife / 4;
							}
							damage = true;
							if (belong < ENEMYMAX)
							{
								float minus = power * (1 - en[belong].defrate);
								en[belong].life -= PBGHOST_ENEMYCOST(minus);
								en[belong].damage = true;
							}
						}
					}
				}
			}
		}

		if(belong != 0xff)
		{
			if(en[belong].exist)
			{
				cenx = en[belong].x;
				ceny = en[belong].y;
			}
			else
			{
				if(en[belong].life >= 0)
				{
					life = 0;
				}
				timer = 0;
				fadeout = true;
			}
		}

		if(life < 0)
		{
			Player::p.nScore += PL_GHOSTBONUS_COLLAPSE(maxlife);

			SE::push(SE_GHOST_DEAD, x);

			fadeout = true;
			timer = 0;
		}
		else if(ac && belong != 0xff)
		{
			angle += ANGLE(speed / ac);

			float tr2 = DIST2(x, y, cenx, ceny);
			float trac2 = ac * ac;
			float tac = fabsf(ac);
			float trac = tac;
			if(tr2 < trac2)
			{
				trac = sqrtf(tr2) + fabsf(speed);
				if(trac > tac)
					trac = tac;
			}
			else if(tr2 > trac2)
			{
				trac = sqrtf(tr2) - fabsf(speed);
				if(trac < tac)
					trac = tac;
			}
			if(ac < 0)
				trac = -trac;

			x = cenx + trac * cost(angle);
			y = ceny + trac * sint(angle);
		}
		else
		{
			x += speed * cost(angle);
			y += speed * sint(angle);
		}

		if(tarID != 0xff)
		{
			tar[tarID].x = x;
			tar[tarID].y = y;
		}

		effghost.MoveTo(x, y);
		effghost.action();

		if(belong == 0xff)
		{
			if(timer > (DWORD)ac)
			{
				life = 0;
				fadeout = true;
				timer = 0;
			}
		}
	}
	else
	{
		effghost.action();
		if(timer == 32)
		{
			effghost.Stop();
			exist = false;
		}
		if(timer == 8)
		{
			if(life > 0)
			{
				IzeZone _ize;
				_ize.x = x;
				_ize.y = y;
				_ize.r = GHOST_IZER(life, maxlife);
				Bullet::izel[BULLETIZE_ITEM].push_back(_ize);
			}
			if(en[belong].life < 0 || !en[belong].exist)
				SE::push(SE_GHOST_DEAD, x);
		}
	}

	damage = false;
	able = exist && !fadeout;
}