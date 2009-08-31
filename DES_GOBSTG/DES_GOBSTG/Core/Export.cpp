#include "Export.h"

char Export::resourcefilename[M_PATHMAX];
char Export::resbinname[M_PATHMAX];
partInfo Export::partinfo[RPYPARTMAX];
replayInfo Export::rpyinfo;
int Export::password = 0;

Export::Export()
{
}

Export::~Export()
{
}

void Export::Release()
{
	hge->Release();
}

void Export::clientInitial(bool usesound)
{
	hge->Resource_SetPath(DEFAULT_RESOURCEPATH);
	char respath[_MAX_PATH];
	strcpy(respath, hge->Resource_MakePath(""));
	if(_access(respath, 00) == -1)
	{
		CreateDirectory(respath, NULL);
	}
	SetCurrentDirectory(hge->Resource_MakePath(""));
	
	hge->System_SetState(HGE_LOGFILE, LOG_STR_FILENAME);
	hge->System_SetState(HGE_FPS, M_DEFAULT_FPS);

	hge->System_SetState(HGE_WINDOWED, true);

	char strtitle[M_STRMAX];
	strcpy(strtitle, GAME_TITLE);
	strcat(strtitle, "  ");
	strcat(strtitle, GAME_VERSION_STR);
	hge->System_SetState(HGE_TITLE, strtitle);
	hge->System_SetState(HGE_SCREENWIDTH, M_CLIENT_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, M_CLIENT_HEIGHT);
	hge->System_SetState(HGE_ZBUFFER, true);

	hge->System_SetState(HGE_DONTSUSPEND, true);
	hge->System_SetState(HGE_USESOUND, usesound);
	hge->System_SetState(HGE_HIDEMOUSE, false);

	SetCurrentDirectory(hge->Resource_MakePath(""));
	SetIni();
}

void Export::clientSetMatrix(float _worldx, float _worldy, float _worldz)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, _worldx, _worldy, _worldz);	
	hge->pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
///*
	D3DXMATRIXA16 matView(
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, -1, 0,
		-M_ACTIVECLIENT_WIDTH / 2, M_ACTIVECLIENT_HEIGHT / 2, M_ACTIVECLIENT_HEIGHT / 2, 1);
//*/
/*
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vEyePt( 220, 240, 240);
	D3DXVECTOR3 vLookatPt( 220, 240, 0 );
	D3DXVECTOR3 vUpVec( 0.0f, -1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
*/
	hge->pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
///*
	D3DXMATRIXA16 matProj(
		M_CLIENT_HEIGHT / M_CLIENT_WIDTH, 0, 0, 0,
		0, 1, 0, 0,
		(M_ACTIVECLIENT_WIDTH - M_CLIENT_WIDTH) / M_CLIENT_WIDTH, 0, 0, 1,
		0, 0, 0, 0);
//*/
/*
	D3DXMATRIXA16 matProj;
	D3DXMatrixScaling(&matProj, 1.0f, 1.0f, -1.0f);
	D3DXMatrixPerspectiveOffCenterLH( &matProj, -220, 420, -240, 240, 240, 0 );
*/
	hge->pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Export::SetIni()
{
	hge->System_SetState(HGE_INIFILE, CONFIG_STR_FILENAME);
}

void Export::clientAdjustWindow()
{
	if (!hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, RESCONFIGDEFAULT_DEFAULTWINDOW))
	{
		int windowleft = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, RESCONFIGDEFAULT_WINDOWLEFT);
		int windowtop = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, RESCONFIGDEFAULT_WINDOWTOP);
		int windowwidth = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, RESCONFIGDEFAULT_WINDOWWIDTH);
		int windowheight = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, RESCONFIGDEFAULT_WINDOWHEIGHT);
		HWND windowtopmost = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, RESCONFIGDEFAULT_WINDOWTOPMOST) ? HWND_TOPMOST: HWND_NOTOPMOST;
		SetWindowPos(hge->hwnd, windowtopmost, windowleft, windowtop, windowwidth, windowheight, SWP_FRAMECHANGED);
	}
}

bool Export::GetResourceFile(bool readbin)
{
	strcpy(resourcefilename, hge->Ini_GetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESOURCEFILE, RESCONFIGDEFAULT_RESOURCEFILE));
	strcpy(resbinname, hge->Ini_GetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESBINNAME, RESCONFIGDEFAULT_RESBINNAME));
	if(strlen(resourcefilename) && !readbin)
	{
		strcpy(resourcefilename, hge->Resource_MakePath(resourcefilename));
		return false;
	}
	else
		strcpy(resourcefilename, resbinname);
	return true;
}

int Export::GetPassword()
{
	char szPassword[M_STRMAX];
	strcpy(szPassword, hge->Ini_GetString(RESCONFIGS_RESOURCE, RESCONFIGN_PASSWORD, RESCONFIGDEFAULT_PASSWORD));
	password = hge->Resource_GetCRC((BYTE *)szPassword, strlen(szPassword));
	return password;
}

bool Export::rpyLoad(const char * filename, replayInfo * _rpyinfo, partInfo * _partinfo, replayFrame * _replayframe)
{
	bool ret = false;

	BYTE * content;
	DWORD _size;

	hge->Resource_AttachPack(filename, password ^ REPLAYPASSWORD_XORMAGICNUM);
	content = hge->Resource_Load(hge->Resource_GetPackFirstFileName(filename), &_size);
	if(content)
	{
		memcpy(_rpyinfo ? _rpyinfo : &rpyinfo, content+RPYOFFSET_RPYINFO, RPYSIZE_RPYINFO);
		memcpy(_partinfo ? _partinfo : partinfo, content+RPYOFFSET_PARTINFO, RPYSIZE_PARTINFO*RPYPARTMAX);
		if (_replayframe)
		{
			memcpy(_replayframe, content+RPYOFFSET_INPUTDATA, _size-RPYOFFSET_INPUTDATA);
		}
		ret = true;
	}
	hge->Resource_Free(content);
	return ret;
}

bool Export::rpySetBias(replayFrame * _replayframe)
{
	if (!_replayframe)
	{
		return false;
	}

	float _bias = hge->Timer_GetFPS() - 60.0f;

	_replayframe->bias = 0;
	if (_bias < 0)
	{
		_replayframe->bias |= 0x80;
	}
	_replayframe->bias |= ((BYTE)(sqrtf(fabsf(_bias) * 256.0f)) & 0x7f);
	return true;
}

float Export::rpyGetReplayFPS(replayFrame _replayframe)
{
	int _bias = _replayframe.bias & 0x7f;
	_bias *= _bias;
	if (_replayframe.bias & 0x80)
	{
		_bias = -_bias;
	}
	return (_bias / 256.0f + 60.0f);
}

bool Export::packFile(const char * zipname, const char * filename)
{
	bool ret = false;
	BYTE * _content;
	DWORD _size;
	_content = hge->Resource_Load(filename, &_size);
	if(_content)
	{
		hgeMemoryFile memfile;
		memfile.data = _content;
		memfile.filename = (char *)filename;
		memfile.size = _size;
		if(hge->Resource_CreatePack(zipname, password, &memfile, NULL))
			ret = true;
		hge->Resource_Free(_content);
	}
	return ret;
}

bool Export::packFolder(const char * zipname, const char * foldername)
{
	bool ret = true;
	WIN32_FIND_DATA SearchData;

	char buffer[_MAX_PATH];
	strcpy(buffer, foldername);
	if(buffer[strlen(buffer)-1]!='\\' && buffer[strlen(buffer)-1]!='/')
	{
		strcat(buffer, "\\");
	}
	strcat(buffer, "*");

	HANDLE hSearch = FindFirstFile(buffer, &SearchData);

	while(hSearch != INVALID_HANDLE_VALUE)
	{
		if((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			strcmp(SearchData.cFileName,".") && strcmp(SearchData.cFileName,".."))
		{
			strcpy(buffer, foldername);
			if(buffer[strlen(buffer)-1]!='\\' && buffer[strlen(buffer)-1]!='/')
			{
				strcat(buffer, "\\");
			}
			strcat(buffer, SearchData.cFileName);
			if(!packFolder(zipname, buffer))
			{
				ret = false;
				break;
			}
			strcpy(buffer, foldername);
		}
		else if(!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			strcpy(buffer, foldername);
			if(buffer[strlen(buffer)-1]!='\\' && buffer[strlen(buffer)-1]!='/')
			{
				strcat(buffer, "\\");
			}
			strcat(buffer, SearchData.cFileName);
			BYTE * _content;
			DWORD _size;
			_content = hge->Resource_Load(buffer, &_size);
			if(_content)
			{
				hgeMemoryFile memfile;
				memfile.data = _content;
				memfile.filename = buffer;
				memfile.size = _size;
				if(!hge->Resource_AddFileInPack(zipname, password, &memfile))
				{
					ret = false;
					break;
				}
				hge->Resource_Free(_content);
			}
		}
		if(!FindNextFile(hSearch, &SearchData))
			break;
	}
	FindClose(hSearch);
	return ret;
}

bool Export::effSave(const char * filename, hgeEffectSystem * eff, int texnum)
{
	if(!filename || !eff || texnum < 0)
		return false;
	FILE * efffile = fopen(hge->Resource_MakePath(filename), "wb");
	HTEXTURE _tex = eff->ebi.tex;
	eff->ebi.tex = (HTEXTURE)texnum;
	fwrite(&(eff->ebi), sizeof(hgeEffectBasicInfo), 1, efffile);
	eff->ebi.tex = _tex;

	WORD _eID = 0;
	WORD _aID = 0;

	CEmitterList * emitterItem = eff->eiList;
	while(emitterItem)
	{
		_eID = (emitterItem->emitter.ID) << 8;
		fwrite(&_eID, sizeof(WORD), 1, efffile);
		fwrite(&(emitterItem->emitter.eei), sizeof(hgeEffectEmitterInfo), 1, efffile);

		CAffectorList * affectorItem = emitterItem->emitter.eaiList;
		while(affectorItem)
		{
			_aID = _eID | (affectorItem->affector.ID);
			fwrite(&_aID, sizeof(WORD), 1, efffile);
			fwrite(&(affectorItem->affector.eai), sizeof(hgeEffectAffectorInfo), 1, efffile);
			affectorItem = affectorItem->next;
		}
		emitterItem = emitterItem->next;
	}

	fclose(efffile);
	return true;
}

int Export::effLoad(const char * filename, hgeEffectSystem * eff, HTEXTURE * tex)
{
	if(!filename || !eff)
		return -1;
	int texnum = -1;
	BYTE * _content;
	DWORD _size;
	DWORD _offset = 0;
	WORD _ID = 0;
	_content = hge->Resource_Load(filename, &_size);
	if(!_content)
		return -1;
	memcpy(&(eff->ebi), _content + _offset, sizeof(hgeEffectBasicInfo));
	texnum = eff->ebi.tex;
	if(texnum < 0 || !tex[texnum])
	{
		hge->Resource_Free(_content);
		return texnum;
	}
	eff->ebi.tex = tex[texnum];
	_offset += sizeof(hgeEffectBasicInfo);
	while(_offset < _size)
	{
		memcpy(&_ID, _content + _offset, sizeof(WORD));
		_offset += sizeof(WORD);
		if((_ID & 0xff) == 0)
		{
			eff->AddEmitter(_ID>>8, (hgeEffectEmitterInfo *)(_content + _offset));
			_offset += sizeof(hgeEffectEmitterInfo);
		}
		else
		{
			eff->AddAffector(_ID>>8, _ID & 0xff, (hgeEffectAffectorInfo *)(_content + _offset));
			_offset += sizeof(hgeEffectAffectorInfo);
		}
	}
	hge->Resource_Free(_content);
	return texnum;
}