#include "stdafx.h"
#include "Plane.h"

void NullPoint(stPoint &Point)
{
    Point.p[0] = Point.p[1] = 0;
}

void NullPlane(stPlane &Plane)
{
    Plane.Point[0].p[0] = Plane.Point[0].p[1] = Plane.Point[1].p[0] = Plane.Point[1].p[1] = 0;
}

stPoint GetSign(stPoint Point)
{
    stPoint Sign;
    if(Point.p[0] < 0) Sign.p[0] = -1; else Sign.p[0] = +1;
    if(Point.p[1] < 0) Sign.p[1] = -1; else Sign.p[1] = +1;
    return Sign;
}

stPoint AddPoints(stPoint P1, stPoint P2)
{
	stPoint Res;
	SetPoint(Res, P1.p[0] + P2.p[0], P1.p[1] + P2.p[1]);
	return Res;
}

stPoint SubtractPoints(stPoint P1, stPoint P2)
{
	stPoint Res;
	SetPoint(Res, P1.p[0] - P2.p[0], P1.p[1] - P2.p[1]);
	return Res;
}

stPoint MultiplyPoints(stPoint P1, stPoint P2)
{
	stPoint Res;
	SetPoint(Res, P1.p[0] * P2.p[0], P1.p[1] * P2.p[1]);
	return Res;
}

stPoint DevidePoints(stPoint P1, stPoint P2)
{
	stPoint Res;
	SetPoint(Res, P1.p[0] / P2.p[0], P1.p[1] / P2.p[1]);
	return Res;
}

bool IsPointNull(stPoint inPoint)
{
    return (inPoint.p[0] == 0 && inPoint.p[1] == 0);
}

bool IsPlaneNull(stPlane inPlane)
{
	return (inPlane.Point[0].p[0] == 0 && inPlane.Point[0].p[1] == 0 && inPlane.Point[1].p[0] == 0 && inPlane.Point[1].p[1] == 0 );
}

bool ComparePoints(stPoint A, stPoint B)
{
     return (A.p[0] == B.p[0] && A.p[1] == B.p[1]);
}

void SetRay(stRay &Ray, stPoint Origin, stPoint Direction)
{
     SetPoint(Ray.Origin, Origin.p[0], Origin.p[1]);
     SetPoint(Ray.Direction, Direction.p[0], Direction.p[1]);
}

void SetRay(stRay &Ray, stPoint Origin, float x, float y)
{
     SetPoint(Ray.Origin, Origin.p[0], Origin.p[1]);
     SetPoint(Ray.Direction, x, y);
}

void SetRay(stRay &Ray, float x1, float y1, float x2, float y2)
{
     SetPoint(Ray.Origin, x1, y1);
     SetPoint(Ray.Direction, x2, y2);
}

void SetLineByTwoPoints(stLine &Line, float x1, float y1, float x2, float y2)
{
    Line.a = y2 - y1;
	Line.b = x1 - x2;
	Line.c = x1 * (y1 - y2) + y1 * (x2 - x1);
}

void SetLineByTwoPoints(stLine &Line, stPoint A, stPoint B)
{
    Line.a = B.p[1] - A.p[1];
	Line.b = A.p[0] - B.p[0];
	Line.c = A.p[0] * (A.p[1] - B.p[1]) + A.p[1] * (B.p[0] - A.p[0]);
}

void SetLineByRay(stLine &Line, stRay Ray)
{
     stPoint B;
     SetPoint(B, Ray.Origin.p[0] + Ray.Direction.p[0], Ray.Origin.p[1] + Ray.Direction.p[1]);
     SetLineByTwoPoints(Line, Ray.Origin, B);
}

bool PointBelognsToLine(stLine Line, stPoint Point)
{
    return !(Line.a* Point.p[0] + Line.b* Point.p[1] + Line.c);
}

bool PointBelongsToSegment(stPlane Side, stPoint Point)
{
    stLine Line;
    SetLineByTwoPoints(Line, Side.Point[0], Side.Point[1]);
    if(PointBelognsToLine(Line, Point))
    {
   	    if (Point.p[0] >= Side.Point[0].p[0] && Point.p[0] <= Side.Point[1].p[0] &&
		    Point.p[1] >= Side.Point[0].p[1] && Point.p[1] <= Side.Point[1].p[1])
		return true;
    }
     
    return false;
}

bool LinesIntersect(stLine Line1, stLine Line2, stPoint& Point)
{
    float Determinant = Line1.a * Line2.b - Line2.a * Line1.b;
    //check on parallels
    if(!Determinant)
    {
        SetPoint(Point, 0, 0);
        return false;
    }
    float Dx = (- Line1.c) * Line2.b - (- Line2.c) * Line1.b;
    float Dy = Line1.a *(- Line2.c) - Line2.a *(- Line1.c);
    SetPoint(Point, Dx/Determinant, Dy/Determinant);
}

float GetVectorLength(stPoint Vector)
{
    return sqrt(Vector.p[0]*Vector.p[0] + Vector.p[1]*Vector.p[1]);
}

float GetSegmentLength(stPoint A, stPoint B)
{
    stPoint Vector;
    SetPoint(Vector, B.p[0] - A.p[0], B.p[1] - A.p[1]);
    
    return GetVectorLength(Vector);
}

stPoint GetPlaneCenter(stPlane Plane)
{
    stPoint CenterPoint;
    SetPoint(CenterPoint, 
        Plane.Point[0].p[0] + (Plane.Point[1].p[0] - Plane.Point[0].p[0]) / 2, 
        Plane.Point[0].p[1] + (Plane.Point[1].p[1] - Plane.Point[0].p[1]) / 2);
    
    return CenterPoint;
}

stPoint GetPlanePoint(stPlane Plane, unsigned char SideNum)
{
    stPoint Point;
    SetPoint(Point, 0, 0);
    if(SideNum > 3) return Point;
    
    switch(SideNum)
    {
        //left bottom
        case 0:
            SetPoint(Point, Plane.Point[0].p[0], Plane.Point[0].p[1]); 
            break;
        //left top
        case 1:
            SetPoint(Point, Plane.Point[0].p[0], Plane.Point[1].p[1]);
            break;
        //right top
        case 2:
            SetPoint(Point, Plane.Point[1].p[0], Plane.Point[1].p[1]); 
            break;
        //right bottom
        case 3:
            SetPoint(Point, Plane.Point[1].p[0], Plane.Point[0].p[1]);
            break; 
    }
    
    return Point;
}

stPlane GetPlaneSide(stPlane Plane, unsigned char SideNum)
{
    stPlane Side;
    SetPlane(Side, 0, 0, 0, 0);
    if(SideNum > 3) return Side;
    
    switch(SideNum)
    {
        //left side
        case 0:
            SetPlane(Side, Plane.Point[0].p[0], Plane.Point[0].p[1], Plane.Point[0].p[0], Plane.Point[1].p[1]); 
            break;
        //top side
        case 1:
            SetPlane(Side, Plane.Point[0].p[0], Plane.Point[1].p[1], Plane.Point[1].p[0], Plane.Point[1].p[1]);
            break;
        //right side
        case 2:
            SetPlane(Side, Plane.Point[1].p[0], Plane.Point[0].p[1], Plane.Point[1].p[0], Plane.Point[1].p[1]); 
            break;
        //bottom side
        case 3:
            SetPlane(Side, Plane.Point[0].p[0], Plane.Point[0].p[1], Plane.Point[1].p[0], Plane.Point[0].p[1]);
            break; 
    }
    
    return Side;
}

bool GetBoxIntersectByRay1(stPlane Box, stRay Ray, stPoint& DiffSize)
{   
    //start test by two directions: axis x and axis y
    //0 - x, 1 - y
    stPoint Test1, Test2;
    float Near = -FLT_MAX;
    float Far = FLT_MAX;
    for(int i=0; i<2; i++)
    {
        //test direction and origin position about plane sides
        //ray parallel with box side in this direction
        //cout<<i<<endl;
        if(Ray.Direction.p[i] == 0)
        {
            //cout<<"Ray "<<i<<" (x or y) direction is zero"<<endl;
            //origion is out of box
            //LogFile<<Ray.Origin.p[i]<<"<"<<Box.Point[0].p[i]<<" || "<<Ray.Origin.p[i]<<">"<<Box.Point[1].p[i]<<endl;
            if(Ray.Origin.p[i] < Box.Point[0].p[i] || Ray.Origin.p[i] > Box.Point[1].p[i])
            {
                //LogFile<<"Ray origin out of box"<<endl;
                return false;
            }
            //origin is in BOX somehow 0_0
        }
        //ray not parallel with box side in this direction
        Test1.p[i] = (Box.Point[0].p[i] - Ray.Origin.p[i]) / Ray.Direction.p[i];
        Test2.p[i] = (Box.Point[1].p[i] - Ray.Origin.p[i]) / Ray.Direction.p[i];
        //cout<<Test1.p[i]<<" "<<Test2.p[i]<<endl;
        //we need nearest side of box
        if(Test1.p[i] > Test2.p[i]) swap(Test1, Test2);
        if(Test1.p[i] > Near) Near = Test1.p[i];
        if(Test2.p[i] < Far) Far = Test2.p[i];
        //cout<<Near<<" "<<Far<<endl;
        if(Near > Far || Far < 0)
        {
            //LogFile<<"Near > Far or Far < 0"<<endl;
            return false;
        }
    }
    SetPoint(DiffSize, Near, Far);
    return true;
}

bool GetBoxIntersectByRay2(stPlane Box, stRay Ray, stPoint& Point, float& Distance, unsigned char& SideNum)
{
    stLine LineSide, RayLine;
    stPlane Side;
    stPoint Intersect;
    float MinDistance = 0;
    Distance = 0;
    SetPoint(Point, 0 ,0);
    
    //get ray line
    //cout<<"Create line by ray:"<<endl;
    SetLineByRay(RayLine, Ray);
    //cout<<RayLine.a<<" "<<RayLine.b<<" "<<RayLine.c<<endl;
    //intersect cycle
    for(int i=0; i<4; i++)
    {
        //get side
        Side = GetPlaneSide(Box, i);    
        //get line by side
        //cout<<"Create "<<i<<" line by points:"<<endl;
        SetLineByTwoPoints(LineSide, Side.Point[0], Side.Point[1]);
        //get intersect point
        LinesIntersect(LineSide, RayLine, Intersect);
        //cout<<i<<" "<<Intersect.p[0]<<", "<<Intersect.p[1]<<endl;
        //check that point on box side
        if(PointInSegment(Intersect, Side))
        {
            //cout<<"Point on side"<<endl;
            //get segment length between intersect point and ray origin
            Distance = GetSegmentLength(Intersect, Ray.Origin);
            if(MinDistance == 0)
            {
                MinDistance = Distance;
                CopyPoint(Intersect, Point);
                SideNum = i;
            }
            if(MinDistance > Distance)
            {
                MinDistance = Distance;
                CopyPoint(Intersect, Point);
                SideNum = i;
            }
        }
    }
    Distance = MinDistance;
    if(Point.p[0] == 0 && Point.p[1] == 0) return false;
    
    return true;
}

void SetPoint(stPoint &dstPoint, float p0, float p1)
{
	dstPoint.p[0] = p0;
	dstPoint.p[1] = p1;
}

void SetPlane(stPlane &dstPlane, stPoint Point0, stPoint Point1)
{
	CopyPoint(dstPlane.Point[0], Point0);
	CopyPoint(dstPlane.Point[1], Point1);
}

void SetPlane(stPlane &dstPlane, float p0, float p1, float p2, float p3)
{
	dstPlane.Point[0].p[0] = p0;
	dstPlane.Point[0].p[1] = p1;
	dstPlane.Point[1].p[0] = p2;
	dstPlane.Point[1].p[1] = p3;
}

void SetPlaneByOffset(stPlane &dstPlane, float x, float y, float width, float height)
{
	dstPlane.Point[0].p[0] = x;
	dstPlane.Point[0].p[1] = y;
	dstPlane.Point[1].p[0] = dstPlane.Point[0].p[0] + width;
	dstPlane.Point[1].p[1] = dstPlane.Point[0].p[1] + height;
}

void SetPlaneByOffset(stPlane &dstPlane, stPoint Start, stPoint Size)
{
    dstPlane.Point[0].p[0] = Start.p[0];
	dstPlane.Point[0].p[1] = Start.p[1];
	dstPlane.Point[1].p[0] = dstPlane.Point[0].p[0] + Size.p[0];
	dstPlane.Point[1].p[1] = dstPlane.Point[0].p[1] + Size.p[1];
}

void GetPlanePoint(stPlane srcPlane, stPoint &dstPoint, bool RT)
{
    if(RT == 0) //left bottom
    { 
        dstPoint.p[0] = srcPlane.Point[0].p[0];
        dstPoint.p[1] = srcPlane.Point[0].p[1];
    }
    else //right top
    {
        dstPoint.p[0] = srcPlane.Point[1].p[0];
        dstPoint.p[1] = srcPlane.Point[1].p[1];
    }
}

void GetPlanesDiff(stPlane srcPlane1, stPlane srcPlane2, stPoint& srcPoint)
{
    srcPoint.p[0] = srcPlane2.Point[1].p[0] - srcPlane1.Point[1].p[0];
    srcPoint.p[1] = srcPlane2.Point[1].p[1] - srcPlane1.Point[1].p[1];
}

void GetPointsDiff(stPoint srcPoint1, stPoint srcPoint2, stPoint& srcPoint)
{
    srcPoint.p[0] = srcPoint2.p[0] - srcPoint1.p[0];
    srcPoint.p[1] = srcPoint2.p[1] - srcPoint1.p[1];
}

float GetPlaneWidth(stPlane srcPlane)
{
	return srcPlane.Point[1].p[0] - srcPlane.Point[0].p[0];
}

float GetPlaneHeight(stPlane srcPlane)
{
	return srcPlane.Point[1].p[1] - srcPlane.Point[0].p[1];
}

void CopyPoint(stPoint srcPoint, stPoint &dstPoint)
{
	dstPoint.p[0] = srcPoint.p[0];
	dstPoint.p[1] = srcPoint.p[1];
}

void CopyPlane(stPlane srcPlane, stPlane &dstPlane)
{
	CopyPoint(srcPlane.Point[0], dstPlane.Point[0]);
	CopyPoint(srcPlane.Point[1], dstPlane.Point[1]);
}

void MovePoint(stPoint &dstPoint, float p0, float p1)
{
    dstPoint.p[0] += p0;
    dstPoint.p[1] += p1;
}

void MovePlane(stPlane &Plane, stPoint Delta)
{
	Plane.Point[0].p[0] += Delta.p[0];
	Plane.Point[0].p[1] += Delta.p[1];
	Plane.Point[1].p[0] += Delta.p[0];
	Plane.Point[1].p[1] += Delta.p[1];
}

void MovePlane(stPlane &Plane, float d0, float d1)
{
	Plane.Point[0].p[0] += d0;
	Plane.Point[0].p[1] += d1;
	Plane.Point[1].p[0] += d0;
	Plane.Point[1].p[1] += d1;
}

bool PlaneCollision(stPlane srcPlane, stPlane dstPlane)
{
	/*=========================================
	with this bug:
    1 - srcPalne 2 - dstPlane
	 -----
	|     |
	|  1 --
	 |     |
	 |  2  |
	  -----
	=========================================*/
	if (srcPlane.Point[1].p[0] <= dstPlane.Point[0].p[0]) return false; //plane right of us
	if (srcPlane.Point[0].p[0] >= dstPlane.Point[1].p[0]) return false; //plane left of us
	if (srcPlane.Point[1].p[1] <= dstPlane.Point[0].p[1]) return false; //plane over us
	if (srcPlane.Point[0].p[1] >= dstPlane.Point[1].p[1]) return false; //plane under us
	return true;
}

bool PlaneCollision2(stPlane srcPlane, stPlane dstPlane)
{
	/*=========================================
    with this bug:
    	 ---
    	|  |
      --------
	 |_______|
	   |  |
	   |__|	
    =========================================*/
    if(PointInSegment(GetPlanePoint(srcPlane, 0), dstPlane)) return true;
    if(PointInSegment(GetPlanePoint(srcPlane, 1), dstPlane)) return true;
    if(PointInSegment(GetPlanePoint(srcPlane, 2), dstPlane)) return true;
    if(PointInSegment(GetPlanePoint(srcPlane, 3), dstPlane)) return true;
    return false;
}

bool PlaneCollision3(stPlane srcPlane, stPlane dstPlane)
{
	if(srcPlane.Point[0].p[1] < dstPlane.Point[0].p[1] && srcPlane.Point[1].p[1] < dstPlane.Point[0].p[1] ) return false; //plane over us
	if(srcPlane.Point[0].p[1] > dstPlane.Point[1].p[1] && srcPlane.Point[1].p[1] > dstPlane.Point[1].p[1] ) return false; //plane under us
	if(srcPlane.Point[0].p[0] < dstPlane.Point[0].p[0] && srcPlane.Point[1].p[0] < dstPlane.Point[0].p[0] ) return false; //plane right of us
	if(srcPlane.Point[0].p[0] > dstPlane.Point[1].p[0] && srcPlane.Point[1].p[0] > dstPlane.Point[1].p[0] ) return false; //plane left of us
	return true;
}

bool PlaneCollision(stPlane PlaneAfter, stPlane dstPlane, char &Direction)
{
	//we over plane
	if (PlaneAfter.Point[0].p[1] < dstPlane.Point[1].p[1] && PlaneAfter.Point[1].p[1] > dstPlane.Point[1].p[1])
	{
		if (PlaneAfter.Point[0].p[0] >= dstPlane.Point[0].p[0] && PlaneAfter.Point[0].p[0] <= dstPlane.Point[1].p[0]) { Direction = COLLISION_LEFTTOP; return true; }
		if (PlaneAfter.Point[1].p[0] >= dstPlane.Point[0].p[0] && PlaneAfter.Point[1].p[0] <= dstPlane.Point[1].p[0]) { Direction = COLLISION_RIGHTTOP; return true; }
	}
	//we under plane
	if (PlaneAfter.Point[1].p[1] > dstPlane.Point[0].p[1] && PlaneAfter.Point[1].p[1] < dstPlane.Point[1].p[1])
	{
		if (PlaneAfter.Point[0].p[0] >= dstPlane.Point[0].p[0] && PlaneAfter.Point[0].p[0] <= dstPlane.Point[1].p[0]) { Direction = COLLISION_LEFTBOTTOM; return true; }
		if (PlaneAfter.Point[1].p[0] >= dstPlane.Point[0].p[0] && PlaneAfter.Point[1].p[0] <= dstPlane.Point[1].p[0]) { Direction = COLLISION_RIGHTBOTTOM; return true; }
	}
	Direction = COLLISION_NONE;
	return false;
}

bool PlaneInSegment(stPlane Plane, stPlane Segment)
{
	if (Plane.Point[0].p[0] >= Segment.Point[0].p[0] && Plane.Point[1].p[0] <= Segment.Point[1].p[0] &&
		Plane.Point[0].p[1] >= Segment.Point[0].p[1] && Plane.Point[1].p[1] <= Segment.Point[1].p[1])
		return true;
	return false;
}

bool PointInSegment(stPoint Point, stPlane Segment)
{
	if (Point.p[0] >= Segment.Point[0].p[0] && Point.p[0] <= Segment.Point[1].p[0] &&
		Point.p[1] >= Segment.Point[0].p[1] && Point.p[1] <= Segment.Point[1].p[1])
		return true;
	return false;
}
