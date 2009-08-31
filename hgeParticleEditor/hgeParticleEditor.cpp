#include "hge.h"
#include "Export.h"
#include "Resource.h"
#include "Data.h"
#include "vld.h"
#include "hgeEffectSystem.h"
#include "EditorRes.h"
#include "EditorUI.h"

#pragma comment(lib, "hge.lib")

HGE * hge = hgeCreate(HGE_VERSION);

bool FrameFunc()
{
	if(hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;
	if(hge->Input_GetKeyState(HGEK_CTRL) && hge->Input_GetKeyState(HGEK_BACKSPACE))
	{
		if(!eres.Load())
			return true;
		ui.timer = 0;
	}
	ui.Update();

	return false;
}

bool RenderFunc()
{
	Export::clientSetMatrix();
	
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);

	ui.Render();

	hge->Gfx_EndScene();
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

	Export::clientInitial(false);

	if(hge->System_Initiate())
	{
		if(eres.Load())
		{
			hge->System_Start();
		}
	}
	eres.Release();

	hge->System_Shutdown();
	hge->Release();
}