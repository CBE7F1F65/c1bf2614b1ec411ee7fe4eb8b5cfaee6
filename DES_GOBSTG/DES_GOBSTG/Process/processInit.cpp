#include "processPrep.h"

int Process::processInit()
{
	bool rebuilddone = false;
	if(_access(CONFIG_STR_FILENAME, 00) == -1)
	{
rebuild:
		if (rebuilddone)
		{
			errorcode = PROC_ERROR_INIFILE;
			return PQUIT;
		}
		DeleteFile(CONFIG_STR_FILENAME);

		hge->	Ini_SetInt(data.translateSection(data.sLinkType(DATAS_HEADER)), data.translateName(data.nLinkType(DATAN_GAMEVERSION)), GAME_VERSION);
		hge->	Ini_SetString(data.translateSection(data.sLinkType(DATAS_HEADER)), data.translateName(data.nLinkType(DATAN_SIGNATURE)), GAME_SIGNATURE);

		hge->	Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESOURCEFILE, RESCONFIGDEFAULT_RESOURCEFILE);
		hge->	Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESBINNAME, RESCONFIGDEFAULT_RESBINNAME);
		hge->	Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_PASSWORD, RESCONFIGDEFAULT_PASSWORD);

		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYBOMB, RESCONFIGDEFAULT_KEYBOMB);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCIRCLE, RESCONFIGDEFAULT_KEYCIRCLE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);

		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYBOMB, RESCONFIGDEFAULT_JOYBOMB);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCIRCLE, RESCONFIGDEFAULT_JOYCIRCLE);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);

		hge->	Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, RESCONFIGDEFAULT_VOLMUSIC);
		hge->	Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, RESCONFIGDEFAULT_VOLSE);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_MAXPLAYER, RESCONFIGDEFAULT_MAXPLAYER);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_DEFAULTLEVEL, RESCONFIGDEFAULT_DEFAULTLV);
		hge->	Ini_SetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, RESCONFIGDEFAULT_USERNAME);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, RESCONFIGDEFAULT_DEFAULTWINDOW);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, RESCONFIGDEFAULT_WINDOWLEFT);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, RESCONFIGDEFAULT_WINDOWTOP);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, RESCONFIGDEFAULT_WINDOWWIDTH);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, RESCONFIGDEFAULT_WINDOWHEIGHT);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, RESCONFIGDEFAULT_WINDOWTOPMOST);
#ifdef __DEBUG
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_DEBUG_JOYSPEEDUP, RESCONFIGDEFAULT_DEBUG_JOYSPEEDUP);
		HGELOG("Config File rebuilt");

		rebuilddone = true;
#endif
	}

	keyUp			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP);
	keyDown			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN);
	keyLeft			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT);
	keyRight		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT);
	keyFire			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE);
	keyBomb			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYBOMB, RESCONFIGDEFAULT_KEYBOMB);
	keySlow			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW);
	keyCircle		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCIRCLE, RESCONFIGDEFAULT_KEYCIRCLE);
	keyPause		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
	keySkip			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
	keyEnter		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
	keyEscape		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
	keyCapture		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);

	joyFire			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
	joyBomb			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYBOMB, RESCONFIGDEFAULT_JOYBOMB);
	joySlow			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
	joyCircle		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCIRCLE, RESCONFIGDEFAULT_JOYCIRCLE);
	joyPause		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);
#ifdef __DEBUG
	debug_joySpeedUp = hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_DEBUG_JOYSPEEDUP, RESCONFIGDEFAULT_DEBUG_JOYSPEEDUP);
#endif

	bgmvol			= hge->Ini_GetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, RESCONFIGDEFAULT_VOLMUSIC);
	sevol			= hge->Ini_GetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, RESCONFIGDEFAULT_VOLSE);

	maxplayer		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_MAXPLAYER, RESCONFIGDEFAULT_MAXPLAYER);
	screenmode		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
	defaultdifflv	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_DEFAULTLEVEL, RESCONFIGDEFAULT_DEFAULTLV);
	strcpy(username, hge->Ini_GetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, RESCONFIGDEFAULT_USERNAME));
	hge->renderSkip	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);
	circlearound	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_CIRCLEAROUND, RESCONFIGDEFAULT_CIRCLEAROUND);

	bool binmode = Export::GetResourceFile();

	data.GetIni();
	//

	if(hge->Ini_GetInt(data.translateSection(data.sLinkType(DATAS_HEADER)), data.translateName(data.nLinkType(DATAN_GAMEVERSION)), -1) != GAME_VERSION)
		goto rebuild;
	for (int i=0; i<13; i++)
	{
		if (keyKey[i] < 0 || keyKey[i] >= M_KEYKEYMAX)
		{
			goto rebuild;
		}
	}
	for (int i=0; i<5; i++)
	{
		if(joyKey[i] < 0 || joyKey[i] >= M_JOYKEYMAX)
		{
			goto rebuild;
		}
	}
#ifdef __DEBUG
	if(debug_joySpeedUp < 0 || debug_joySpeedUp >= M_JOYKEYMAX)
		goto rebuild;
#endif
	if(maxplayer < 0 || maxplayer > M_PL_PLAYERMAX - 1)
		goto rebuild;
	if(screenmode < 0 || screenmode > 1)
		goto rebuild;
	if(defaultdifflv < 0 || defaultdifflv > M_DIFFI_EXTRA)
		goto rebuild;
	if(bgmvol < 0 || bgmvol > 100)
		goto rebuild;
	if(sevol < 0 || sevol > 100)
		goto rebuild;
	if (hge->renderSkip < 0 || hge->renderSkip > 3)
	{
		goto rebuild;
	}

#ifdef __DEBUG
	HGELOG("Gained data from Config File.");
#endif

	if(binmode)
	{
		data.binmode = true;
		scr.binmode = true;
#ifdef __RELEASE
//		hge->System_SetState(HGE_LOGFILE, "");
#endif // __RELEASE
	}
	else
	{
		if(!res.Fill())
		{
#ifdef __DEBUG
			HGELOG("Error in Filling Resource Data.");
#endif
			errorcode = PROC_ERROR_RESOURCE;
			return PQUIT;
		}
		if(!scr.LoadAll())
		{
			errorcode = PROC_ERROR_SCRIPT;
			return PQUIT;
		}
		if(!res.Pack(strdesc, scr.customconstName))
		{
#ifdef __DEBUG
			HGELOG("Error in Packing Resource Data.");
#endif
			errorcode = PROC_ERROR_TRANSLATE;
			return PQUIT;
		}
		if(!res.SetDataFile())
		{
			errorcode = PROC_ERROR_DATA;
			return PQUIT;
		}
	}
	if(!res.Gain(strdesc, scr.customconstName))
	{
#ifdef __DEBUG
		HGELOG("Error in Gaining Resource Data.");
#endif
		errorcode = PROC_ERROR_DATA;
		return PQUIT;
	}
	if(scr.binmode && !scr.LoadAll())
	{
		errorcode = PROC_ERROR_SCRIPT;
		return PQUIT;
	}
	if(!data.SetFile(Export::resourcefilename, DATA_RESOURCEFILE))
	{
#ifdef __DEBUG
		HGELOG("Error in Setting Resource File");
#endif
		errorcode = PROC_ERROR_DATA;
		return PQUIT;
	}

	if(!res.LoadPackage())
	{
		errorcode = PROC_ERROR_PACKAGE;
		return PQUIT;
	}

	BGLayer::Init();

	SE::vol = sevol;
	if(!SE::Initial())
	{
		errorcode = PROC_ERROR_SOUND;
		return PQUIT;
	}

	if(FPS_font)
		delete FPS_font;

	FPS_font = new hgeFont(res.resdata.normalfontfilename);
#ifdef __DEBUG
	if(FPS_font == NULL)
	{
		HGELOG("%s\nFailed in loading Font File %s.", HGELOG_ERRSTR, res.resdata.normalfontfilename);
		errorcode = PROC_ERROR_FONT;
		return PQUIT;
	}
	else
	{
		HGELOG("Gained data from Font File %s.", res.resdata.normalfontfilename);
	}
#endif
	FPS_font->SetColor(0xffffffff);

	if(info_font)
	{
		delete info_font;
	}
	info_font = new hgeFont(res.resdata.normalfontfilename);
	info_font->SetColor(0xceffffff);
	info_font->SetScale(1.5f);

	if(chat_font)
		hge->Font_Free(chat_font);
	chat_font = hge->Font_Load(res.resdata.widefontname, 20);
	Fontsys::font = chat_font;

	char tnbuffer[M_STRMAX];
	for(int i=1;i<TEXMAX;i++)
	{
		if(tex[i])
			hge->Texture_Free(tex[i]);
		tex[i] = NULL;

		strcpy(tnbuffer, res.resdata.texfilename[i]);
		if(!strlen(tnbuffer))
		{
			if(i < TEX_FREEBEGIN)
				strcpy(tnbuffer, res.resdata.texfilename[TEX_WHITE]);
			else
				continue;
		}

		tex[i] = hge->Texture_Load(tnbuffer);
#ifdef __DEBUG
		if(tex[i] == NULL)
		{
			HGELOG("%s\nFailed in loading Texture File %s.(To be assigned to Index %d)", HGELOG_ERRSTR, tnbuffer, i);
			errorcode = PROC_ERROR_TEXTURE;
			return PQUIT;
		}
		else
		{
			HGELOG("Texture File %s loaded.(Assigned to Index %d)", tnbuffer, i);
		}
#endif
	}

	tex[TEX_WORD] = hge->Texture_Load(res.resdata.texfilename[TEX_WORD]);
#ifdef __DEBUG
	if(tex[TEX_WORD] == NULL)
	{
		HGELOG("%s\nFailed in loading Texture File %s.(To be assigned to Index %d)", HGELOG_ERRSTR, res.resdata.texfilename[TEX_WORD], TEX_WORD);
		errorcode = PROC_ERROR_TEXTURE;
		return PQUIT;
	}
	else
	{
		HGELOG("Texture File %s loaded.(Assigned to Index %d)", res.resdata.texfilename[TEX_WORD], TEX_WORD);
	}
#endif

	Fontsys::Init();
	//Heatup
	Fontsys::HeatUp();
	/*
	hgeQuad quad;
	quad.blend = BLEND_DEFAULT;

	quad.v[0].tx = 0;	quad.v[0].ty = 0;
	quad.v[1].tx = 1;	quad.v[1].ty = 0;
	quad.v[2].tx = 1;	quad.v[2].ty = 1;
	quad.v[3].tx = 0;	quad.v[3].ty = 1;

	quad.v[0].x = M_CLIENT_LEFT;	quad.v[0].y = M_CLIENT_TOP;
	quad.v[1].x = M_CLIENT_RIGHT;	quad.v[1].y = M_CLIENT_TOP;
	quad.v[2].x = M_CLIENT_RIGHT;	quad.v[2].y = M_CLIENT_BOTTOM;
	quad.v[3].x = M_CLIENT_RIGHT;	quad.v[3].y = M_CLIENT_BOTTOM;

	quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = 0xffffffff;
	hge->Gfx_BeginScene();
	for (int i=0; i<TEXMAX; i++)
	{
		if (tex[i])
		{
			quad.tex = tex[i];
			hge->Gfx_RenderQuad(&quad);
		}
	}
	hge->Gfx_EndScene();
	*/

	Selector::Clear();

	pauseret	= false;
	luchara		= 0;
	nowdifflv	= defaultdifflv;
	randi		= 0;
	//
	
	hge->System_SetState(HGE_WINDOWED, !(bool)screenmode);
	hge->System_SetState(HGE_HIDEMOUSE, (bool)screenmode);

	if(!screenmode)
	{
		Export::clientAdjustWindow();
	}

#ifdef __DEBUG
	HGELOG("All resources loaded.\n");
#endif

	errorcode = PROC_ERROR_NONE;

	//
	titleselect = 0;

	replaymode = false;
	practicemode = false;
	rangemode = false;

	playing = false;
	playtimeStart = 0;

	time = 0;
	state = STATE_TITLE;
	return PTURN;
}