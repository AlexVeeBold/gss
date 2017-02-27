///////////////////////////////////////////////////////////////
//
// gtaScanData: GTA signatures
//
//
//   29.11.2016 00:02 - created (moved out from gtaScan)
//


#include <ulib/uDef.h>

#include "gssSign.h"


//////// //////// //////// //////// GTA Signatures //////// //////// //////// //////// 

BYTE GSign[] = {            // data order must correspond to GOff struct, GTASIGNID enum & GtaSignatures struct
#define _no_    0xFA
#define _XXX    0xFD
    // III 0137
    0x8D, 0x45, 0x10, 0x89, 0xE9, 0x6A, 0x02, 0x50, 0xE8, _no_, _no_, _no_, _no_, 0xA1, _no_, _no_, 
    _no_, _no_, 0x8B, 0x0D, _XXX, _XXX, _XXX, _XXX, 0x50, 0xE8, _XXX, _XXX, _XXX, _XXX, 0x0F, 0xBF, 
    0x40, 0x5C, 0x3B, 0x05, _no_, _no_, _no_, _no_, 0x75, 0x05, 0xB0, 0x01, 
    // VC 0137
    0x8D, 0x46, 0x10, 0x89, 0xF1, 0x6A, 0x02, 0x50, 0xE8, _no_, _no_, _no_, _no_, 0xA1, _no_, _no_, 
    _no_, _no_, 0x8B, 0x0D, _XXX, _XXX, _XXX, _XXX, 0x50, 0xE8, _XXX, _XXX, _XXX, _XXX, 0x0F, 0xBF, 
    0x40, 0x5C, 0x3B, 0x05, _no_, _no_, _no_, _no_, 0x75, 0x04, 0xB0, 0x01, 
/*                  III
8D 45 10 89 E9 6A 02 50 E8 .. .. .. .. A1 .. ..     uSD CRunningScript::CollectParameters() , uVA dw_6ED460_scArgs
.. .. 8B 0D ++ ++ ++ ++ 50 E8 ++ ++ ++ ++ 0F BF     <!> VA g_pVehiclePool <!> , <!> SD CVehiclePool::GetAt() <!>
40 5C 3B 05 .. .. .. .. 75 05 B0 01                 uVA dw_6ED460_scArgs
*/
/*                  Vice City
8D 46 10 89 F1 6A 02 50  E8 .. .. .. .. A1 .. ..    uSD CScriptThread__getNumberParams() , uVA opcodeParams
.. .. 8B 0D ++ ++ ++ ++  50 E8 ++ ++ ++ ++ 0F BF    <!> VA gVehiclePool <!> , <!> SD CPool_CVehicle__atHandle() <!>
40 5C 3B 05 .. .. .. ..  75 04 B0 01                uVA opcodeParams
*/
    // III 033F
    0x8B, 0x44, 0x24, _no_, 0x8B, 0x4C, 0x24, _no_, 0x83, 0xC0, 0x10, 0x6A, 0x02, 0x50, 0xE8, _no_, 
    _no_, _no_, _no_, 0x0F, 0xB7, 0x05, _XXX, _XXX, _XXX, _XXX, 0xD9, 0x05, _no_, _no_, _no_, _no_, 
    0x8D, 0x8C, 0x24, _no_, _no_, _no_, _no_, 0x69, 0xC0, 0x14, 0x04, 0x00, 0x00, 0xD9, 0x98, _XXX, 
    _XXX, _XXX, _XXX, 0xD9, 0x05, _no_, _no_, _no_, _no_, 0xD9, 0x98, _no_, _no_, _no_, _no_, 0xE8, 
    _no_, _no_, _no_, _no_, 0x30, 0xC0, 0x81, 0xC4, _no_, _no_, _no_, _no_, 0x5D, 0x5F, 0x5E, 0x5B, 
    0xC2, 0x04, 0x00,
    // VC 033F
    0x8B, 0x44, 0x24, _no_, 0x8B, 0x4C, 0x24, _no_, 0x83, 0xC0, 0x10, 0x6A, 0x02, 0x50, 0xE8, _no_, 
    _no_, _no_, _no_, 0x0F, 0xB7, 0x05, _XXX, _XXX, _XXX, _XXX, 0xD9, 0x05, _no_, _no_, _no_, _no_, 
    0x8D, 0x8C, 0x24, _no_, _no_, _no_, _no_, 0x69, 0xC0, 0xF4, 0x00, 0x00, 0x00, 0xD9, 0x98, _XXX, 
    _XXX, _XXX, _XXX, 0xD9, 0x05, _no_, _no_, _no_, _no_, 0xD9, 0x98, _no_, _no_, _no_, _no_, 0xE8, 
    _no_, _no_, _no_, _no_, 0x30, 0xC0, 0x81, 0xC4, _no_, _no_, _no_, _no_, 0x5D, 0x5F, 0x5E, 0x5B, 
    0xC2, 0x04, 0x00,
/*                  III         (TextInfo size = 0x414 bytes)
8B 44 24 --  8B 4C 24 --  83 C0 10 6A  02 50 E8 ..    uOF [unused stack offset] (0x10) , uOF (0x10) , uSD CRunningScript::CollectParameters()
.. .. .. 0F  B7 05 ++ ++  ++ ++ D9 05  .. .. .. ..    <!> VA w_95CC88_current_text_info <!> , uVA dw_6ED460_scArg_1
8D 8C 24 --  -- -- -- 69  C0 14 04 00  00 D9 98 ++    uOF (0x140) , <!> VA f_70EA68_text_scale_width <!>
++ ++ ++ D9  05 .. .. ..  .. D9 98 ..  .. .. .. E8    uVA dw_6ED464_scArg_2 , uVA f_70EA6C_text_scale_height
.. .. .. ..  30 C0 81 C4  -- -- -- --  5D 5F 5E 5B    uSD CMatrix::~CMatrix() , unused stack size (0x398)
C2 04 00                                          
*/
/*                  Vice City   (TextInfo size = 0xF4 bytes)
8B 44 24 --  8B 4C 24 --  83 C0 10 6A  02 50 E8 ..    uOF (0x18) , uOF (0x18) , uSD CScriptThread__getNumberParams()
.. .. .. 0F  B7 05 ++ ++  ++ ++ D9 05  .. .. .. ..    <!> VA currentTextDrawer <!> , uVA opcodeParams+0
8D 8C 24 --  -- -- -- 69  C0 F4 00 00  00 D9 98 ++    uOF (0x120) , <!> VA _textDrawers.fScaleWidth <!>
++ ++ ++ D9  05 .. .. ..  .. D9 98 ..  .. .. .. E8    uVA opcodeParams+4 , uVA _textDrawers.fScaleHeight
.. .. .. ..  30 C0 81 C4  -- -- -- --  5D 5F 5E 5B    uSD CMatrix::~CMatrix() , unused stack size (0x2D0)
C2 04 00                                          
*/
    // III RSP
    0x53, 0x89, 0xCB, 0x80, 0x7B, 0x79, 0x00, 0x74, 0x05, 0xE8, _no_, _no_, _no_, _no_, 0x80, 0xBB, 
    0x85, 0x00, 0x00, 0x00, 0x00, 0x74, 0x1F, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x75, 0x16, 
    0x66, 0x83, 0x7B, 0x2C, 0x01, 0x75, 0x0F, 0x66, 0xFF, 0x4B, 0x2C, 0x0F, 0xB7, 0x4B, 0x2C, 0x8B, 
    0x44, 0x8B, 0x14, 0x89, 0x43, 0x10, 0x8B, 0x43, 0x7C, 0x8B, 0x15, _XXX, _XXX, _XXX, _XXX, 0x39, 
    0xC2, 0x72, 0x0D, 0x89, 0xD9, 0xE8, _XXX, _XXX, _XXX, _XXX, 0x84, 0xC0, 0x74, 0xF5, 0xEB, 0x67, 
    0x80, 0x7B, 0x7A, 0x00, 0x74, 0x61, 0x30, 0xC9, 0x66, 0x83, 0x3D, _no_, _no_, _no_, _no_, 0x00, 
    0x74, 0x13, 0x31, 0xC0, 0x66, 0x83, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x75, 0x01, 0x40, 0x84, 
    0xC0, 0x74, 0x02, 0xB1, 0x01, 0x84, 0xC9, 0x74, 0x3E, 0x31, 0xC0, 0xC7, 0x43, 0x7C, 0x00, 0x00, 
    0x00, 0x00, 0x31, 0xD2, 0x83, 0xBA, _no_, _no_, _no_, _no_, 0x00, 0x74, 0x0A, 0xC7, 0x82, _no_, 
    _no_, _no_, _no_, 0x00, 0x00, 0x00, 0x00, 0x40, 0x81, 0xC2, 0xB0, 0x00, 0x00, 0x00, 0x66, 0x83, 
    0xF8, 0x06, 0x7C, 0xE0, 0x83, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x74, 0x0A, 0xC7, 0x05, _no_, 
    _no_, _no_, _no_, 0x00, 0x00, 0x00, 0x00, 0x5B, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    // VC RSP
    0x53, 0x89, 0xCB, 0x80, 0x7B, 0x7A, 0x00, 0x74, 0x05, 0xE8, _no_, _no_, _no_, _no_, 0x80, 0xBB, 
    0x85, 0x00, 0x00, 0x00, 0x00, 0x74, 0x1F, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x75, 0x16, 
    0x66, 0x83, 0x7B, 0x2C, 0x01, 0x75, 0x0F, 0x66, 0xFF, 0x4B, 0x2C, 0x0F, 0xB7, 0x4B, 0x2C, 0x8B, 
    0x44, 0x8B, 0x14, 0x89, 0x43, 0x10, 0x8B, 0x43, 0x7C, 0x8B, 0x15, _XXX, _XXX, _XXX, _XXX, 0x39, 
    0xC2, 0x72, 0x0D, 0x89, 0xD9, 0xE8, _XXX, _XXX, _XXX, _XXX, 0x84, 0xC0, 0x74, 0xF5, 0xEB, 0x67, 
    0x80, 0x7B, 0x7B, 0x00, 0x74, 0x61, 0x30, 0xC9, 0x66, 0x83, 0x3D, _no_, _no_, _no_, _no_, 0x00, 
    0x74, 0x13, 0x31, 0xC0, 0x66, 0x83, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x75, 0x01, 0x40, 0x84, 
    0xC0, 0x74, 0x02, 0xB1, 0x01, 0x84, 0xC9, 0x74, 0x3E, 0x31, 0xC0, 0xC7, 0x43, 0x7C, 0x00, 0x00, 
    0x00, 0x00, 0x31, 0xD2, 0x83, 0xBA, _no_, _no_, _no_, _no_, 0x00, 0x74, 0x0A, 0xC7, 0x82, _no_, 
    _no_, _no_, _no_, 0x00, 0x00, 0x00, 0x00, 0x40, 0x81, 0xC2, 0xB0, 0x00, 0x00, 0x00, 0x66, 0x83, 
    0xF8, 0x06, 0x7C, 0xE0, 0x83, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x74, 0x0A, 0xC7, 0x05, _no_, 
    _no_, _no_, _no_, 0x00, 0x00, 0x00, 0x00, 0x30, 0xC0, 0x5B, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 
/*                  III
53 89 CB 80 7B 79 00 74  05 E8 .. .. .. .. 80 BB   uSD CRunningScript::DoDeatharrestCheck()
85 00 00 00 00 74 1F 80  3D .. .. .. .. 01 75 16   uVA by_95CD41_mission_state_maybe
66 83 7B 2C 01 75 0F 66  FF 4B 2C 0F B7 4B 2C 8B   
44 8B 14 89 43 10 8B 43  7C 8B 15 ++ ++ ++ ++ 39   <!> VA dw_885B48_game_time_maybe <!>
C2 72 0D 89 D9 E8 ++ ++  ++ ++ 84 C0 74 F5 EB 67   <!> SD CRunningScript::ProcessOneCommand() <!>
80 7B 7A 00 74 61 30 C9  66 83 3D .. .. .. .. 00   uVA w_6F0380_pad_button_maybe
74 13 31 C0 66 83 3D ..  .. .. .. 00 75 01 40 84   uVA w_6F03AA_pad_button_maybe
C0 74 02 B1 01 84 C9 74  3E 31 C0 C7 43 7C 00 00   
00 00 31 D2 83 BA .. ..  .. .. 00 74 0A C7 82 ..   uVA dw_773628_script_message_stuff , uVA dw_773634_script_message_stuff
.. .. .. 00 00 00 00 40  81 C2 B0 00 00 00 66 83   
F8 06 7C E0 83 3D .. ..  .. .. 00 74 0A C7 05 ..   uVA dw_8786E0_script_message_stuff , uVA dw_8786EC_script_message_stuff
.. .. .. 00 00 00 00 5B  C3 00 00 00 00 00 00 00 
*/
/*                  Vice City
53 89 CB 80 7B 7A 00 74  05 E8 .. .. .. .. 80 BB   uSD CRunningScript::DoDeatharrestCheck()
85 00 00 00 00 74 1F 80  3D .. .. .. .. 01 75 16   uVA CTheScripts::FailCurrentMission
66 83 7B 2C 01 75 0F 66  FF 4B 2C 0F B7 4B 2C 8B   
44 8B 14 89 43 10 8B 43  7C 8B 15 ++ ++ ++ ++ 39   <!> VA gIngameTimeMs <!>
C2 72 0D 89 D9 E8 ++ ++  ++ ++ 84 C0 74 F5 EB 67   <!> SD CRunningScript::ProcessOneCommand() <!>
80 7B 7B 00 74 61 30 C9  66 83 3D .. .. .. .. 00   uVA _pads.activePadState.Button16
74 13 31 C0 66 83 3D ..  .. .. .. 00 75 01 40 84   uVA _pads.prevPadState.Button16
C0 74 02 B1 01 84 C9 74  3E 31 C0 C7 43 7C 00 00   
00 00 31 D2 83 BA .. ..  .. .. 00 74 0A C7 82 ..   uVA dword_861010 , uVA dword_86101C
.. .. .. 00 00 00 00 40  81 C2 B0 00 00 00 66 83   
F8 06 7C E0 83 3D .. ..  .. .. 00 74 0A C7 05 ..   uVA dword_9458D0 , uVA dword_9458DC
.. .. .. 00 00 00 00 30  C0 5B C3 00 00 00 00 00   
*/
    // III GP
    0xE8, _no_, _no_, _no_, _no_, 0xB9, _no_, _no_, _no_, _no_, 0x6A, 0x00, 0xE8, _no_, _no_, _no_, 
    _no_, 0xA1, _no_, _no_, _no_, _no_, 0x85, 0xC0, 0x74, 0x0A, 0x83, 0xF8, 0x02, 0x74, 0x05, 0x83, 
    0xF8, 0x01, 0x75, 0x14, 0xB9, _no_, _no_, _no_, _no_, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 
    0x00, 0x6A, 0x00, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x80, 0x3D, _no_, 
    _no_, _no_, _no_, 0x00, 0x75, 0x13, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x75, 0x0A, 0xB9, 
    _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x80, 0x3D, 
    _no_, _no_, _no_, _no_, 0x00, 0x0F, 0x85, 0x4D, 0x01, 0x00, 0x00, 0x80, 0x3D, _no_, _no_, _no_, 
    _no_, 0x00, 0x0F, 0x85, 0x40, 0x01, 0x00, 0x00, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, 
    _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0xE8, _XXX, _XXX, _XXX, _XXX, 0xE8, _no_, _no_, _no_, _no_, 0xE8, 
    _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, 
    _no_, 0xB9, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x75, 0x05, 0xE8, _no_, 
    _no_, _no_, _no_, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x74, 0x05, 0xE8, _no_, _no_, _no_, 
    _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xB9, 
    _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x84, 0xC0, 0x74, 0x0A, 
    0xB9, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x80, 
    0x3D, _no_, _no_, _no_, _no_, 0x01, 0x74, 0x05, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, 
    _no_, 0xE8, _no_, _no_, _no_, _no_, 0xB9, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x74, 0x0F, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    // VC GP
    0x53, 0x55, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x80, 0x3D, _no_, _no_, 
    _no_, _no_, 0x00, 0x75, 0x13, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x75, 0x0A, 0xB9, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x81, 0x3D, _no_, 
    _no_, _no_, _no_, 0x20, 0x0B, 0x20, 0x00, 0x72, 0x41, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x00, 
    0x75, 0x38, 0x50, 0x53, 0x51, 0xB8, _no_, _no_, _no_, _no_, 0xBB, _no_, _no_, _no_, _no_, 0xB9, 
    _no_, _no_, _no_, _no_, 0xFF, 0x15, _no_, _no_, _no_, _no_, 0x3D, _no_, _no_, _no_, _no_, 0x74, 
    0x0F, 0xC6, 0x05, _no_, _no_, _no_, _no_, 0x02, 0xEB, 0x0D, 0x8D, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0xC6, 0x05, _no_, _no_, _no_, _no_, 0x01, 0x59, 0x5B, 0x58, 0xE8, _no_, _no_, _no_, _no_, 0x89, 
    0xC3, 0xE8, _no_, _no_, _no_, _no_, 0x31, 0xD2, 0xF7, 0xF3, 0x89, 0xC5, 0xE8, _no_, _no_, _no_, 
    _no_, 0xE8, _no_, _no_, _no_, _no_, 0x89, 0xC3, 0xE8, _no_, _no_, _no_, _no_, 0x31, 0xD2, 0xC7, 
    0x05, _no_, _no_, _no_, _no_, 0x00, 0x00, 0x00, 0x00, 0xF7, 0xF3, 0x29, 0xE8, 0x80, 0x3D, _no_, 
    _no_, _no_, _no_, 0x00, 0x89, 0xC3, 0x0F, 0x85, 0x99, 0x01, 0x00, 0x00, 0x80, 0x3D, _no_, _no_, 
    _no_, _no_, 0x00, 0x0F, 0x85, 0x8C, 0x01, 0x00, 0x00, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, 
    _no_, 0xE8, _XXX, _XXX, _XXX, _XXX, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, 
    _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xB9, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, 
    _no_, 0x83, 0xFB, 0x02, 0x73, 0x30, 0xE8, _no_, _no_, _no_, _no_, 0x89, 0xC3, 0xE8, _no_, _no_, 
    _no_, _no_, 0x31, 0xD2, 0xF7, 0xF3, 0x6A, 0x01, 0x89, 0xC5, 0xE8, _no_, _no_, _no_, _no_, 0x59, 
    0xE8, _no_, _no_, _no_, _no_, 0x89, 0xC3, 0xE8, _no_, _no_, _no_, _no_, 0x31, 0xD2, 0xF7, 0xF3, 
    0x29, 0xE8, 0x89, 0xC3, 0xEB, 0x08, 0x6A, 0x00, 0xE8, _no_, _no_, _no_, _no_, 0x59, 0xE8, _no_, 
    _no_, _no_, _no_, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x00, 0x75, 0x05, 0xE8, _no_, _no_, _no_, 
    _no_, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x74, 0x05, 0xE8, _no_, _no_, _no_, _no_, 0xE8, 
    _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xB9, _no_, _no_, 
    _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, 
    _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0x84, 
    0xC0, 0x74, 0x0A, 0xB9, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, 
    _no_, _no_, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x74, 0x05, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, 
    _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xB9, _no_, _no_, _no_, _no_, 0xE8, _no_, 
    _no_, _no_, _no_, 0x80, 0x3D, _no_, _no_, _no_, _no_, 0x01, 0x74, 0x19, 0x83, 0xFB, 0x02, 0x73, 
    0x05, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 0xE8, _no_, _no_, _no_, _no_, 
    0xE8, _no_, _no_, _no_, _no_, 0x5D, 0x5B, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/*                  III
      0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
850  E8 .. .. .. .. B9 .. ..  .. .. 6A 00 E8 .. .. ..     uSD CPad::UpdatePads() , uVA of unk_6FACF8 , uSD of CCamera::SetMotionBlurAlpha()
860  .. A1 .. .. .. .. 85 C0  74 0A 83 F8 02 74 05 83   1 uVA of dword_6FADA0
870  F8 01 75 14 B9 .. .. ..  .. 6A 00 6A 00 6A 00 6A     uVA of unk_6FACF8
880  00 6A 00 E8 .. .. .. ..  E8 .. .. .. .. 80 3D ..   3 uSD CCamera::SetMotionBlur() , [U] SD of CCutsceneMgr::Update() , [U] VA of byte_95CD9F
890  .. .. .. 00 75 13 80 3D  .. .. .. .. 00 75 0A B9     uVA byte_95CDB1
8A0  .. .. .. .. E8 .. .. ..  .. E8 .. .. .. .. 80 3D   5 uVA dword_8F59D8 , uSD CMenuManager::Process() , uSD CStreaming::Update()
8B0  .. .. .. .. 00 0F 85 4D  01 00 00 80 3D .. .. ..     uVA byte_95CD7C , uVA byte_95CDB1
8C0  .. 00 0F 85 40 01 00 00  E8 .. .. .. .. E8 .. ..   7 uSD CTheZones::Update() , uSD CSprite2d::SetRecipNearClip()
8D0  .. .. E8 .. .. .. .. E8  .. .. .. .. E8 .. .. ..     uSD CSprite2d::InitPerFrame() , uSD CFont::InitPerFrame() , uSD CRecordDataForGame::SaveOrRetrieveDataForThisFrame()
8E0  .. E8 .. .. .. .. E8 ..  .. .. .. E8 .. .. .. ..   9 uSD CRecordDataForChase::SaveOrRetrieveDataForThisFrame() , uSD CPad::DoCheats() , uSD CClock::Update()
8F0  E8 .. .. .. .. E8 ++ ++  ++ ++ E8 .. .. .. .. E8     uSD CWeather::Update() , <!> SD CTheScripts::Process() <!> , uSD CCollision::Update()
900  .. .. .. .. E8 .. .. ..  .. E8 .. .. .. .. E8 ..   B uSD CTrain::UpdateTrains() , uSD CPlane::UpdatePlanes() , uSD CHeli::UpdateHelis() , uSD CDarkel::Update()
910  .. .. .. E8 .. .. .. ..  E8 .. .. .. .. E8 .. ..     uSD CSkidmarks::Update() , uSD CAntennas::Update() , uSD CGlass::Update()
920  .. .. E8 .. .. .. .. E8  .. .. .. .. E8 .. .. ..   D uSD sub_585570() , uSD CEventList::Update() , uSD CParticle::Update()
930  .. B9 .. .. .. .. E8 ..  .. .. .. E8 .. .. .. ..     uVA _fireManager , uSD CFireManager::Update() , uSD sub_4F39A0()
940  E8 .. .. .. .. 80 3D ..  .. .. .. 00 75 05 E8 ..   F uSD sub_55C310() , uVA byte_95CCF5 , uSD sub_542F40()
950  .. .. .. 80 3D .. .. ..  .. 01 74 05 E8 .. .. ..     uVA byte_95CD5B , uSD CCranes::UpdateCranes()
960  .. E8 .. .. .. .. E8 ..  .. .. .. E8 .. .. .. ..  11 uSD CClouds::Update() , uSD CMovingThings::Update() , uSD CWaterCannons::Update()
970  E8 .. .. .. .. E8 .. ..  .. .. E8 .. .. .. .. B9     uSD CUserDisplay::Process() , uSD sub_593170() , uSD CWorld::Process()
980  .. .. .. .. E8 .. .. ..  .. E8 .. .. .. .. E8 ..  13 uVA unk_87FD10 , uSD CAccidentManager::Update() , uSD CPacManPickups::Update() , uSD CPickups::Update()
990  .. .. .. E8 .. .. .. ..  E8 .. .. .. .. E8 .. ..     uSD sub_421E40() , uSD CRubbish::Update() , uSD sub_518D40()
9A0  .. .. E8 .. .. .. .. E8  .. .. .. .. 84 C0 74 0A  15 uSD sub_4ABF40() , uSD sub_597680()
9B0  B9 .. .. .. .. E8 .. ..  .. .. E8 .. .. .. .. 80     uVA unk_6FACF8 , uSD CCamera::Process() , uSD CCullZones::Update()
9C0  3D .. .. .. .. 01 74 05  E8 .. .. .. .. E8 .. ..  17 uVA byte_95CD5B , uSD CGameLogic::Update() , uSD CBridge::Update()
9D0  .. .. E8 .. .. .. .. E8  .. .. .. .. E8 .. .. ..     uSD CCoronas::DoSunAndMoon() , uSD CCoronas::Update() , uSD CShadows::UpdateStaticShadows()
9E0  .. E8 .. .. .. .. B9 ..  .. .. .. E8 .. .. .. ..  19 uSD CShadows::UpdatePermanentShadows() , uVA unk_732A20 , uSD CPhoneInfo::Update()
9F0  80 3D .. .. .. .. 01 74  0F E8 .. .. .. .. E8 ..     uVA byte_95CD5B , uSD CCarCtrl::GenerateRandomCars() , uSD CRoadBlocks::GenerateRoadBlocks()
A00  .. .. .. E8 .. .. .. ..  C3 00 00 00 00 00 00 00  1B uSD CCarCtrl::RemoveDistantCars()
      0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
*/
/*                  Vice City
      0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
410  53 55 E8 .. .. .. .. E8  .. .. .. .. 80 3D .. ..     uSD CPad::UpdatePads() , uSD CCutsceneMgr::Update() , uVA CCutsceneMgr::ms_cutsceneProcessing
420  .. .. 00 75 13 80 3D ..  .. .. .. 00 75 0A B9 ..   1 uVA cd_not_in_drive , uVA FrontEndMenuManager
430  .. .. .. E8 .. .. .. ..  E8 .. .. .. .. 81 3D ..     uSD CMenuManager::Process() , uSD CTheZones::Update() , uVA gIngameTimeMs
440  .. .. .. 20 0B 20 00 72  41 80 3D .. .. .. .. 00   3 uVA byte_A10B2B
450  75 38 50 53 51 B8 -- --  -- -- BB -- -- -- -- B9     uDW (0x1902) , uDW (0x1CF)
460  -- -- -- -- FF 15 .. ..  .. .. 3D -- -- -- -- 74   5 uDW (0x2428) , uVA unknown_call_destination() , uDW (0x2428)
470  0F C6 05 .. .. .. .. 02  EB 0D 8D 80 00 00 00 00     uVA byte_A10B2B
480  C6 05 .. .. .. .. 01 59  5B 58 E8 .. .. .. .. 89   7 uVA byte_A10B2B , uSD CTimer::GetCurrentTimeInCycles()
490  C3 E8 .. .. .. .. 31 D2  F7 F3 89 C5 E8 .. .. ..     uSD CTimer::GetCyclesPerMillisecond() , uSD sub_40EE80()
4A0  .. E8 .. .. .. .. 89 C3  E8 .. .. .. .. 31 D2 C7   9 uSD CTimer::GetCurrentTimeInCycles() , uSD CTimer::GetCyclesPerMillisecond()
4B0  05 .. .. .. .. 00 00 00  00 F7 F3 29 E8 80 3D ..     uVA dword_A0D1F0 , uVA CTimer::m_XXXrPause
4C0  .. .. .. 00 89 C3 0F 85  99 01 00 00 80 3D .. ..   B uVA cd_not_in_drive
4D0  .. .. 00 0F 85 8C 01 00  00 E8 .. .. .. .. E8 ..     uSD CSprite2d::SetRecipNearClip() , uSD CSprite2d::InitPerFrame()
4E0  .. .. .. E8 .. .. .. ..  E8 .. .. .. .. E8 .. ..   D uSD CFont::InitPerFrame() , uSD CRecordDataForGame::SaveOrRetrieveDataForThisFrame() , uSD CRecordDataForChase::SaveOrRetrieveDataForThisFrame()
4F0  .. .. E8 .. .. .. .. E8  .. .. .. .. E8 .. .. ..     uSD CPad::DoCheats() , uSD CClock::Update() , uSD CWeather::Update()
500  .. E8 ++ ++ ++ ++ E8 ..  .. .. .. E8 .. .. .. ..   F <!> SD CTheScripts::Process() <!> , uSD CCollision::Update() , uSD CScriptPaths::Update()
510  E8 .. .. .. .. E8 .. ..  .. .. E8 .. .. .. .. E8     uSD CTrain::UpdateTrains() , uSD CPlane::UpdatePlanes() , uSD CHeli::UpdateHelis()
520  .. .. .. .. E8 .. .. ..  .. E8 .. .. .. .. E8 ..  11 uSD CDarkel::Update() , uSD CSkidmarks::Update() , uSD CAntennas::Update() , uSD CGlass::Update()
530  .. .. .. E8 .. .. .. ..  E8 .. .. .. .. E8 .. ..     uSD CSceneEdit::Update() , uSD CSetPieces::Update() , uSD CEventList::Update()
540  .. .. E8 .. .. .. .. B9  .. .. .. .. E8 .. .. ..  13 uSD CParticle::Update() , uVA gFireManager , uSD CFireManager::Update()
550  .. 83 FB 02 73 30 E8 ..  .. .. .. 89 C3 E8 .. ..     uSD CTimer::GetCurrentTimeInCycles() , uSD CTimer::GetCyclesPerMillisecond()
560  .. .. 31 D2 F7 F3 6A 01  89 C5 E8 .. .. .. .. 59  15 uSD CPopulation::Update()
570  E8 .. .. .. .. 89 C3 E8  .. .. .. .. 31 D2 F7 F3     uSD CTimer::GetCurrentTimeInCycles() , uSD CTimer::GetCyclesPerMillisecond()
580  29 E8 89 C3 EB 08 6A 00  E8 .. .. .. .. 59 E8 ..  17 uSD CPopulation::Update() , uSD CWeapon::UpdateWeapons()
590  .. .. .. 80 3D .. .. ..  .. 00 75 05 E8 .. .. ..     uVA CCutsceneMgr::ms_running , uSD CTheCarGenerators::Process()
5A0  .. 80 3D .. .. .. .. 01  74 05 E8 .. .. .. .. E8  19 uVA CReplay::Mode , uSD CCranes::UpdateCranes()
5B0  .. .. .. .. E8 .. .. ..  .. E8 .. .. .. .. E8 ..     uSD CClouds::Update() , uSD CMovingThings::Update() , uSD CWaterCannons::Update() , uSD CUserDisplay::Process()
5C0  .. .. .. E8 .. .. .. ..  E8 .. .. .. .. B9 .. ..  1B uSD CReplay::Update() , uSD CWorld::Process() , uVA gAccidentManager
5D0  .. .. E8 .. .. .. .. E8  .. .. .. .. E8 .. .. ..     uSD CAccidentManager::Update() , uSD CPacManPickups::Update() , uSD CPickups::Update()
5E0  .. E8 .. .. .. .. E8 ..  .. .. .. E8 .. .. .. ..  1D uSD CGarages::Update() , uSD CRubbish::Update() , uSD CSpecialFX::Update()
5F0  E8 .. .. .. .. E8 .. ..  .. .. E8 .. .. .. .. 84     uSD CRopes::Update() , uSD CTimeCycle::Update() , uSD CReplay::ShouldStandardCameraBeProcessed()
600  C0 74 0A B9 .. .. .. ..  E8 .. .. .. .. E8 .. ..  1F uVA TheCamera , uSD CCamera::Process() , uSD TestPlayerLeavingTown()
610  .. .. 80 3D .. .. .. ..  01 74 05 E8 .. .. .. ..     uVA CReplay::Mode , uSD sub_42BB70()
620  E8 .. .. .. .. E8 .. ..  .. .. E8 .. .. .. .. E8  21 uSD CBridge::Update() , uSD CCoronas::DoSunAndMoon() , uSD CCoronas::Update()
630  .. .. .. .. E8 .. .. ..  .. B9 .. .. .. .. E8 ..     uSD CShadows::UpdateStaticShadows() , uSD CShadows::UpdatePermanentShadows() , uVA gPhoneInfo , uSD CPhoneInfo::Update()
640  .. .. .. 80 3D .. .. ..  .. 01 74 19 83 FB 02 73  23 uVA CReplay::Mode
650  05 E8 .. .. .. .. E8 ..  .. .. .. E8 .. .. .. ..     uSD CCarCtrl::GenerateRandomCars() , uSD CRoadBlocks::GenerateRoadBlocks() , uSD CCarCtrl::RemoveDistantCars()
660  E8 .. .. .. .. 5D 5B C3  00 00 00 00 00 00 00 00  25 uSD CCarCtrl::RemoveCarsIfThePoolGetsFull()
*/
    // III TSI
    0x53, 0x56, 0x55, 0x83, 0xEC, 0x10, 0x31, 0xC0, 0x0F, 0xEF, 0xC0, 0x89, 0xC0, 0x8D, 0x40, 0x00, 
    0x0F, 0x7F, 0x80, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x68, 0x08, 0x0F, 0x7F, 0x85, _XXX, _XXX, _XXX, 
    _XXX, 0x8D, 0x70, 0x10, 0x0F, 0x7F, 0x86, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x48, 0x18, 0x0F, 0x7F, 
    0x81, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x50, 0x20, 0x0F, 0x7F, 0x82, _XXX, _XXX, _XXX, _XXX, 0x8D, 
    0x58, 0x28, 0x0F, 0x7F, 0x83, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x68, 0x30, 0x0F, 0x7F, 0x85, _XXX, 
    _XXX, _XXX, _XXX, 0x8D, 0x70, 0x38, 0x83, 0xC0, 0x40, 0x0F, 0x7F, 0x86, _XXX, _XXX, _XXX, _XXX, 
    0x3D, 0x00, 0x80, 0x02, 0x00, 0x7C, 0xA9, 0x31, 0xED, 0xC7, 0x05, _no_, _no_, _no_, _no_, 0x00, 
    // VC TSI
    0x53, 0x56, 0x57, 0x55, 0x83, 0xEC, 0x10, 0x31, 0xC0, 0x0F, 0xEF, 0xC0, 0x8D, 0x44, 0x20, 0x00, 
    0x0F, 0x7F, 0x80, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x68, 0x08, 0x0F, 0x7F, 0x85, _XXX, _XXX, _XXX, 
    _XXX, 0x8D, 0x50, 0x10, 0x0F, 0x7F, 0x82, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x70, 0x18, 0x0F, 0x7F, 
    0x86, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x58, 0x20, 0x0F, 0x7F, 0x83, _XXX, _XXX, _XXX, _XXX, 0x8D, 
    0x68, 0x28, 0x0F, 0x7F, 0x85, _XXX, _XXX, _XXX, _XXX, 0x8D, 0x48, 0x30, 0x0F, 0x7F, 0x81, _XXX, 
    _XXX, _XXX, _XXX, 0x8D, 0x50, 0x38, 0x83, 0xC0, 0x40, 0x0F, 0x7F, 0x82, _XXX, _XXX, _XXX, _XXX, 
    0x3D, 0x60, 0xF9, 0x03, 0x00, 0x7C, 0xA9, 0x89, 0xC6, 0xC6, 0x80, _XXX, _XXX, _XXX, _XXX, 0x00, 
/*                  III
      0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
790  53 56 55 83 EC 10 31 C0  0F EF C0 89 C0 8D 40 00    
7A0  0F 7F 80 ++ ++ ++ ++ 8D  68 08 0F 7F 85 ++ ++ ++   1  <!> VA by_74B248_script_data_128k <!> , <!> VA by_74B248_script_data_128k <!>
7B0  ++ 8D 70 10 0F 7F 86 ++  ++ ++ ++ 8D 48 18 0F 7F      <!> VA by_74B248_script_data_128k <!>
7C0  81 ++ ++ ++ ++ 8D 50 20  0F 7F 82 ++ ++ ++ ++ 8D   3  <!> VA by_74B248_script_data_128k <!> , <!> VA by_74B248_script_data_128k <!>
7D0  58 28 0F 7F 83 ++ ++ ++  ++ 8D 68 30 0F 7F 85 ++      <!> VA by_74B248_script_data_128k <!> , <!> VA by_74B248_script_data_128k <!>
7E0  ++ ++ ++ 8D 70 38 83 C0  40 0F 7F 86 ++ ++ ++ ++   5  <!> VA by_74B248_script_data_128k <!>
7F0  3D 00 80 02 00 7C A9 31  ED C7 05 .. .. .. .. 00      uVA dword_9430D4
...  ................................................  [partial signature]
*/
/*                  Vice City
      0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
330  53 56 57 55 83 EC 10 31  C0 0F EF C0 8D 44 20 00      
340  0F 7F 80 ++ ++ ++ ++ 8D  68 08 0F 7F 85 ++ ++ ++   1  <!> VA CTheScripts::ScriptSpace <!> , <!> VA CTheScripts::ScriptSpace <!>
350  ++ 8D 50 10 0F 7F 82 ++  ++ ++ ++ 8D 70 18 0F 7F      <!> VA CTheScripts::ScriptSpace <!>
360  86 ++ ++ ++ ++ 8D 58 20  0F 7F 83 ++ ++ ++ ++ 8D   3  <!> VA CTheScripts::ScriptSpace <!> , <!> VA CTheScripts::ScriptSpace <!>
370  68 28 0F 7F 85 ++ ++ ++  ++ 8D 48 30 0F 7F 81 ++      <!> VA CTheScripts::ScriptSpace <!> , <!> VA CTheScripts::ScriptSpace <!>
380  ++ ++ ++ 8D 50 38 83 C0  40 0F 7F 82 ++ ++ ++ ++   5  <!> VA CTheScripts::ScriptSpace <!>
390  3D 60 F9 03 00 7C A9 89  C6 C6 80 ++ ++ ++ ++ 00      <!> VA CTheScripts::ScriptSpace <!>
...  ................................................  [partial signature]
*/
    // III/VC SNVV
    0x8A, 0x44, 0x24, 0x08, 0x8A, 0x4C, 0x24, 0x04, 0x88, 0x0D, _XXX, _XXX, _XXX, _XXX, 0xA2, _XXX, 
    _XXX, _XXX, _XXX, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/*                  III / Vice City
820  8A 44 24 08 8A 4C 24 04  88 0D ++ ++ ++ ++ A2 ++   <!> VA by_5FF2EC_next_car_variant1 <!> , <!> VA by_5FF2ED_next_car_variant2 <!>
830  ++ ++ ++ C3 00 00 00 00  00 00 00 00 00 00 00 00   
*/
    // III/VC PMS
    0x53, 0x56, 0x57, 0x55, 0x83, 0xEC, 0x08, 0x8B, 0x6C, 0x24, 0x3C, 0x89, 0xE8, 0x8B, 0x5C, 0x24, 
    0x38, 0x88, 0xDA, 0x8B, 0x74, 0x24, 0x20, 0x6A, 0x01, 0xFF, 0x74, 0x24, 0x44, 0x50, 0x8A, 0x44, 
    0x24, 0x40, 0x8A, 0x4C, 0x24, 0x3C, 0x52, 0x50, 0x8A, 0x44, 0x24, 0x40, 0x51, 0x50, 0x89, 0xF0, 
    0xFF, 0x74, 0x24, 0x44, 0xFF, 0x74, 0x24, 0x44, 0x50, 0xFF, 0x74, 0x24, 0x44, 0xE8, _XXX, _XXX, 
    _XXX, _XXX, 0xD9, 0x05, _no_, _no_, _no_, _no_, 0x89, 0xC7, 0x83, 0xC4, 0x2C, 0x89, 0xE8, 0x88, 
    0xDA, 0xD8, 0x4C, 0x24, 0x28, 0x6A, 0x02, 0xFF, 0x74, 0x24, 0x44, 0x50, 0x8A, 0x44, 0x24, 0x40, 
/*                  III / Vice City
B80  53 56 57 55 83 EC 08 8B  6C 24 3C 89 E8 8B 5C 24   
B90  38 88 DA 8B 74 24 20 6A  01 FF 74 24 44 50 8A 44   
BA0  24 40 8A 4C 24 3C 52 50  8A 44 24 40 51 50 89 F0   
BB0  FF 74 24 44 FF 74 24 44  50 FF 74 24 44 E8 ++ ++   <!> SD C3dMarker::PlaceMarker() <!>
BC0  ++ ++ D9 05 .. .. .. ..  89 C7 83 C4 2C 89 E8 88   uVA f_5FF1D4_sphere_smaller_radius_0_93
BD0  DA D8 4C 24 28 6A 02 FF  74 24 44 50 8A 44 24 40   
...  ................................................  [partial signature]?
*/
    // IV ScriptHashTableGet / NativeHashTableGet
    0x57, 0x8B, 0x3D, _XXX, _XXX, _XXX, _XXX, 0x85, 0xFF, 0x75, 0x04, 0x33, 0xC0, 0x5F, 0xC3, 0x33, 
    0xD2, 0x8B, 0xC6, 0xF7, 0xF7, 0x53, 0x8B, 0x1D, _XXX, _XXX, _XXX, _XXX, 0x8B, 0xCE, 0x8B, 0x04, 
    0xD3, 0x3B, 0xC6, 0x74, 0x17, 0x85, 0xC0, 0x74, 0x17, 0xD1, 0xE9, 0x83, 0xC1, 0x01, 0x8D, 0x04, 
    0x11, 0x33, 0xD2, 0xF7, 0xF7, 0x8B, 0x04, 0xD3, 0x3B, 0xC6, 0x75, 0xE9, 0x85, 0xC0, 0x75, 0x05, 
    0x5B, 0x33, 0xC0, 0x5F, 0xC3, 0x8B, 0x44, 0xD3, 0x04, 0x5B, 0x5F, 0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 
/*
?getNativeAddress@scrThread@rage@@SGPAXI@Z signature and
CHashtable__get signature (distinguished by references from other procedures)
 57 8B 3D ++ ++ ++ ++ 85  FF 75 04 33 C0 5F C3 33       // VA dw_table_size
 D2 8B C6 F7 F7 53 8B 1D  ++ ++ ++ ++ 8B CE 8B 04       // VA dw_item_array
 D3 3B C6 74 17 85 C0 74  17 D1 E9 83 C1 01 8D 04       
 11 33 D2 F7 F7 8B 04 D3  3B C6 75 E9 85 C0 75 05       
 5B 33 C0 5F C3 8B 44 D3  04 5B 5F C3 CC CC CC CC       
*/
    // IV LoadScript
    0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x81, 0xEC, 0x4C, 0x02, 0x00, 0x00, 0x8B, 0x45, 0x0C, 0x53,
    0x56, 0x57, 0xE8, _XXX, _XXX, _XXX, _XXX, 0x8B, 0xF0, 0x89, 0x44, 0x24, 0x34, 0xE8, _XXX, _XXX, 
    _XXX, _XXX, 0x85, 0xC0, 0x74, 0x19, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x66, 0x01, 0x48, 0x18, 0x01,
    0x0D, _no_, _no_, _no_, _no_, 0x8B, 0x40, 0x04, 0x5F, 0x5E, 0x5B, 0x8B, 0xE5, 0x5D, 0xC3, 0x8B,
/*
__loadScript signature
 55 8B EC 83 E4 F8 81 EC  4C 02 00 00 8B 45 0C 53       
 56 57 E8 .. .. .. .. 8B  F0 89 44 24 34 E8 .. ..       // SD of hashStringLowercase_0 , SD of CHashtable__get  (this one used for scripts, other for natives)
 .. .. 85 C0 74 19 B9 01  00 00 00 66 01 48 18 01       
 0D .. .. .. .. 8B 40 04  5F 5E 5B 8B E5 5D C3 8B       // VA of __dwLoadedScripts
// rest is skipped
*/
#undef _no_
#undef _XXX
};
BYTE GMask[sizeof(GSign)] = {
    0,
};
SIOFFSET GOff[] = {         // order must correspond to GSign array, GTASIGNID enum & GtaSignatures struct
    // III 0137
    {SI_DISPLACEMENT, 0x09}, {SI_DWORD, 0x0E}, 
    {SI_ADDRESS, 0x14}, {SI_DISPLACEMENT, 0x1A}, 
    {SI_ADDRESS, 0x24}, 
    // VC 0137
    {SI_DISPLACEMENT, 0x09}, {SI_DWORD, 0x0E}, 
    {SI_ADDRESS, 0x14}, {SI_DISPLACEMENT, 0x1A}, 
    {SI_ADDRESS, 0x24}, 
    // III 033F
    {SI_BYTE, 0x03}, {SI_BYTE, 0x07}, {SI_DISPLACEMENT, 0x0F}, 
    {SI_ADDRESS, 0x16}, {SI_ADDRESS, 0x1C}, 
    {SI_DWORD, 0x23}, {SI_ADDRESS, 0x2F}, 
    {SI_ADDRESS, 0x35}, {SI_ADDRESS, 0x3B}, 
    {SI_DISPLACEMENT, 0x40}, {SI_DWORD, 0x48}, 
    // VC 033F
    {SI_BYTE, 0x03}, {SI_BYTE, 0x07}, {SI_DISPLACEMENT, 0x0F}, 
    {SI_ADDRESS, 0x16}, {SI_ADDRESS, 0x1C}, 
    {SI_DWORD, 0x23}, {SI_ADDRESS, 0x2F}, 
    {SI_ADDRESS, 0x35}, {SI_ADDRESS, 0x3B}, 
    {SI_DISPLACEMENT, 0x40}, {SI_DWORD, 0x48}, 
    // III RSP
    {SI_DISPLACEMENT, 0x0A}, 
    {SI_ADDRESS, 0x19}, 
    {SI_ADDRESS, 0x3B}, 
    {SI_DISPLACEMENT, 0x46}, 
    {SI_ADDRESS, 0x5B}, 
    {SI_ADDRESS, 0x67}, 
    {SI_ADDRESS, 0x86}, 
    {SI_ADDRESS, 0x8F}, 
    {SI_ADDRESS, 0xA6}, 
    {SI_ADDRESS, 0xAF}, 
    // VC RSP
    {SI_DISPLACEMENT, 0x0A}, 
    {SI_ADDRESS, 0x19}, 
    {SI_ADDRESS, 0x3B}, 
    {SI_DISPLACEMENT, 0x46}, 
    {SI_ADDRESS, 0x5B}, 
    {SI_ADDRESS, 0x67}, 
    {SI_ADDRESS, 0x86}, 
    {SI_ADDRESS, 0x8F}, 
    {SI_ADDRESS, 0xA6}, 
    {SI_ADDRESS, 0xAF}, 
    // III GP
    {SI_DISPLACEMENT, 0x01}, {SI_ADDRESS, 0x06}, {SI_DISPLACEMENT, 0x0D}, //0..2
    {SI_ADDRESS, 0x12}, 
    {SI_ADDRESS, 0x25}, 
    {SI_DISPLACEMENT, 0x34}, {SI_DISPLACEMENT, 0x39}, {SI_ADDRESS, 0x3F}, 
    {SI_ADDRESS, 0x48}, 
    {SI_ADDRESS, 0x50}, {SI_DISPLACEMENT, 0x55}, {SI_DISPLACEMENT, 0x5A}, 
    {SI_ADDRESS, 0x60}, {SI_DISPLACEMENT, 0x6D}, 
    {SI_DISPLACEMENT, 0x79}, {SI_DISPLACEMENT, 0x7E}, //14..15
    {SI_DISPLACEMENT, 0x83}, {SI_DISPLACEMENT, 0x88}, {SI_DISPLACEMENT, 0x8D}, 
    {SI_DISPLACEMENT, 0x92}, {SI_DISPLACEMENT, 0x97}, {SI_DISPLACEMENT, 0x9C}, 
    {SI_DISPLACEMENT, 0xA1}, {SI_DISPLACEMENT, 0xA6}, {SI_DISPLACEMENT, 0xAB}, //22..24  //TSP=23
    {SI_DISPLACEMENT, 0xB0}, {SI_DISPLACEMENT, 0xB5}, {SI_DISPLACEMENT, 0xBA}, {SI_DISPLACEMENT, 0xBF}, 
    {SI_DISPLACEMENT, 0xC4}, {SI_DISPLACEMENT, 0xC9}, {SI_DISPLACEMENT, 0xCE}, 
    {SI_DISPLACEMENT, 0xD3}, {SI_DISPLACEMENT, 0xD8}, {SI_DISPLACEMENT, 0xDD}, 
    {SI_ADDRESS, 0xE2}, {SI_DISPLACEMENT, 0xE7}, {SI_DISPLACEMENT, 0xEC}, 
    {SI_DISPLACEMENT, 0xF1}, {SI_ADDRESS, 0xF7}, {SI_DISPLACEMENT, 0xFF}, //38..40
    {SI_ADDRESS, 0x105}, {SI_DISPLACEMENT, 0x10D}, 
    {SI_DISPLACEMENT, 0x112}, {SI_DISPLACEMENT, 0x117}, {SI_DISPLACEMENT, 0x11C}, 
    {SI_DISPLACEMENT, 0x121}, {SI_DISPLACEMENT, 0x126}, {SI_DISPLACEMENT, 0x12B}, 
    {SI_ADDRESS, 0x130}, {SI_DISPLACEMENT, 0x135}, {SI_DISPLACEMENT, 0x13A}, {SI_DISPLACEMENT, 0x13F}, 
    {SI_DISPLACEMENT, 0x144}, {SI_DISPLACEMENT, 0x149}, {SI_DISPLACEMENT, 0x14E}, //53..55
    {SI_DISPLACEMENT, 0x153}, {SI_DISPLACEMENT, 0x158}, 
    {SI_ADDRESS, 0x161}, {SI_DISPLACEMENT, 0x166}, {SI_DISPLACEMENT, 0x16B}, 
    {SI_ADDRESS, 0x171}, {SI_DISPLACEMENT, 0x179}, {SI_DISPLACEMENT, 0x17E}, 
    {SI_DISPLACEMENT, 0x183}, {SI_DISPLACEMENT, 0x188}, {SI_DISPLACEMENT, 0x18D}, //64..66
    {SI_DISPLACEMENT, 0x192}, {SI_ADDRESS, 0x197}, {SI_DISPLACEMENT, 0x19C}, 
    {SI_ADDRESS, 0x1A2}, {SI_DISPLACEMENT, 0x1AA}, {SI_DISPLACEMENT, 0x1AF}, 
    {SI_DISPLACEMENT, 0x1B4}, //73
    // VC GP
    {SI_DISPLACEMENT, 0x03}, {SI_DISPLACEMENT, 0x08}, {SI_ADDRESS, 0x0E}, //0..2
    {SI_ADDRESS, 0x17}, {SI_ADDRESS, 0x1F},
    {SI_DISPLACEMENT, 0x24}, {SI_DISPLACEMENT, 0x29}, {SI_ADDRESS, 0x2F}, 
    {SI_ADDRESS, 0x3B}, 
    {SI_DWORD, 0x46}, {SI_DWORD, 0x4B}, 
    {SI_DWORD, 0x50}, {SI_ADDRESS, 0x56}, {SI_DWORD, 0x5B}, //11..13
    {SI_ADDRESS, 0x63}, 
    {SI_ADDRESS, 0x72}, {SI_DISPLACEMENT, 0x7B}, 
    {SI_DISPLACEMENT, 0x82}, {SI_DISPLACEMENT, 0x8D}, 
    {SI_DISPLACEMENT, 0x92}, {SI_DISPLACEMENT, 0x99}, 
    {SI_ADDRESS, 0xA1}, {SI_ADDRESS, 0xAF}, //21..22
    {SI_ADDRESS, 0xBE}, 
    {SI_DISPLACEMENT, 0xCA}, {SI_DISPLACEMENT, 0xCF}, 
    {SI_DISPLACEMENT, 0xD4}, {SI_DISPLACEMENT, 0xD9}, {SI_DISPLACEMENT, 0xDE}, 
    {SI_DISPLACEMENT, 0xE3}, {SI_DISPLACEMENT, 0xE8}, {SI_DISPLACEMENT, 0xED}, //29..31
    {SI_DISPLACEMENT, 0xF2}, {SI_DISPLACEMENT, 0xF7}, {SI_DISPLACEMENT, 0xFC}, //TSP=32
    {SI_DISPLACEMENT, 0x101}, {SI_DISPLACEMENT, 0x106}, {SI_DISPLACEMENT, 0x10B}, 
    {SI_DISPLACEMENT, 0x110}, {SI_DISPLACEMENT, 0x115}, {SI_DISPLACEMENT, 0x11A}, {SI_DISPLACEMENT, 0x11F}, 
    {SI_DISPLACEMENT, 0x124}, {SI_DISPLACEMENT, 0x129}, {SI_DISPLACEMENT, 0x12E}, //42..44
    {SI_DISPLACEMENT, 0x133}, {SI_ADDRESS, 0x138}, {SI_DISPLACEMENT, 0x13D}, 
    {SI_DISPLACEMENT, 0x147}, {SI_DISPLACEMENT, 0x14E}, 
    {SI_DISPLACEMENT, 0x15B}, 
    {SI_DISPLACEMENT, 0x161}, {SI_DISPLACEMENT, 0x168}, //51..52
    {SI_DISPLACEMENT, 0x179}, {SI_DISPLACEMENT, 0x17F}, 
    {SI_ADDRESS, 0x185}, {SI_DISPLACEMENT, 0x18D}, 
    {SI_ADDRESS, 0x193}, {SI_DISPLACEMENT, 0x19B}, 
    {SI_DISPLACEMENT, 0x1A0}, {SI_DISPLACEMENT, 0x1A5}, {SI_DISPLACEMENT, 0x1AA}, {SI_DISPLACEMENT, 0x1AF}, //59..62
    {SI_DISPLACEMENT, 0x1B4}, {SI_DISPLACEMENT, 0x1B9}, {SI_ADDRESS, 0x1BE}, 
    {SI_DISPLACEMENT, 0x1C3}, {SI_DISPLACEMENT, 0x1C8}, {SI_DISPLACEMENT, 0x1CD}, 
    {SI_DISPLACEMENT, 0x1D2}, {SI_DISPLACEMENT, 0x1D7}, {SI_DISPLACEMENT, 0x1DC}, //69..71
    {SI_DISPLACEMENT, 0x1E1}, {SI_DISPLACEMENT, 0x1E6}, {SI_DISPLACEMENT, 0x1EB}, 
    {SI_ADDRESS, 0x1F4}, {SI_DISPLACEMENT, 0x1F9}, {SI_DISPLACEMENT, 0x1FE}, 
    {SI_ADDRESS, 0x204}, {SI_DISPLACEMENT, 0x20C}, 
    {SI_DISPLACEMENT, 0x211}, {SI_DISPLACEMENT, 0x216}, {SI_DISPLACEMENT, 0x21B}, //80..82
    {SI_DISPLACEMENT, 0x220}, {SI_DISPLACEMENT, 0x225}, {SI_ADDRESS, 0x22A}, {SI_DISPLACEMENT, 0x22F}, 
    {SI_ADDRESS, 0x235}, 
    {SI_DISPLACEMENT, 0x242}, {SI_DISPLACEMENT, 0x247}, {SI_DISPLACEMENT, 0x24C}, 
    {SI_DISPLACEMENT, 0x251}, //91
    // III TSI
    {SI_ADDRESS, 0x13}, {SI_ADDRESS, 0x1D}, 
    {SI_ADDRESS, 0x27}, 
    {SI_ADDRESS, 0x31}, {SI_ADDRESS, 0x3B}, 
    {SI_ADDRESS, 0x45}, {SI_ADDRESS, 0x4F}, 
    {SI_ADDRESS, 0x5C}, 
    {SI_ADDRESS, 0x6B}, 
    // VC TSI
    {SI_ADDRESS, 0x13}, {SI_ADDRESS, 0x1D}, 
    {SI_ADDRESS, 0x27}, 
    {SI_ADDRESS, 0x31}, {SI_ADDRESS, 0x3B}, 
    {SI_ADDRESS, 0x45}, {SI_ADDRESS, 0x4F}, 
    {SI_ADDRESS, 0x5C}, 
    {SI_ADDRESS, 0x6B}, 
    // III/VC SNVV
    {SI_ADDRESS, 0x0A}, {SI_ADDRESS, 0x0F}, 
    // III/VC PMS
    {SI_DISPLACEMENT, 0x3E}, {SI_ADDRESS, 0x44}, 
    // IV SHTG/NHTG
    {SI_ADDRESS, 0x03}, {SI_ADDRESS, 0x18}, 
    // IV LSC
    {SI_DISPLACEMENT, 0x13}, {SI_DISPLACEMENT, 0x1E}, {SI_ADDRESS, 0x31},

};

enum GTASIGNS {
    // signatures
    SIG_G3_0137_size = 0x2C,
    SIG_VC_0137_size = 0x2C,
    SIG_G3_033F_size = 0x53,
    SIG_VC_033F_size = 0x53,
    SIG_G3_VPI_size = 0x30,
    SIG_G3_GVS_size = 0x70,
    SIG_G3_GRV_size = 0x80,
    SIG_G3_RSP_size = 0xC0,
    SIG_VC_RSP_size = 0xC0,
    SIG_G3_GP_size = 0x1C0,
    SIG_VC_GP_size = 0x260,
    SIG_G3_TSI_size = 0x70,
    SIG_VC_TSI_size = 0x70,
    SIG_G3VC_SNVV_size = 0x20,
    SIG_G3VC_PMS_size = 0x60,
    SIG_IV_HTG_size = 0x50,
    SIG_IV_LSC_size = 0x40,
    // offsets
    SOF_G3_0137_size = 5,
    SOF_VC_0137_size = 5,
    SOF_G3_033F_size = 11,
    SOF_VC_033F_size = 11,
    SOF_G3_VPI_size = 0,
    SOF_G3_GVS_size = 2,
    SOF_G3_GRV_size = 0,
    SOF_G3_RSP_size = 10,
    SOF_VC_RSP_size = 10,
    SOF_G3_GP_size = 74,
    SOF_VC_GP_size = 92,
    SOF_G3_TSI_size = 9,//26,
    SOF_VC_TSI_size = 9,
    SOF_G3VC_SNVV_size = 2,
    SOF_G3VC_PMS_size = 2,
    SOF_IV_HTG_size = 2,
    SOF_IV_LSC_size = 3,
    // offset indices
    SIDX_G3_0137_VEHICLE_POOL = 2,
    SIDX_G3_0137_VEHPOOL_GET_AT = 3,
    SIDX_VC_0137_VEHICLE_POOL = 2,
    SIDX_VC_0137_VEHPOOL_GET_AT = 3,
    SIDX_G3_033F_CURRENT_TEXTINFO = 3,
    SIDX_G3_033F_TEXTINFO_POOL = 6,
    SIDX_VC_033F_CURRENT_TEXTINFO = 3,
    SIDX_VC_033F_TEXTINFO_POOL = 6,
    SIDX_G3_GVS_GARAGE_VEHICLE_CREATE = 0,
    SIDX_G3_GVS_WORLD_ADD_PHYSICAL = 1,
    SIDX_G3_RSP_GAME_TIME = 2,
    SIDX_G3_RSP_PROCESS_ONE_COMMAND = 3,
    SIDX_VC_RSP_GAME_TIME = 2,
    SIDX_VC_RSP_PROCESS_ONE_COMMAND = 3,
    SIDX_G3_GP_THESCRIPTS_PROCESS = 23,
    SIDX_VC_GP_THESCRIPTS_PROCESS = 32,
    SIDX_G3_TSI_SCRIPT_DATA_BUFFER = 0,
    SIDX_VC_TSI_SCRIPT_DATA_BUFFER = 0,
    SIDX_G3VC_SNVV_NEXT_VEH_VARIATION1 = 0,
    SIDX_G3VC_SNVV_NEXT_VEH_VARIATION2 = 1,
    SIDX_G3VC_PMS_PLACE_MARKER = 0,
    SIDX_IV_HTG_TABLE_SIZE = 0,
    SIDX_IV_HTG_ITEM_ARRAY = 1,
    SIDX_IV_LSC_GET_LOWERCASE_STRING_HASH = 0,
    SIDX_IV_LSC_HASHTABLE_GET = 1,
};

enum GTASIGNID : DWORD {    // order must correspond to GSign array, GOff struct & GtaSignatures struct
    SIGN_G3_Opcode0137,
    SIGN_VC_Opcode0137,
    SIGN_G3_Opcode033F,
    SIGN_VC_Opcode033F,
    SIGN_G3_RunningScriptProcess,
    SIGN_VC_RunningScriptProcess,
    SIGN_G3_GameProcess,
    SIGN_VC_GameProcess,
    SIGN_G3_TheScriptsInit,
    SIGN_VC_TheScriptsInit,
    SIGN_G3VC_SetNextVehicleVariation,
    SIGN_G3VC_PlaceMarkerSet,
    SIGN_IV_HashTableGet,
    SIGN_IV_LoadScript,

    // counter (must be the last one)
    NUM_GTA_SIGNATURES
};
SIHDR GtaSignatures[NUM_GTA_SIGNATURES] = {
    {1, SOF_G3_0137_size, SIG_G3_0137_size, },
    {1, SOF_VC_0137_size, SIG_VC_0137_size, },
    {1, SOF_G3_033F_size, SIG_G3_033F_size, },
    {1, SOF_VC_033F_size, SIG_VC_033F_size, },
    {1, SOF_G3_RSP_size, SIG_G3_RSP_size, },
    {1, SOF_VC_RSP_size, SIG_VC_RSP_size, },
    {1, SOF_G3_GP_size, SIG_G3_GP_size, },
    {1, SOF_VC_GP_size, SIG_VC_GP_size, },
    {1, SOF_G3_TSI_size, SIG_G3_TSI_size, },
    {1, SOF_VC_TSI_size, SIG_VC_TSI_size, },
    {1, SOF_G3VC_SNVV_size, SIG_G3VC_SNVV_size, },
    {1, SOF_G3VC_PMS_size, SIG_G3VC_PMS_size, },
    {2, SOF_IV_HTG_size, SIG_IV_HTG_size, },
    {1, SOF_IV_LSC_size, SIG_IV_LSC_size, },
};

enum GTA30TAILHOOKID {            // order must be the same as in following array
    STHI_G30_TheScriptsProcess,
    STHI_G30_TheScriptsInit,

    // counter (must be the last one)
    NUM_GTA30_TAILHOOKS
};
SITH G3TailHooks[NUM_GTA30_TAILHOOKS] = {
    {0x3B8, 0, {0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, // 0x3B8 = (0x004393F8 - 0x00439040)  // G3_TheScriptsProcess
    {0x814, 1, {0x5B, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, // 0x814 = (0x00438FA4 - 0x00438790)  // G3_TheScriptsInit
};
SITH VCTailHooks[NUM_GTA30_TAILHOOKS] = {
    {0x398, 2, {0x5E, 0x5B, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00}}, // 0x398 = (0x00450268 - 0x0044FED0)  // VC_TheScriptsProcess
    {0x944, 2, {0x5E, 0x5B, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00}}, // 0x944 = (0x00450C74 - 0x00450330)  // VC_TheScriptsInit
};


