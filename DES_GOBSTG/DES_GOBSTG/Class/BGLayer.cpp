#include "BGLayer.h"
#include "Main.h"

BGLayer * ubg[UBGLAYERMAX];
BGLayerSet BGLayer::set[BGLAYERSETMAX];
BGLayer bg[BGLAYERMAX];
BGLayer fg[FGLAYERMAX];

BGLayer bgmask;
BGLayer fgpanel;
BGLayer fgpause;
BGLayer fgdiff;

WORD BGLayer::setindex = 0;

BGLayer::BGLayer()
{
	exist	= false;
}

BGLayer::~BGLayer()
{
}

void BGLayer::Init()
{
	for(int i=0;i<BGLAYERMAX;i++)
	{
		ubg[i] = & bg[i];
	}
	for(int i=0;i<FGLAYERMAX;i++)
	{
		ubg[i+BGLAYERMAX] = & fg[i];
	}
	ubg[BGLAYERMAX + FGLAYERMAX] = & bgmask;
	ubg[BGLAYERMAX + FGLAYERMAX + 1] = & fgpanel;
	ubg[BGLAYERMAX + FGLAYERMAX + 2] = & fgpause;
	ubg[BGLAYERMAX + FGLAYERMAX + 3] = & fgdiff;

	for(int i=0; i<BGLAYERSETMAX; i++)
	{
		set[i].sID = 0;
		set[i].timer = 0;
	}
	setindex = 0;
}

void BGLayer::valueSet(HTEXTURE tex, float cenx, float ceny, float width, float height, float column, float row, float nColumn, float nRow, DWORD col)
{
	valueSet(tex, cenx - width/2, ceny - height/2, 0, width, height, 0, 0, 0, 0, (column-1) / nColumn, (row-1) / nRow, 1 / nColumn, 1 / nRow, 0, 9000, false, false, col);
}

void BGLayer::valueSet(HTEXTURE tex, float x, float y, float z, float w, float h, int rotx, int roty, int rotz, float paral, float tx, float ty, float tw, float th, float _speed /* = 0 */, int _angle /* = 9000 */, bool _move /* = false */, bool _rotate /* = false */, DWORD col /* = 0xffffffff */)
{
	width	=	w;
	height	=	h;
	speed	=	_speed;
	angle	=	_angle;
	flag	=	BG_NONE;
	timer	=	0;
	changetimer	=	0;

	quad.tex = tex;

	rectSet(x, y, z, w, h, rotx, roty, rotz);
	parallelogram(paral);
/*
	if (rotx)
	{
		/////////ROTATIONX//////////
		for (int i=1; i<4; i++)
		{
			float _diffy = quad.v[i].y - quad.v[0].y;
			float _diffz = quad.v[i].z - quad.v[0].z;
			quad.v[i].y += ROTATION1(_diffy, _diffz, rotx);
			quad.v[i].z += ROTATION2(_diffy, _diffz, rotx);
		}
	}
	if (roty)
	{
		/////////ROTATIONY//////////
		for (int i=1; i<4; i++)
		{
			float _diffx = quad.v[i].x - quad.v[0].x;
			float _diffz = quad.v[i].z - quad.v[0].z;
			quad.v[i].x += ROTATION1(_diffx, _diffz, roty);
			quad.v[i].z += ROTATION2(_diffx, _diffz, roty);
		}
	}
	if (rotz)
	{
		/////////ROTATIONZ//////////
		for (int i=1; i<4; i++)
		{
			float _diffx = quad.v[i].x - quad.v[0].x;
			float _diffy = quad.v[i].y - quad.v[0].y;
			quad.v[i].x += ROTATION1(_diffx, _diffy, rotz);
			quad.v[i].y += ROTATION2(_diffx, _diffy, rotz);
		}
	}
*/

	texWidth = hge->Texture_GetWidth(tex);
	texHeight = hge->Texture_GetHeight(tex);

	quad.v[0].tx = tx;		quad.v[0].ty = ty;
	quad.v[1].tx = (tx+tw);	quad.v[1].ty = ty;
	quad.v[2].tx = (tx+tw);	quad.v[2].ty = (ty+th);
	quad.v[3].tx = tx;		quad.v[3].ty = (ty+th);

	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = col;
	ocol[0] = ocol[1] = ocol[2] = ocol[3] = col;

	quad.blend	=	BLEND_DEFAULT;

	exist = true;
	move = _move;
	rotate = _rotate;
}

void BGLayer::texRectSet(float texx, float texy, float texw, float texh)
{
	quad.v[0].tx = texx / texWidth;				quad.v[0].ty = texy / texHeight;
	quad.v[1].tx = (texx+texw) / texWidth;		quad.v[1].ty = texy / texHeight;
	quad.v[2].tx = (texx+texw) / texWidth;		quad.v[2].ty = (texy+texh) / texHeight;
	quad.v[3].tx = texx / texWidth;				quad.v[3].ty = (texy+texh) / texHeight;
}

void BGLayer::scaleSet(float hscale, float vscale)
{
	quad.v[0].x = quad.v[3].x = (quad.v[0].x + quad.v[1].x) / 2 - hscale * (quad.v[1].x - quad.v[0].x) / 2;
	quad.v[1].x = quad.v[2].x = (quad.v[0].x + quad.v[1].x) / 2 + hscale * (quad.v[1].x - quad.v[0].x) / 2;
	quad.v[0].y = quad.v[1].y = (quad.v[0].y + quad.v[3].y) / 2 - vscale * (quad.v[3].y - quad.v[0].y) / 2;
	quad.v[2].y = quad.v[3].y = (quad.v[0].y + quad.v[3].y) / 2 + vscale * (quad.v[3].y - quad.v[0].y) / 2;

	width *= hscale;
	height *= vscale;
}

void BGLayer::zSet(float z0, float z1, float z2, float z3)
{
	quad.v[0].z = z0;
	quad.v[1].z = z1;
	quad.v[2].z = z2;
	quad.v[3].z = z3;
}

void BGLayer::colorSet(DWORD col0, DWORD col1, DWORD col2, DWORD col3)
{
	quad.v[0].col = ocol[0] = col0;
	quad.v[1].col = ocol[1] = col1;
	quad.v[2].col = ocol[2] = col2;
	quad.v[3].col = ocol[3] = col3;
}

void BGLayer::moveSet(bool _move, bool _rotate)
{
	move = _move;
	rotate = _rotate;
}

void BGLayer::rectSet(float x, float y, float z, float w, float h, int rotx, int roty, int rotz)
{
	float wx = w, wy = 0, wz = 0;
	float hx = 0, hy = h, hz = 0;

	if (rotx)
	{
		float _hy = hy;
		hy = ROTATION1(_hy, 0, rotx);
		hz = ROTATION2(_hy, 0, rotx);
	}
	if (roty)
	{
		float _wx = wx;
		float _wz = wz;
		wx = ROTATION1(_wx, _wz, roty);
		wz = ROTATION2(_wx, _wz, roty);
	}
	if (rotz)
	{
		float _wx = wx;
		float _wy = wy;
		wx = ROTATION1(_wx, _wy, rotz);
		wy = ROTATION2(_wx, _wy, rotz);
		float _hx = hx;
		float _hy = hy;
		hx = ROTATION1(_hx, _hy, rotz);
		hy = ROTATION2(_hx, _hy, rotz);
	}

	quad.v[0].x	= x;			quad.v[0].y = y;			quad.v[0].z = z;
	quad.v[1].x	= x + wx;		quad.v[1].y = y + wy;		quad.v[1].z = z + wz;
	quad.v[2].x	= x + wx + hx;	quad.v[2].y = y + wy + hy;	quad.v[2].z = z + wz + hz;
	quad.v[3].x	= x + hx;		quad.v[3].y = y + hy;		quad.v[3].z = z + hz;
}

void BGLayer::parallelogram(float paral)
{
	quad.v[2].x += paral;
	quad.v[3].x += paral;
}

void BGLayer::vertexSet(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	quad.v[0].x = x0;	quad.v[0].y = y0;	quad.v[0].z = z0;
	quad.v[1].x = x1;	quad.v[1].y = y1;	quad.v[1].z = z1;
	quad.v[2].x = x2;	quad.v[2].y = y2;	quad.v[2].z = z2;
	quad.v[3].x = x3;	quad.v[3].y = y3;	quad.v[3].z = z3;
}

void BGLayer::action()
{
	timer++;

	float costa = cost(angle);
	float sinta = sint(angle);

	if(!move)
	{
		//roll the layer
		if(angle!=9000)
		{
//			float xt = speed * cos(arc);
//			float yt = speed * sin(arc);
			float xt = speed * costa;
			float yt = speed * sinta;

			quad.v[0].tx -= xt;	quad.v[0].ty -= yt;
			quad.v[1].tx -= xt;	quad.v[1].ty -= yt;
			quad.v[2].tx -= xt;	quad.v[2].ty -= yt;
			quad.v[3].tx -= xt;	quad.v[3].ty -= yt;
		}
		else if(speed != 0)
		{
			quad.v[0].ty -= speed;
			quad.v[1].ty -= speed;
			quad.v[2].ty -= speed;
			quad.v[3].ty -= speed;
		}
	}
	else if(!rotate)
	{
		//move the layer
//		float xt = speed * cos(arc);
//		float yt = speed * sin(arc);
		float xt = speed * costa;
		float yt = speed * sinta;

		quad.v[0].x += xt;	quad.v[0].y += yt;
		quad.v[1].x += xt;	quad.v[1].y += yt;
		quad.v[2].x += xt;	quad.v[2].y += yt;
		quad.v[3].x += xt;	quad.v[3].y += yt;
	}
	else
	{
		//rotate the layer for XY-plane
		float x = ((quad.v[0].x + quad.v[2].x) / 2);
		float y = ((quad.v[0].y + quad.v[2].y) / 2);
//		float cost = cos(arc);
//		float sint = sin(arc);
				
		quad.v[0].x  = (-width/2)*costa - (-height/2)*sinta + x;
		quad.v[0].y  = (-width/2)*sinta + (-height/2)*costa + y;	

		quad.v[1].x  = (width/2)*costa - (-height/2)*sinta + x;
		quad.v[1].y  = (width/2)*sinta + (-height/2)*costa + y;	

		quad.v[2].x  = (width/2)*costa - (height/2)*sinta + x;
		quad.v[2].y  = (width/2)*sinta + (height/2)*costa + y;	

		quad.v[3].x  = (-width/2)*costa - (height/2)*sinta + x;
		quad.v[3].y  = (-width/2)*sinta + (height/2)*costa + y;	

		if(angle > 0)
			angle += (int)speed*100;
		else
			angle -= (int)speed*100;
	}
	//set diffuse color for special usage
	if(flag)
	{
		changetimer++;
		if(changetimer == 1)
		{
			switch(flag)
			{
			case BG_WHITEFLASH:
				mtimer = 32;
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xc0ffffff;
				}
				break;
			case BG_REDFLASH:
				mtimer = 32;
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xc0ff0000;
				}
				break;
			case BG_YELLOWFLASH:
				mtimer = 32;
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xc0ffff00;
				}
				break;

			case BG_WHITEOUT:
				mtimer = 128;
				acol[0] = acol[1] = 0xffffffff;
				acol[2] = acol[3] = 0x80ffffff;
				break;
			case BG_REDOUT:
				mtimer = 128;
				acol[0] = acol[1] = 0xffff0000;
				acol[2] = acol[3] = 0x80ff0000;
				break;

			case BG_LIGHTUP:
				mtimer = 64;
				for(int i=0;i<4;i++)
				{
					acol[i] = 0x00000000;
					ocol[i] = 0xffffffff;
				}
				break;
			case BG_LIGHTRED:
				mtimer = 64;
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xffff0000;
					ocol[i] = 0xffffffff;
				}
				break;

			case FG_PAUSEIN:
				mtimer = 24;
				for(int i=0;i<4;i++)
				{
					acol[i] = ocol[i];
					ocol[i] = 0xf0000000;
				}
				break;
			case BG_FADEOUT:
			case FG_PAUSEOUT:
				mtimer = 24;
				for(int i=0;i<4;i++)
				{
					acol[i] = ocol[i];
					ocol[i] = 0x00000000;
				}
				break;
			}
		}
		
		if(changetimer < mtimer)
		{
			for(int i=0;i<4;i++)
			{
				int toa = GETA(ocol[i]);
				int tor = GETR(ocol[i]);
				int tog = GETG(ocol[i]);
				int tob = GETB(ocol[i]);
				int taa = GETA(acol[i]);
				int tar = GETR(acol[i]);
				int tag = GETG(acol[i]);
				int tab = GETB(acol[i]);
				quad.v[i].col = ARGB(
					(toa-taa)*changetimer/mtimer + taa,
					(tor-tar)*changetimer/mtimer + tar,
					(tog-tag)*changetimer/mtimer + tag,
					(tob-tab)*changetimer/mtimer + tab
					);
			}
		}
		if(changetimer == mtimer)
		{
			for(int i=0;i<4;i++)
			{
				quad.v[i].col = ocol[i];
			}
			changetimer = 0;
			flag = BG_NONE;
		}
	}
}