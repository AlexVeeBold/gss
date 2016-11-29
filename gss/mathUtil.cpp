///////////////////////////////////////////////////////////////
//
// mathUtil: math stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 02:09 - created (moved from gssMain)
//   19.12.2014 08:42 - moved in rightTriangle and zobbInside stuff
//   14.07.2016 04:06 - fixed wrong z.o.b.b. offsets calculation
//   20.11.2016 04:13 - replaced non-const ptrs with const refs
//


#include <math.h>       // pow, floor, sqrt, sinf, cosf

#include "uDef.h"

#include "gssDef.h"



FLOAT fAbs(FLOAT f)
{
    if(f < 0.0f)
    {
        return -f;
    }
    return f;
}

FLOAT fmin(FLOAT fa, FLOAT fb)
{
    if(fa < fb)
    {
        return fa;
    }
    return fb;
}

FLOAT fmax(FLOAT fa, FLOAT fb)
{
    if(fa > fb)
    {
        return fa;
    }
    return fb;
}

DWORD uiFloor(FLOAT f)
{
    DWORD ui;
    FLOAT fv;
    fv = (FLOAT) floor(f);
    ui = (fv < 0.0) ? 0 : (DWORD)f;
    return ui;
}

FLOAT fCrop(FLOAT f, DWORD numPointDigits)
{
    DOUBLE dfMult;
    DOUBLE df;
    df = f;
    dfMult = pow(2.0, (DOUBLE)numPointDigits);
    df = floor(df * dfMult);
    df /= dfMult;
    return (FLOAT) df;
}

FLOAT fRound(FLOAT f)
{
    DOUBLE df;
    DOUBLE mod;
    DOUBLE inc;
    DOUBLE one;
    df = f;
    one = (df > 0.0) ? 1.0 : -1.0;      // df == 0.0  ->  mod == 0.0  ->  inc == 0.0
    mod = fmod(df, 1.0);
    inc = (mod > 0.5) ? one : 0.0;
    df = floor(f) + inc;
    return (FLOAT) df;
}

FLOAT fAlign(FLOAT f, FLOAT fAlign)
{
    DOUBLE df;
    df = fRound(f / fAlign);
    df *= fAlign;
    return (FLOAT) df;
}


FLOAT rightTriangleHypotenuse(FLOAT legA, FLOAT legB)
{
    FLOAT hyp;
    hyp = (FLOAT) sqrt( pow(legA, 2.0f) + pow(legB, 2.0f) );
    return hyp;
}

FLOAT rightTriangleLeg(FLOAT otherLeg, FLOAT hypotenuse)
{
    FLOAT leg;
    leg = (FLOAT) sqrt( pow(hypotenuse, 2.0f) - pow(otherLeg, 2.0f) );
    return leg;
}


FLOAT distance2d(const FloatVector3& fvA, const FloatVector3& fvB)
{
    FloatVector3 fv;
    FLOAT dist;
    fv.X = fvB.X - fvA.X;
    fv.Y = fvB.Y - fvA.Y;
    fv.X *= fv.X;
    fv.Y *= fv.Y;
    dist = sqrt(fv.X + fv.Y);
    return dist;
}

FLOAT distance3d(const FloatVector3& fvA, const FloatVector3& fvB)
{
    FloatVector3 fv;
    FLOAT dist;
    fv.X = fvB.X - fvA.X;
    fv.Y = fvB.Y - fvA.Y;
    fv.Z = fvB.Z - fvA.Z;
    fv.X *= fv.X;
    fv.Y *= fv.Y;
    fv.Z *= fv.Z;
    dist = sqrt(fv.X + fv.Y + fv.Z);
    return dist;
}

BOOL aabbInside3d(const FloatVector3& fvPos, const FloatVector3& fvAABBMin, const FloatVector3& fvAABBMax)
{
    BOOL bInside = TRUE;
    bInside &= (fvPos.X > fvAABBMin.X);
    bInside &= (fvPos.X < fvAABBMax.X);
    if(bInside != FALSE)
    {
        bInside &= (fvPos.Y > fvAABBMin.Y);
        bInside &= (fvPos.Y < fvAABBMax.Y);
        if(bInside != FALSE)
        {
            bInside &= (fvPos.Z > fvAABBMin.Z);
            bInside &= (fvPos.Z < fvAABBMax.Z);
        }
    }
    return bInside;
}

BOOL aabbOutside3d(const FloatVector3& fvPos, const FloatVector3& fvAABBMin, const FloatVector3& fvAABBMax)
{
    BOOL bInside = FALSE;
    bInside |= (fvPos.X < fvAABBMin.X);
    bInside |= (fvPos.X > fvAABBMax.X);
    if(bInside == FALSE)
    {
        bInside |= (fvPos.Y < fvAABBMin.Y);
        bInside |= (fvPos.Y > fvAABBMax.Y);
        if(bInside == FALSE)
        {
            bInside |= (fvPos.Z < fvAABBMin.Z);
            bInside |= (fvPos.Z > fvAABBMax.Z);
        }
    }
    return bInside;
}


BOOL zobbInsideCornerTriangle(const FloatVector3& fvPos, const FloatZOBBox3& ZOBBox, DWORD iSideCorner)
{
    // perform quick boundary test
    FLOAT fBoundaryX;
    BOOL bInsideTriangle;
    // a -> A          b
    //  +---.---------+
    //  |iii `.       | | (offset)
    //  |iii   `.     | V
    //  |iii     `.   |
    //D .iii       `. |          i = inside
    //  |iii         `. B
    //  |iii`.       /|
    //^ |iii  `.    / |
    //| |iii    `. /  |
    //  +---X-----`---+
    // d          C <- c
    switch(iSideCorner)
    {
    case SIDE_NORTH:    // before A
        fBoundaryX = ZOBBox.min.X + ZOBBox.offset.X;
        bInsideTriangle = (fvPos.X < fBoundaryX);
        break;
    case SIDE_EAST:     // after A
        fBoundaryX = ZOBBox.min.X + ZOBBox.offset.X;
        bInsideTriangle = (fvPos.X > fBoundaryX);
        break;
    case SIDE_SOUTH:    // before C
        fBoundaryX = ZOBBox.max.X - ZOBBox.offset.X;
        bInsideTriangle = (fvPos.X > fBoundaryX);
        break;
    case SIDE_WEST:     // after C
        fBoundaryX = ZOBBox.max.X - ZOBBox.offset.X;
        bInsideTriangle = (fvPos.X < fBoundaryX);
        break;
    }

    // point is outside of boundary? skip to another side
    if(bInsideTriangle == FALSE)
    {
        return FALSE;
    }

    // select line to work with
    FloatVector2 lineStart;
    FloatVector2 lineEnd;
    switch(iSideCorner)
    {
    case SIDE_NORTH:    // D to A
        lineStart.X = ZOBBox.min.X;
        lineStart.Y = ZOBBox.min.Y + ZOBBox.offset.Y;
        lineEnd.X = fBoundaryX;
        lineEnd.Y = ZOBBox.max.Y;
        break;
    case SIDE_EAST:     // A to B
        lineStart.X = fBoundaryX;
        lineStart.Y = ZOBBox.max.Y;
        lineEnd.X = ZOBBox.max.X;
        lineEnd.Y = ZOBBox.max.Y - ZOBBox.offset.Y;
        break;
    case SIDE_SOUTH:    // C to B
        lineStart.X = fBoundaryX;
        lineStart.Y = ZOBBox.min.Y;
        lineEnd.X = ZOBBox.max.X;
        lineEnd.Y = ZOBBox.max.Y - ZOBBox.offset.Y;
        break;
    case SIDE_WEST:     // D to C
        lineStart.X = ZOBBox.min.X;
        lineStart.Y = ZOBBox.min.Y + ZOBBox.offset.Y;
        lineEnd.X = fBoundaryX;
        lineEnd.Y = ZOBBox.min.Y;
        break;
    }
    
    // calculate line y for given x
    //
    // pos.x / pos.y = line.x / line.y                -- position on a line             -- line start == [0, 0]
    // (pos.x-start.x) / (pos.y-start.y)  =  (line.x-start.x) / (line.y-start.y)        -- line start != [0, 0]
    // (pos.x-start.x) / (pos.y-start.y) * (line.y-start.y)  =  (line.x-start.x)        --  *= (line.y-start.y)
    // (pos.x-start.x) * (line.y-start.y)  =  (pos.y-start.y) * (line.x-start.x)        --  *= (pos.y-start.y)
    // (line.y-start.y)  =  (pos.y-start.y) * (line.x-start.x) / (pos.x-start.x)        --  /= (pos.x-start.x)
    // line.y  =  start.y + ( (pos.y-start.y) * (line.x-start.x) / (pos.x-start.x) )
    //
    FLOAT fLineY;
    fLineY = lineStart.Y + ((fvPos.X - lineStart.X) * (lineEnd.Y - lineStart.Y) / (lineEnd.X - lineStart.X));
    // a -> A         b
    //  +---.--------+
    //  |iii .       | | (offset)
    //  |ii   `.     | V
    //  |i      `.   |
    //D .         `. |          i = inside
    //  |`.         `. B
    //  |  `.       /|
    //^ |    `.    / |
    //| |      `. /  |
    //  +---X----`---+
    // d         C <- c
    switch(iSideCorner)
    {
    case SIDE_NORTH:    // D to A
    case SIDE_EAST:     // A to B
        bInsideTriangle = (fLineY < fvPos.Y);    // point is above the line
        break;
    case SIDE_SOUTH:    // C to B
    case SIDE_WEST:     // D to C
        bInsideTriangle = (fLineY > fvPos.Y);    // point is below the line
        break;
    }

    return bInsideTriangle;
}

BOOL zobbInside3d(const FloatVector3& fvPos, const FloatZOBBox3& ZOBBox)
{
    // The Procedure is named Inside, so point have to be INSIDE to pass, boundaries are NOT included
    BOOL bInside;
    // test AABB first
    bInside = aabbInside3d(fvPos, ZOBBox.min, ZOBBox.max);
    if(bInside)
    {
        // a -> A         b
        //  +---.--------+
        //  |  / `.      | | (offset)
        //  | /    `.    | V
        //  |/       `.  |
        //D .          `.|
        //  x`.          ` B
        //  |  `.       /|
        //^ |    `.    / |
        //| |      `. /  |
        //  +---X----`---+
        // d         C <- c

        // then test ZOBB (unaligned only)
        if((ZOBBox.offset.X != 0.0) & (ZOBBox.offset.Y != 0.0))     // small offsets should be bound to 0.0
        {
            DWORD iSide;
            // test each corner
            BOOL bInsideCorner = FALSE;
            for(iSide = SIDE_NORTH; iSide <= SIDE_WEST; iSide++)
            {
                bInsideCorner |= zobbInsideCornerTriangle(fvPos, ZOBBox, iSide);
                bInside &= !bInsideCorner;
                if(bInside == FALSE)
                {
                    break;
                }
            }
        }
    }

    return bInside;
}




