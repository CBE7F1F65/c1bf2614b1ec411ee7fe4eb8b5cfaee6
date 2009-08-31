#include "Fontsys.h"
#include "Main.h"
#include "Scripter.h"

char Fontsys::text[FONTSYSMAX][M_STRMAX];
HTARGET Fontsys::tar[FONTSYSMAX];
hgeQuad Fontsys::quad[FONTSYSMAX];
int Fontsys::lines[FONTSYSMAX];
int Fontsys::changeID;

HD3DFONT Fontsys::font = NULL;

void Fontsys::Release()
{
	for(int i=0; i<FONTSYSMAX; i++)
	{
		if(tar[i])
			hge->Target_Free(tar[i]);
		tar[i] = NULL;
	}
}

void Fontsys::Init()
{
	Release();
	changeID = -1;
}

void Fontsys::HeatUp()
{
	for (int i=0; i<scr.strdescIndex; i+=(i+1))
	{
		SignUp(FONTSYS_CHATUSE, strdesc[i]);
	}
}

void Fontsys::FocusChanged()
{
	Release();
	SignUp(FONTSYS_CHATUSE);
	SignUp(FONTSYS_RANGENAMEUSE);
	if (changeID >= 0)
	{
		for (int i=0; i<FONTSYSMAX; i++)
		{
			SignUp(i);
		}
	}
}

int Fontsys::strTranslate(char * dtext, const char * stext)
{
	int ret = 1;
	int j=0;
	strcpy(dtext, "");
	for(int i=0;i<(int)strlen(stext);i++)
	{
		if (stext[i] == '\n')
		{
			ret++;
		}
		if(stext[i] == FONTSYS_TRANSCHAR)
		{
			i++;
			if(stext[i] != FONTSYS_TRANSCHAR)
			{
				int strcodecon = stext[i] - '0';
				if(!strcodecon)
				{
					int strcodeval = (stext[i+1] - '0') * 10 +	(stext[i+2] - '0');
					while(j < strcodeval)
					{
						strcat(dtext, " ");
						j++;
					}
					i += 2;
					continue;
				}
				else
				{
					int strcodeval1 = (stext[i+1] - '0') * 10 +	(stext[i+2] - '0');
					int strcodeval2 = (stext[i+3] - '0') * 10 +	(stext[i+4] - '0');
					while(j < strcodeval1)
					{
						strcat(dtext, " ");
						j++;
					}
					i += 4;
					char buffer[32];
					int k = 0;
					while(!(stext[i+1] == FONTSYS_TRANSCHAR && stext[i+2] == FONTSYS_CONTROLCHAR))
					{
						i++;
						buffer[k] = stext[i];
						k++;
					}
					buffer[k] = 0;
					for(int l=k;l<strcodeval2-strcodeval1;l++)
					{
						if(strcodecon == 2)
							strcat(dtext, "0");
						else
							strcat(dtext, " ");
						j++;
					}
					strcat(dtext, buffer);
					i += 2;
					j += k;
					continue;
				}
			}
		}
		char tbuff[2];
		tbuff[0] = stext[i];
		tbuff[1] = 0;
		strcat(dtext, tbuff);
		j++;
	}
	return ret;
}

void Fontsys::SignUp(BYTE ID, const char * _text)
{
	HTEXTURE tex;

	if (_text != NULL)
	{
		lines[ID] = strTranslate(text[ID], _text);
	}
	if (!tar[ID])
	{
		tar[ID] = hge->Target_Create(FONTSYS_TARGETWIDTH, FONTSYS_TARGETHEIGHT, false);
	}

	hge->Gfx_BeginScene(tar[ID]);
	hge->Gfx_Clear(0x00000000);
	hge->Gfx_RenderText(font, text[ID], 0, 0, FONTSYS_TARGETWIDTH, FONTSYS_TARGETHEIGHT, 0xffffffff);
	hge->Gfx_EndScene();

	tex = hge->Target_GetTexture(tar[ID]);

	float h = lines[ID] * M_FONTHEIGHT;

	quad[ID].tex = tex;
	quad[ID].blend = BLEND_DEFAULT;
	quad[ID].v[0].tx = 0;	quad[ID].v[0].ty = 0;
	quad[ID].v[1].tx = 1;	quad[ID].v[1].ty = 0;
	quad[ID].v[2].tx = 1;	quad[ID].v[2].ty = h / FONTSYS_TARGETHEIGHT;
	quad[ID].v[3].tx = 0;	quad[ID].v[3].ty = h / FONTSYS_TARGETHEIGHT;
	quad[ID].v[0].z = quad[ID].v[1].z = quad[ID].v[2].z = quad[ID].v[3].z = 0;
	quad[ID].v[0].x = 0;					quad[ID].v[0].y = 0;
	quad[ID].v[1].x = FONTSYS_TARGETWIDTH;	quad[ID].v[1].y = 0;
	quad[ID].v[2].x = FONTSYS_TARGETWIDTH;	quad[ID].v[2].y = h;
	quad[ID].v[3].x = 0;					quad[ID].v[3].y = h;
}

void Fontsys::Render(BYTE ID, float x, float y, DWORD ucol, DWORD dcol, float hext, float vext)
{
	float w = quad[ID].v[1].x - quad[ID].v[0].x;
	float h = quad[ID].v[2].y - quad[ID].v[0].y;

	quad[ID].v[0].col = quad[ID].v[1].col = ucol;
	quad[ID].v[2].col = quad[ID].v[3].col = dcol;

	quad[ID].v[0].x = x - hext;		quad[ID].v[0].y = y - vext;
	quad[ID].v[1].x = x + w + hext;	quad[ID].v[1].y = y - vext;
	quad[ID].v[2].x = x + w + hext;	quad[ID].v[2].y = y + h + vext;
	quad[ID].v[3].x = x - hext;		quad[ID].v[3].y = y + h + vext;

	hge->Gfx_RenderQuad(&quad[ID]);

	quad[ID].v[0].x = x;		quad[ID].v[0].y = y;
	quad[ID].v[1].x = x + w;	quad[ID].v[1].y = y;
	quad[ID].v[2].x = x + w;	quad[ID].v[2].y = y + h;
	quad[ID].v[3].x = x;		quad[ID].v[3].y = y + h;
}