#include "Process.h"
#include "BGLayer.h"
#include "SE.h"
#include "Selector.h"
#include "InfoSelect.h"
#include "Scripter.h"

int Process::processDifficultSelect()
{
	time++;
	if(time == 1)
	{
		for(int i=0;i<BGLAYERMAX;i++)
			bg[i].exist = false;
		for(int i=0;i<FGLAYERMAX;i++)
			fg[i].exist = false;
		Selector::Clear();
		infoselect.clear();
		bgmask.exist = false;
		fgpanel.exist = false;
		fgdiff.exist = false;
		if(nowdifflv >= M_DIFFI_EXTRA)
			nowdifflv = defaultdifflv;
	}
	scr.controlExecute(STATE_DIFFICULT_SELECT, time);

	if(hge->Input_GetDIKey(KS_BOMB, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_CANCEL);
		time = 0;
		if(practicemode)
		{
			state = STATE_RANGE;
			return PTURN;
		}
		state = STATE_TITLE;
		return PTURN;
	}
	return PGO;
}
