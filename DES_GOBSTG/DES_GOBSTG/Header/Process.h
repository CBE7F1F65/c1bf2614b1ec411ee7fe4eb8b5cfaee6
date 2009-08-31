#ifndef _PROCESS_H
#define _PROCESS_H

#include "Main.h"
#include "Export.h"
#include "Const.h"

#define KS_UP		keyUp
#define KS_DOWN		keyDown
#define KS_LEFT		keyLeft
#define KS_RIGHT	keyRight
#define KS_FIRE		keyFire
#define KS_BOMB		keyBomb
#define KS_SLOW		keySlow
#define KS_CIRCLE	keyCircle
#define KS_PAUSE	keyPause
#define KS_SKIP		keySkip
#define KS_ENTER	keyEnter
#define KS_ESCAPE	keyEscape
#define KS_CAPTURE	keyCapture

#define KS_UP_MP		mp.keyUp
#define KS_DOWN_MP		mp.keyDown
#define KS_LEFT_MP		mp.keyLeft
#define KS_RIGHT_MP		mp.keyRight
#define KS_FIRE_MP		mp.keyFire
#define KS_BOMB_MP		mp.keyBomb
#define KS_SLOW_MP		mp.keySlow
#define KS_CIRCLE_MP	mp.keyCircle
#define KS_PAUSE_MP		mp.keyPause
#define KS_SKIP_MP		mp.keySkip
#define KS_ENTER_MP		mp.keyEnter
#define KS_ESCAPE_MP	mp.keyEscape
#define KS_CAPTURE_MP	mp.keyCapture

//process return value
enum
{
	PQUIT,
	PGO,
	POK,
	PBACK,
	PTURN,
	PSKIP
};
//state indicator
enum
{
	STATE_START,
	STATE_PAUSE,
	STATE_CONTINUE,
	STATE_CLEAR,
	STATE_ENDING,
	STATE_TITLE,
	STATE_PLAYER_SELECT,
	STATE_DIFFICULT_SELECT,
	STATE_OVER,
	STATE_RANGE,
	STATE_REPLAY,
	STATE_RESAULT,
	STATE_OPTION,
	STATE_MUSIC,
	STATE_INIT,
};
//errorcode
enum
{
	PROC_ERROR_NONE,
	PROC_ERROR_INIFILE,
	PROC_ERROR_RESOURCE,
	PROC_ERROR_SCRIPT,
	PROC_ERROR_TRANSLATE,
	PROC_ERROR_DATA,
	PROC_ERROR_PACKAGE,
	PROC_ERROR_SOUND,
	PROC_ERROR_FONT,
	PROC_ERROR_TEXTURE
};

class Process
{
public:
	Process();
	virtual ~Process();

	void	Initial();
	void	Realease();

	bool	reload();

	int		getInput();
	char	getInputNowChar(bool wide);

	int		processStart();
	int		processPause();
	int		processContinue();
	int		processClear();
	int		processEnding();
	int		processTitle();
	int		processPlayerSelect();
	int		processDifficultSelect();
	int		processOver();
	int		processRange();
	int		processReplay();
	int		processResult();
	int		processOption();
	int		processMusic();
	int		processInit();

	int		frame();
	int		render();

	void	startPrep();
	void	clearPrep(bool bclearkey = true);

	void	frameStart();
	void	frameEnd();

	void	playSE(const char * sample_name,int pan = 0);
	void	musicChange(BYTE ID, bool force = false);
	void	musicSlide(float slidetime, int tovol = 0);

	void	SnapShot();

public:
	union{
		struct{
			int keyUp;
			int keyDown;
			int keyLeft;
			int keyRight;
			int keyFire;
			int keyBomb;
			int keySlow;
			int keyCircle;
			int keyPause;
			int keySkip;
			int keyEnter;
			int keyEscape;
			int keyCapture;
		};
		int keyKey[13];
	};
	union{
		struct{
			int	joyFire;
			int	joyBomb;
			int	joySlow;
			int	joyCircle;
			int	joyPause;
		};
		int		joyKey[5];
	};
#ifdef __DEBUG
	int debug_joySpeedUp;
#endif

	//texture
	HTEXTURE	tex[TEXMAX];

	//read ini
	char	username[RPYINFO_USERNAMEMAX];
	int		screenmode;
	int		bgmvol;
	int		sevol;
	int		maxplayer;
	int		defaultdifflv;
	int		circlearound;

	//replay
	char	rpyfilename[M_STRITOAMAX];
	DWORD	seed;
	DWORD	framecounter;
	bool	replaymode;
	bool	replayend;
	//playtime
	LONGLONG	playtimeStart;
	bool		playing;
	//font
	hgeFont *	FPS_font;
	hgeFont *	info_font;
	HD3DFONT	chat_font;
	//music
	HSTREAM		stream;
	HCHANNEL	channel;
	float		musicpos;

	//main
	DWORD lasttime;
	DWORD alltime;
	bool	active;

	int retvalue;

	DWORD	scene;
	DWORD	startscene;
	DWORD	endscene;
	//practice
	bool	practicemode;
	bool	rangemode;
	//pause return
	bool	pauseret;
	bool	pauseinit;
	//Replayinfo
	float	replayFPS;
	//error
	int	errorcode;

	//indicator
	BYTE	nowdifflv;
	BYTE	luchara;
	BYTE	titleselect;
	BYTE	state;
	//input
	BYTE	nowInput;
	BYTE	saveInput;
};

extern Process mp;

extern int randi;
extern BYTE tari;

extern replayFrame replayframe[M_SAVEINPUTMAX];
extern DWORD replayIndex;

extern float worldx;
extern float worldy;
extern float worldz;

#endif