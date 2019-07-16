#include "stdafx.h"
#include "Level.h"

MLevel::MLevel()
{
	CurrentUnit[0] = CurrentUnit[1] = 0;
	ZonesCount[0] = ZonesCount[1] = 0;
	Ready = false;
	EnableEdges = true;
}

MLevel::~MLevel()
{
}

void MLevel::SetMaxElement()
{
	HighestElement.Position[0] = abs((int)(CurrentUnit[0] - BoxUnitSize[0]));
	HighestElement.Position[1] = CurrentUnit[1];
	HighestElement.Size[0] = BoxUnitSize[0];
	HighestElement.Size[1] = BoxUnitSize[1];
}

void MLevel::MergeTest(stZone& Zone)
{
	if(Zone.Element.size() > 1)
	{
		unsigned int last = Zone.Element.size() - 1;
		if(Zone.Element[last-1].Body.Point[0].p[1] == Zone.Element[last].Body.Point[0].p[1] && 
			Zone.Element[last-1].Body.Point[1].p[1] == Zone.Element[last].Body.Point[1].p[1] &&
			Zone.Element[last-1].Body.Point[1].p[0] == Zone.Element[last].Body.Point[0].p[0])
		{
			//increare box size
			LogFile<<"Before merge: "<<Zone.Element[last-1].Body.Point[1].p[0]<<" "<<Zone.Element[last-1].Sub.size()<<endl;
			Zone.Element[last-1].Body.Point[1].p[0] = Zone.Element[last].Body.Point[1].p[0];
			//insert subs
			Zone.Element[last-1].Sub.insert(Zone.Element[last-1].Sub.end(), Zone.Element[last].Sub.begin(), Zone.Element[last].Sub.end());
			//remove subs and element
			Zone.Element[last].Sub.clear();
			Zone.Element.erase(Zone.Element.end() - 1);
			LogFile<<"After merge: "<<Zone.Element[last-1].Body.Point[1].p[0]<<" "<<Zone.Element[last-1].Sub.size()<<endl;
			
			//dont forget decrease current unit		
		}
	}
}

stElement MLevel::GenerateElement()
{
	stElement Element;
	stPoint Unit;
	
	//create bounding box
	if(BoxUnitLimitX[0] != BoxUnitLimitX[1]) BoxUnitSize[0] = (rand() % BoxUnitLimitX[1]) + BoxUnitLimitX[0];
	else BoxUnitSize[0] = BoxUnitLimitX[0];
	if(BoxUnitLimitY[0] != BoxUnitLimitY[1]) BoxUnitSize[1] = (rand() % BoxUnitLimitY[1]) + BoxUnitLimitY[0];
	else BoxUnitSize[1] = BoxUnitLimitY[0];
	Unit.p[0] = ZoneOffset[0] + CurrentUnit[0] * UnitSize.p[0];
	Unit.p[1] = ZoneOffset[1] + CurrentUnit[1] * UnitSize.p[1];
	SetPlane(Element.Body, Unit.p[0], Unit.p[1], Unit.p[0] + UnitSize.p[0] * BoxUnitSize[0], Unit.p[1] + UnitSize.p[1] * BoxUnitSize[1]);
	LogFile<<"Level: Generic unit size ["<<BoxUnitSize[0]<<" ,"<<BoxUnitSize[1]<<"] start ["<<CurrentUnit[0]<<" ,"<<CurrentUnit[1]<<"]"<<endl;
	
	//create sub boxes
	MObject AddObject;
	for(unsigned int i=0; i<BoxUnitSize[0]; i++)
	{
		for(unsigned int j=0; j<BoxUnitSize[1]; j++)
		{
			AddObject.CreateByPoints(Unit.p[0] + i * UnitSize.p[0], Unit.p[1] + j * UnitSize.p[1], 
				Unit.p[0] + (i + 1) * UnitSize.p[0], Unit.p[1] + (j + 1) * UnitSize.p[1], 
				1, 1, Texture[rand() % TextureCount].Id);
			Element.Sub.push_back(AddObject);
		}
	}
	
	//increse use unit X
	CurrentUnit[0] += BoxUnitSize[0];
	
	return Element;
}

stElement MLevel::CreateElement(unsigned int BoxUnitX, unsigned int BoxUnitY)
{
	stElement Element;
	stPoint Unit;
	
	//save data (need to highest element)
	BoxUnitSize[0] = BoxUnitX;
	BoxUnitSize[1] = BoxUnitY;
	
	//create bounding box
	Unit.p[0] = ZoneOffset[0] + CurrentUnit[0] * UnitSize.p[0];
	Unit.p[1] = ZoneOffset[1] + CurrentUnit[1] * UnitSize.p[1];
	SetPlane(Element.Body, Unit.p[0], Unit.p[1], Unit.p[0] + UnitSize.p[0] * BoxUnitX, Unit.p[1] + UnitSize.p[1] * BoxUnitY);
	LogFile<<"Level: Static unit size ["<<BoxUnitX<<" ,"<<BoxUnitY<<"] start ["<<CurrentUnit[0]<<" ,"<<CurrentUnit[1]<<"]"<<endl;
	
	//create sub boxes
	MObject AddObject;
	for(unsigned int i=0; i<BoxUnitX; i++)
	{
		for(unsigned int j=0; j<BoxUnitY; j++)
		{
			AddObject.CreateByPoints(Unit.p[0] + i * UnitSize.p[0], Unit.p[1] + j * UnitSize.p[1], 
				Unit.p[0] + (i + 1) * UnitSize.p[0], Unit.p[1] + (j + 1) * UnitSize.p[1], 
				1, 1, Texture[rand() % TextureCount].Id);
			Element.Sub.push_back(AddObject);
		}
	}
	
	//increse use unit X
	CurrentUnit[0] += BoxUnitSize[0];
	
	return Element;
}

stElement MLevel::CreateConnectElement()
{
	stElement Element;
	
	//get start element unit data
	unsigned int SaveLimitX[2], SaveLimitY[2];
	
	LogFile<<"Level: Connect element"<<endl;
		
	//save default values
	SaveLimitX[0] = BoxUnitLimitX[0];
	SaveLimitX[1] = BoxUnitLimitX[1];
	SaveLimitY[0] = BoxUnitLimitY[0];
	SaveLimitY[1] = BoxUnitLimitY[1];
	
	//change default values
	BoxUnitLimitX[0] = 1; 
	if(HighestElement.Size[0] > 3) BoxUnitLimitX[1] = HighestElement.Size[0] - 1; // > 2
	else BoxUnitLimitX[1] = 1;
	BoxUnitLimitY[0] = 1;
	BoxUnitLimitY[1] = 1;
	
	//create connect element
	CurrentUnit[0] = HighestElement.Position[0] + 1;
	CurrentUnit[1] = HighestElement.Position[1] + HighestElement.Size[1] + 1;
	Element = GenerateElement();
	
	//save connect data
	ConnectElement.Position[0] = HighestElement.Position[0] + 1;
	ConnectElement.Position[1] = HighestElement.Position[1] + HighestElement.Size[1] + 1;
	ConnectElement.Size[0] = CurrentUnit[0] - ConnectElement.Position[0];
	ConnectElement.Size[1] = CurrentUnit[1] - ConnectElement.Position[1];
	
	//return default values
	BoxUnitLimitX[0] = SaveLimitX[0]; 
	BoxUnitLimitX[1] = SaveLimitX[1];
	BoxUnitLimitY[0] = SaveLimitY[0]; 
	BoxUnitLimitY[1] = SaveLimitY[1]; 
	
	//if it more to right then move from left to element
	if(CurrentUnit[0] > TotalUnit[0] / 2)
	{
		CurrentUnit[0] = 0;
		MoveUnitLimit[0] = HighestElement.Position[0] + 1;
	}
	//if it more to left then move from element to right
	else
	{
		CurrentUnit[0] = HighestElement.Position[0] + BoxUnitSize[0] + 2;
		MoveUnitLimit[0] = TotalUnit[0];
	}
	
	//increase Y shift
	CurrentUnit[1] += 1;
	
	return Element;
}

stZone MLevel::GenerateZone(unsigned int ZoneNumberX, unsigned int ZoneNumberY)
{
	stZone Zone;
	stUnit PrevHighestElement;
	bool WasHole = true;
	unsigned int FixOffset;
	
	//clear initial global data
	CanConnect = true;
	CurrentUnit[0] = 0;
	CurrentUnit[1] = 0;
	MoveUnitLimit[0] = TotalUnit[0];
	MoveUnitLimit[1] = TotalUnit[1];
	HighestElement.Position[0] = 0;
	HighestElement.Position[1] = 0;
	HighestElement.Size[0] = 0;
	HighestElement.Size[1] = 0;
	
	//zone offset
	ZoneOffset[0] = ZoneNumberX * TotalUnit[0] * UnitSize.p[0];
	ZoneOffset[1] = ZoneNumberY * TotalUnit[1] * UnitSize.p[1];
	
	//set edge
	SetPlaneByOffset(Zone.Edge, ZoneOffset[0], ZoneOffset[1], TotalUnit[0] * UnitSize.p[0], TotalUnit[1] * UnitSize.p[1]);
	
	//gerenator is going line by line until reached top right unit
	while(CurrentUnit[1] < MoveUnitLimit[1]) //-1
	{
		LogFile<<"Level: New line"<<endl;
		//check that we can create a box with maximum permited heigh
		//if we cant use height random create 1 static and make him max
		if((CurrentUnit[1] + (BoxUnitLimitY[1] - 1 + BoxUnitLimitY[0])) > MoveUnitLimit[1] - 1)
		{
			BoxUnitSize[0] = (rand() % BoxUnitLimitX[1]) + BoxUnitLimitX[0];
			if((int)(ConnectElement.Position[0] - 1 - BoxUnitSize[0]) > 0) CurrentUnit[0] = ConnectElement.Position[0] - 1 - BoxUnitSize[0];
			else CurrentUnit[0] = ConnectElement.Position[0] + ConnectElement.Size[0] + 1;
			Zone.Element.push_back(CreateElement(BoxUnitSize[0], MoveUnitLimit[1] - CurrentUnit[1]));
			break;
		}
		//check that we can create connect element
		if(CurrentUnit[1] + (BoxUnitLimitY[1] - 1 + BoxUnitLimitY[0]) > MoveUnitLimit[1] - 1) CanConnect = false;
			
		while(CurrentUnit[0] < MoveUnitLimit[0])
		{	
			//hole test (prevent two holes one by one)
			if(rand() % 2 && !WasHole)
			{
				CurrentUnit[0] += HoleUnitLimitX;
				LogFile<<"Level: hole!"<<endl;
				WasHole = true;
				continue;
			}
			
			//generate element
			LogFile<<"Level: element "<<Zone.Element.size()<<endl;
			Zone.Element.push_back(GenerateElement());
			
			//test element on MAX (because last element may be fisrt max)
			if(Zone.Element.back().Body.Point[1].p[1] >= (HighestElement.Position[1] + HighestElement.Size[1]) * UnitSize.p[1])
			{
				PrevHighestElement = HighestElement;
				SetMaxElement();
			}
			
			//merge element if they have same height
			if(CurrentUnit[0] < MoveUnitLimit[0] - 2 ) MergeTest(Zone);
			
			//clear hole test
			WasHole = false;
		}
		
		//extended last box X test on zone X limit
		if(CurrentUnit[0] >= MoveUnitLimit[0])
		{
			LogFile<<"Level: Limit warning! Trying recreate element"<<endl;
			//remove
			Zone.Element.erase(Zone.Element.end());
			//calculate right size
			FixOffset = BoxUnitSize[0] - (CurrentUnit[0] - MoveUnitLimit[0]);
			//try recreate
			if(FixOffset > 1)
			{
				FixOffset -= 1;
				CurrentUnit[0] -= BoxUnitSize[0];
				Zone.Element.push_back(CreateElement(FixOffset, BoxUnitSize[1]));
				//again test element on MAX (because last element may be fisrt max)
				if(Zone.Element.back().Body.Point[1].p[1] >= (HighestElement.Position[1] + HighestElement.Size[1]) * UnitSize.p[1])	SetMaxElement();
				//merge test
				MergeTest(Zone);
				LogFile<<"Level: Element recreate success!"<<endl;
			}
			else
			{
				BoxUnitSize[1] = 1;
				HighestElement = PrevHighestElement;
				LogFile<<"Level: Element recreate fail!"<<endl;
			}
		}
		//create connect box if we can
		if(CanConnect) Zone.Element.push_back(CreateConnectElement());
		else break; //endless cycle protect ?
	}
	
	return Zone;
}

bool MLevel::GenerateLevel()
{
	if(!TotalUnit[0] || !TotalUnit[0])
	{
		LogFile<<"Level: Total units is not set"<<endl;
		return false;
	}
	if(!BoxUnitLimitX[0]|| !BoxUnitLimitX[1] || !BoxUnitLimitY[0] || !BoxUnitLimitY[1] || !HoleUnitLimitX)
	{
		LogFile<<"Level: Unit limits is not set"<<endl;
		return false;
	}
	if(!Texture || !TextureCount)
	{
		LogFile<<"Level: Texture data is not set"<<endl;
		return false;
	}
	if(!ZonesCount[0] || !ZonesCount[1] || ZonesCount[0] > 32 || ZonesCount[1] > 32)
	{
		LogFile<<"Level: Zones count not set or wrong set"<<endl;
		return false;
	}
	
	LogFile<<"Level: [New level]"<<endl;
	
	//prepare
	Ready = false;
	
	for(unsigned int j=0; j<ZonesCount[1]; j++)
	{
		for(unsigned int i=0; i<ZonesCount[0]; i++)
		{
			Zone.push_back(GenerateZone(i, j));
		}
	}
	Ready = true;
	
	return Ready;
}

bool MLevel::SetUnit(float UnitSizeX, float UnitSizeY, unsigned int TotalUnitX, unsigned int TotalUnitY)
{
	if(!TotalUnitX || !TotalUnitX)
	{
		LogFile<<"Level: Wrong Total units count"<<endl;
		return false;
	}
	
	SetPoint(UnitSize, UnitSizeX, UnitSizeY);
	TotalUnit[0] = TotalUnitX;
	TotalUnit[1] = TotalUnitY;
	
	return true;
}

bool MLevel::SetLimits(unsigned int BoxUnitLimitX0, unsigned int BoxUnitLimitX1, unsigned int BoxUnitLimitY0, unsigned int BoxUnitLimitY1, unsigned int HoleUnitLimitX0)
{
	if(!BoxUnitLimitX0 || !BoxUnitLimitX1 || !BoxUnitLimitY0 || !BoxUnitLimitY1 || !HoleUnitLimitX0)
	{
		LogFile<<"Level: Wrong unit limits"<<endl;
		return false;
	}
	
	BoxUnitLimitX[0] = BoxUnitLimitX0;
	BoxUnitLimitX[1] = BoxUnitLimitX1;
	BoxUnitLimitY[0] = BoxUnitLimitY0;
	BoxUnitLimitY[1] = BoxUnitLimitY1;
	HoleUnitLimitX = HoleUnitLimitX0;
	
	return true;
}

bool MLevel::SetTexture(stTexture* inTexture, unsigned int inTextureCount)
{
	if(!inTexture || !inTextureCount)
	{
		LogFile<<"Level: Wrong texture data"<<endl;
		return false;
	}
	Texture = inTexture;
	TextureCount = inTextureCount;
	
	return true;
}

bool MLevel::SetZonesCount(unsigned int ZonesCountX, unsigned int ZonesCountY)
{
	if(!ZonesCountX || !ZonesCountY)
	{
		LogFile<<"Level: Wrong zones count"<<endl;
		return false;
	}
	
	ZonesCount[0] = ZonesCountX;
	ZonesCount[1] = ZonesCountY;
	
	return true;
}

bool MLevel::GetReady()
{
	return Ready;
}

void MLevel::Draw()
{
	for(unsigned int i=0; i<Zone.size(); i++)
	{
		//draw elements
		for(unsigned int j=0; j<Zone[i].Element.size(); j++)
		{
			for(unsigned int k=0; k<Zone[i].Element[j].Sub.size(); k++)
			{
				Zone[i].Element[j].Sub[k].Draw();
			}
		}
		//draw edges if needed
		if(EnableEdges)
		{
			glDisable(GL_TEXTURE_2D);
			glColor3f(1,0,0);
			glBegin(GL_LINE_LOOP);
				glVertex2f(Zone[i].Edge.Point[0].p[0], Zone[i].Edge.Point[0].p[1]);
				glVertex2f(Zone[i].Edge.Point[0].p[0], Zone[i].Edge.Point[1].p[1]);
				glVertex2f(Zone[i].Edge.Point[1].p[0], Zone[i].Edge.Point[1].p[1]);
				glVertex2f(Zone[i].Edge.Point[1].p[0], Zone[i].Edge.Point[0].p[1]);
			glEnd();
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);
		}
	}
}

void MLevel::Close()
{
	for(unsigned int i=0; i<Zone.size(); i++)
	{
		for(unsigned int j=0; j<Zone[i].Element.size(); j++)
		{
			Zone[i].Element[j].Sub.clear();
		}
	}
	Zone.clear();
	Ready = false;
}
