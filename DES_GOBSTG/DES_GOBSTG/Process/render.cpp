#include "processPrep.h"

int Process::render()
{
	//BGLayer
	for(int i=0;i<BGLAYERMAX;i++)
	{
		if(bg[i].exist)
		{
			hge->Gfx_RenderQuad(& bg[i].quad);
		}
	}
	if(bgmask.exist)
	{
		hge->Gfx_RenderQuad(& bgmask.quad);
	}

	if(Player::p.exist || state == STATE_CONTINUE)
	{
		if (bossinfo.flag && bossinfo.range)
		{
			bossinfo.RenderTimeCircle();
		}
		if(!Player::p.bCircle || Player::p.ID == M_CHARAENUM_BUSINIAO)
		{
			for(int i=0;i<GHOSTMAX;i++)
			{
				if(gh[i].exist)
					gh[i].Render();
			}
		}
		if(Player::p.exist)
			Player::p.Render();
		if (pb.size)
		{
			DWORD i = 0;
			DWORD size = pb.size;
			for (pb.toBegin(); i<size; pb.toNext(), i++)
			{
				if (pb.isValid())
				{
					(*pb).Render();
				}
				
			}
		}
		/*
		for(list<PlayerBullet>::iterator i=pb.begin();i!=pb.end(); i++)
		{
			i->Render();
		}
		*/
		if(Player::p.ID == M_CHARAENUM_BUSINIAO && Player::p.flag & PLAYER_SHOOT)
		{
			PlayerBullet::RenderBeam();
		}
		if(Player::p.exist)
		{
			Player::p.RenderEffect();
		}

		for(int i=0;i<ENEMYMAX;i++)
		{
			if(en[i].exist)
				en[i].Render();
		}
		for(int i=0;i<ENEMYMAX;i++)
		{
			if(en[i].exist)
				en[i].RenderEffect();
		}

		//3D objs
		for(int i=0; i<EFFECTSYSMAX; i++)
		{
			if(effsys[i].exist)
			{
				effsys[i].Render();
			}
		}

		if (be.size)
		{
			DWORD i = 0;
			DWORD size = be.size;
			for (be.toBegin(); i<size; be.toNext(), i++)
			{
				if (be.isValid())
				{
					(*be).Render();
				}
			}
		}

		if (bu.size)
		{
			for (int i=BULLETTYPEMAX-1; i>=0; i--)
			{
				if (Bullet::renderDepth[i].haveType)
				{
					for (bu.toIndex(Bullet::renderDepth[i].startIndex); bu.index != Bullet::renderDepth[i].endIndex; bu.toNext())
					{
						if (bu.isValid() && (*bu).type == i)
						{
							(*bu).Render();
						}
					}
				}
			}
			/*
			for (bu.toBegin(); i<size; bu.toNext(), i++)
			{
				if (bu.isValid())
				{
					(*bu).Render();
				}
			}
			*/
		}
		if(BossInfo::flag)
		{
			bossinfo.Render();
		}

		if(Player::p.bCircle && Player::p.ID != M_CHARAENUM_BUSINIAO)
		{
			for(int i=0;i<GHOSTMAX;i++)
			{
				if(gh[i].exist)
					gh[i].Render();
			}
		}
		if (mi.size)
		{
			DWORD i = 0;
			DWORD size = mi.size;
			for (mi.toBegin(); i<size; mi.toNext(), i++)
			{
				if (mi.isValid())
				{
					(*mi).Render();
				}
			}
		}
		/*
		for(VectorList<Item>::iterator i=mi.begin();i!=mi.end(); i++)
		{
			i->Render();
		}
		*/

		if(Chat::chatting)
		{
			for(int i = 0; i < CHATTERMAX; i++)
			{
				hge->Gfx_RenderQuad(&chatter[i].quad);
			}
		}

		if(Chat::chatting && chat_font)
		{
			Fontsys::Render(FONTSYS_CHATUSE, 70, 340, chat.col, chat.col);
		}

		if (Item::infofont.size)
		{
			DWORD i = 0;
			DWORD size = Item::infofont.size;
			for (Item::infofont.toBegin(); i<size; Item::infofont.toNext(), i++)
			{
				if (!Item::infofont.isValid())
				{
					continue;
				}
				infoFont * _i = &(*(Item::infofont));
				if(state != STATE_PAUSE)
					_i->timer++;
				if(_i->yellow)
					Item::item_font->SetColor(0xffffff00);
				else
					Item::item_font->SetColor(0xffffffff);
				if(_i->timer < 16)
					Item::item_font->printf(_i->x, _i->y-10-_i->timer, HGETEXT_CENTER, "%s", _i->cScore);
				else
				{
					Item::infofont.pop();
				}
			}
		}
		/*
		for(VectorList<infoFont>::iterator i = Item::infofont.begin(); i != Item::infofont.end(); i++)
		{
		}
		*/

		DWORD tcolor;
		if(Player::p.x < 170 && Player::p.y > 420)
		{
			tcolor = 0x20ffffff;
		}
		else
		{
			tcolor = 0xc0ffffff;
		}
		Player::p.esCircleBox.alpha = (BYTE)((tcolor>>24) + 0x3f);
		Player::p.esCircleBox.Render();
		if(Player::p.bCircle)
		{
			for(int i=0;i<PL_NESCIRCLE;i++)
			{
				Player::p.esCircle[i].sprite->SetColor(tcolor);
				Player::p.esCircle[i].sprite->RenderEx(18.0f*i+44.0f, 450.0f, ARC(Player::p.esCircle[i].angle+Player::p.esCircle[i].headangle), Player::p.esCircle[i].hscale*0.8f);
			}
			if(Ghost::totalghost - Ghost::totalhalfghost)
			{
				if(tcolor == 0xc0ffffff)
					tcolor = ((((ROLL(time, 0x08)) * 0x20) << 24)) + 0xffffff;
				Player::p.esCircle[0].sprite->SetColor(tcolor);
				Player::p.esCircle[0].sprite->RenderEx(160, 450, 0, 0.8f);
			}
		}
	}

	for(int i=0;i<FGLAYERMAX;i++)
	{
		if(fg[i].exist)
		{
			hge->Gfx_RenderQuad(& fg[i].quad);
		}
	}
	if(fgpause.exist)
	{
		hge->Gfx_RenderQuad(& fgpause.quad);
	}
	for(list<Selector>::iterator i = sel.begin();i != sel.end(); i++)
	{
		(i->sprite)->RenderEx(i->x, i->y, 0, i->hscale, i->vscale);
	}
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
	{
		if(InfoSelect::select == i->ID && !i->nonactive)
			Fontsys::Render(i->ID, i->x-2, i->y-2, i->ucol, i->dcol, 3, 1);
		else
			Fontsys::Render(i->ID, i->x, i->y, i->ucol-0x20000000, i->dcol-0x20000000);
	}

	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, 0, 0, 0);
	hge->pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if(fgpanel.exist)
	{
		hge->Gfx_RenderQuad(& fgpanel.quad);
	}
	if(Player::p.exist && BossInfo::flag)
	{
		BossInfo::spbossx->Render(en[ENEMY_MAINBOSSINDEX].x, 472);
	}
	if(fgdiff.exist)
	{
		hge->Gfx_RenderQuad(& fgdiff.quad);
	}

	
	if(Player::p.exist || state == STATE_CONTINUE)
	{
		hge->Gfx_RenderQuad(&iqfPower.quad);

		if(info_font)
		{
			info_font->SetColor(0xffffffff);
			if(Player::p.nHiScore < Player::p.nScore)
				Player::p.nHiScore = Player::p.nScore;
			info_font->printfb(0, 35, 620, 20, HGETEXT_MIDDLE | HGETEXT_RIGHT, "%I64d", (__int64)Player::p.nHiScore);
			info_font->printfb(0, 60, 620, 20, HGETEXT_MIDDLE | HGETEXT_RIGHT, "%I64d", (__int64)Player::p.nScore);

			if(Player::p.nPower < PL_POWERMAX)
				info_font->printf(505, 170, HGETEXT_LEFT, "%d", Player::p.nPower);
			else
				info_font->printf(505, 170, HGETEXT_LEFT, "M A X");
			info_font->printf(505, 195, HGETEXT_LEFT, "%d", Player::p.nGraze);
			info_font->printf(505, 220, HGETEXT_LEFT, "%4d/%4d", Player::p.nPoint, Player::p.nNext);

			info_font->printf(530, 250, HGETEXT_LEFT, "%d", Player::p.nAliveness);
			
//			info_font->SetColor(0xffffffff);
			char buffer[M_STRITOAMAX];
			int i;
			for(i=0;i<Player::p.nLife;i++)
				buffer[i] = 0x1A;
			buffer[i] = 0;
			info_font->printf(500, 105, HGETEXT_LEFT, "%s", buffer);
			for(i=0;i<Player::p.nBomb;i++)
				buffer[i] = 0x1B;
			buffer[i] = 0;
			info_font->printf(500, 130, HGETEXT_LEFT, "%s", buffer);
		}
		
	}
	if(FPS_font)
	{
		if (replaymode)
		{
			FPS_font->printf(370, 445, 0, "%.2f", replayFPS);
		}
		FPS_font->printf(
#ifdef __DEBUG
			540,
			420,
			0,
			"%f\n%f\n%f",
			hge->Timer_GetWorstFPS(35),
			hge->Timer_GetFPS(),
#else
			540,
			440,
			0,
			"%.2f\n%.2f",
			hge->Timer_GetFPS(35),
#endif
			hge->Timer_GetTime()
			);
#ifdef __DEBUG
		if (Player::p.exist)
		{
//			FPS_font->printf(370, 465, 0, "%d / %d", bu.size, renderedbu);
			FPS_font->printf(0, 460, 0, "%d / %d", scene, time);
		}
#endif // __DEBUG
	}
	return PGO;
}