#include "stdafx.h"
#include "Game2.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
	MGame2* Game2 = new MGame2;
	
	if(!Game2->CreateMainWindow(hInstance)) return 0;
	if(!Game2->Initialize())
	{
		Game2->OnClose();
		return 0;
	}
	Game2->Run();
	if(Game2) delete Game2;
	LogFile<<"Closing application"<<endl;
	LogFile.close();
	
	return 0;
}
