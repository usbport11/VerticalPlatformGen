#ifndef objectH
#define objectH

class MObject
{
private:
	unsigned int TexId;
protected:
	stPlane TexCoord;
public:
	stPlane Body;
	MObject();
	~MObject();
	void Draw();
	void FlipTexX();
	void FlipTexY();
	void SetTexId(unsigned int inTexId);
	void SetTexCoord(stPlane inTexCoord);
	void SetTexCoord(float X, float Y);
	void SetTexCoordX(float X);
	void SetTexCoordY(float Y);
	bool CreateByPoints(stPoint Point1, stPoint Point2);
	bool CreateByPoints(float x0, float y0, float x1, float y1);
	bool CreateByPoints(float x0, float y0, float x1, float y1, float tx, float ty, unsigned int inTexId);
	bool CreateBySize(stPoint Point, stPoint Size);
	bool CreateBySize(stPoint Point, stPoint Size, stPoint UnitSize, unsigned int inTexId);
	bool CreateByPlane(stPlane Plane);
};

#endif
