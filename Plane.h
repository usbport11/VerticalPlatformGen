#ifndef planeH
#define planeH

using namespace std;

#define COLLISION_NONE 0
#define COLLISION_LEFTTOP 1
#define COLLISION_LEFTBOTTOM 2
#define COLLISION_RIGHTTOP 3
#define COLLISION_RIGHTBOTTOM 4
#define FLT_MAX 3.40282347E+38F

struct stPoint
{
	float p[2];
	//0 - x
	//1 - y
};

struct stPlane
{
	stPoint Point[2];
	//0 - bottom left
	//1 - top right
};

struct stRay
{
    stPoint Origin;
    stPoint Direction;
};

struct stLine
{
    float a;
    float b;
    float c;
};

void NullPoint(stPoint& Point);
void NullPlane(stPlane& Plane);
stPoint GetSign(stPoint P1);
stPoint AddPoints(stPoint P1, stPoint P2);
stPoint SubtractPoints(stPoint P1, stPoint P2);
stPoint MultiplyPoints(stPoint P1, stPoint P2);
stPoint DevidePoints(stPoint P1, stPoint P2);
bool IsPointNull(stPoint inPoint);
bool IsPlaneNull(stPlane inPlane);
bool ComparePoints(stPoint A, stPoint B);
void SetRay(stRay &Ray, stPoint Origin, stPoint Direction);
void SetRay(stRay &Ray, stPoint Origin, float x, float y);
void SetRay(stRay &Ray, float x1, float y1, float x2, float y2);
void SetLineByTwoPoints(stLine& Line, float x1, float y1, float x2, float y2);
void SetLineByTwoPoints(stLine& Line, stPoint A, stPoint B);
void SetLineByRay(stLine& Line, stRay Ray);
bool PointBelognsToLine(stLine Line, stPoint Point);
bool PointBelongsToSegment(stPlane Side, stPoint Point);
bool LinesIntersect(stLine Line1, stLine Line2, stPoint& Point);
float GetVectorLength(stPoint Vector);
float GetSegmentLength(stPoint A, stPoint B);
stPoint GetPlaneCenter(stPlane Plane);
stPoint GetPlanePoint(stPlane Plane, unsigned char SideNum);
stPlane GetPlaneSide(stPlane Plane, unsigned char SideNum);
bool GetBoxIntersectByRay1(stPlane Box, stRay Ray, stPoint& DiffSize);
bool GetBoxIntersectByRay2(stPlane Box, stRay Ray, stPoint& Point, float& Distance, unsigned char& SideNum);

void SetPoint(stPoint& dstPoint, float p0, float p1);
void SetPlane(stPlane& dstPlane, stPoint Point0, stPoint Point1);
void SetPlane(stPlane& dstPlane, float p0, float p1, float p2, float p3);
void SetPlaneByOffset(stPlane& dstPlane, float x, float y, float width, float height);
void SetPlaneByOffset(stPlane& dstPlane, stPoint Start, stPoint Size);
void GetPlanePoint(stPlane srcPlane, stPoint& dstPoint, bool RT);
void GetPlanesDiff(stPlane srcPlane1, stPlane srcPlane2, stPoint& srcPoint);
void GetPointsDiff(stPoint srcPoint1, stPoint srcPoint2, stPoint& srcPoint);
float GetPlaneWidth(stPlane srcPlane);
float GetPlaneHeight(stPlane srcPlane);
void CopyPoint(stPoint srcPoint, stPoint& dstPoint);
void CopyPlane(stPlane srcPlane, stPlane& dstPlane);
void MovePoint(stPoint& dstPoint, float p0, float p1);
void MovePlane(stPlane& Plane, stPoint Delta);
void MovePlane(stPlane& Plane, float d0, float d1);
bool PlaneCollision(stPlane srcPlane, stPlane dstPlane);
bool PlaneCollision2(stPlane srcPlane, stPlane dstPlane);
bool PlaneCollision3(stPlane srcPlane, stPlane dstPlane);
bool PlaneCollision(stPlane PlaneAfter, stPlane dstPlane, char &Direction);
bool PlaneInSegment(stPlane Plane, stPlane Segment);
bool PointInSegment(stPoint Point, stPlane Segment);

#endif
