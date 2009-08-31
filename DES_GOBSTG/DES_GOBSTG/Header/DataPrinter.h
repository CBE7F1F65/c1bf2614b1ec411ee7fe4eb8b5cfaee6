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

#define DPS_HEAD_TITLE			"记录文本版"
#define DPS_HEAD_EXPLAIN_1		"这个程序是当前记录的缓存程序。"
#define DPS_HEAD_EXPLAIN_2		"即使改变此程序的内容，也不会反映到游戏里去。"
#define DPS_HEAD_EXPLAIN_3		"这个程序可以自由利用和转载，即使格式化也不要紧。"
#define DPS_HEAD_TIMESTAMPSIGN	"时间戳："

#define DPS_COMMENTDOUBLELINE	"================================="
#define DPS_COMMENTLINE			"---------------------------------"

#define DPS_TITLE_TOP			"最高记录一览"
#define DPS_TITLE_RANGE			"领域界一览		本篇取得/	本篇遭遇/	练习取得/	练习遭遇	(难易度 最高得点)"
#define DPS_TITLE_TOTAL			"其他状态一览"

#define DPS_TITLE_REPLAYINFO	"记录信息"
#define DPS_TITLE_PARTINFO		"分段信息"
#define DPS_TITLE_REPLAYFRAME	"其他信息"

#define DPS_TAG_TOPNUM			"No."
#define DPS_TAG_RANGEPRIFIX		"Range-"
#define DPS_TAG_PART			"Stage"

#define DPS_TOP_TAGSTR_RANGELIST				"取得领域界一览		(总取得回数/总遭遇回数)"
#define DPS_RPYINFO_TAGSTR_RANGELIST			"取得领域界一览"
#define DPS_REPLAYFRAME_TAGSTR_AVERAGE			"总平均FPS"
#define DPS_REPLAYFRAME_TAGSTR_TOTAL_AVERAGE	"300帧内平均FPS"
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