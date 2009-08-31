/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: timer
*/


#include "hge_impl.h"


float CALL HGE_Impl::Timer_GetTime()
{
	return fTime;
}
//edit by Thor/h5nc
//begin
float CALL HGE_Impl::Timer_GetDelta()
{
	return fDeltaTime;
}
//end

float CALL HGE_Impl::Timer_GetFPS(int mod)
{
	if(mod <= 1)
		return fFPS;
	else
	{
		static float nowFPS = 0;
		static float fLastTime = 0;
		static DWORD fLastFrameCounter = 0;
		if(frameCounter % mod == 1)
		{
			if (frameCounter > fLastFrameCounter && fTime > fLastTime)
			{
				nowFPS = 1 / ((fTime - fLastTime) / (frameCounter - fLastFrameCounter));
			}
			fLastTime = fTime;
			fLastFrameCounter = frameCounter;
		}
		return nowFPS;
	}
}

float CALL HGE_Impl::Timer_GetWorstFPS(int mod)
{
	if(mod <= 1)
		return fFPS;
	static float worstFPS = HGEFPS_MAX;
	static float worstFPSMod = worstFPS;
	if(worstFPS > fFPS)
		worstFPS = fFPS;
	if(frameCounter % mod == 1)
	{
		worstFPSMod = worstFPS;
		worstFPS = HGEFPS_MAX;
	}
	return worstFPSMod;
}
