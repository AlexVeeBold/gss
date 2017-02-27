///////////////////////////////////////////////////////////////
//
// gtaParkZone: parking lot zone (trigger) markup helper
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 01:06 - created (moved from gssMain)
//   02.12.2014 08:28 - written & tested zone corners adjust & draw code
//   02.12.2014 14:15 - updated zone corners code a little
//   03.12.2014 15:50 - added inner rectangle orthogonal point calculation
//   04.12.2014 04:45 - tested i.r.o.p.c: working badly with current bbox adjusting system, what needs rewrite
//   04.12.2014 08:08 - written & tested 3-point rectangle adjusting: good, but need better
//   04.12.2014 19:40 - rewritten 3-point rect. adjusting: working juuust fine
//   05.12.2014 07:04 - updated 3-point code, added move-center mode & tested them codes again
//   08.12.2014 10:38 - added bbox calculation for 3-point rect.
//   08.12.2014 22:54 - applied a huge update to zoneDef and ZoneStateMachine
//   09.12.2014 12:49 - tested and fixed update bugs (3-point adjusting is much more friendly now)
//   10.12.2014 00:48 - cleaned up a little
//   10.12.2014 15:03 - cleaned up more
//   10.12.2014 20:08 - cleaned up even more
//   11.12.2014 01:16 - written & tested z.o.b.b. test (earlier: 3-point changed to 4-point)
//   12.12.2014 00:57 - tested new 4-point adjusting in III: not really friendly
//   13.12.2014 02:27 - written part of another 3-point adjusting system (should be friendly for real this time)
//   18.12.2014 06:24 - fixed & tested 3-point z-oriented bounding box adjusting system
//   19.12.2014 05:03 - cleaned up again
//   04.01.2015 10:56 - rewritten veh. spawn test & moved it into zonedef (also added multi-lane support)
//   13.07.2016 02:43 - rewritten button-zone interaction, adding divide, multiply & decimal buttons (num-pad)
//   14.07.2016 03:52 - fixed wrong z.o.b.b. offsets calculation
//   05.12.2016 21:14 - moved keys/buttons/switches into classes, written base skeleton for park-editor class
//


#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <algorithm>    // std::swap

#include <ulib/uDef.h>
#include <ulib/tunic.h>
#include <ulib/uLog.h>

#include "gssDef.h"
#include <internal/gssScriptCall.h>

#include "mathUtil.h"

#include "parkDef.h"

#include "parkSys.h"

#include <internal/gtaMisc.h>
#include <internal/gtaVehicle.h>


#pragma warning(disable:4351)       // warning C4351: new behavior: elements of array will be default initialized


//////// //////// //////// //////// buttons //////// //////// //////// ////////

enum NPBUTTONS {
    NPB_WEST,    // num4 left    left
    NPB_EAST,    // num6 right   right
    NPB_NORTH,   // num8 up      up
    NPB_SOUTH,   // num2 down    down
    NPB_TOP,     // num7 home
    NPB_BOTTOM,  // num1 end
    NPB_PREV,    // num9 pgup
    NPB_NEXT,    // num3 pgdn
    NPB_ENTER,   // num5 clear
    NPB_LEAVE,   // num0 ins
    NPB_DEC,     // num-
    NPB_INC,     // num+
    NPB_DIVIDE,      // num/
    NPB_MULTIPLY,    // num*
    NPB_DECIMAL,     // num.
    //
    NUM_NPBUTTONS,
};

class NumPadButtons {
private:
    BYTE m_keys[2][256];
    DWORD m_iKeysCurrent;   // 0 or 1
    DWORD m_iKeysPrevious;  // 1 or 0
    inline BOOL isKeyPressedNow(DWORD vkey);
    inline BOOL wasKeyPressedBefore(DWORD vkey);
    inline BOOL isKeyPressed(DWORD vkey, BOOL pressedJustNow);
public:
    NumPadButtons()
        : m_iKeysCurrent(0), m_iKeysPrevious(1), m_keys(), button()
    {
    }

    void update(void);
    BOOL button[NUM_NPBUTTONS];
};

BOOL NumPadButtons::isKeyPressedNow(DWORD vkey)
{
    return ((m_keys[m_iKeysCurrent][vkey] & 0x80) != 0);
}

BOOL NumPadButtons::wasKeyPressedBefore(DWORD vkey)
{
    return ((m_keys[m_iKeysPrevious][vkey] & 0x80) != 0);
}

BOOL NumPadButtons::isKeyPressed(DWORD vkey, BOOL pressedJustNow)
{
    BOOL bIsPressed;
    bIsPressed = this->isKeyPressedNow(vkey);   // is down
    if(pressedJustNow)
    {
        // and is just pressed
        bIsPressed &= (this->wasKeyPressedBefore(vkey) == FALSE);
    }
    return bIsPressed;
}

void NumPadButtons::update(void)
{
    BOOL bUseArrows = FALSE;
    BOOL bSuccess;
    BOOL bOneStepMove = TRUE;   // one key press = one move step
    BOOL bOneStepSwitch = TRUE; // one key press = one mode change
    std::swap(m_iKeysCurrent, m_iKeysPrevious);
    bSuccess = ::GetKeyboardState(m_keys[m_iKeysCurrent]);
    if(bSuccess != FALSE)
    {
        button[NPB_WEST] = this->isKeyPressed(VK_NUMPAD4, bOneStepMove);
        button[NPB_EAST] = this->isKeyPressed(VK_NUMPAD6, bOneStepMove);
        button[NPB_NORTH] = this->isKeyPressed(VK_NUMPAD8, bOneStepMove);
        button[NPB_SOUTH] = this->isKeyPressed(VK_NUMPAD2, bOneStepMove);
        if(bUseArrows != FALSE)
        {
            button[NPB_WEST] |= this->isKeyPressed(VK_LEFT, bOneStepMove);
            button[NPB_EAST] |= this->isKeyPressed(VK_RIGHT, bOneStepMove);
            button[NPB_NORTH] |= this->isKeyPressed(VK_UP, bOneStepMove);
            button[NPB_SOUTH] |= this->isKeyPressed(VK_DOWN, bOneStepMove);
        }
        button[NPB_TOP] = this->isKeyPressed(VK_NUMPAD7, bOneStepMove);
        button[NPB_BOTTOM] = this->isKeyPressed(VK_NUMPAD1, bOneStepMove);
        button[NPB_PREV] = this->isKeyPressed(VK_NUMPAD9, bOneStepSwitch);
        button[NPB_NEXT] = this->isKeyPressed(VK_NUMPAD3, bOneStepSwitch);
        button[NPB_ENTER] = this->isKeyPressed(VK_NUMPAD5, bOneStepSwitch);
        button[NPB_LEAVE] = this->isKeyPressed(VK_NUMPAD0, bOneStepSwitch);
        button[NPB_DEC] = this->isKeyPressed(VK_SUBTRACT, bOneStepSwitch);
        button[NPB_INC] = this->isKeyPressed(VK_ADD, bOneStepSwitch);
        button[NPB_DIVIDE] = this->isKeyPressed(VK_DIVIDE, bOneStepSwitch);
        button[NPB_MULTIPLY] = this->isKeyPressed(VK_MULTIPLY, bOneStepSwitch);
        button[NPB_DECIMAL] = this->isKeyPressed(VK_DECIMAL, bOneStepSwitch);
    }
}


class ButtonSwitchBoolean {
private:
    DWORD m_keySwitch;
    DWORD* m_pVariable;
    NumPadButtons* m_pNPButtons;

public:
    ButtonSwitchBoolean(NumPadButtons* pNPButtons, DWORD* pVar, DWORD keySwitch)
    {
        m_keySwitch = keySwitch;
        m_pNPButtons = pNPButtons;
        m_pVariable = pVar;
    }

    void update(void)
    {
        if(m_pNPButtons->button[m_keySwitch])
        {
            *m_pVariable = !*m_pVariable;
        }
    }
};


class ButtonSwitch2Values {
private:
    DWORD m_keySwitch;
    DWORD m_first;
    DWORD m_second;
    DWORD* m_pVariable;
    NumPadButtons* m_pNPButtons;

public:
    ButtonSwitch2Values(NumPadButtons* pNPButtons, DWORD* pVar, 
        DWORD keySwitch, DWORD modeFirst, DWORD modeSecond)
    {
        m_keySwitch = keySwitch;
        m_first = modeFirst;
        m_second = modeSecond;
        m_pNPButtons = pNPButtons;
        m_pVariable = pVar;
    }

    void update(void)
    {
        if(m_pNPButtons->button[m_keySwitch])
        {
            DWORD value = *m_pVariable;
            if(value == m_first)
            {
                value = m_second;
            }
            else
            {
                value = m_first;
            }
            *m_pVariable = value;
        }
    }
};


class ButtonSwitchLooped {
private:
    DWORD m_keyBack;
    DWORD m_loopBackFrom;
    DWORD m_loopBackTo;
    DWORD m_keyForward;
    DWORD m_loopForwardFrom;
    DWORD m_loopForwardTo;
    DWORD* m_pVariable;
    NumPadButtons* m_pNPButtons;

public:
    ButtonSwitchLooped(NumPadButtons* pNPButtons, DWORD* pVar, 
        DWORD keyBack, DWORD modeLoopBackFrom, DWORD modeLoopBackTo, 
        DWORD keyFwd, DWORD modeLoopFwdFrom, DWORD modeLoopFwdTo)
    {
        m_keyBack = keyBack;
        m_loopBackFrom = modeLoopBackFrom;
        m_loopBackTo = modeLoopBackTo;
        m_keyForward = keyFwd;
        m_loopForwardFrom = modeLoopFwdFrom;
        m_loopForwardTo = modeLoopFwdTo;
        m_pNPButtons = pNPButtons;
        m_pVariable = pVar;
    }

    void update(void)
    {
        DWORD value = *m_pVariable;
        if(m_pNPButtons->button[m_keyBack])
        {
            value--;
            if(value == m_loopBackFrom)
            {
                value = m_loopBackTo;
            }
        }
        if(m_pNPButtons->button[m_keyForward])
        {
            value++;
            if(value == m_loopForwardFrom)
            {
                value = m_loopForwardTo;
            }
        }
        *m_pVariable = value;
    }
};


class ButtonSwitchNumber {
private:
    DWORD m_keySubtractOne;
    DWORD m_keyAddOne;
    INT m_valueMin;
    INT m_valueMax;
    INT* m_pVariable;
    NumPadButtons* m_pNPButtons;

public:
    ButtonSwitchNumber(NumPadButtons* pNPButtons, INT* pVar, 
        DWORD keySubtract, DWORD keyAdd, INT minValue, INT maxValue)
    {
        m_keySubtractOne = keySubtract;
        m_keyAddOne = keyAdd;
        m_valueMin = minValue;
        m_valueMax = maxValue;
        m_pNPButtons = pNPButtons;
        m_pVariable = pVar;
    }

    void update(void)
    {
        INT value = *m_pVariable;
        if(m_pNPButtons->button[m_keySubtractOne])
        {
            value--;
            if(value < m_valueMin)
            {
                value = m_valueMax;
            }
        }
        if(m_pNPButtons->button[m_keyAddOne])
        {
            value++;
            if(value > m_valueMax)
            {
                value = m_valueMin;
            }
        }
        *m_pVariable = value;
    }
};




//////// //////// //////// //////// park zone //////// //////// //////// ////////


enum PARKEDITOR_MODE {
    PEM__LOOP_BACK,
    PEM_SETCENTER,
    PEM_SETQUADS,
    PEM_SETLANES,
    PEM_TESTPOS,
    PEM__LOOP_FWD,
    PEM__LOOP_BACK_TO = PEM_TESTPOS,
    PEM__LOOP_FWD_TO = PEM_SETCENTER,
};

enum PARKEDITOR_QUAD {
    PEQ_INNER,
    PEQ_OUTER,
};
enum PARKEDITOR_QUADCORNER {
    PEC__LOOP_BACK,
    PEC_A,
    PEC_B,
    PEC_HEIGHT,
    PEC__LOOP_FWD,
    PEC__LOOP_BACK_TO = PEC_HEIGHT,
    PEC__LOOP_FWD_TO = PEC_A,
};


class SSPCX {    // Parking Editor/Creator
private:
    DWORD m_mode;
    BOOL m_bMoveLock;
    BOOL m_bMoving;
    DWORD m_quad;
    DWORD m_corner;
    INT m_laneIndex;
    NumPadButtons m_NPButtons;
    ButtonSwitchLooped m_SwitchMode;
    ButtonSwitchBoolean m_SwitchMoveLock;
    ButtonSwitchBoolean m_SwitchMoving;
    ButtonSwitch2Values m_SwitchQuad;
    ButtonSwitchLooped m_SwitchCorner;
    ButtonSwitchNumber m_SwitchLane;
public:
    SSPCX() :
        m_mode(PEM_SETCENTER),
        m_bMoveLock(FALSE),
        m_bMoving(FALSE),
        m_quad(PEQ_INNER),
        m_corner(PEC_A),
        m_laneIndex(0),
        m_SwitchMode(&m_NPButtons, &m_mode, NPB_PREV, PEM__LOOP_BACK, PEM__LOOP_BACK_TO, NPB_NEXT, PEM__LOOP_FWD, PEM__LOOP_FWD_TO),
        m_SwitchMoveLock(&m_NPButtons, &m_bMoveLock, NPB_DIVIDE),
        m_SwitchMoving(&m_NPButtons, &m_bMoving, NPB_ENTER),
        m_SwitchQuad(&m_NPButtons, &m_quad, NPB_LEAVE, PEQ_INNER, PEQ_OUTER),
        m_SwitchCorner(&m_NPButtons, &m_corner, NPB_NORTH, PEC__LOOP_BACK, PEC__LOOP_BACK_TO, NPB_SOUTH, PEC__LOOP_FWD, PEC__LOOP_FWD_TO),
        m_SwitchLane(&m_NPButtons, &m_laneIndex, NPB_DEC, NPB_INC, 0, PSD_MAX_PARKING_LANES - 1)
    {
    }
    void FSM(AStringStream& display, GtaPlayerState& playerState);
};
//BTNSW_BOOLEAN bswZoneMoveLock = {BT_DIVIDE};
//BTNSW_BOOLEAN bswZoneMoving = {BT_ENTER};
//BTNSW_2VALUES bswZoneQuads = {BT_LEAVE, ZB_OUTER, ZB_INNER};
//BTNSW_2VALUES bswZoneLanePoints = {BT_LEAVE, ZL_START, ZL_END};
//BTNSW_LOOPED bswZoneMode = {BT_PREV, ZM__LOOP_BACK, ZM__LOOP_BACK_TO, BT_NEXT, ZM__LOOP_FWD, ZM__LOOP_FWD_TO};
//BTNSW_LOOPED bswZoneCorner = {BT_NORTH, ZBC__LOOP_BACK, ZBC__LOOP_BACK_TO, BT_SOUTH, ZBC__LOOP_FWD, ZBC__LOOP_FWD_TO};
//BTNSW_NUMBER bswZoneLaneIndex = {BT_NORTH, BT_SOUTH, ZONE_LANE_FIRST, ZONE_LANE_LAST};
//BTNSW_NUMBER bswZoneNumVehicles = {BT_DEC, BT_INC, MIN_TEST_VEHICLES, MAX_TEST_VEHICLES};

void SSPCX::FSM(AStringStream& display, GtaPlayerState& playerState)
{
    m_NPButtons.update();

    m_SwitchMoveLock.update();
    if(m_bMoveLock == FALSE)
    {
        m_SwitchMoving.update();
    }
    else
    {
        display << " (L)";
    }
    if(m_bMoving)
    {
        display << " (!!!)";
    }

    m_SwitchMode.update();
    switch(m_mode)
    {
    case PEM_SETCENTER:
        display << " Center";
        break;
    case PEM_SETQUADS:
        display <<  " Quad";

        m_SwitchQuad.update();
        switch(m_quad)
        {
        case PEQ_INNER:
            display << " Inner";
            break;
        case PEQ_OUTER:
            display << " Outer";
            break;
        }

        m_SwitchCorner.update();
        switch(m_corner)
        {
        case PEC_A:
            display << " A";
            break;
        case PEC_B:
            display << " B";
            break;
        case PEC_HEIGHT:
            display << " H";
            break;
        }
        break;
    case PEM_SETLANES:
        m_SwitchLane.update();
        display <<  " Lane " << m_laneIndex;
        break;
    case PEM_TESTPOS:
        display <<  " Test";
        break;
    }

}

SSPCX sspcx;


void parkEditorStateMachine(AStringStream& display, GtaPlayerState& playerState)
{
    sspcx.FSM(display, playerState);
}



/*
//////// //////// //////// //////// buttons //////// //////// //////// ////////

enum ZBUTTONS {
    BT_WEST,    // num4 left    left
    BT_EAST,    // num6 right   right
    BT_NORTH,   // num8 up      up
    BT_SOUTH,   // num2 down    down
    BT_TOP,     // num7 home
    BT_BOTTOM,  // num1 end
    BT_PREV,    // num9 pgup
    BT_NEXT,    // num3 pgdn
    BT_ENTER,   // num5 clear
    BT_LEAVE,   // num0 ins
    BT_DEC,     // num-
    BT_INC,     // num+
    BT_DIVIDE,      // num/
    BT_MULTIPLY,    // num*
    BT_DECIMAL,     // num.
    //
    NUM_ZBUTTONS,
};
BOOL zButton[NUM_ZBUTTONS] = {0, };

BYTE allKeysNow[256];
BYTE allKeysPrev[256];
inline BOOL isKeyPressed(DWORD vkey, BOOL bJustPressed)
{
    BOOL bPressed;
    // is down
    bPressed = ((allKeysNow[vkey] & 0x80) != 0);
    // and (is not just pressed) or (is just pressed)
    bPressed &= !bJustPressed | ((allKeysPrev[vkey] & 0x80) == 0);
    return bPressed;
}
void readButtons(BOOL bUseArrows)
{
    BOOL bSuccess;
    BOOL bOneStepMove = TRUE;   // one key press = one move step
    BOOL bOneStepSwitch = TRUE; // one key press = one mode change
    bSuccess = ::GetKeyboardState(allKeysNow);
    if(bSuccess != FALSE)
    {
        zButton[BT_WEST] = isKeyPressed(VK_NUMPAD4, bOneStepMove);
        zButton[BT_EAST] = isKeyPressed(VK_NUMPAD6, bOneStepMove);
        zButton[BT_NORTH] = isKeyPressed(VK_NUMPAD8, bOneStepMove);
        zButton[BT_SOUTH] = isKeyPressed(VK_NUMPAD2, bOneStepMove);
        if(bUseArrows != FALSE)
        {
            zButton[BT_WEST] |= isKeyPressed(VK_LEFT, bOneStepMove);
            zButton[BT_EAST] |= isKeyPressed(VK_RIGHT, bOneStepMove);
            zButton[BT_NORTH] |= isKeyPressed(VK_UP, bOneStepMove);
            zButton[BT_SOUTH] |= isKeyPressed(VK_DOWN, bOneStepMove);
        }
        zButton[BT_TOP] = isKeyPressed(VK_NUMPAD7, bOneStepMove);
        zButton[BT_BOTTOM] = isKeyPressed(VK_NUMPAD1, bOneStepMove);
        zButton[BT_PREV] = isKeyPressed(VK_NUMPAD9, bOneStepSwitch);
        zButton[BT_NEXT] = isKeyPressed(VK_NUMPAD3, bOneStepSwitch);
        zButton[BT_ENTER] = isKeyPressed(VK_NUMPAD5, bOneStepSwitch);
        zButton[BT_LEAVE] = isKeyPressed(VK_NUMPAD0, bOneStepSwitch);
        zButton[BT_DEC] = isKeyPressed(VK_SUBTRACT, bOneStepSwitch);
        zButton[BT_INC] = isKeyPressed(VK_ADD, bOneStepSwitch);
        zButton[BT_DIVIDE] = isKeyPressed(VK_DIVIDE, bOneStepSwitch);
        zButton[BT_MULTIPLY] = isKeyPressed(VK_MULTIPLY, bOneStepSwitch);
        zButton[BT_DECIMAL] = isKeyPressed(VK_DECIMAL, bOneStepSwitch);
    }
    memcpy(&allKeysPrev[0], &allKeysNow[0], sizeof(allKeysPrev));
}



//////// //////// //////// //////// zone buttons //////// //////// //////// ////////

struct BTNSW_BOOLEAN {
    DWORD keySwitch;
};

struct BTNSW_2VALUES {
    DWORD keySwitch;
    DWORD modeFirst;
    DWORD modeSecond;
};

struct BTNSW_LOOPED {
    DWORD keyBack;
    DWORD modeLoopBack;
    DWORD modeLoopBackTo;
    DWORD keyForward;
    DWORD modeLoopForward;
    DWORD modeLoopForwardTo;
};

struct BTNSW_NUMBER {
    DWORD keySubtractOne;
    DWORD keyAddOne;
    INT valueFirst;     // min
    INT valueLast;      // max
};



BOOL buttonsHandleBooleanSwitch(BOOL mode, BTNSW_BOOLEAN* pBS)
{
    if(zButton[pBS->keySwitch] != FALSE)
    {
        mode = !mode;
    }
    return mode;
}

DWORD buttonsHandleTwoValuesSwitch(DWORD mode, BTNSW_2VALUES* p2VS)
{
    if(zButton[p2VS->keySwitch] != FALSE)
    {
        if(mode == p2VS->modeFirst)
        {
            mode = p2VS->modeSecond;
        }
        else
        {
            mode = p2VS->modeFirst;
        }
    }
    return mode;
}

DWORD buttonsHandleLoopedSwitch(DWORD mode, BTNSW_LOOPED* pLS)
{
    if(zButton[pLS->keyBack] != FALSE)
    {
        mode--;
        if(mode == pLS->modeLoopBack)
        {
            mode = pLS->modeLoopBackTo;
        }
    }
    if(zButton[pLS->keyForward] != FALSE)
    {
        mode++;
        if(mode == pLS->modeLoopForward)
        {
            mode = pLS->modeLoopForwardTo;
        }
    }
    return mode;
}

INT buttonsHandleNumberSwitch(INT value, BTNSW_NUMBER* pNS)
{
    if(zButton[pNS->keySubtractOne] != FALSE)
    {
        value--;
        if(value < pNS->valueFirst)
        {
            value = pNS->valueLast;
        }
    }
    if(zButton[pNS->keyAddOne] != FALSE)
    {
        value++;
        if(value > pNS->valueLast)
        {
            value = pNS->valueFirst;
        }
    }
    return value;
}



//////// //////// //////// //////// zone def //////// //////// //////// ////////

enum ZONEMODE {
    ZM__LOOP_BACK,
    ZM_SETCENTER,
    ZM_SETQUADS,
    ZM_SETLANES,
    ZM_TESTPOS,
    ZM__LOOP_FWD,
    ZM__LOOP_BACK_TO = ZM_TESTPOS,
    ZM__LOOP_FWD_TO = ZM_SETCENTER,
};
enum ZONEBOX {
    ZB_OUTER,
    ZB_INNER,
};
enum ZONEBOXCORNER {
    ZBC__LOOP_BACK,
    ZBC_A,
    ZBC_B,
    ZBC_HEIGHT,
    ZBC__LOOP_FWD,
    ZBC__LOOP_BACK_TO = ZBC_HEIGHT,
    ZBC__LOOP_FWD_TO = ZBC_A,
};
//       A          
//   +---x---------+
//   |  / `.       |
//   | /    `.  H  |
//   |/       `x   |
//XA x        / `. |
//   |`.     /    `x B
//   |  `.  /     /|
//   |    `x     / |
//   | adj. `.  /  |
//   +--------`x---+
//            XB     
enum ZORIENTEDQUADPOINT {
    ZOQ_A = 0,  // point a
    ZOQ_B,      // point b
    ZOQ_XB,     // b opposite
    ZOQ_XA,     // a opposite
    ZOQ_H,      // helper point
    //
    NUM_ZOQUADPOINTS
};
enum ZONELANEPOINT {
    ZL_START,
    ZL_END,
};

struct ZOrientedQuad {
    FLOAT slope;        // delta y for delta x = +1.0
    FLOAT orthoSlope;   // [orthogonal] delta y for delta x = +1.0
    FLOAT slopeLength;  // x = 1.0 (leg a), y = slope (leg b), slopeLentgh: hypotenuse
    FLOAT width;
    FLOAT height;
    FloatVector3 point[NUM_ZOQUADPOINTS];
};
struct ZONEQUADDEF {
    ZOrientedQuad quad;
    BOOL bIsAxisAligned;
    DWORD alignAxis;
    FloatZOBBox3 bbox;
};

enum ZONELANES {
    ZONE_LANE_FIRST = 0,
    ZONE_LANE_LAST = PSD_MAX_PARKING_LANES - 1,
};
struct ZONELANEDEF {
    FloatAngledVector3 start;
    FloatAngledVector3 end;
    DWORD numSpaces;
};

enum ZONETEST {
    MIN_TEST_VEHICLES = 1,
    MAX_TEST_VEHICLES = PSD_MAX_LANE_SPACES,
};
struct ZONETESTDEF {
    BOOL bSpawning;
    FLOAT fLaneDistance;
    FloatAngledVector3 spawnPos;
    FloatAngledVector3 spawnPosInc;
    DWORD veh[MAX_TEST_VEHICLES];
//    VEHLIC31 vehToSpawn;
//    GTAVEH30 vehToSpawn;
    DWORD vehSpawning;
    DWORD numVehiclesToSpawn;
};
struct ZONEDEF {
    DWORD zoneMode;
    DWORD zoneBox;
    DWORD zoneBoxCorner;
    DWORD zoneLanePoint;
    DWORD zoneLaneIndex;
    ZONETESTDEF test;
    BOOL bIsSet;
    BOOL bIsMoving;
    BOOL bIsMoveLocked;
    BOOL bOuterBBoxIsSmall;
    ZONEQUADDEF outerQuad;
    ZONEQUADDEF innerQuad;
    ZONELANEDEF lane[PSD_MAX_PARKING_LANES];
};

ZONEDEF zoneDefX = {ZM_SETCENTER, ZB_OUTER, ZBC_A, ZL_START, 0, {FALSE, 0.0f, }, FALSE, FALSE, FALSE, FALSE, };


BTNSW_BOOLEAN bswZoneMoveLock = {BT_DIVIDE};
BTNSW_BOOLEAN bswZoneMoving = {BT_ENTER};
BTNSW_2VALUES bswZoneQuads = {BT_LEAVE, ZB_OUTER, ZB_INNER};
BTNSW_2VALUES bswZoneLanePoints = {BT_LEAVE, ZL_START, ZL_END};
BTNSW_LOOPED bswZoneMode = {BT_PREV, ZM__LOOP_BACK, ZM__LOOP_BACK_TO, BT_NEXT, ZM__LOOP_FWD, ZM__LOOP_FWD_TO};
BTNSW_LOOPED bswZoneCorner = {BT_NORTH, ZBC__LOOP_BACK, ZBC__LOOP_BACK_TO, BT_SOUTH, ZBC__LOOP_FWD, ZBC__LOOP_FWD_TO};
BTNSW_NUMBER bswZoneLaneIndex = {BT_NORTH, BT_SOUTH, ZONE_LANE_FIRST, ZONE_LANE_LAST};
BTNSW_NUMBER bswZoneNumVehicles = {BT_DEC, BT_INC, MIN_TEST_VEHICLES, MAX_TEST_VEHICLES};




//////// //////// //////// //////// gta marker //////// //////// //////// ////////

struct ZONEMARKCFG {
    DWORD type;
    FLOAT radius;
    ByteVector3 color;
    DWORD zero;
};

enum ZONEMARKERCOLOR {
    ZMC_YELLOW  = 0x0000FFFF,
    ZMC_AQUA    = 0x00FFFF00,
    ZMC_RED     = 0x001F1FFF,
    ZMC_BLUE    = 0x00FF1F1F,
    ZMC_ORANGE  = 0x001F9FFF,
    ZMC_PURPLE  = 0x00FF1FBF,
    ZMC_LIGHTORANGE  = 0x004FAFFF,
    ZMC_LIGHTPURPLE  = 0x00FF4FCF,
    ZMC_PINK = 0x009F9FFF,
    ZMC_DARKRED     = 0x000000BF,
    ZMC_DARKPURPLE  = 0x007F006F,
    ZMC_DARKORANGE  = 0x00004F8F,
    ZMC_PURERED     = 0x000000FF,
    ZMC_PUREBLUE    = 0x00FF0000,
};

enum ZONEMARKTYPE {     // THESE ARE ARRAY INDICES! DON'T MESS! //
    ZMT_START = 0,
    ZMT_END,
    ZMT_BBOX_OUTER,
    ZMT_CORNER_OUTER_0,
    ZMT_CORNER_OUTER_1,
    ZMT_CORNER_OUTER_2,
    ZMT_CORNER_OUTER_3,
    ZMT_CENTER_OUTER,
    ZMT_BBOX_INNER,
    ZMT_CORNER_INNER_0,
    ZMT_CORNER_INNER_1,
    ZMT_CORNER_INNER_2,
    ZMT_CORNER_INNER_3,
    ZMT_CENTER_INNER,
    //
    NUM_ZONEMARKTYPES,
};
ZONEMARKCFG zoneMarkerCfg[NUM_ZONEMARKTYPES] = {
    {G3CT_CIRCLE,   0.5f, {ZMC_YELLOW}}, // PMT_START
    {G3CT_CIRCLE,   0.5f, {ZMC_AQUA}}, // PMT_END
    {G3CT_HEX,      0.375f, {ZMC_PURERED}}, // PMT_BBOX_OUTER
    {G3CT_HEX,      0.25f, {ZMC_LIGHTORANGE}}, // PMT_CORNER_OUTER_0
    {G3CT_CIRCLE,   0.25f, {ZMC_LIGHTORANGE}}, // PMT_CORNER_OUTER_1
    {G3CT_CIRCLE,   0.25f, {ZMC_DARKORANGE}}, // PMT_CORNER_OUTER_2
    {G3CT_RING,     0.25f, {ZMC_DARKORANGE}}, // PMT_CORNER_OUTER_3
    {G3CT_HEX,      0.625f, {ZMC_RED}}, // PMT_CENTER_OUTER
    {G3CT_HEX,      0.375f, {ZMC_PUREBLUE}}, // PMT_BBOX_INNER
    {G3CT_HEX,      0.25f, {ZMC_LIGHTPURPLE}}, // PMT_CORNER_INNER_0
    {G3CT_CIRCLE,   0.25f, {ZMC_LIGHTPURPLE}}, // PMT_CORNER_INNER_1
    {G3CT_CIRCLE,   0.25f, {ZMC_DARKPURPLE}}, // PMT_CORNER_INNER_2
    {G3CT_RING,     0.25f, {ZMC_DARKPURPLE}}, // PMT_CORNER_INNER_3
    {G3CT_RING,     0.5f, {ZMC_BLUE}}, // PMT_CENTER_INNER
};

DWORD zoneDrawMarker(DWORD markerId, FloatVector3* pPos, DWORD zoneMarkerType)
{
    ZONEMARKCFG* zmc;
    zmc = &zoneMarkerCfg[zoneMarkerType];
    SN::MARKER::DRAW_CORONA(markerId, *pPos, zmc->radius, zmc->type, FALSE, zmc->color);
    return 1;   // number of markers drawn
}

void GtaGetGroundZ(FloatVector3* pPos)
{
    pPos->Z = 100.0f;
    gtaWorldGetGroundZ(pPos);
    if(pPos->Z < 0.0f)
    {
        pPos->Z = 0.0f;
    }
}

DWORD zoneDrawBBox(DWORD markerIdBase, FloatZOBBox3* pBox, DWORD zoneMarkType)
{
    FloatVector3 pos;
    pos.X = pBox->min.X;
    pos.Y = pBox->min.Y;
    GtaGetGroundZ(&pos);
    zoneDrawMarker(markerIdBase + 0, &pos, zoneMarkType);   // min min ground(min)
    pos.Y = pBox->max.Y;
    GtaGetGroundZ(&pos);
    zoneDrawMarker(markerIdBase + 1, &pos, zoneMarkType);   // min MAX ground(min)
    pos.X = pBox->max.X;
    GtaGetGroundZ(&pos);
    zoneDrawMarker(markerIdBase + 2, &pos, zoneMarkType);   // MAX MAX ground(min)
    pos.Y = pBox->min.Y;
    GtaGetGroundZ(&pos);
    zoneDrawMarker(markerIdBase + 3, &pos, zoneMarkType);   // MAX min ground(min)
    return 4;   // number of markers drawn
}

DWORD zoneDrawQuad(DWORD markerIdBase, ZONEQUADDEF* pQuadDef, DWORD zoneMarkTypeBase)
{
    DWORD markerStart;
    markerStart = markerIdBase;
    markerIdBase += zoneDrawBBox(markerIdBase, &pQuadDef->bbox, zoneMarkTypeBase);
    zoneMarkTypeBase++;
    GtaGetGroundZ(&pQuadDef->quad.point[ZOQ_A]);
    GtaGetGroundZ(&pQuadDef->quad.point[ZOQ_B]);
    GtaGetGroundZ(&pQuadDef->quad.point[ZOQ_XA]);
    GtaGetGroundZ(&pQuadDef->quad.point[ZOQ_XB]);
    zoneDrawMarker(markerIdBase + 0, &pQuadDef->quad.point[ZOQ_A], zoneMarkTypeBase + 0);
    zoneDrawMarker(markerIdBase + 1, &pQuadDef->quad.point[ZOQ_B], zoneMarkTypeBase + 1);
    zoneDrawMarker(markerIdBase + 2, &pQuadDef->quad.point[ZOQ_XB], zoneMarkTypeBase + 2);
    zoneDrawMarker(markerIdBase + 3, &pQuadDef->quad.point[ZOQ_XA], zoneMarkTypeBase + 3);
    markerIdBase += 4;
    return (markerIdBase - markerStart);    // number of markers drawn
}

DWORD zoneDraw(ZONEDEF* pZoneDef)
{
    DWORD markerId;
    markerId = 0;
    // draw outer/inner quads
    markerId += zoneDrawQuad(markerId, &pZoneDef->outerQuad, ZMT_BBOX_OUTER);
    markerId += zoneDrawQuad(markerId, &pZoneDef->innerQuad, ZMT_BBOX_INNER);
    // draw start/end markers
    markerId += zoneDrawMarker(markerId, &pZoneDef->lane[pZoneDef->zoneLaneIndex].start.pos, ZMT_START);
    markerId += zoneDrawMarker(markerId, &pZoneDef->lane[pZoneDef->zoneLaneIndex].end.pos, ZMT_END);
    return markerId;    // number of markers drawn
}



//////// //////// //////// //////// zone //////// //////// //////// ////////


void zoneHandleMovingPos(FloatVector3* pPointPos, FLOAT* pPointAngle, FloatVector3* pSrcPos, FLOAT fSrcAngle)
{
    pPointPos->X = fCrop(pSrcPos->X, 4);
    pPointPos->Y = fCrop(pSrcPos->Y, 4);
    pPointPos->Z = fCrop(pSrcPos->Z, 4);
    *pPointAngle = fAlign(fSrcAngle, 7.5);
}



BOOL IsOuterZOBBoxTooSmallForInnerZOBBox(FloatZOBBox3* pBBoxOuter, FloatZOBBox3* pBBoxInner, FLOAT minDistance)
{
    BOOL bSmall = FALSE;
    // only x & y, because z is adjusted later while adding data to i.p.d.
    bSmall |= (pBBoxOuter->min.X > (pBBoxInner->min.X - minDistance));
    bSmall |= (pBBoxOuter->min.Y > (pBBoxInner->min.Y - minDistance));
    bSmall |= (pBBoxOuter->max.X < (pBBoxInner->max.X + minDistance));
    bSmall |= (pBBoxOuter->max.Y < (pBBoxInner->max.Y + minDistance));
    return bSmall;
}

void UpdateQuadBBox(ZONEQUADDEF* pQuadDef)
{
    DWORD iAxis;
    DWORD iOrthoAxis;
    DWORD OrthogonalAxis[2] = {AXIS_Y, AXIS_X};
    DWORD iPoint;
    FLOAT fMin;
    FLOAT fMax;
    FLOAT fPos;
    FLOAT fOffset;
    BOOL bAxisAligned;
    bAxisAligned = pQuadDef->bIsAxisAligned;
    // only x & y, because z is adjusted later while adding data to i.p.d.
    for(iAxis = AXIS_X; iAxis <= AXIS_Y; iAxis++)
    {
        iOrthoAxis = OrthogonalAxis[iAxis];
        fMin = pQuadDef->quad.point[ZOQ_A].f[iAxis];
        fOffset = pQuadDef->quad.point[ZOQ_A].f[iOrthoAxis];
        fMax = fMin;
        for(iPoint = ZOQ_B; iPoint <= ZOQ_XA; iPoint++)
        {
            fPos = pQuadDef->quad.point[iPoint].f[iAxis];
            if(fMin > fPos)
            {
                fMin = fPos;
            }
            if(fMax < fPos)
            {
                fMax = fPos;
                if(bAxisAligned == FALSE)
                {
                    // store other axis position at max.pos. (offset.x = cornerPointWithMaxY.x)
                    fOffset = pQuadDef->quad.point[iPoint].f[iOrthoAxis];
                }
            }
        }
        pQuadDef->bbox.min.f[iAxis] = fMin;
        pQuadDef->bbox.max.f[iAxis] = fMax;
        if(bAxisAligned == FALSE)
        {
            pQuadDef->bbox.offset.f[iOrthoAxis] = fOffset;
        }
    }
//   --->A          
//   +---x---------+ |
//   |  / `.       | |
//   | /    `.  H  | |
//   |/       `x   | |
//XA x        / `. | V
//   |`.     /    `x B
//   |  `.  /     /|
//   |    `x     / |
//   | adj. `.  /  |
//   +--------`x---+
//            XB     
    if(bAxisAligned == FALSE)
    {
        // convert pos. to offset
        pQuadDef->bbox.offset.f[AXIS_X] = pQuadDef->bbox.offset.f[AXIS_X] - pQuadDef->bbox.min.f[AXIS_X];    // cornerPointWithMaxY.x
        pQuadDef->bbox.offset.f[AXIS_Y] = pQuadDef->bbox.max.f[AXIS_Y] - pQuadDef->bbox.offset.f[AXIS_Y];    // cornerPointWithMaxX.y
    }
    else
    {
        // both offsets are zero
        pQuadDef->bbox.offset.f[AXIS_X] = 0.0f;
        pQuadDef->bbox.offset.f[AXIS_Y] = 0.0f;
    }
}

void zoneInitQuad(ZONEQUADDEF* pQuadDef, FloatVector3* pPos, FLOAT margin)
{
    pQuadDef->bbox.min.X = pPos->X - margin;
    pQuadDef->bbox.min.Y = pPos->Y - margin;
    pQuadDef->bbox.max.X = pPos->X + margin;
    pQuadDef->bbox.max.Y = pPos->Y + margin;
    pQuadDef->bIsAxisAligned = TRUE;
    //  A ---H-- B
    //  |        |
    //  |    .   |
    //  |        |
    // XA ------ XB
    FLOAT z;
    z = pPos->Z;
    pQuadDef->quad.point[ZOQ_A].X = pQuadDef->bbox.min.X;
    pQuadDef->quad.point[ZOQ_A].Y = pQuadDef->bbox.max.Y;
    pQuadDef->quad.point[ZOQ_A].Z = z;
    pQuadDef->quad.point[ZOQ_B].X = pQuadDef->bbox.max.X;
    pQuadDef->quad.point[ZOQ_B].Y = pQuadDef->bbox.max.Y;
    pQuadDef->quad.point[ZOQ_B].Z = z;
    pQuadDef->quad.point[ZOQ_XA].X = pQuadDef->bbox.min.X;
    pQuadDef->quad.point[ZOQ_XA].Y = pQuadDef->bbox.min.Y;
    pQuadDef->quad.point[ZOQ_XA].Z = z;
    pQuadDef->quad.point[ZOQ_XB].X = pQuadDef->bbox.max.X;
    pQuadDef->quad.point[ZOQ_XB].Y = pQuadDef->bbox.min.Y;
    pQuadDef->quad.point[ZOQ_XB].Z = z;
    pQuadDef->quad.point[ZOQ_H].X = pPos->X;
    pQuadDef->quad.point[ZOQ_H].Y = pQuadDef->bbox.max.Y;
    pQuadDef->quad.point[ZOQ_H].Z = z;
}

void zoneInitAtPos(ZONEDEF* pd, FloatVector3* pPos)
{
    FLOAT fMargin = 8.0f;
    FLOAT fMarginInc = 4.0f;
    FLOAT fLaneMargin = 2.0f;
    FloatVector3 pos;
    DWORD numDigits = 3;
    pos.X = fCrop(pPos->X, numDigits);
    pos.Y = fCrop(pPos->Y, numDigits);
    pos.Z = pPos->Z;
    gtaWorldGetGroundZ(&pos);
    zoneInitQuad(&pd->innerQuad, &pos, fMargin);
    fMargin += fMarginInc;
    zoneInitQuad(&pd->outerQuad, &pos, fMargin);
    DWORD iLane;
    for(iLane = 0; iLane < PSD_MAX_PARKING_LANES; iLane++)
    {
        pd->lane[iLane].start.pos.X = pos.X + fLaneMargin;
        pd->lane[iLane].start.pos.Y = pos.Y;
        pd->lane[iLane].start.pos.Z = pos.Z;
        pd->lane[iLane].start.angle = 0.0f;
        pd->lane[iLane].end.pos.X = pos.X - fLaneMargin;
        pd->lane[iLane].end.pos.Y = pos.Y;
        pd->lane[iLane].end.pos.Z = pos.Z;
        pd->lane[iLane].end.angle = 0.0f;
        pd->lane[iLane].numSpaces = 2;
    }
}

void zoneCornerMeshAlignXPoints(ZONEQUADDEF* pQuadDef)
{
    // align to .125 grid
    pQuadDef->quad.point[ZOQ_XB].X = fCrop(pQuadDef->quad.point[ZOQ_XB].X, 3);
    pQuadDef->quad.point[ZOQ_XB].Y = fCrop(pQuadDef->quad.point[ZOQ_XB].Y, 3);
    pQuadDef->quad.point[ZOQ_XA].X = fCrop(pQuadDef->quad.point[ZOQ_XA].X, 3);
    pQuadDef->quad.point[ZOQ_XA].Y = fCrop(pQuadDef->quad.point[ZOQ_XA].Y, 3);
}

void zoneCornerUpdateXPoints(ZONEQUADDEF* pQuadDef)
{
    FLOAT dist;
    FLOAT orthoSlope;
    FloatVector3 point;
    orthoSlope = pQuadDef->quad.orthoSlope;
    dist = pQuadDef->quad.height;
    if(pQuadDef->quad.point[ZOQ_XA].X > pQuadDef->quad.point[ZOQ_XB].X)
    {
        dist = -dist;
    }
    dist /= pQuadDef->quad.slopeLength;  // width is proportional to diagonal
    point.X = dist / orthoSlope;                            // normalize width
    point.Y = point.X * orthoSlope;                         // calculate height
    pQuadDef->quad.point[ZOQ_XB].X = point.X + pQuadDef->quad.point[ZOQ_B].X;
    pQuadDef->quad.point[ZOQ_XB].Y = point.Y + pQuadDef->quad.point[ZOQ_B].Y;
    pQuadDef->quad.point[ZOQ_XA].X = point.X + pQuadDef->quad.point[ZOQ_A].X;
    pQuadDef->quad.point[ZOQ_XA].Y = point.Y + pQuadDef->quad.point[ZOQ_A].Y;
    zoneCornerMeshAlignXPoints(pQuadDef);
}

void zoneCornerAxisAlignXPoints(ZONEQUADDEF* pQuadDef, FLOAT height)
{
    DWORD alignAxis;
    DWORD orthoAxis;
    alignAxis = pQuadDef->alignAxis;
    switch(alignAxis)
    {
    case AXIS_X:
        orthoAxis = AXIS_Y;
        break;
    case AXIS_Y:
        orthoAxis = AXIS_X;
        break;
    }
    pQuadDef->quad.point[ZOQ_XB].f[orthoAxis] = pQuadDef->quad.point[ZOQ_B].f[orthoAxis];
    pQuadDef->quad.point[ZOQ_XB].f[alignAxis] = pQuadDef->quad.point[ZOQ_B].f[alignAxis] + height;
    pQuadDef->quad.point[ZOQ_XA].f[orthoAxis] = pQuadDef->quad.point[ZOQ_A].f[orthoAxis];
    pQuadDef->quad.point[ZOQ_XA].f[alignAxis] = pQuadDef->quad.point[ZOQ_A].f[alignAxis] + height;
    zoneCornerMeshAlignXPoints(pQuadDef);
}

void zoneCornerAxisAlignPoints(ZONEQUADDEF* pQuadDef, DWORD quadCornerToAlign)
{
    FloatVector3* pPointOrigin;
    FloatVector3* pPointAlign;
    DWORD alignAxis;
    DWORD orthoAxis;
    FLOAT height;
    BOOL bInvert = FALSE;
    // align a & b
    switch(quadCornerToAlign)
    {
    case ZOQ_A:
        pPointOrigin = &pQuadDef->quad.point[ZOQ_B];
        pPointAlign = &pQuadDef->quad.point[ZOQ_A];
        break;
    case ZOQ_B:
        pPointOrigin = &pQuadDef->quad.point[ZOQ_A];
        pPointAlign = &pQuadDef->quad.point[ZOQ_B];
        break;
    }
    alignAxis = pQuadDef->alignAxis;
    pPointAlign->f[alignAxis] = pPointOrigin->f[alignAxis];
    // align xa & xb
    switch(alignAxis)
    {
    case AXIS_X:
        orthoAxis = AXIS_Y;
        bInvert = (pQuadDef->quad.point[ZOQ_A].f[orthoAxis] < pQuadDef->quad.point[ZOQ_B].f[orthoAxis]);
        break;
    case AXIS_Y:
        orthoAxis = AXIS_X;
        bInvert = (pQuadDef->quad.point[ZOQ_A].f[orthoAxis] > pQuadDef->quad.point[ZOQ_B].f[orthoAxis]);
        break;
    }
    height = pQuadDef->quad.height;
    if(bInvert != FALSE)
    {
        height = -height;
    }
    zoneCornerAxisAlignXPoints(pQuadDef, height);
}

void zoneCornerSetPoint(ZONEQUADDEF* pQuadDef, DWORD quadCorner, FloatVector3* pPos)
{
    FloatVector3* pPoint;
    pPoint = &pQuadDef->quad.point[quadCorner];
    pPoint->X = fCrop(pPos->X, 3);
    pPoint->Y = fCrop(pPos->Y, 3);
    pPoint->Z = fCrop(pPos->Z, 3);
    gtaWorldGetGroundZ(pPoint);

    BOOL bAlignedX;
    BOOL bAlignedY;
    BOOL bIsAligned;
    FLOAT fNearBoundary = 1.0f;
    bAlignedX = (fAbs(pQuadDef->quad.point[ZOQ_B].X - pQuadDef->quad.point[ZOQ_A].X) < fNearBoundary);
    bAlignedY = (fAbs(pQuadDef->quad.point[ZOQ_B].Y - pQuadDef->quad.point[ZOQ_A].Y) < fNearBoundary);
    bIsAligned = bAlignedX | bAlignedY;
    pQuadDef->bIsAxisAligned = bIsAligned;

    FLOAT slope;
    FLOAT orthoSlope;
    FLOAT slopeLength;
    if(bIsAligned != FALSE)
    {
        slope = 0.0f;
        orthoSlope = 0.0f;
        slopeLength = 0.0f;
    }
    else
    {
        slope = (pQuadDef->quad.point[ZOQ_B].Y - pQuadDef->quad.point[ZOQ_A].Y) / (pQuadDef->quad.point[ZOQ_B].X - pQuadDef->quad.point[ZOQ_A].X);
        orthoSlope = -1.0f / slope;
        slopeLength = rightTriangleHypotenuse(1.0f, slope);
    }
    pQuadDef->quad.slope = slope;
    pQuadDef->quad.orthoSlope = orthoSlope;
    pQuadDef->quad.slopeLength = slopeLength;

    FLOAT dist;
    dist = distance2d(pQuadDef->quad.point[ZOQ_A], pQuadDef->quad.point[ZOQ_B]);
    pQuadDef->quad.width = dist;

    if(bIsAligned == FALSE)
    {
        zoneCornerUpdateXPoints(pQuadDef);
    }
    else
    {
        if(bAlignedX != FALSE)
        {
            pQuadDef->alignAxis = AXIS_X;
        }
        else if(bAlignedY != FALSE)
        {
            pQuadDef->alignAxis = AXIS_Y;
        }
        zoneCornerAxisAlignPoints(pQuadDef, quadCorner);
    }
    UpdateQuadBBox(pQuadDef);
}

void zoneCornerSetHeight(ZONEQUADDEF* pQuadDef, FloatVector3* pPos)
{
    FLOAT slope;
    FLOAT orthoSlope;
    FloatVector3 point;
    FLOAT dist;
    //    K = slope                        -1/K = orthoSlope
    //  ay0 = pQuadDef->quad.pointA.Y       ax0 = pQuadDef->quad.pointA.X
    //  ay1 = pQuadDef->quad.pointB.Y       ax1 = pQuadDef->quad.pointB.X
    //  by0 = pPos->Y                       bx0 = pPos->X
    //   yN = point.Y                        xN = point.X
    if(pQuadDef->bIsAxisAligned == FALSE)
    {
        //K = (ay1-ay0)/(ax1-ax0)
        slope = pQuadDef->quad.slope;
        //-1/K = -1 / (ay1-ay0)/(ax1-ax0)
        orthoSlope = pQuadDef->quad.orthoSlope;
        //xN = (bx0*(-1/K) - ax0*(K) + ay0 - by0) / ((-1/K) - (K))
        point.X = ((pPos->X * orthoSlope) - (pQuadDef->quad.point[ZOQ_A].X * slope) + pQuadDef->quad.point[ZOQ_A].Y - pPos->Y) / (orthoSlope - slope);
        //yN = (xN-ax0)*(K) + ay0
        point.Y = ((point.X - pQuadDef->quad.point[ZOQ_A].X) * slope) + pQuadDef->quad.point[ZOQ_A].Y;
        point.Z = pPos->Z;
        gtaWorldGetGroundPos(&point, &pQuadDef->quad.point[ZOQ_H]);

        dist = distance2d(point, *pPos);
        BOOL bInvert;
        bInvert = (slope < 0.0f) & (point.X > pPos->X);
        bInvert |= (orthoSlope < 0.0f) & (point.X < pPos->X);
        if(pQuadDef->quad.point[ZOQ_A].X > pQuadDef->quad.point[ZOQ_B].X)
        {
            bInvert = !bInvert;
        }
        if(bInvert != FALSE)
        {
            dist = -dist;
        }
        pQuadDef->quad.height = dist;
        zoneCornerUpdateXPoints(pQuadDef);
    }
    else
    {
        DWORD alignAxis;
        alignAxis = pQuadDef->alignAxis;
        dist = pPos->f[alignAxis] - pQuadDef->quad.point[ZOQ_A].f[alignAxis];
        pQuadDef->quad.height = dist;
        zoneCornerAxisAlignXPoints(pQuadDef, dist);
    }
    UpdateQuadBBox(pQuadDef);
}



void zoneInitQuadWithZOBBox(ZONEQUADDEF* pQuadDef, FloatZOBBox3* pZOBB)
{
    BOOL bAxisAligned;
    FLOAT fNearBoundary = 1.0f;

    // is zobb axis-aligned?
    bAxisAligned = FALSE;
    if((pZOBB->offset.X <= fNearBoundary) || (pZOBB->offset.Y <= fNearBoundary))
    {
        bAxisAligned = TRUE;
    }

    if(bAxisAligned != FALSE)
    {
        pQuadDef->bbox.offset.X = 0.0f;
        pQuadDef->bbox.offset.Y = 0.0f;
    }
    else
    {
        pQuadDef->bbox.offset.X = fCrop(pZOBB->offset.X, 4);
        pQuadDef->bbox.offset.Y = fCrop(pZOBB->offset.Y, 4);
    }
    pQuadDef->bbox.min.X = fCrop(pZOBB->min.X, 4);
    pQuadDef->bbox.min.Y = fCrop(pZOBB->min.Y, 4);
    pQuadDef->bbox.min.Z = fCrop(pZOBB->min.Z, 4);
    pQuadDef->bbox.max.X = fCrop(pZOBB->max.X, 4);
    pQuadDef->bbox.max.Y = fCrop(pZOBB->max.Y, 4);
    pQuadDef->bbox.max.Z = fCrop(pZOBB->max.Z, 4);
    pQuadDef->bIsAxisAligned = bAxisAligned;

//   --->A          
//   +---x---------+ |
//   |  / `.       | |
//   | /    `.  H  | |
//   |/       `x   | |
//XA x        / `. | V
// ^ |`.     /    `x B
// | |  `.  /     /|
// | |    `x     / |
// | | adj. `.  /  |
// | +--------`x---+
//            XB<---
    // x offset (added to min.x) for corner with max.y
    // y offset (added to min.y) for corner with min.x
    // for other two corners same offset subtracted from max.coord
    pQuadDef->quad.point[ZOQ_A].X = fCrop(pZOBB->min.X, 4) + pQuadDef->bbox.offset.X;
    pQuadDef->quad.point[ZOQ_A].Y = fCrop(pZOBB->max.Y, 4);

    pQuadDef->quad.point[ZOQ_B].X = fCrop(pZOBB->max.X, 4);
    pQuadDef->quad.point[ZOQ_B].Y = fCrop(pZOBB->max.Y, 4) - pQuadDef->bbox.offset.Y;

    pQuadDef->quad.point[ZOQ_XA].X = fCrop(pZOBB->min.X, 4);
    pQuadDef->quad.point[ZOQ_XA].Y = fCrop(pZOBB->min.Y, 4) + pQuadDef->bbox.offset.Y;

    pQuadDef->quad.point[ZOQ_XB].X = fCrop(pZOBB->max.X, 4) - pQuadDef->bbox.offset.X;
    pQuadDef->quad.point[ZOQ_XB].Y = fCrop(pZOBB->min.Y, 4);

    pQuadDef->quad.point[ZOQ_H].X = (pQuadDef->quad.point[ZOQ_A].X + pQuadDef->quad.point[ZOQ_B].X) / 2.0f;
    pQuadDef->quad.point[ZOQ_H].Y = (pQuadDef->quad.point[ZOQ_A].Y + pQuadDef->quad.point[ZOQ_B].Y) / 2.0f;

    FLOAT z = fCrop(pZOBB->min.Z, 4);
    pQuadDef->quad.point[ZOQ_A].Z = z;
    pQuadDef->quad.point[ZOQ_B].Z = z;
    pQuadDef->quad.point[ZOQ_XA].Z = z;
    pQuadDef->quad.point[ZOQ_XB].Z = z;
    pQuadDef->quad.point[ZOQ_H].Z = z;

    // calc all slopes etc.
    FloatVector3 fvPos[3];
    fvPos[0] = pQuadDef->quad.point[ZOQ_A];
    fvPos[1] = pQuadDef->quad.point[ZOQ_B];
    fvPos[2] = pQuadDef->quad.point[ZOQ_XB];
    zoneCornerSetPoint(pQuadDef, ZOQ_A, &fvPos[0]);
    zoneCornerSetPoint(pQuadDef, ZOQ_B, &fvPos[1]);
    zoneCornerSetHeight(pQuadDef, &fvPos[2]);
}

void zoneInitWithActiveParking(ZONEDEF* pDef)
{
    DWORD iParkActive;
    DWORD iLane;
    DWORD numLanes;
    FLOAT numSteps;
    PARKLOTHDR* pPLH;
    FloatVector4* pfvPosSlots;
    FloatVector4* pfvPtr;
    ZONELANEDEF* pLane;
    if(parkSysGetActiveParking(&iParkActive) != FALSE)
    {
        pPLH = parkSysGetParkHdr(iParkActive, &pfvPosSlots);
        zoneInitQuadWithZOBBox(&pDef->innerQuad, &pPLH->bbEnter);
        zoneInitQuadWithZOBBox(&pDef->outerQuad, &pPLH->bbLeave);
        numLanes = pPLH->numLanes;
        pfvPtr = pfvPosSlots;
        for(iLane = 0; iLane < numLanes; iLane++)
        {
            pLane = &pDef->lane[iLane];
            pLane->start.pos.X = pfvPtr->X;
            pLane->start.pos.Y = pfvPtr->Y;
            pLane->start.pos.Z = pfvPtr->Z;
            pLane->start.angle = pfvPtr->A;
            pfvPtr++;
            pLane->end.pos.X = pfvPtr->X;
            pLane->end.pos.Y = pfvPtr->Y;
            pLane->end.pos.Z = pfvPtr->Z;
            pLane->end.angle = pfvPtr->A;
            pfvPtr++;
            pLane->numSpaces = (pPLH->laneInfo[iLane].laneType & PSD_LANE_NUMSPACES_MASK);
            numSteps = (FLOAT)(pLane->numSpaces - 1);
            pLane->end.pos.X = pLane->start.pos.X + (pLane->end.pos.X * numSteps);
            pLane->end.pos.Y = pLane->start.pos.Y + (pLane->end.pos.Y * numSteps);
            pLane->end.pos.Z = pLane->start.pos.Z + (pLane->end.pos.Z * numSteps);
            pLane->end.angle = pLane->start.angle + (pLane->end.angle * numSteps);
        }
    }
}



//DWORD zoneStateMachine(ACHAR* pszDisp, FloatVector3* pPos, BOOL bInCar, DWORD car, FLOAT fCarHeading)
void zoneStateMachine(AStringStream& display, GtaPlayerState& playerState)
{
    FloatVector3* pPos = &playerState.fvPosChar;
    BOOL bInCar = playerState.bIsInCar;
    DWORD car = playerState.vehiclePlayer;
    FLOAT fCarHeading = playerState.fHeadingVehicle;
    FLOAT fVehicleInterval = 2.75f;//3.5; //2.625f;
    FLOAT fOuterBBoxPadding = 5.0f;
    DWORD numVehiclesSuggested;
    FLOAT fGroundZ;
    FloatVector3 groundPos;
    ZONEDEF* pd;
    ZONELANEDEF* pLane;
    pd = &zoneDefX;

    readButtons(FALSE);

    pd->test.fLaneDistance = distance3d(pd->lane[pd->zoneLaneIndex].start.pos, pd->lane[pd->zoneLaneIndex].end.pos);
    numVehiclesSuggested = uiFloor(pd->test.fLaneDistance / fVehicleInterval);

    if(pd->bIsSet != FALSE)
    {
        zoneDraw(pd);
        pd->zoneMode = buttonsHandleLoopedSwitch(pd->zoneMode, &bswZoneMode);
    }

    pd->bIsMoveLocked = buttonsHandleBooleanSwitch(pd->bIsMoveLocked, &bswZoneMoveLock);
    if(pd->bIsMoveLocked == FALSE)
    {
        if(zButton[BT_MULTIPLY] != FALSE)
        {
            zoneInitWithActiveParking(pd);
        }
    }

    switch(pd->zoneMode)
    {
    case ZM_SETQUADS:
    case ZM_SETLANES:
        if(pd->bIsMoveLocked == FALSE)
        {
            pd->bIsMoving = buttonsHandleBooleanSwitch(pd->bIsMoving, &bswZoneMoving);
        }
        break;
    }

    if(pd->bIsMoveLocked != FALSE)
    {
        display << " (L)";
    }
    if(pd->bIsMoving != FALSE)
    {
        display << " (!!!)";
    }

    switch(pd->zoneMode)
    {
    case ZM_SETCENTER:
        display << " Center";
        if(pd->bIsMoveLocked == FALSE)
        {
            if(zButton[BT_ENTER] != FALSE)
            {
                zoneInitAtPos(pd, pPos);
                pd->bIsSet = TRUE;
            }
        }
        SN::WORLD::GET_GROUND_Z_FOR_3D_COORD(*pPos, &fGroundZ);
        display << " pos(" << pPos->X << " " << pPos->Y << " " << pPos->Z << ")(" << fGroundZ << ")(" << fCarHeading << ")";
        break;

    case ZM_SETQUADS:
        display <<  " Quad";
        ZONEQUADDEF* pQuadDef;
        pd->zoneBox = buttonsHandleTwoValuesSwitch(pd->zoneBox, &bswZoneQuads);
        pd->zoneBoxCorner = buttonsHandleLoopedSwitch(pd->zoneBoxCorner, &bswZoneCorner);

        switch(pd->zoneBox)
        {
        case ZB_OUTER:
            pQuadDef = &pd->outerQuad;
            display << " outer";
            break;
        case ZB_INNER:
            pQuadDef = &pd->innerQuad;
            display << " inner";
            break;
        }

        switch(pd->zoneBoxCorner)
        {
        case ZBC_A:
            display << " A ";
            if(pd->bIsMoving != FALSE)
            {
                zoneCornerSetPoint(pQuadDef, ZOQ_A, pPos);
            }
            break;
        case ZBC_B:
            display << " B ";
            if(pd->bIsMoving != FALSE)
            {
                zoneCornerSetPoint(pQuadDef, ZOQ_B, pPos);
            }
            break;
        case ZBC_HEIGHT:
            display << " H ";
            if(pd->bIsMoving != FALSE)
            {
                zoneCornerSetHeight(pQuadDef, pPos);
            }
            break;
        }
        display << " S(" << pQuadDef->quad.slope << ") W(" << pQuadDef->quad.width << ") H(" << pQuadDef->quad.height << ")";

        if(pd->bIsMoving != FALSE)
        {
            pd->bOuterBBoxIsSmall = IsOuterZOBBoxTooSmallForInnerZOBBox(&pd->outerQuad.bbox, &pd->innerQuad.bbox, fOuterBBoxPadding);
        }
        if(pQuadDef->bIsAxisAligned != FALSE)
        {
            display << " ALIGNED";
        }
        if(pd->bOuterBBoxIsSmall != FALSE)
        {
            display << " SMALL";
        }

        BOOL bAABB;
        BOOL bZOBB;
        if(pd->bIsMoving == FALSE)
        {
            pQuadDef->bbox.min.Z = 0.0f;
            pQuadDef->bbox.max.Z = 100.0f;
            bAABB = aabbInside3d(*pPos, pQuadDef->bbox.min, pQuadDef->bbox.max);
            bZOBB = zobbInside3d(*pPos, pQuadDef->bbox);
            if(bAABB != FALSE)
            {
                display << " AA";
            }
            if(bZOBB != FALSE)
            {
                display << " ZO";
            }
        }

        //SN::WORLD::GET_GROUND_Z_FOR_3D_COORD(*pPos, &fGroundZ);
        //display << " pos(" << pPos->X << " " << pPos->Y << " " << pPos->Z << ")(" << fGroundZ << ")(" << fCarHeading << ")";
        break;

    case ZM_SETLANES:
        FloatAngledVector3* pld;
        pd->zoneLanePoint = buttonsHandleTwoValuesSwitch(pd->zoneLanePoint, &bswZoneLanePoints);
        pd->zoneLaneIndex = buttonsHandleNumberSwitch(pd->zoneLaneIndex, &bswZoneLaneIndex);
        pLane = &pd->lane[pd->zoneLaneIndex];
        display << " L" << pd->zoneLaneIndex;

        switch(pd->zoneLanePoint)
        {
        case ZL_START:
            display << " Start";
            pld = &pLane->start;
            break;
        case ZL_END:
            display << " End";
            pld = &pLane->end;
            break;
        }

        if(bInCar != FALSE)
        {
            if(pd->bIsMoving != FALSE)
            {
                gtaWorldGetGroundPos(pPos, &groundPos);
                zoneHandleMovingPos(&pld->pos, &pld->angle, &groundPos, fCarHeading);
            }
        }

        display << "(" << pld->pos.X << "," << pld->pos.Y << "," << pld->pos.Z << " " << pld->angle;
        display << "(" << (pLane->end.pos.X - pLane->start.pos.X) 
            << "," << (pLane->end.pos.Y - pLane->start.pos.Y) 
            << "," << (pLane->end.pos.Z - pLane->start.pos.Z) 
            << " " << (pLane->end.angle - pLane->start.angle);
        display << " (d " << pd->test.fLaneDistance << ", n " << numVehiclesSuggested << ")";
        break;

    case ZM_TESTPOS:
        display << " L" << pd->zoneLaneIndex << " Test";
        pd->zoneLaneIndex = buttonsHandleNumberSwitch(pd->zoneLaneIndex, &bswZoneLaneIndex);
        pLane = &pd->lane[pd->zoneLaneIndex];
        pLane->numSpaces = buttonsHandleNumberSwitch(pLane->numSpaces, &bswZoneNumVehicles);
        pd->test.numVehiclesToSpawn = pLane->numSpaces;
        FLOAT fSteps;
        FLOAT fInterval;
        fSteps = (FLOAT)(pd->test.numVehiclesToSpawn - 1);
        fInterval = pd->test.fLaneDistance / fSteps;
        SN::WORLD::GET_GROUND_Z_FOR_3D_COORD(*pPos, &fGroundZ);
        display << "(n " << pd->test.numVehiclesToSpawn << ", i " << fInterval << ", dist " << pd->test.fLaneDistance << ", num " << numVehiclesSuggested;
        display << " pos(" << pPos->X << " " << pPos->Y << " " << pPos->Z << ")(" << fGroundZ << ")(" << fCarHeading << ")";

        if(zButton[BT_ENTER] != FALSE)
        {
            if(bInCar != FALSE)
            {
        //        VehicleLicenseFill((BYTE*)&pd->test.vehToSpawn, car);
        //        pd->test.spawnPosInc.pos.X = (pLane->end.pos.X - pLane->start.pos.X) / fSteps;
        //        pd->test.spawnPosInc.pos.Y = (pLane->end.pos.Y - pLane->start.pos.Y) / fSteps;
        //        pd->test.spawnPosInc.pos.Z = (pLane->end.pos.Z - pLane->start.pos.Z) / fSteps;
        //        pd->test.spawnPosInc.angle = (pLane->end.angle - pLane->start.angle) / fSteps;
        //        pd->test.spawnPos.pos.X = pLane->start.pos.X;
        //        pd->test.spawnPos.pos.Y = pLane->start.pos.Y;
        //        pd->test.spawnPos.pos.Z = pLane->start.pos.Z;
        //        pd->test.spawnPos.angle = pLane->start.angle;
        //        pd->test.vehSpawning = 0;
        //        pd->test.bSpawning = TRUE;
            }
        }
        if(pd->test.bSpawning != FALSE)
        {
        //    pd->test.veh[pd->test.vehSpawning] = VehicleCreate((BYTE*)&pd->test.vehToSpawn, &pd->test.spawnPos.pos, pd->test.spawnPos.angle);
        //    if(pd->test.vehSpawning > 0)
        //    {
        //        SN::VEHICLE::MARK_CAR_AS_NO_LONGER_NEEDED(&pd->test.veh[pd->test.vehSpawning - 1]);
        //    }
        //    pd->test.spawnPos.pos.X += pd->test.spawnPosInc.pos.X;
        //    pd->test.spawnPos.pos.Y += pd->test.spawnPosInc.pos.Y;
        //    pd->test.spawnPos.pos.Z += pd->test.spawnPosInc.pos.Z;
        //    pd->test.spawnPos.angle += pd->test.spawnPosInc.angle;
        //    pd->test.vehSpawning++;
        //    if(pd->test.vehSpawning >= pd->test.numVehiclesToSpawn)
        //    {
        //        pd->test.bSpawning = FALSE;
        //        SN::VEHICLE::MARK_CAR_AS_NO_LONGER_NEEDED(&pd->test.veh[pd->test.vehSpawning - 1]);
        //    }
        }

        if(zButton[BT_LEAVE] != FALSE)
        {
            FloatZOBBox3* pbb;
//            FLOAT fPos[4];  // x,y,z,h

            // inner
            pbb = &pd->innerQuad.bbox;
            lss << UL::INFO << "zone inner: " << pbb->min.X << ", " << pbb->min.Y << ", " << pbb->min.Z << ",   "
                << pbb->max.X << ", " << pbb->max.Y << ", " << pbb->max.Z
                << pbb->offset.X << ", " << pbb->offset.Y << ",   "
                << pd->innerQuad.quad.width << ", " << pd->innerQuad.quad.height << UL::ENDL;

            // outer
            pbb = &pd->outerQuad.bbox;
            lss << UL::INFO << "zone outer: " << pbb->min.X << ", " << pbb->min.Y << ", " << pbb->min.Z << ",   "
                << pbb->max.X << ", " << pbb->max.Y << ", " << pbb->max.Z
                << pbb->offset.X << ", " << pbb->offset.Y << ",   "
                << pd->outerQuad.quad.width << ", " << pd->outerQuad.quad.height << UL::ENDL;

            // lanes
            for(DWORD iLane = 0; iLane < PSD_MAX_PARKING_LANES; iLane++)
            {
                FloatAngledVector3 incPos;
                pLane = &pd->lane[iLane];       // reuse pLane
                // start
                lss << UL::INFO << "lane " << iLane << " start: " << pLane->start.pos.X << ", " << pLane->start.pos.Y << ", " << pLane->start.pos.Z << ", " << pLane->start.angle << UL::ENDL;
                // end
                lss << UL::INFO << "lane " << iLane << " end: " << pLane->start.pos.X << ", " << pLane->start.pos.Y << ", " << pLane->start.pos.Z << ", " << pLane->start.angle << UL::ENDL;
                // inc
                incPos.pos.X = fCrop((pLane->end.pos.X - pLane->start.pos.X) / fSteps, 6);
                incPos.pos.Y = fCrop((pLane->end.pos.Y - pLane->start.pos.Y) / fSteps, 6);
                incPos.pos.Z = fCrop((pLane->end.pos.Z - pLane->start.pos.Z) / fSteps, 6);
                incPos.angle = fCrop((pLane->end.angle - pLane->start.angle) / fSteps, 6);
                lss << UL::INFO << "lane " << iLane << " inc: " << incPos.pos.X << ", " << incPos.pos.Y << ", " << incPos.pos.Z << ", " << incPos.angle << UL::ENDL;
                // num spaces
                lss << UL::INFO << "lane " << iLane << " num: " << pLane->numSpaces << UL::ENDL;
            }
        }
        break;
    }
}
*/

