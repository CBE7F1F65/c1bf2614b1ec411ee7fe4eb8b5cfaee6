#include "BResource.h"
#include "data.h"

Resource res;

Resource::Resource()
{
	ZeroMemory(&resdata, sizeof(resourceData));
}

Resource::~Resource()
{
}

//Scripter::LoadAll
bool Resource::Fill()
{
	DWORD sec;
	DWORD name;

	//resource
	strcpy(resdata.normalfontfilename, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_FONT), data.nLinkType(RESDATAN_FILENAME), RESDEFAULT_FONTFILENAME));
	strcpy(resdata.widefontname, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_FONT), data.nLinkType(RESDATAN_FONT), RESDEFAULT_FONTFONT));

	sec = data.sLinkType(RESDATAS_FOLDER);
	strcpy(resdata.scriptfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SCRIPTFOLDER), RESDEFAULT_SCRIPTFOLDER));
	if(_access(resdata.scriptfoldername, 00) == -1)
	{
		CreateDirectory(resdata.scriptfoldername, NULL);
	}
	strcpy(resdata.snapshotfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SNAPSHOTFOLDER), RESDEFAULT_SNAPSHOTFOLDER));
	if(_access(resdata.snapshotfoldername, 00) == -1)
	{
		CreateDirectory(resdata.snapshotfoldername, NULL);
	}
	strcpy(resdata.replayfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_REPLAYFOLDER), RESDEFAULT_REPLAYFOLDER));
	if(_access(resdata.replayfoldername, 00) == -1)
	{
		CreateDirectory(resdata.replayfoldername, NULL);
	}
	strcpy(resdata.datafoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_DATAFOLDER), RESDEFAULT_DATAFOLDER));
	if(_access(resdata.datafoldername, 00) == -1)
	{
		CreateDirectory(resdata.datafoldername, NULL);
	}
	strcpy(resdata.effectsysfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_EFFECTSYSFOLDER), RESDEFAULT_DATAFOLDER));
	if(_access(resdata.effectsysfoldername, 00) == -1)
	{
		CreateDirectory(resdata.effectsysfoldername, NULL);
	}

	char buffer[M_STRMAX];

	sec = data.sLinkType(RESDATAS_DATAFILE);

	strcpy(resdata.binname, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_BINFILE), RESDEFAULT_DATABIN));
	strcpy(resdata.binfilename, resdata.datafoldername);
	strcat(resdata.binfilename, resdata.binname);
	//copy to data
	//

	strcpy(resdata.rabinname, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_RANGEACCESSFILE), RESDEFAULT_DATARANGEACCESS));
	strcpy(resdata.rangeaccessfilename, resdata.datafoldername);
	strcat(resdata.rangeaccessfilename, resdata.rabinname);
	//copy to data
	//

	strcpy(resdata.scrbinname, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SCRIPTFILE), RESDEFAULT_DATASCRIPT));
	strcpy(resdata.scriptfilename, resdata.datafoldername);
	strcat(resdata.scriptfilename, resdata.scrbinname);
	//copy to data
	//

	strcpy(resdata.customconstfilename, resdata.datafoldername);
	strcat(resdata.customconstfilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_CUSTOMCONSTFILE), RESDEFAULT_DATACUSTOMCONST));
	//copy to data
	//

	strcpy(resdata.rangedefinefilename, resdata.datafoldername);
	strcat(resdata.rangedefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_RANGEDEFINEFILE), RESDEFAULT_DATARANGEDEFINE));
	//copy to data
	//

	strcpy(resdata.musicdefinefilename, resdata.datafoldername);
	strcat(resdata.musicdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_MUSICDEFINEFILE), RESDEFAULT_DATAMUSICDEFINE));
	//copy to data
	//

	name = data.nLinkType(RESDATAN_TYPE);
	for(int i=0;i<PACKAGEMAX;i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.packagefilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_PACKAGE), name, ""));
	}

	for(int i=0;i<TEXMAX;i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.texfilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_TEXTURE), name, ""));
	}

	for(int i=0;i<SEMAX;i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.sefilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_SE), name, ""));
	}

	for(int i=0; i<EFFECTSYSTYPEMAX; i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.effectsysfilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_EFFECTSYS), name, ""));
	}

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_EXTENSION), data.nLinkType(RESDATAN_SCRIPTEXT7), RESDEFAULT_SCRIPTEXT7));
	if(strlen(buffer) > 8)
	{
#ifdef __DEBUG
		HGELOG("%s\nExtension %s for Script File too long.(Need to be less than 8)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.scriptextensionname7, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_EXTENSION), data.nLinkType(RESDATAN_REPLAYEXT7), RESDEFAULT_REPLAYEXT7));
	if(strlen(buffer) > 8)
	{
#ifdef __DEBUG
		HGELOG("%s\nExtension %s for Replay File too long.(Need to be less than 8)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replayextensionname7, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYSIGNATURE11), RESDEFAULT_RPYSIGNATURE11));
	if(strlen(buffer) > 12)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Signature %s too long.(Need to be less than 12)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaysignature11, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYTEMPSIGN3), RESDEFAULT_RPYTEMPSIGN3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Temp Sign %s too long.(Need to be less than 4)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaytempsign3, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYCOMPLETESIGN3), RESDEFAULT_RPYCOMPLETESIGN3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Complete Sign %s too long.(Need to be less than 4)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaycompletesign3, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYTAG3), RESDEFAULT_RPYTAG3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Tag %s too long.(Need to be less than 4)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaytag3, buffer);

	CopyData();

	//music
	sec = data.sLinkType(DATAS_MUSIC);
	for(int i=0; i<MUSICMAX; i++)
	{
		sec = data.sLinkNum(sec, i+1);
		strcpy(musdata[i].musicname, data.sRead(DATA_MUSICDEFINEFILE, sec, data.nLinkType(DATAN_MUSICNAME), ""));
		strcpy(musdata[i].musicfilename, data.sRead(DATA_MUSICDEFINEFILE, sec, data.nLinkType(DATAN_MUSICFILENAME), ""));
		musdata[i].startpos = data.lRead(DATA_MUSICDEFINEFILE, sec, data.nLinkType(DATAN_STARTPOS), 0);
		musdata[i].endpos = data.lRead(DATA_MUSICDEFINEFILE, sec, data.nLinkType(DATAN_ENDPOS), 0);
		musdata[i].looppos = data.lRead(DATA_MUSICDEFINEFILE, sec, data.nLinkType(DATAN_LOOPPOS), 0);

		name = data.nLinkType(DATAN_EXPLAIN);
		strcpy(musdata[i].explain_1, data.sRead(DATA_MUSICDEFINEFILE, sec, data.nLinkNum(name, 1), ""));
		strcpy(musdata[i].explain_2, data.sRead(DATA_MUSICDEFINEFILE, sec, data.nLinkNum(name, 2), ""));
		strcpy(musdata[i].explain_3, data.sRead(DATA_MUSICDEFINEFILE, sec, data.nLinkNum(name, 3), ""));
		strcpy(musdata[i].explain_4, data.sRead(DATA_MUSICDEFINEFILE, sec, data.nLinkNum(name, 4), ""));
	}

	//range
	rdata.clear();
	sec = data.sLinkType(DATAS_RANGE);
	for(int i=S0; i<S1; i++)
	{
		sec = data.sLinkNum(sec, i);
		int _turntoscene = data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_TURNTOSCENE), 0);
		if(_turntoscene)
		{
			rangeData _rdata;
			_rdata.sno = i;
			_rdata.isrange = (bool)data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_ISRANGE), 0);
			_rdata.rangenumber = data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_RANGENUMBER), 0);
			strcpy(_rdata.rangename, data.sRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_RANGENAME), ""));
			_rdata.timelimit = data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_TIMELIMIT), 0);
			_rdata.remain = data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_REMAIN), 0);
			strcpy(_rdata.enemyname, data.sRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_ENEMYNAME), ""));
			strcpy(_rdata.enemypinyin, data.sRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_ENEMYPINYIN), ""));
			_rdata.bonus = data.lRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_BONUS), 0);
			_rdata.turntoscene = _turntoscene;
			_rdata.iswait = (bool)data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_ISWAIT), 0);
			_rdata.autorank = data.iRead(DATA_RANGEDEFINEFILE, sec, data.nLinkType(DATAN_AUTORANK), 0);

			name = data.nLinkType(DATAN_EXPLAIN);
			strcpy(_rdata.explain_1, data.sRead(DATA_RANGEDEFINEFILE, sec, data.nLinkNum(name, 1), ""));
			strcpy(_rdata.explain_2, data.sRead(DATA_RANGEDEFINEFILE, sec, data.nLinkNum(name, 2), ""));

			rdata.push_back(_rdata);
		}
	}

	return true;
}

bool Resource::Pack(void * pStrdesc, void * pCustomConstName)
{
	if(pStrdesc == NULL || pCustomConstName == NULL)
		return false;

	DWORD size = M_BINHEADER_OFFSET + 
		sizeof(resourceData) + 
		sizeof(char) * M_STRMAX * STRINGDESCMAX + 
		sizeof(char) * M_STRMAX * SCR_CUSTOMCONSTMAX + 
		sizeof(musicData) * MUSICMAX +
		sizeof(rangeData) * rdata.size();
	BYTE * content = (BYTE *)malloc(size);
	if(!content)
		return false;

	DWORD offset;

	BYTE * _header = data.CreateMemHeader(DATA_RESOURCEFILE);
	if(!_header)
		return false;
	memcpy(content, _header, M_BINHEADER_OFFSET);
	offset = M_BINHEADER_OFFSET;
	free(_header);

	memcpy(content+offset, &resdata, sizeof(resourceData));
	offset += sizeof(resourceData);
	memcpy(content+offset, pStrdesc, sizeof(char) * M_STRMAX * STRINGDESCMAX);
	offset += sizeof(char) * M_STRMAX * STRINGDESCMAX;
	memcpy(content+offset, pCustomConstName, sizeof(char) * M_STRMAX * SCR_CUSTOMCONSTMAX);
	offset += sizeof(char) * M_STRMAX * SCR_CUSTOMCONSTMAX;
	memcpy(content+offset, musdata, sizeof(musicData) * MUSICMAX);
	offset += sizeof(musicData) * MUSICMAX;
	for(vector<rangeData>::iterator i=rdata.begin(); i!=rdata.end(); i++)
	{
		memcpy(content+offset, &(*i), sizeof(rangeData));
		offset += sizeof(rangeData);
	}

	hgeMemoryFile memfile;
	memfile.data = content;
	memfile.filename = data.resbinname;
	memfile.size = size;

	bool ret = false;
	ret = hge->Resource_CreatePack(hge->Resource_MakePath(data.resbinname), data.password, &memfile, NULL);

	free(content);

	return ret;
}

bool Resource::Gain(void * pStrdesc, void * pCustomConstName)
{
	BYTE * content;
	DWORD size;
	bool ret = false;

	hge->Resource_AttachPack(data.resourcefilename, data.password);
	content = hge->Resource_Load(data.resbinname, &size);
	if(content)
	{
		rdata.clear();
		if(data.CheckMemHeader(content, size, DATA_RESOURCEFILE))
		{
			DWORD offset = M_BINHEADER_OFFSET;
			memcpy(&resdata, content+offset, sizeof(resourceData));
			offset += sizeof(resourceData);
			if(pStrdesc)
				memcpy(pStrdesc, content+offset, sizeof(char) * M_STRMAX * STRINGDESCMAX);
			offset += sizeof(char) * M_STRMAX * STRINGDESCMAX;
			if(pCustomConstName)
				memcpy(pCustomConstName, content+offset, sizeof(char) * M_STRMAX * SCR_CUSTOMCONSTMAX);
			offset += sizeof(char) * M_STRMAX * SCR_CUSTOMCONSTMAX;
			memcpy(musdata, content+offset, sizeof(musicData) * MUSICMAX);
			offset += sizeof(musicData) * MUSICMAX;
			while(offset < size)
			{
				rangeData _rdata;
				memcpy(&_rdata, content+offset, sizeof(rangeData));
				offset += sizeof(rangeData);
				rdata.push_back(_rdata);
			}
			CopyData();
			if(SetDataFile())
				ret = true;
		}
	}
	hge->Resource_Free(content);

	if(_access(resdata.snapshotfoldername, 00) == -1)
	{
		CreateDirectory(resdata.snapshotfoldername, NULL);
	}
	if(_access(resdata.replayfoldername, 00) == -1)
	{
		CreateDirectory(resdata.replayfoldername, NULL);
	}
	return ret;
}

bool Resource::LoadPackage()
{
	for(int i=0; i<PACKAGEMAX; i++)
	{
		if(strlen(resdata.packagefilename[i]) && !hge->Resource_AttachPack(resdata.packagefilename[i], data.password))
		{
#ifdef __DEBUG
			HGELOG("%s\nFailed in Loading Package File %s.", HGELOG_ERRSTR, resdata.packagefilename[i]);
#endif
			return false;
		}
	}
	return true;
}

void Resource::CopyData()
{
	data.binname = resdata.binname;
	data.binfilename = resdata.binfilename;
	data.rabinname = resdata.rabinname;
	data.rangeaccessfilename = resdata.rangeaccessfilename;
	data.scrbinname = resdata.scrbinname;
	data.scriptfilename = resdata.scriptfilename;
	if (!data.binmode)
	{
		data.customconstfilename = resdata.customconstfilename;
		data.rangedefinefilename = resdata.rangedefinefilename;
		data.musicdefinefilename = resdata.musicdefinefilename;
	}
}

bool Resource::SetDataFile()
{
	if(!data.SetFile(data.binfilename, DATA_BINFILE))
		return false;
	if(!data.SetFile(data.rangeaccessfilename, DATA_RANGEACCESSFILE))
		return false;
	if(!data.SetFile(data.scriptfilename, DATA_SCRIPTFILE))
		return false;
	if (!data.binmode)
	{
		if(!data.SetFile(data.customconstfilename, DATA_CUSTOMCONSTFILE))
			return false;
		if(!data.SetFile(data.rangedefinefilename, DATA_RANGEDEFINEFILE))
			return false;
		if(!data.SetFile(data.musicdefinefilename, DATA_MUSICDEFINEFILE))
			return false;
	}
	return true;
}