#ifndef game2H
#define game2H

#include "Window.h"
#include "Level.h"
#include "TextureLoader.h"

class MGame2: public MWindow
{
private:
	//statuses
	bool Pause;
	bool* Key;
	
	//clases
	MTextureLoader* TextureLoader;
	MLevel* Level;
	stTexture* txBox;
	unsigned int txBox_cnt;
	
	//viewbox
	stViewBox ViewBox;
	
	//config
    int UnitSize[2];
    bool LinearMode;
    unsigned int txParams[2];
    string FileString;
    string TextureFileName;
	
	//overload virtual functions
	void OnDraw();
	void OnKeyUp(WPARAM wParam);
	void OnKeyDown(WPARAM wParam);
	void OnMainTimer();
	void OnFrameTimer();
	void OnSize();
	
	//local
	void Start();
	void Stop();
	
public:
	MGame2();
	~MGame2();
	bool Initialize();
	bool LoadConfig();
	void OnClose();
};

#endif
