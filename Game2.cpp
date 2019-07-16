#include "stdafx.h"
#include "Game2.h"

MGame2::MGame2():MWindow() //IMPORTANT
{
	Pause = true;
    Key = new bool [256];
    memset(Key, 0, 256);
}

MGame2::~MGame2()
{
}

bool MGame2::Initialize()
{	
    LogFile<<"Game: initialize"<<endl;
    
    if(!WindowInitialized)
    {
    	LogFile<<"Window was not initialized"<<endl;
    	return 0;
	}
	
	//randomize
    LogFile<<"Game: randomize rand by time"<<endl; 
    srand(time(NULL));

	//read config file    
    ifstream ConfigFile("config.txt");
    getline(ConfigFile, FileString);
    TextureFileName = FileString;
    getline(ConfigFile, FileString);
    UnitSize[0] = atoi(FileString.c_str());
    getline(ConfigFile, FileString);
    UnitSize[1] = atoi(FileString.c_str());
    getline(ConfigFile, FileString);
    txParams[0] = atoi(FileString.c_str());
    getline(ConfigFile, FileString);
    txParams[1] = atoi(FileString.c_str());
    getline(ConfigFile, FileString);
    if(FileString == "true") LinearMode = true;
    else LinearMode = false;
    
    //textures
    LogFile<<"Game: texture loading"<<endl;
    TextureLoader = new MTextureLoader;
    TextureLoader->LinearMode = LinearMode;
    txBox = TextureLoader->LoadTexture(TextureFileName.c_str(), txParams[0], txParams[1], 0, txBox_cnt);
    if(!txBox) return false;
    
    //create level
    Level = new MLevel;
    if(!Level->SetUnit(UnitSize[0], UnitSize[1], 32, 24)) return false;
	if(!Level->SetLimits(3, 5, 1, 2, 1)) return false;
	if(!Level->SetTexture(txBox, txBox_cnt)) return false;
	if(!Level->SetZonesCount(1, 1)) return false;
    if(!Level->GenerateLevel()) return false;
    
    //viewbox
    ViewBox.Enabled = true;
    ViewBox.SetSize((float)WindowWidth, (float)WindowHeight);
   
    //VERY IMPORTANT! (Do not use game resources before textures loads)
    //start capture buttons. scene always drawing
    Pause = false;
    LogFile<<"Game: pause is "<<Pause<<endl;
    
    return true;
}

void MGame2::Start()
{
    Pause = false;
}

void MGame2::Stop()
{
    Pause = true;
}

void MGame2::OnDraw()
{
	if(Level->GetReady()) Level->Draw();
}

void MGame2::OnKeyDown(WPARAM wParam)
{
	Key[wParam] = 1;
	switch(wParam)
	{
		case VK_ESCAPE:
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
			break;
		case 'R':
			Level->Close();
			Level->GenerateLevel();
			break;
	}
}

void MGame2::OnKeyUp(WPARAM wParam)
{
	Key[wParam] = 0;
}

void MGame2::OnClose()
{
	Stop();
	LogFile<<"Game: free game resources"<<endl;
	if(Key) delete [] Key;
	Level->Close();
	TextureLoader->DeleteTexture(txBox, txBox_cnt);
	TextureLoader->Close();
    if(TextureLoader) delete TextureLoader;
}

void MGame2::OnMainTimer()
{
	//start-stop game
	if(Key[VK_RETURN])
	{
		Pause = !Pause;
		LogFile<<"Game: pause is "<<Pause<<endl;
		Key[VK_RETURN] = 0;
	}
}

void MGame2::OnFrameTimer()
{
}

void MGame2::OnSize()
{
	ViewBox.SetSize((float)WindowWidth, (float)WindowHeight);
}
