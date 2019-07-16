#ifndef stdafxH
#define stdafxH

//==========================================headers
//png
#include <png.h>
//windows
#include <windows.h>
#include <process.h>
//opengl
#include <gl/gl.h>
#include <gl/glu.h>
//dos
#include <time.h>
#include <stdio.h>
#include <math.h>
//file stats
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//stl
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <map>
//point, plane, level, zone
#include "Plane.h"

//global log file variable - inited in stdafx.cpp
extern ofstream LogFile;

struct stTexture
{
    unsigned int Id;
};

struct stAnimation
{
	stTexture* Texture;
	unsigned int Count;
	unsigned char Type;//1 - repeat animation (default), 2 - finite animation
	unsigned int Next;//if next == current - skip, if next != current and type == 2 - change animation
	void SetParameters(unsigned int inType, unsigned int inNext)
	{
		if(inType < 1 || inType > 2) Type = 1;
		else Type = inType;
		//if(inNext >= 24 ) Next = 1;
		if(inNext >= Count ) Next = Count - 1;
		else Next = inNext;
	}
};

struct stViewBox
{
	bool Enabled;
	stPoint Size;
	stPoint Offset;
	void SetSize(float x, float y)
	{
		SetPoint(Size, x, y);
		NullPoint(Offset);
	}
	void SetOffsetByPlane(stPlane Plane)
	{
		SetPoint(Offset, 
			(Plane.Point[1].p[0] + Plane.Point[0].p[0])/2 - Size.p[0]/2, 
			(Plane.Point[1].p[1] + Plane.Point[0].p[1])/2 - Size.p[1]/2);
	}
	stPlane GetPlane()
	{
		stPlane retPlane;
		SetPlane(retPlane, Offset.p[0], Offset.p[1],
			Offset.p[0] + Size.p[0], Offset.p[1] + Size.p[1]);
		return retPlane;
	}
};

struct stTimer
{
	HWND hWnd;
	unsigned int Id;
	unsigned int Value;
	TIMERPROC TimerProc;
	bool Enabled;
	
	bool Initialize(HWND inhWnd, unsigned int inId, unsigned int inValue, TIMERPROC inTimerProc=NULL)
	{
		if(!inhWnd && !inTimerProc)
		{
			LogFile<<"Can not create timer. Empty HWND and TimerProc"<<endl;
			return false;
		}
		if(inValue < 10 || inValue > 900000)
		{
			LogFile<<"Can not create timer. Wrong value"<<endl;
			return false;
		}
		
		hWnd = inhWnd;
		TimerProc = inTimerProc;
		Value = inValue;
		
		//without HWND timer id is generating, even if id was sended it will be ignored
		if(!hWnd)
		{
			Id = SetTimer(NULL, 0, Value, TimerProc);
			if(!Id)
			{
				LogFile<<"Some error ocurred"<<endl;
				return false;
			}
		}
		else
		{
			Id = inId;
			if(!SetTimer(hWnd, Id, Value, TimerProc))
			{
				LogFile<<"Some error ocurred"<<endl;
				return false;
			}
		}
		
		return true;
	}
	
	bool Update(unsigned int inValue)
	{
		if(inValue < 10 || inValue > 900000)
		{
			LogFile<<"Can not create timer. Wrong value"<<endl;
			return false;
		}
		Value = inValue;
		return SetTimer(hWnd, Id, Value, TimerProc);
	}
	
	void Close()
	{
		KillTimer(hWnd, Id);
		LogFile<<"Killed timer: "<<Id<<endl;
	}
};

struct stFrameControl
{
    //usual time
    DWORD FrameStart; //time of start draw one frame (ms)
    DWORD FrameEnd; //time of end draw one frame (ms)
    DWORD liStartTime; //time for start count frames (ms count)
	DWORD liEndTime; //time for end count frames (ms count)
	DWORD liDiffTime; //time of draw one frame (ms count)
	int Frames;
	float FPS;
	void Initialize()
	{
        Frames = 0;
        liStartTime = GetTickCount();
		liEndTime = liStartTime;
	}
	void Calculate()
	{
        Frames ++;
		liEndTime = GetTickCount();
		liDiffTime = liEndTime - liStartTime;
		if(liDiffTime >= 1000)
		{
            FPS = (float)Frames/liDiffTime*1000;
            liStartTime = liEndTime;
            Frames = 0;
        }
	}
};

#endif
