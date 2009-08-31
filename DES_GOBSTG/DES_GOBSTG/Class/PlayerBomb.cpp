#include "Player.h"
#include "SE.h"
#include "BGLayer.h"
#include "Process.h"

void Player::bombAction()
{
	static DWORD col[4];
	/*
	if(bgmask.flag)
	{
		for(int i=0;i<4;i++)
			col[i] = bgmask.quad.v[i].col;
	}
	*/
	int ttimer;
	if(flag & PLAYER_BOMB)
		ttimer = bombtimer;
	else
		ttimer = deadbombtimer;

	if (ttimer < 15)
	{
		worldx = sint(ttimer * 4800) * 10;
		worldy = cost(ttimer * 4800) * 10;
	}
	else if (ttimer == 15)
	{
		worldx = 0;
		worldy = 0;
	}

	switch(ID)
	{
	case M_CHARAENUM_SHI:
		if(ttimer == 1)
		{
			effBomb[0].MoveTo(220, 480, 0, true);
			//
			for(int i=20;i<65;i++)
				esBomb[i].valueSet(EFFECT_BOMB_0, -256, 0, 0, 0, false);
			esBomb[0].valueSet(EFFECT_BOMB_0, -128, 120, 0, 17.4f, false);
			esBomb[0].hscale = 2;
		}
		else if(ttimer == 21)
		{
			esBomb[0].speed = 0;
			esBomb[1].valueSet(EFFECT_BOMB_0, 220, -128, 9000, 18.4f, false);
			esBomb[1].hscale = 2;
		}
		else if(ttimer == 41)
		{
			esBomb[1].speed = 0;
			esBomb[2].valueSet(EFFECT_BOMB_0, 504, 240, 18000, 10.7f, false);
		}
		else if(ttimer == 61)
		{
			esBomb[2].speed = 0;
			esBomb[3].valueSet(EFFECT_BOMB_0, 150, 544, -9000, 12, false);
		}
		else if(ttimer == 81)
		{
			esBomb[3].speed = 0;
			esBomb[4].valueSet(EFFECT_BOMB_0, -128, 348, 0, 17.4f, false);
			esBomb[4].hscale = 2.5f;
		}
		else if(ttimer == 101)
		{
			esBomb[4].speed = 0;
			for(int i=5;i<11;i++)
			{
				esBomb[i].valueSet(EFFECT_BOMB_2, 220, 240, 0, 0, false);
				esBomb[i].hscale = 0;
			}
		}
		else if(ttimer > 101 && ttimer < 240)
		{
			for(int i=5;i<11;i++)
				esBomb[i].hscale += 0.1f*(i-4);
			if(ttimer > 120)
			{
				for(int i=0;i<5;i++)
				{
					if(i&1)
						esBomb[i].headangle += 500;
					else
						esBomb[i].headangle -= 500;
					esBomb[i].speed = 4.2f;
					esBomb[i].angle = esBomb[i].aMainAngle(220, 240, 18000);
				}
			}
		}
		for(int i=20;i<65;i++)
		{
			if(esBomb[i%5].exist)
			{
				if(esBomb[i%5].speed!=0)
				{
					esBomb[i].x = esBomb[i%5].x - esBomb[i%5].speed*cost(esBomb[i%5].angle)*((i-15)/5);
					esBomb[i].y = esBomb[i%5].y - esBomb[i%5].speed*sint(esBomb[i%5].angle)*((i-15)/5);
				}
				else
				{
					esBomb[i].x += (esBomb[i%5].x - esBomb[i].x)/((i-15)/5);
					esBomb[i].y += (esBomb[i%5].y - esBomb[i].y)/((i-15)/5);
				}
				esBomb[i].hscale = esBomb[i%5].hscale;
				esBomb[i].angle = esBomb[i%5].angle;
				esBomb[i].headangle = esBomb[i%5].headangle/9*((i-15)/5);
				esBomb[i].alpha = 0x7f/9*((i-15)/5);
			}
		}
		break;
	case M_CHARAENUM_LINGYE:
		if (ttimer == 1)
		{
			effBomb[1].MoveTo(440, 240, 0, true);
		}
		if(ttimer <= 60)
		{
			esBomb[ttimer-1].valueSet(EFFECT_BOMB_1, 220, 240, rand(), 0, false);
			esBomb[ttimer-1].hscale = 0;
			for(int i=0;i<ttimer-1;i++)
			{
				float xs = (rand()%160)/1000.0f;
				float ys = (rand()%80)/1000.0f;
				esBomb[i].hscale += xs;
				esBomb[i].vscale += ys;
				esBomb[i].x += xs*128 * cost(esBomb[i].angle)/2;
				esBomb[i].y += ys*128 * sint(esBomb[i].angle)/2;
			}
		}
		else if(ttimer == 100)
		{
			esBomb[60].valueSet(EFFECT_BOMB_3, 220, -60, 9000, 4, false);
			esBomb[60].hscale = 2.0f;
		}
		else if(ttimer == 180)
		{
			esBomb[60].exist = false;
			for(int i=0;i<60;i++)
				esBomb[i].speed = 8;
		}
		break;
	case M_CHARAENUM_NUSIKUIMA:
		if(ttimer == 1)
		{
			esBomb[60].valueSet(EFFECT_BOMB_0, 220+2*(x-220), 608, 0, 0, false);
			for(int i=61;i<69;i++)
				esBomb[i].valueSet(EFFECT_BOMB_0, 220+2*(x-220) ,608, 0, 0, false);
			esBomb[60].angle = esBomb[60].aMainAngle(x, y);
			esBomb[60].speed = DIST(esBomb[60].x, esBomb[60].y, x+128*cost(esBomb[60].angle), y+128*sint(esBomb[60].angle)) / 60;
			esBomb[60].hscale = 2;
		}
		else if(ttimer > 60 && ttimer <= 120)
		{
			esBomb[60].speed = 0;
			esBomb[ttimer-61].valueSet(EFFECT_BOMB_2, 220, 240, rand(), (rand()%200)/40, false);
		}
		if(ttimer > 60)
		{
			for(int i=0;i<60;i++)
			{
				if(!esBomb[i].exist)
					continue;
				if(ttimer <= 160)
				{
					esBomb[i].sprite->quad.v[0].z = esBomb[i].sprite->quad.v[1].z = 
						esBomb[i].sprite->quad.v[2].z = esBomb[i].sprite->quad.v[3].z += (rand()%200)/100;
					int theadangle = ANGLE(speed / 30 + 0.1f);
					if(angle > 18000)
						esBomb[i].headangle += theadangle;
					else
						esBomb[i].headangle -= theadangle;
				}
				else if(ttimer == 161)
				{
					if(i < 26)
					{
						esBomb[i].angle = esBomb[i].aMainAngle(76+(i%13)*24+i/13*12, 160+i/13*18);
						esBomb[i].speed = DIST(esBomb[i].x, esBomb[i].y, 76+(i%13)*24+i/13*12, 160+i/13*18) / 60;
					}
					else
					{
						esBomb[i].angle = esBomb[i].aMainAngle(204+(i-26)/17*32, 40+((i-26)%17)*24+(i-26)/17*12);
						esBomb[i].speed = DIST(esBomb[i].x, esBomb[i].y, 204+(i-26)/17*32, 40+((i-26)%17)*24+(i-26)/17*12) / 60;
					}
				}
				else if(ttimer <= 220)
				{
					esBomb[i].sprite->quad.v[0].z = esBomb[i].sprite->quad.v[1].z = 
						esBomb[i].sprite->quad.v[2].z = esBomb[i].sprite->quad.v[3].z -= esBomb[i].sprite->quad.v[0].z/60;
					int theadangle = ANGLE(esBomb[i].timer * (esBomb[i].speed/30+0.1f));
					if(angle > 18000)
						esBomb[i].headangle -= theadangle;
					else
						esBomb[i].headangle += theadangle;
					if(i >= 13 && i < 26 || i >= 43)
						esBomb[i].headangle += 300;
				}
				else if(ttimer == 221)
				{
					esBomb[i].speed = 0;
					if(i >= 13 && i < 26 || i>=43)
						esBomb[i].headangle = 18000;
					else
						esBomb[i].headangle = 0;
					esBomb[i].angle = 9000;
				}
			}
		}
		for(int i=61;i<69;i++)
		{
			if(esBomb[60].exist)
			{
				if(ttimer <= 160)
				{
					if(esBomb[60].speed!=0)
					{
						esBomb[i].x = esBomb[60].x - esBomb[60].speed*cost(esBomb[60].angle)*(i-60);
						esBomb[i].y = esBomb[60].y - esBomb[60].speed*sint(esBomb[60].angle)*(i-60);
					}
					else
					{
						esBomb[i].x += (esBomb[60].x - esBomb[i].x)/(i-60);
						esBomb[i].y += (esBomb[60].y - esBomb[i].y)/(i-60);
					}
					esBomb[i].hscale = esBomb[60].hscale;
					esBomb[i].headangle = esBomb[60].headangle;
				}
				else
				{
					esBomb[i].hscale += (i-60)/10.0f;
					esBomb[i].vscale += (i-60)/2.0f;
					esBomb[i].headangle += 100*(i-60);
					if(esBomb[i].hscale > 24)
						esBomb[i].exist = false;
				}
				esBomb[i].angle = esBomb[60].angle;
				esBomb[i].alpha = 0x7f/9*(i-60);
			}
		}
		break;
	case M_CHARAENUM_GEJI:
		if(ttimer == 1)
		{
			effBomb[1].MoveTo(220, 240, true);
			esBomb[60].valueSet(EFFECT_BOMB_3, 220, 240, 0, 0, false);
		}
		/*
		if(ttimer <= 60)
		{
			esBomb[ttimer-1].valueSet(EFFECT_BOMB_1, rand()%440, rand()%480, 0, 2.0f, false);
		}
		for(int i=0;i<60;i++)
		{
			if(esBomb[i].exist)
			{
				if(ttimer <= 160)
				{
					esBomb[i].angle = esBomb[i].aMainAngle(220, 240, 6000);
					esBomb[i].speed += 0.1f;
					esBomb[i].hscale -= 0.02f;
				}
				else if(ttimer == 161)
				{
					esBomb[i].angle = esBomb[i].aMainAngle(220, 240, 18000);
				}
				else
				{
					esBomb[i].hscale += 0.05f;
				}
			}
		}
		*/
		if(ttimer <= 160)
			esBomb[60].hscale += 0.01f;
		else
			esBomb[60].hscale += 0.1f;
		break;
	case M_CHARAENUM_CILAN:
		if(ttimer == 1)
		{
			for(int i=60;i<69;i++)
			{
				esBomb[i].valueSet(EFFECT_BOMB_2, 220, 240, rand(), 0, false);
				esBomb[i].hscale = 10;
			}
		}
		if(ttimer < 61)
		{
			esBomb[ttimer-1].valueSet(EFFECT_BOMB_0, 220+120*cost(ttimer*600), 240+120*sint(ttimer*600), rand(), 0, false);
			esBomb[ttimer-1].colorSet(0xffffff);
		}
		else if(ttimer == 120)
		{
			for(int i=0;i<60;i++)
				esBomb[i].speed = 4.0f;
		}
		for(int i=60;i<69;i++)
		{
			esBomb[i].headangle = -esBomb[i].angle;
			if(esBomb[i].hscale > 2.0f)
				esBomb[i].hscale -= (i-59)*0.1f;
			else if(ttimer > 120)
			{
				esBomb[i].speed = 4.0f;
				esBomb[i].angle += 100;
			}
		}
		break;
	case M_CHARAENUM_ZANGYUE:
		if (ttimer == 1)
		{
			effBomb[1].MoveTo(220, 480, 0, true);
		}
		else if(ttimer > 20 && ttimer < 180)
		{
			if(rand()&1)
				esBomb[ttimer%64].valueSet(EFFECT_BOMB_1, -rand()%40-64, -rand()%40-64, rand()%4500+2750, 16.0f, false);
			else
				esBomb[ttimer%64].valueSet(EFFECT_BOMB_1, rand()%40+504, -rand()%40-64, rand()%4500+1750+9000, 16.0f, false);
			for(int i=0;i<64;i++)
			{
				if(!esBomb[i].exist)
					continue;
				if(esBomb[i].timer > 40)
					esBomb[i].hscale *= 0.85f;
				if(esBomb[i].hscale < 0.5f)
				{
					if(rand()&1)
						esBomb[i].valueSet(EFFECT_BOMB_1, -rand()%40-64, -rand()%40-64, rand()%4500+2750, 16.0f, false);
					else
						esBomb[i].valueSet(EFFECT_BOMB_1, rand()%40+504, -rand()%40-64, rand()%4500+1750+9000, 16.0f, false);
				}
			}
		}
		break;
	case M_CHARAENUM_WUSHUANG:
		if (ttimer == 1)
		{
			effBomb[0].MoveTo(220, 0, 0, true);
		}
		if(ttimer < 61)
		{
			esBomb[ttimer-1].valueSet(EFFECT_BOMB_0, rand()%440, -40, rand()%4500+6750, (rand()%100)/20.0f+0.8f, false);
		}
		for(int i=0;i<60;i++)
		{
			if(ttimer > 60 && (esBomb[i].y > 544 || esBomb[i].x < -64 || esBomb[i].x > 504))
			{
				esBomb[i].valueSet(EFFECT_BOMB_0, rand()%440, -40, rand()%4500+6750, (rand()%100)/20.0f, false);
			}
			else
			{
				if(angle > 4500)
					esBomb[i].headangle += ANGLE(speed / 15);
				else
					esBomb[i].headangle -= ANGLE(speed / 15);
				if(ttimer > 120)
				{
					esBomb[i].angle = esBomb[i].aMainAngle(220, 240, 6000);
					esBomb[i].headangle = 18000;
					esBomb[i].speed += 0.08f;
					esBomb[i].hscale *= 0.985f;
				}
			}
		}
		break;
	case M_CHARAENUM_BUSINIAO:
		if (ttimer == 1)
		{
			effBomb[1].MoveTo(x, y, 0, true);
		}
		else
			effBomb[1].MoveTo(x, y);
		/*
		if(ttimer < 61)
		{
			if(ttimer < 31)
				esBomb[ttimer-1].valueSet(EFFECT_BOMB_1, x, y, ttimer*1200, 0, false);
			else
				esBomb[ttimer-1].valueSet(EFFECT_BOMB_1, x, y, -(ttimer-30)*1200+600, 0, false);
			esBomb[ttimer-1].colorSet((rand()&0xff)<<16|(rand()&0x7f)<<8);
			esBomb[ttimer-1].sprite->SetHotSpot(0, 20);
			esBomb[ttimer-1].hscale = 6;
			esBomb[ttimer-1].vscale = 0.01f;
		}
		for(int i=0;i<60;i++)
		{
			if(ttimer < 121)
				esBomb[i].vscale += 0.012f;
			else if(ttimer > 180)
				esBomb[i].vscale += 0.02f;
		}
		*/
		break;
	}
	for(int i=0;i<80;i++)
	{
		if(ttimer == 240)
			esBomb[i].exist = false;
		if(esBomb[i].exist)
		{
			esBomb[i].action();
		}
	}

	if(ttimer == 1)
	{
		effBomb[ID&1].Fire();
		esBomb[78].valueSet(EFFECT_BOMB_CUTIN, 140, 660, -9000, 4, false);
		esBomb[78].headangle = 9000;
		esBomb[79].valueSet(EFFECT_BOMB_NAME, -150, 320, 0, 3, false);
//		for(int i=0;i<4;i++)
//			col[i] = bgmask.quad.v[i].col;
	}
	else if(ttimer == 91)
	{
		esBomb[79].speed = 0;
		esBomb[78].speed = 0;
	}
	else if (ttimer == 240)
	{
		effBomb[ID&1].Stop();
	}
	/*
	else if(ttimer == 240)
	{
		for(int i=0;i<4;i++)
			bgmask.quad.v[i].col = col[i];
		worldx = 0;
		worldy = 0;
	}
	else if(ttimer % 5 == 0)
	{
		DWORD tcol = 0x7f000000|((rand()&0xff)<<16)|((rand()&0xff)<<8)|(rand()&0xff);
		switch((ttimer/5)%4)
		{
		case 0:
			worldx = -5;
			worldy = 2;
			for(int i=0;i<4;i++)
				bgmask.quad.v[i].col = tcol;
			break;
		case 1:
			worldx = -2;
			worldy = 5;
			break;
		case 2:
			worldx = 5;
			worldy = -2;
			break;
		case 3:
			worldx = 2;
			worldy = -5;
			break;
		}
	}
	*/
}