#ifndef levelH
#define levelH

#include "Object.h"

struct stUnit
{
	unsigned int Position[2];
	unsigned int Size[2];
};

struct stConnectData
{
	stUnit HighestElement;
	stUnit ConnectElement;
	bool WasConnect;
	int RemainUnitY;
};

struct stElement
{
	stPlane Body;
	vector<MObject> Sub;
};

struct stZone
{
	vector<stElement> Element;
	stPlane Edge;
};

class MLevel
{
private:
	stPoint UnitSize; //points per unit
	unsigned int HoleUnitLimitX; //hole size
	unsigned int BoxUnitLimitX[2]; //x size limits
	unsigned int BoxUnitLimitY[2]; //y size limits
	unsigned int ZonesCount[2]; //total zones count
	unsigned int CurrentUnit[2]; //current unit pos
	unsigned int TotalUnit[2]; //total units count in one zone
	unsigned int MoveUnitLimit[2]; //limits used in conditions while line generating
	unsigned int BoxUnitSize[2]; //generated box size
	unsigned int ZoneOffset[2]; //zone offset calcualted by zone number
	vector<stZone> Zone;
	vector<MObject> Edges;
	stTexture* Texture;
	unsigned int TextureCount;
	bool EnableEdges; //draw edges
	
	stConnectData ConnectData; //used for connecting last X zones, needed while ZonesByY more than 1
	stUnit HighestElement; //highest element in line
	stUnit ConnectElement;
	bool CanConnect; //we can create connect element
	bool Ready; //level is created and ready to draw

	void SetMaxElement();
	void MergeTest(stZone& Zone);
	stElement GenerateElement();
	stElement CreateElement(unsigned int BoxUnitX, unsigned int BoxUnitY);
	stElement CreateConnectElement();
	stZone GenerateZone(unsigned int ZoneNumberX, unsigned int ZoneNumberY);
	
public:
	MLevel();
	~MLevel();
	bool SetUnit(float UnitSizeX, float UnitSizeY, unsigned int TotalUnitX, unsigned int TotalUnitY);
	bool SetLimits(unsigned int BoxUnitLimitX0, unsigned int BoxUnitLimitX1, unsigned int BoxUnitLimitY0, unsigned int BoxUnitLimitY1, unsigned int HoleUnitLimitX0);
	bool SetTexture(stTexture* inTexture, unsigned int inTextureCount);
	bool SetZonesCount(unsigned int ZonesCountX, unsigned int ZonesCountY);
	bool GenerateLevel();
	bool GetReady();
	void Draw();
	void Close();
};

#endif
