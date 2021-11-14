// Fill out your copyright notice in the Description page of Project Settings.

#include "MappingAlgo.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values for this component's properties
UMappingAlgo::UMappingAlgo()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool UMappingAlgo::onSegment(FVector p, FVector q, FVector r)
{
    if (q.X <= FGenericPlatformMath::Max(p.X, r.X) && q.X >= FGenericPlatformMath::Min(p.X, r.X) &&
        q.Y <= FGenericPlatformMath::Max(p.Y, r.Y) && q.Y >= FGenericPlatformMath::Min(p.Y, r.Y))
        return true;
    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int UMappingAlgo::orientation(FVector p, FVector q, FVector r)
{
    int val = (q.Y - p.Y) * (r.Y - q.Y) -
        (q.X - p.X) * (r.Y - q.Y);

    if (val == 0) return 0; // collinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool UMappingAlgo::doIntersect(FVector p1, FVector q1, FVector p2, FVector q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

// Returns true if the point p lies inside the polygon[] with n vertices
bool UMappingAlgo::isInside(TArray<FVector> polygon, int32 n, FVector p)
{
    // There must be at least 3 vertices in polygon[]
    if (n < 3) return false;

    // Create a point for line segment from p to infinite
    FVector extreme = FVector(inf, p.Y, 0);

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i + 1) % n;

        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // If the point 'p' is collinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);

            count++;
        }
        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise
    return count & 1; // Same as (count%2 == 1)
}


bool UMappingAlgo::isSquareInside(TArray<FVector> c, FVector p) {
    FVector cUR = FVector(p.X + tileSize, p.Y, 0); //upper right
    FVector cLL = FVector(p.X, p.Y + tileSize,0); //lower left
    FVector cLR = FVector(p.X + tileSize, p.Y + tileSize, 0);
    int32 cSize = c.Num();
    return isInside(c, cSize, p) && isInside(c, cSize, cUR)
        && isInside(c, cSize, cLL) && isInside(c, cSize, cLR);
}


TArray<FVector> UMappingAlgo::GeneratePoints(TArray<FVector> Outline)
{//there HAS to be a more efficient way of doing this.
    //Map Outline, get min and max values of the x,y values 
    for (int32 i = 0; i < MapOutline.Num(); i++)
    {
        if (MapOutline[i].X > highestX) highestX = MapOutline[i].X;
        if (MapOutline[i].Y > highestY) highestY = MapOutline[i].Y;
        if (MapOutline[i].X < lowestX) lowestX = MapOutline[i].X;
        if (MapOutline[i].Y < lowestY) lowestY = MapOutline[i].Y;
    }
    //Get all possible point coords 
    for (int row = lowestY; row < highestY; row += tileSize) {
        for (int col = lowestX; col < highestX; col += tileSize) {
            FVector p = FVector(col, row, 0);
            //Are we inside the shape?
            if (isSquareInside(MapOutline, p)) {
                newCoordinates.Add(p);
            }
        }
    }
    return newCoordinates;
}

