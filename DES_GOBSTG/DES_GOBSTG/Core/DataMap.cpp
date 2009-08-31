#include "Data.h"

dataMapTable dataMap[] = 
{
	{"_DATASTART_",		0xffffffff},

	{"_E",				0x00100000},
	{"_N",				0x00200000},
	{"_H",				0x00300000},
	{"_X",				0x00400000},
	{"_P",				0x00500000},
	{"_L",				0x00600000},

	{"_A",				0x00010000},
	{"_B",				0x00020000},
	{"_C",				0x00030000},
	{"_D",				0x00040000},

	{"Header",			0x01000000},
	{"GameVersion",		0x01010000},
	{"Signature",		0x01020000},
	{"FileType",		0x01030000},

	{"Top",				0x02000000},
	{"Score",			0x02010000},
	{"LastStage",		0x02020000},
	{"Username",		0x02030000},
	{"Year",			0x02040000},
	{"Month",			0x02050000},
	{"Day",				0x02060000},
	{"Hour",			0x02070000},
	{"Minute",			0x02080000},
	{"Lost",			0x02090000},
	{"CircleRate",		0x020A0000},
	{"FastRate",		0x020B0000},
	{"AllTime",			0x020C0000},
	{"RangeGet",		0x020D0000},
	{"MaxPlayer",		0x020E0000},
	{"Point",			0x020F0000},
	{"Aliveness",		0x02100000},
	{"Miss",			0x02110000},
	{"Bomb",			0x02120000},
	{"Continue",		0x02130000},
	{"Pause",			0x02140000},
	{"GetRange",		0x02150000},

	{"Range",			0x03000000},
	{"GetGame",			0x03010000},
	{"MeetGame",		0x03020000},
	{"GetPractice",		0x03030000},
	{"MeetPractice",	0x03040000},
	{"TopBonus",		0x03050000},

	{"StagePractice",	0x04000000},
	{"TryTime",			0x04010000},
	{"TopScore",		0x04020000},

	{"Total",			0x05000000},
	{"PlayTime",		0x05010000},
	{"ClearTime",		0x05020000},
	{"PracticeTime",	0x05030000},
	{"FirstRunTime",	0x05040000},
	{"TotalPlayTime",	0x05050000},

	{"IsRange",			0x06010000},
	{"RangeNumber",		0x06020000},
	{"RangeName",		0x06030000},
	{"TimeLimit",		0x06040000},
	{"Remain",			0x06050000},
	{"EnemyName",		0x06060000},
	{"EnemyPinYin",		0x06070000},
	{"Bonus",			0x06080000},
	{"TurnToScene",		0x06090000},
	{"IsWait",			0x060A0000},
	{"AutoRank",		0x060B0000},
	{"Explain",			0x060C0000},

	{"Music",			0x07000000},
	{"MusicName",		0x07010000},
	{"MusicFileName",	0x07020000},
	{"StartPos",		0x07030000},
	{"EndPos",			0x07040000},
	{"LoopPos",			0x07050000},

	{"CustomConst",		0x08000000},

	{"StringDesc",		0x09000000},

	{"_RESOURCESTART_",	0xffffffff},

	//////////RESOURCE//////////

	{"FileName",			0x10010000},
	{"Name",				0x10020000},
	{"Type",				0x10030000},

	{"Font",				0x12000000},
	{"FontName",			0x12010000},

	{"DataFile",			0x13000000},
	{"BinFile",				0x13010000},
	{"RangeAccessFile",		0x13020000},
	{"ScriptFile",			0x13030000},
	{"StringDescFile",		0x13210000},
	{"CustomConstFile",		0x13220000},
	{"RangeDefineFile",		0x13230000},
	{"MusicDefineFile",		0x13240000},

	{"Package",				0x20000000},
	{"Texture",				0x21000000},
	{"SoundEffect",			0x22000000},
	{"Ghost",				0x23000000},
	{"Effectsys",			0x24000000},

	{"Folder",				0x32000000},
	{"ScriptFolder",		0x32010000},
	{"SnapShotFolder",		0x32020000},
	{"ReplayFolder",		0x32030000},
	{"DataFolder",			0x32040000},
	{"EffectsysFolder",		0x32050000},

	{"Extension",			0x33000000},
	{"ScriptExt_7",			0x33010000},
	{"ReplayExt_7",			0x33020000},

	{"ReplayHeader",		0x40000000},
	{"Signature_11",		0x40010000},
	{"TempSign_3",			0x40020000},
	{"CompleteSign_3",		0x40030000},
	{"Tag_3",				0x40040000},

	{"_END_",				0xffffffff}
};