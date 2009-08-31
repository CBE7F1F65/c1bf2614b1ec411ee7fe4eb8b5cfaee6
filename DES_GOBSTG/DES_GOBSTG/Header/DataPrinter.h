#ifndef _DATAPRINTER_H
#define _DATAPRINTER_H

#include "MainDependency.h"
#include "Export.h"

#define DPS_SCOREFILENAME	"Score"
#define DPS_EXTENSION		"txt"

#define DPS_SECTION_HEADER		"[Header]\n"
#define DPS_SECTION_TOP10		"[Top10]\n"
#define DPS_SECTION_RANGE		"[Range]\n"
#define DPS_SECTION_TOTAL		"[Total]\n"

#define DPS_SECTION_REPLAYINFO	"[ReplayInfo]\n"
#define DPS_SECTION_PARTNFO		"[PartInfo]\n"
#define DPS_SECTION_REPLAYFRAME	"[ReplayFrame]\n"

#define DPS_COMMENT				"# "
#define DPS_RETURN				"\n"
#define DPS_TABLE_1				"\t"
#define DPS_TABLE_2				"\t\t"
#define DPS_TABLE_3				"\t\t\t"
#define DPS_TABLE_4				"\t\t\t\t"
#define DPS_LEFTANGLEBRACKET	"<"
#define DPS_RIGHTANGLEBRACKET	">"
#define DPS_LEFTBRACKET			"("
#define DPS_RIGHTBRACKET		")"
#define DPS_SLASH				"/"
#define DPS_SPACE				" "
#define DPS_COLON				":"
#define DPS_PERCENT				"%"

#define DPS_RANGENAME_NFORMAT	40

#define DPS_HEAD_TITLE			"��¼�ı���"
#define DPS_HEAD_EXPLAIN_1		"��������ǵ�ǰ��¼�Ļ������"
#define DPS_HEAD_EXPLAIN_2		"��ʹ�ı�˳�������ݣ�Ҳ���ᷴӳ����Ϸ��ȥ��"
#define DPS_HEAD_EXPLAIN_3		"�����������������ú�ת�أ���ʹ��ʽ��Ҳ��Ҫ����"
#define DPS_HEAD_TIMESTAMPSIGN	"ʱ�����"

#define DPS_COMMENTDOUBLELINE	"================================="
#define DPS_COMMENTLINE			"---------------------------------"

#define DPS_TITLE_TOP			"��߼�¼һ��"
#define DPS_TITLE_RANGE			"�����һ��		��ƪȡ��/	��ƪ����/	��ϰȡ��/	��ϰ����	(���׶� ��ߵõ�)"
#define DPS_TITLE_TOTAL			"����״̬һ��"

#define DPS_TITLE_REPLAYINFO	"��¼��Ϣ"
#define DPS_TITLE_PARTINFO		"�ֶ���Ϣ"
#define DPS_TITLE_REPLAYFRAME	"������Ϣ"

#define DPS_TAG_TOPNUM			"No."
#define DPS_TAG_RANGEPRIFIX		"Range-"
#define DPS_TAG_PART			"Stage"

#define DPS_TOP_TAGSTR_RANGELIST				"ȡ�������һ��		(��ȡ�û���/����������)"
#define DPS_RPYINFO_TAGSTR_RANGELIST			"ȡ�������һ��"
#define DPS_REPLAYFRAME_TAGSTR_AVERAGE			"��ƽ��FPS"
#define DPS_REPLAYFRAME_TAGSTR_TOTAL_AVERAGE	"300֡��ƽ��FPS"
#define DPS_REPLAYFRAME_AVERAGE_INTERVAL		300

class DataPrinter
{
public:
	DataPrinter();
	~DataPrinter();

	static bool PrintScore();
	static bool PrintReplayData(const char * foldername, const char * filename);
	static void getHeader();

	static string str;
	static replayInfo rpyinfo;
	static partInfo partinfo[RPYPARTMAX];
	static replayFrame rpyframe[M_SAVEINPUTMAX];
};

extern HGE * hge;

#endif