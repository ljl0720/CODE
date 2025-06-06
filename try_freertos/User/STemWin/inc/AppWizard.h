/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2020  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.16 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : AppWizard.h
Purpose     : Interface of AppWizard
---------------------------END-OF-HEADER------------------------------
*/

#ifndef APPWIZARD_H
#define APPWIZARD_H

#include <stddef.h>

#include "DIALOG.h"

#include "BUTTON_Private.h"
#include "WINDOW_Private.h"
#include "IMAGE_Private.h"
#include "TEXT_Private.h"
#include "SLIDER_Private.h"
#include "ROTARY_Private.h"
#include "SWITCH_Private.h"
#include "EDIT_Private.h"
#include "QRCODE_Private.h"
#include "GAUGE_Private.h"
#include "KEYBOARD_Private.h"
#include "MULTIEDIT_Private.h"

#if (GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV && WM_SUPPORT_TRANSPARENCY && GUI_SUPPORT_ROTATION)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Dispose flags (coordinates)
//
#define APPW_DISPOSE_FLAG_X0 (1 << 0)
#define APPW_DISPOSE_FLAG_Y0 (1 << 1)
#define APPW_DISPOSE_FLAG_X1 (1 << 2)
#define APPW_DISPOSE_FLAG_Y1 (1 << 3)

#define APPW_DISPOSE_DONE (APPW_DISPOSE_FLAG_X0 | APPW_DISPOSE_FLAG_Y0 | APPW_DISPOSE_FLAG_X1 | APPW_DISPOSE_FLAG_Y1)

#define APPW_DISPOSE_FLAG_ISROOT    (1 << 0)

//
// Manage screens as persistent
//
#define APPW_STATE_PERSISTENT (1 << 0)

//
// Interaction flag(s)
//
#define APPW_ACTION_FLAG_DONTGETVALUEFROMEMITTER (1UL << 16)

//
// Dispose modes
//
#define DISPOSE_MODE_NULL        0  // No dispose information
#define DISPOSE_MODE_REL_DISPLAY 1  // Relative to display
#define DISPOSE_MODE_REL_PARENT  2  // Relative to parent
#define DISPOSE_MODE_REL_SIBLING 3  // Relative to sibling (opposite edge)
#define DISPOSE_MODE_REL_PARTNER 4  // Relative to sibling (same edge)
// The following 4 modes are currently not supported by the AppWizard:
#define DISPOSE_MODE_MID_DISPLAY 5  // Relative to center of display
#define DISPOSE_MODE_MID_PARENT  6  // Relative to center of parent
#define DISPOSE_MODE_TAB_DISPLAY 7  // Tabular layout relative to display
#define DISPOSE_MODE_TAB_PARENT  8  // Tabular layout relative to parent

//
// Dispose indices
//
#define DISPOSE_INDEX_X0 0
#define DISPOSE_INDEX_Y0 1
#define DISPOSE_INDEX_X1 2
#define DISPOSE_INDEX_Y1 3
#define DISPOSE_INDEX_MX 4  // Not used for disposing
#define DISPOSE_INDEX_MY 5  // Not used for disposing

//
// Edges
//
#define APPW_EDGE_LEFT   0
#define APPW_EDGE_RIGHT  1
#define APPW_EDGE_TOP    2
#define APPW_EDGE_BOTTOM 3

//
// Messages
//
#define APPW_MSG_GET_DISPOSE    (APPW_MSG +  0)
#define APPW_MSG_GET_FUNCSETUP  (APPW_MSG +  1)
#define APPW_MSG_SET_LANG       (APPW_MSG +  2)
#define APPW_MSG_GET_JOBS       (APPW_MSG +  3)
#define APPW_MSG_SETUP_FINISHED (APPW_MSG +  4)
#define APPW_MSG_GET_STATE      (APPW_MSG +  5)
#define APPW_MSG_SET_VALUE      (APPW_MSG +  6)
#define APPW_MSG_SET_VAR        (APPW_MSG +  7)
#define APPW_MSG_SET_TEXT       (APPW_MSG +  8)
#define APPW_MSG_DISPOSE_DONE   (APPW_MSG +  9)
#define APPW_MSG_GET_VALUE      (APPW_MSG + 10)
#define APPW_MSG_GET_TEXT       (APPW_MSG + 11)
#define APPW_MSG_GET_FOCUSRECT  (APPW_MSG + 12)
#define APPW_MSG_GET_TYPE       (APPW_MSG + 13)

//
// Draw modes
//
#define APPW_DRAW_MODE_FILL  0
#define APPW_DRAW_MODE_GRADH 1
#define APPW_DRAW_MODE_GRADV 2

//
// Swiping modes
//
#define APPW_SWIPE_REPLACE  0
#define APPW_SWIPE_DISCLOSE 1
#define APPW_SWIPE_CUSTOM   2

//
// Off, On, Toggle
//
#define APPW_SET_OFF    0
#define APPW_SET_ON     1
#define APPW_SET_TOGGLE 2

//
// Please do not change existing values because they are used to determine the font type when reading XBF fonts!
//
#define MODE_FONT_1BPP     0       // Monochrome mode
#define MODE_FONT_2BPP     1       // 4  gray scales for antialiased fonts
#define MODE_FONT_4BPP     2       // 16 gray scales for antialiased fonts
#define MODE_FONT_1EXT     3       // Monochrome mode with extended character information
#define MODE_FONT_1FRM     4       // Monochrome mode with extended character information and frame
#define MODE_FONT_2BPP_EXT 5       // 4  gray scales for antialiased fonts with extended character information
#define MODE_FONT_4BPP_EXT 6       // 16 gray scales for antialiased fonts with extended character information

//
// Modes of Text-object
//
#define APPW_TEXTMODE_TEXT 0
#define APPW_TEXTMODE_DEC  1
#define APPW_TEXTMODE_HEX  2

//
// Modes of Edit-object
//
#define APPW_EDITMODE_TEXT 0
#define APPW_EDITMODE_DEC  1

//
// Text state
//
#define APPW_STATE_TEXT_INVALID  (1U << 0)
#define APPW_STATE_TEXT_HASTEXT  (1U << 1)
#define APPW_STATE_TEXT_HASVALUE (1U << 2)

//
// Conditions
//
#define APPW_IS_VAL 0  // Constant
#define APPW_IS_VAR 1  // Variable
#define APPW_IS_OBJ 2  // Object

//
// Modes of file access
//
#define APPW_IMAGE_MODE_INTERN 0
#define APPW_IMAGE_MODE_EXTERN 1

//
// Spy jobs
//
#define APPW_JOB_REQUEST_OBJECTS   (JOB_QUIT + 1)
#define APPW_JOB_REQUEST_VARIABLES (JOB_QUIT + 2)
#define APPW_JOB_REQUEST_TIME      (JOB_QUIT + 3)
#define APPW_JOB_SEND_PID          (JOB_QUIT + 4)
#define APPW_JOB_SEND_KEY          (JOB_QUIT + 5)

//
// Macros
//
#ifndef GENERATE_JOB
  #define GENERATE_JOB(Action, pFunc, HasReceiver) { Action, (int(*)(WM_HWIN, WM_HWIN, APPW_PARA_ITEM *, int))pFunc, HasReceiver }
#endif

//
// Object types
//
enum {
  TYPE_OBJECT_GROUP = -1,
  TYPE_OBJECT_ROOT,
  TYPE_OBJECT_WINDOW,
  TYPE_OBJECT_BOX,
  TYPE_OBJECT_BUTTON,
  TYPE_OBJECT_IMAGE,
  TYPE_OBJECT_TEXT,
  TYPE_OBJECT_SLIDER,
  TYPE_OBJECT_ROTARY,
  TYPE_OBJECT_SWITCH,
  TYPE_OBJECT_EDIT,
  TYPE_OBJECT_QRCODE,
  TYPE_OBJECT_GAUGE,
  TYPE_OBJECT_KEYBOARD,
  TYPE_OBJECT_PROGBAR,
  TYPE_OBJECT_TIMER,
  TYPE_OBJECT_MULTIEDIT
};

//
// Notification codes
//
enum {
  APPW_NOTIFICATION_INITDIALOG = (WM_NOTIFICATION_WIDGET + 0),
  APPW_NOTIFICATION_CREATE,
  APPW_NOTIFICATION_DELETE,
  APPW_NOTIFICATION_MOTION,
  APPW_NOTIFICATION_ANIMCOORD,
  APPW_NOTIFICATION_ANIMEND,
  APPW_NOTIFICATION_ANIMSTART,
  APPW_NOTIFICATION_TIMER,
  APPW_NOTIFICATION_PIDPRESSED,
  APPW_NOTIFICATION_PIDRELEASED,
  APPW_NOTIFICATION_FIXED,
  APPW_NOTIFICATION_UNPINNED
};

//
// Atom index for start, end and destination
//
enum {
  ATOM_INDEX_START = 0,
  ATOM_INDEX_END,
  ATOM_INDEX_DEST
};

//
// Time index for start and end
//
enum {
  ANIM_INDEX_TS = 0,
  ANIM_INDEX_TE
};

//
// Animation 'atoms'
//
enum {
  ANIM_ATOM_OBJECT_VALUE = 0,
  ANIM_ATOM_VARIABLE,
  ANIM_ATOM_OBJECT_GEO,
  ANIM_ATOM_SCREEN_GEO,
  ANIM_ATOM_CONSTANT
};

//
// Animation details
//
enum {
  ANIM_DETAIL_X0 = DISPOSE_INDEX_X0,
  ANIM_DETAIL_Y0 = DISPOSE_INDEX_Y0,
  ANIM_DETAIL_X1 = DISPOSE_INDEX_X1,
  ANIM_DETAIL_Y1 = DISPOSE_INDEX_Y1,
  ANIM_DETAIL_XSIZE,
  ANIM_DETAIL_YSIZE
};

//
// Jobs of interactions
//
enum {
//--------------------------------------------------------------------------------
// Value                               Purpose
//--------------------------------------------------------------------------------
  APPW_JOB_NULL = -1,
  /*  1 */ APPW_JOB_SETENABLE = 1,  // (pPara + 0)->v     - 1 = On, 0 = Off
  /*  2 */ APPW_JOB_SETVIS,         // (pPara + 0)->v     - 1 = On, 0 = Off
  /*  3 */ APPW_JOB_ANIMCOORD,      // (pPara + 0)->v     - End value
                                    // (pPara + 1)->v     - Index of coordinate
                                    // (pPara + 2)->pFunc - Ease func
                                    // (pPara + 3)->v     - Period
  /*  4 */ APPW_JOB_ANIMVALUE,      // (pPara + 0)->v     - End value
                                    // (pPara + 1)->pFunc - Ease func
                                    // (pPara + 2)->v     - Period
  /*  5 */ APPW_JOB_ANIMRANGE,      // (pPara + 0)->v     - Start value
                                    // (pPara + 1)->v     - End value
                                    // (pPara + 2)->pFunc - Ease func
                                    // (pPara + 3)->v     - Period
  /*  6 */ APPW_JOB_SETCOORD,       // (pPara + 0)->v     - Value
                                    // (pPara + 1)->v     - Index of coordinate
  /*  7 */ APPW_JOB_SWAPSCREEN,     // (pPara + 0)->v     - Screen Id
  /*  8 */ APPW_JOB_SETCOLOR,       // (pPara + 0)->v     - Color to be used
  /*  9 */ APPW_JOB_SETBKCOLOR,     // (pPara + 0)->v     - BkColor to be used
  /* 10 */ APPW_JOB_ADDVALUE,       // (pPara + 0)->v     - Value to be added
  /* 11 */ APPW_JOB_SETVALUE,       // (pPara + 0)->v     - Value to be set
  /* 12 */ APPW_JOB_SETPERIOD,      // (pPara + 0)->v     - Value to be set
  /* 13 */ APPW_JOB_SETSIZE,        // (pPara + 0)->v     - Value to be used
                                    // (pPara + 1)->v     - Index of axis
  /* 14 */ APPW_JOB_CASCADECOORD,   // (pPara + 0)->v     - End value
                                    // (pPara + 1)->v     - Index of coordinate
                                    // (pPara + 2)->v     - Emitter Id of trigger
  /* 15 */ APPW_JOB_SHIFTSCREEN,    // (pPara + 0)->v     - Screen Id
                                    // (pPara + 1)->v     - Index of edge
                                    // (pPara + 2)->pFunc - Ease func
                                    // (pPara + 3)->v     - Period
                                    // (pPara + 4)->v     - Disclose
  /* 16 */ APPW_JOB_SHIFTWINDOW,    // (pPara + 0)->v     - Window Id
                                    // (pPara + 1)->v     - Index of edge
                                    // (pPara + 2)->pFunc - Ease func
                                    // (pPara + 3)->v     - Period
                                    // (pPara + 4)->v     - Disclose
  /* 17 */ APPW_JOB_SET,            // === NO PARAMETERS ===
  /* 18 */ APPW_JOB_CLEAR,          // === NO PARAMETERS ===
  /* 19 */ APPW_JOB_TOGGLE,         // === NO PARAMETERS ===
  /* 20 */ APPW_JOB_START,          // === NO PARAMETERS ===
  /* 21 */ APPW_JOB_STOP,           // === NO PARAMETERS ===
  /* 22 */ APPW_JOB_SETTEXT,        // (pPara + 0)->v     - Text id (if (pPara + 0)->p == NULL)
                                    // (pPara + 0)->p     - Handle  (if (pPara + 0)->v < 0)
  /* 23 */ APPW_JOB_SHOWSCREEN,     // (pPara + 0)->v     - Screen Id
  /* 24 */ APPW_JOB_SETLANG,        // (pPara + 0)->v     - Index of language 
  /* 25 */ APPW_JOB_SETFOCUS,       // === NO PARAMETERS ===
  /* 26 */ APPW_JOB_ENABLEPID,      // (pPara + 0)->v     - 1 = On, 0 = Off
  /* 27 */ APPW_JOB_CLOSESCREEN,    // (pPara + 0)->v     - Screen Id
  /* 28 */ APPW_JOB_SETX0,          // (pPara + 0)->v     - Value
  /* 29 */ APPW_JOB_SETY0,          // (pPara + 0)->v     - Value
  /* 30 */ APPW_JOB_SETX1,          // (pPara + 0)->v     - Value
  /* 31 */ APPW_JOB_SETY1,          // (pPara + 0)->v     - Value
  /* 32 */ APPW_JOB_MODALMESSAGE,   // (pPara + 0)->v     - Screen Id
  /* 33 */ APPW_JOB_CALC,           // === NO PARAMETERS ===
  /* 34 */ APPW_JOB_ANIMCREATE,     // (pPara + 0)->p     - GUI: Pointer to APPW_ANIM_DATA structure, AppWizard: NULL
                                    // (pPara + 0)->v     - GUI: 0,                                   AppWizard: Animation Id
  /* 35 */ APPW_JOB_ANIMSTOP,       // (pPara + 0)->v     - Animation Id
                                    // (pPara + 1)->v     - 1 = Delete animation, 0 = Remain animation
  /* 36 */ APPW_JOB_ANIMSTART,      // (pPara + 0)->v     - Animation Id
                                    // (pPara + 1)->v     - Number of loops (<0 = endless)
  /* 37 */ APPW_JOB_SETBITMAP       // (pPara + 0)->v     - Index
                                    // (pPara + 1)->p     - INT: Pointer to Image-Data / EXT: Pointer to Image-Filename (EXT)
                                    // (pPara + 1)->v     - 0 (INT) / 1 (EXT)
                                    // (pPara + 2)->v     - FileSize
                                    // (pPara + 3)->p     - Source file name (=== In AppWizard application only ===)
                                    // (pPara + 3)->v     - If set to 1 the source file name is in const memory
};

//
// Commands for setting properties !!! INCLUDING USAGE DESCRIPTION FOR REFERENCE !!!
//
// IMPORTANT: Sequence has to be identical to _aSetup[] in APP_StreamWrite.cpp!!!
//
enum {
//--------------------------------------------------------------------------------
// Value                                  Purpose
//--------------------------------------------------------------------------------
  /*  0 */ APPW_SET_PROP_COLOR   = 0,  // pSetup->aPara [0].v     = Color
  /*  1 */ APPW_SET_PROP_COLORS     ,  // pSetup->aPara [0:3].v   = Colors for unpressed, pressed and disabled
  /*  2 */ APPW_SET_PROP_BKCOLOR    ,  // === same as APPW_SET_PROP_COLOR ===
  /*  3 */ APPW_SET_PROP_BKCOLORS   ,  // === same as APPW_SET_PROP_COLORS ===
  /*  4 */ APPW_SET_PROP_SBITMAP    ,  // === same as unpressed image of APPW_SET_PROP_SBITMAPS ===
  /*  5 */ APPW_SET_PROP_JPEG       ,  // pSetup->aPara [0].p     = INT: Pointer to Image-Data / EXT: Pointer to Image-Filename (EXT)
                                       // pSetup->aPara [0].v     = 0 (INT) / 1 (EXT)
                                       // pSetup->aExtra[0].p     = Source file name (=== In AppWizard application only ===)
                                       // pSetup->aExtra[0].v     = If set to 1 the source file names are in const memory
                                       // pSetup->aPara [1].v     = File size
  /*  6 */ APPW_SET_PROP_BMP        ,  // === same as APPW_SET_PROP_JPEG ===
  /*  7 */ APPW_SET_PROP_GIF        ,  // === same as APPW_SET_PROP_JPEG ===
  /*  8 */ APPW_SET_PROP_SBITMAPS   ,  // pSetup->aPara [0:3].p   = pObj->apDraw[0:2].pData - unpressed, pressed, disabled, DTA-Data (INT) / DTA-Filename (EXT)
                                       // pSetup->aPara [0:3].v   = 0 (INT) / 1 (EXT)
                                       // pSetup->aExtra[0:3].p   = Source file names (=== In AppWizard application only ===)
                                       // pSetup->aExtra[0:3].v   = If set to 1 the source file names are in const memory
  /*  9 */ APPW_SET_PROP_REPEAT     ,  // pSetup->aPara [0].v     = Delay
                                       // pSetup->aPara [1].v     = Period
  /* 10 */ APPW_SET_PROP_TOGGLE     ,  // === NO PARAMETERS ===
  /* 11 */ APPW_SET_PROP_TEXTID     ,  // pSetup->aPara [0].p     = Pointer to pointer containing Id-string
                                       // pSetup->aPara [0].v     = Index value of text calculated in APP_DataUpdateObjects()
  /* 12 */ APPW_SET_PROP_ALIGNTEXT  ,  // pSetup->aPara [0:2].v   = pObj->AlignBm, pObj->xOffBm, pObj->yOffBm
  /* 13 */ APPW_SET_PROP_ALIGNBITMAP,  // pSetup->aPara [0:2].v   = pObj->AlignBm, pObj->xOffBm, pObj->yOffBm
  /* 14 */ APPW_SET_PROP_FONT       ,  // pSetup->aPara [0].p     = XBF-Data (INT) / Pointer to APPW_FILE_INFO structure (EXT)
                                       // pSetup->aPara [0].v     = 0 (INT) / 1 (EXT)
  /* 15 */ APPW_SET_PROP_GRADH      ,  // pSetup->aPara [0].v     = n Number of colors
                                       // pSetup->aPara [0].p     = Pointer to n GUI_GRADIENT_INFO elements
  /* 16 */ APPW_SET_PROP_GRADV      ,  // === same as APPW_SET_PROP_GRADH ===
  /* 17 */ APPW_SET_PROP_MOTIONH    ,  // pSetup->aPara [0].p     = Pointer to APPW_ROOT_INFO of left screen (H) top screen (V)
                                       // pSetup->aExtra[0].p     = Pointer to Id-string          "" (=== In AppWizard application only ===)
                                       // pSetup->aPara [0].v     = Swiping mode                  ""
                                       // pSetup->aPara [1].p     = Pointer to APPW_ROOT_INFO  of right screen (H) bottom screen (V)
                                       // pSetup->aExtra[1].p     = Pointer to Id-string          "" (=== In AppWizard application only ===)
                                       // pSetup->aPara [1].v     = Swiping mode                  ""
                                       // pSetup->aPara [2].v     = 'Snap in' period              ""
  /* 18 */ APPW_SET_PROP_MOTIONV    ,  // === same as APPW_SET_PROP_MOTIONH ===
  /* 19 */ APPW_SET_PROP_RANGE      ,  // pSetup->aPara [0].v     = Minimum value
                                       // pSetup->aPara [1].v     = Maximum value
  /* 20 */ APPW_SET_PROP_OFFSET     ,  // pSetup->aPara [0].v     = Offset
  /* 21 */ APPW_SET_PROP_POS        ,  // pSetup->aPara [0].v     = Position
  /* 22 */ APPW_SET_PROP_DECMODE    ,  // pSetup->aPara [0].v     = Len   (parameter of func TEXT_SetDec())
                                       // pSetup->aPara [1].v     = Shift (parameter of func TEXT_SetDec())
                                       // pSetup->aPara [2].v     = Sign  (parameter of func TEXT_SetDec())
                                       // pSetup->aPara [3].v     = Space (parameter of func TEXT_SetDec())
  /* 23 */ APPW_SET_PROP_HEXMODE    ,  // pSetup->aPara [0].v     = Len   (parameter of func TEXT_SetDec())
  /* 24 */ APPW_SET_PROP_VALUE      ,  // pSetup->aPara [0].v     = Value to be used
  /* 25 */ APPW_SET_PROP_INVERT     ,  // === NO PARAMETERS ===
  /* 26 */ APPW_SET_PROP_PERIOD     ,  // pSetup->aPara [0].v     = Sets period when widget stops motion
  /* 27 */ APPW_SET_PROP_SNAP       ,  // pSetup->aPara [0].v     = Sets sets snap position in 1/10 degrees
  /* 28 */ APPW_SET_PROP_SPAN       ,  // pSetup->aPara [0].v     = Minimum value returned by object
                                       // pSetup->aPara [1].v     = Maximum value returned by object
  /* 29 */ APPW_SET_PROP_VERTICAL   ,  // === NO PARAMETERS ===
  /* 30 */ APPW_SET_PROP_PERSISTENT ,  // === NO PARAMETERS ===
  /* 31 */ APPW_SET_PROP_ROTATE     ,  // === NO PARAMETERS ===
  /* 32 */ APPW_SET_PROP_FADE       ,  // === NO PARAMETERS ===
  /* 33 */ APPW_SET_PROP_TILE       ,  // === NO PARAMETERS ===
  /* 34 */ APPW_SET_PROP_WRAP       ,  // === NO PARAMETERS ===
  /* 35 */ APPW_SET_PROP_ROUNDEDVAL ,  // === NO PARAMETERS ===
  /* 36 */ APPW_SET_PROP_ROUNDEDEND ,  // === NO PARAMETERS ===
  /* 37 */ APPW_SET_PROP_OVERWRITE  ,  // === NO PARAMETERS ===
  /* 38 */ APPW_SET_PROP_AUTORESTART,  // === NO PARAMETERS ===
  /* 39 */ APPW_SET_PROP_OPAQUE     ,  // === NO PARAMETERS ===
  /* 40 */ APPW_SET_PROP_STAYONTOP  ,  // === NO PARAMETERS ===
  /* 41 */ APPW_SET_PROP_UNTOUCHABLE,  // === NO PARAMETERS ===
  /* 42 */ APPW_SET_PROP_TEXTID2    ,  // === same as APPW_SET_PROP_TEXTID ===
  /* 43 */ APPW_SET_PROP_ALIGN      ,  // pSetup->aPara [0].v     = pObj->Align
  /* 44 */ APPW_SET_PROP_RADIUS     ,  // pSetup->aPara [0].v     = Radius
  /* 45 */ APPW_SET_PROP_FRAME      ,  // pSetup->aPara [0].v     = Frame
  /* 46 */ APPW_SET_PROP_BORDER     ,  // pSetup->aPara [0].v     = Border
  /* 47 */ APPW_SET_PROP_LENGTH     ,  // pSetup->aPara [0].v     = Length
  /* 48 */ APPW_SET_PROP_ROTATION   ,  // pSetup->aPara [0].p     = Pointer to GUI_ROTATION structure
  /* 49 */ APPW_SET_PROP_ECCLEVEL   ,  // pSetup->aPara [0].v     = Error correction level
  /* 50 */ APPW_SET_PROP_SIZE       ,  // pSetup->aPara [0].v     = Size (for example PixelSize of a QRCode module)
  /* 51 */ APPW_SET_PROP_VERSION    ,  // pSetup->aPara [0].v     = Version (for example 'version' of a QRCode object)
  /* 52 */ APPW_SET_PROP_VALUES     ,  // pSetup->aPara [0].v     = Value 0
                                       // pSetup->aPara [1].v     = Value 1
  /* 53 */ APPW_SET_PROP_FONT2      ,  // === same as APPW_SET_PROP_FONT ===
  /* 54 */ APPW_SET_PROP_SLAYOUT    ,  // pSetup->aPara [0].p     = Pointer to layout data
                                       // pSetup->aExtra[0].p     = Source file name (=== In AppWizard application only ===)
                                       // pSetup->aPara [0].v     = File size
                                       // === Used to set a possible second gradient (Progbar) ===
  /* 55 */ APPW_SET_PROP_GRADH2     ,  // === same as APPW_SET_PROP_GRADH ===
  /* 56 */ APPW_SET_PROP_GRADV2     ,  // === same as APPW_SET_PROP_GRADV ===
  /* 57 */ APPW_SET_PROP_COLOR2     ,  // === same as APPW_SET_PROP_COLOR ===
  /* 58 */ APPW_SET_PROP_PWMODE     ,  // === NO PARAMETERS ===
  /* 59 */ APPW_SET_PROP_FOCUSABLE     // pSetup->aPara [0].v     = 0 - No focus, 1 - Focus visible, 2 - Focus invisible
};

//
// Bitmap indices
//
#define APPW_BI_UNPRESSED 0
#define APPW_BI_PRESSED   1
#define APPW_BI_DISABLED  2

#define APPW_MAX_RANGE    0xFFFFu

//
// Use of parameter
//
#define ARG_V(x)     { (U32)x, (const void *)0, (void (*)(void))0 }
#define ARG_P(x)     { (U32)0, (const void *)x, (void (*)(void))0 }
#define ARG_VP(x, y) { (U32)x, (const void *)y, (void (*)(void))0 }
#define ARG_F(x)     { (U32)0, (const void *)0, (void (*)(void))x }

//
// Internal configuration macros
//
#ifndef   APPW_SETUP_ITEM_EXT
  #define APPW_SETUP_ITEM_EXT
#endif

#ifndef   APPW_ACTION_ITEM_EXT
  #define APPW_ACTION_ITEM_EXT
#endif

#ifndef   APPW_JOB_EXT
  #define APPW_JOB_EXT
#endif

//
// Internal macros
//
#define MANAGE_GET_DISPOSE_FUNCSETUP_JOBS(OBJECT_TYPE)                                      \
  case APPW_MSG_GET_DISPOSE:                                                                \
    pMsg->Data.p = (const void *)&((WM_##OBJECT_TYPE *)GUI_ALLOC_h2p(pMsg->hWin))->Dispose; \
    break;                                                                                  \
  case APPW_MSG_GET_TYPE:                                                                   \
    pMsg->Data.v = TYPE_##OBJECT_TYPE;                                                      \
    break;                                                                                  \
  case APPW_MSG_GET_FUNCSETUP:                                                              \
    pMsg->Data.pFunc = (void(*)(void))_Setup;                                               \
    break;                                                                                  \
  case APPW_MSG_GET_JOBS:                                                                   \
    pMsg->Data.p = (const void *)_aJobs;                                                    \
    pMsg->MsgId  = GUI_COUNTOF(_aJobs);                                                     \
    break

#define MANAGE_GET_DISPOSE_FUNCSETUP(OBJECT_TYPE)                                           \
  case APPW_MSG_GET_DISPOSE:                                                                \
    pMsg->Data.p = (const void *)&((WM_##OBJECT_TYPE *)GUI_ALLOC_h2p(pMsg->hWin))->Dispose; \
    break;                                                                                  \
  case APPW_MSG_GET_TYPE:                                                                   \
    pMsg->Data.v = TYPE_##OBJECT_TYPE;                                                      \
    break;                                                                                  \
  case APPW_MSG_GET_FUNCSETUP:                                                              \
    pMsg->Data.pFunc = (void(*)(void))_Setup;                                               \
    break

//
// Macros for interface definitions
//
#define DEFAULT_CRCB(Object)                                                             \
  void    WM_OBJECT_##Object##_cb    (WM_MESSAGE * pMsg);                                \
  WM_HWIN WM_OBJECT_##Object##_Create(APPW_DISPOSE * pDispose, WM_HWIN hParent, int Id)

#define DEFAULT_FUNC(Object)                                                             \
  DEFAULT_CRCB(Object);                                                                  \
  int     WM_OBJECT_##Object##_Setup (WM_HWIN hWin, APPW_SETUP_ITEM * pSetup)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW_DISPOSE_COORD
*/
typedef struct {
  int Mode;
  int Off;
  int Id;
  int Valid;
} APPW_DISPOSE_COORD;

/*********************************************************************
*
*       APPW_DISPOSE
*/
typedef struct {
  APPW_DISPOSE_COORD aDisposeCoord[4];
  int                xSize;
  int                ySize;
  int                xSizeMin;
  int                ySizeMin;
  U8                 State;
  U32                Flags;
} APPW_DISPOSE;

/*********************************************************************
*
*       APPW_TEXT_INIT
*/
typedef struct {
  const char ** appData;
  int           NumItems;
} APPW_TEXT_INIT;

/*********************************************************************
*
*       APPW_ANIM_PARA
*/
typedef struct  {
  I32                Start;
  I32                End;
  WM_CRITICAL_HANDLE chItem;
  WM_CRITICAL_HANDLE chWinSrc;
  U16                IdSrc;
  U16                IdDst;
  int                RootIndex;
} APPW_ANIM_PARA;

/*********************************************************************
*
*       APPW_ANIM_ITEM_PARA
*/
typedef struct {
  I32 Start;
  I32 End;
} APPW_ANIM_ITEM_PARA;

/*********************************************************************
*
*       APPW_ANIM_PARA_CASCADE
*/
typedef struct {
  APPW_ANIM_PARA AnimPara;
  U16            IdDst;
  GUI_HMEM       hNext;
} APPW_ANIM_PARA_CASCADE;

/*********************************************************************
*
*       APPW_ANIM_COORD_PARA
*/
typedef struct {
  APPW_ANIM_PARA AnimPara;
  int            Index;
} APPW_ANIM_COORD_PARA;

/*********************************************************************
*
*       APPW_ANIM_SHIFT_PARA
*/
typedef struct {
  int                EdgeIndex;
  I32                x0, x1;
  I32                y0, y1;
  WM_CRITICAL_HANDLE chWinOld;
  WM_CRITICAL_HANDLE chWinNew;
  APPW_DISPOSE       DisposeOld;
  APPW_DISPOSE       DisposeNew;
  GUI_ANIM_HANDLE    hAnim;
  U8                 Disclose;
} APPW_ANIM_SHIFT_PARA;

/*********************************************************************
*
*       APPW_ANIM_ATOM
*/
typedef struct {
  U8  Item;   // Item specification
  U8  Detail; // Dispose detail to be used
  U32 Value;  // LW: ObjectGeo | ObjectValue ? ObjectId : Variable ? VariableId : Value (LW)
              // HW: ObjectGeo | ObjectValue | ScreenGeo ? ScreenId : Value (HW)
} APPW_ANIM_ATOM;

/*********************************************************************
*
*       APPW_ANIM_ITEM
*/
typedef struct {
  GUI_ANIM_GETPOS_FUNC Ease;      // Ease function
  GUI_TIMER_TIME       aTime[2];  // Timeline coordinates for start and end
  APPW_ANIM_ATOM       aAtom[3];  // Item definitions for start, end and destination
} APPW_ANIM_ITEM;

/*********************************************************************
*
*       APPW_ANIM_ITEM_INFO
*/
typedef struct {
  I32 Start;               // Used to save the start value of the item
  I32 End;                 // Used to save the end value of the item
  APPW_ANIM_ITEM * pItem;  // Pointer to item
} APPW_ANIM_ITEM_INFO;

/*********************************************************************
*
*       APPW_ANIM_DATA
*/
typedef struct {
  U16                    Id;         // Anmation Id to be used
  GUI_TIMER_TIME         Period;     // Period of complete animation
  U8                     AutoStart;  // If != 0 the animation immediately starts after creation
  int                    NumLoops;   // Number of loops, <0 means endless loop
  int                    NumItems;   // Number of animation items
  const APPW_ANIM_ITEM * pItem;      // Pointer to first item
} APPW_ANIM_DATA;

/*********************************************************************
*
*       APPW_CREATE_ITEM
*/
typedef struct {
  WM_HWIN   (* pfCreate)(APPW_DISPOSE * pDispose, WM_HWIN hParent, int Id);
  int          Id;
  int          IdParent;
  APPW_DISPOSE Dispose;
} APPW_CREATE_ITEM;

/*********************************************************************
*
*       APPW_PARA_ITEM
*/
typedef struct {
  U32          v;
  const void * p;
  void      (* pFunc)(void);
} APPW_PARA_ITEM;

/*********************************************************************
*
*       APPW_SETUP_ITEM
*/
typedef struct {
  int            Id;
  unsigned       Command;
  APPW_PARA_ITEM aPara[6];
  APPW_SETUP_ITEM_EXT
} APPW_SETUP_ITEM;

/*********************************************************************
*
*       APPW_COMP_ITEM
*
* Purpose:
*   Comparison item which could be a constant, a variable id or an object id
*/
typedef struct {
  U8  What;
  I32 Value;
} APPW_COMP_ITEM;

/*********************************************************************
*
*       APPW_CALC_ITEM
*
* Purpose:
*   Calculation item which could be a constant, a variable id or an object id
*/
typedef struct {
  U8  What;
  U16 Index;
  I32 Value;
} APPW_CALC_ITEM;

/*********************************************************************
*
*       APPW_COND_COMP
*
* Purpose:
*   Comparison with 2 items to be compared by the given function pointer
*/
typedef struct {
  APPW_COMP_ITEM   aElem[2];
  int           (* pFunc)(I32 v0, I32 v1);
} APPW_COND_COMP;

/*********************************************************************
*
*       APPW_COND
*
* Purpose:
*   Condition with pointer to comparison table and a pointer to a
*   term to be evaluated.
*/
typedef struct {
  const char           * pTerm;  // Pointer to term to be calculated
  const APPW_COND_COMP * pComp;  // Pointer to first comparison
  unsigned               nComp;  // Number of comparisons
} APPW_COND;

/*********************************************************************
*
*       APPW_CALC
*
* Purpose:
*   Pointer to a term to be calculated, a pointer to operands to be
*   used and a pointer to an I32 array for the operands values.
*/
typedef struct {
  const char           * pTerm;  // Pointer to term to be calculated
  unsigned               nItem;  // Number of operands
  const APPW_CALC_ITEM * pItem;  // Pointer to first operand
  I32                  * pVal;   // Array for calculating operand values
} APPW_CALC;

/*********************************************************************
*
*       APPW_ACTION_ITEM
*
* Purpose:
*   Represents an interaction (including parameters) associated to a root object.
*   Each root object contains a list of interactions defining the behavior of the screen.
*/
typedef struct APPW_ACTION_ITEM APPW_ACTION_ITEM;

struct APPW_ACTION_ITEM {
  int               IdSrc;                               // Emitter
  int               NCode;                               // Signal
  int               IdDst;                               // Receiver
  int               IdJob;                               // Job
  void           (* pfSlot)(APPW_ACTION_ITEM * pAction,  // Pointer to action
                            WM_HWIN            hScreen,  // Handle of screen
                            WM_MESSAGE       * pMsg,     // pMsg->hWnd: receiver, pMsg->hWndSrc: emitter
                            int * pResult);              // Pointer to result
  APPW_PARA_ITEM    aPara[6];                            // Optional job specific parameters
  U32               Flags;                               // Bitmask: 0 means -> aPara[x].v = value
                                                         //          1 means -> aPara[x].v = Variable-Id
  const APPW_COND * pCond;                               // Condition to be fulfilled
  APPW_ACTION_ITEM_EXT
};

/*********************************************************************
*
*       APPW_JOB
*
* Purpose:
*   Defines the function to be called for a specific job
*/
typedef struct {
  int          IdJob;                               // Id of job
  APPW_JOB_EXT
  int       (* pFunc)(WM_HWIN          hWinDst,     // Handle of receiver
                      WM_HWIN          hWinSrc,     // Handle of emitter
                      APPW_PARA_ITEM * pPara,       // Parameters to be used
                      int              RootIndex);
  int          HasReceiver;
} APPW_JOB;

/*********************************************************************
*
*       APPW_OBJ_SETUP_T
*
* Purpose:
*   Common type definition of setup function
*/
typedef int APPW_OBJ_SETUP_T(const APPW_SETUP_ITEM * pSetup, WM_HWIN hParent);

/*********************************************************************
*
*       APPW_ROOT_INFO
*
* Purpose:
*   Structure to be passed to APPW_CreateRoot()
*/
typedef struct {
  U16                      RootId;
  APPW_CREATE_ITEM       * pCreateInfo;
  unsigned                 NumCreateInfo;
  const APPW_SETUP_ITEM  * pSetupInfo;
  unsigned                 NumSetupInfo;
  const APPW_ACTION_ITEM * pActionInfo;
  unsigned                 NumActionInfo;
  WM_CALLBACK            * cb;
  WM_HWIN                  hWin;
} APPW_ROOT_INFO;

/*********************************************************************
*
*       APPW_VAR_OBJECT
*
* Purpose:
*   Configurable variables to be used
*/
typedef struct {
  U16               Id;       // Used to address the variables
  U8                IsDirty;  // Is set to 1 if content is 'dirty' and needs to be processed
  U32               Data;     // Could be anything except a pointer
  const APPW_CALC * pCalc;    // Pointer to calculation object containing term and operands
} APPW_VAR_OBJECT;

/*********************************************************************
*
*       APPW_FILE_INFO
*
* Purpose:
*   ...
*/
typedef struct {
  const char * pFileName;
  void       * pVoid;
} APPW_FILE_INFO;

/*********************************************************************
*
*       APPW_DRAW_OBJECT
*
* Purpose:
*   Structure for drawing bitmaps (streamed or C)
*/
typedef struct {
  void      (* pfDraw)(const void * p, int x, int y);  // Drawing function
  const void * pData;                                  // Data pointer (bitmap) or file name (streamed bitmap)
  int          xSize;                                  // (obvious)
  int          ySize;                                  // (obvious)
  U32          FileSize;                               // (obvious)
} APPW_DRAW_OBJECT;

/*********************************************************************
*
*       APPW_FILEACCESS
*
* Purpose:
*   File access
*/
typedef struct {
  GUI_GET_DATA_FUNC        * pfGetDataBGJ;                                                   // Function for getting data of (B)MP, (G)IF and (J)PEG files
  GUI_GET_DATA_FUNC        * pfGetDataImage;                                                 // Function for getting data of streamed bitmaps and text resource files
  GUI_XBF_GET_DATA_FUNC    * pfGetDataFont;                                                  // Function for getting data of XBF fonts
  U32                     (* pfGetDataFile)  (void * p, U8 * pData, U32 NumBytes, U32 Off);  // Function for getting data of generic files
  void                  * (* pfOpenFontFile) (const char * pFilename);                       // Opens a file in resource (sub)folder 'Font'
  void                  * (* pfOpenImageFile)(const char * pFilename);                       // Opens a file in resource (sub)folder 'Image'
  void                  * (* pfOpenTextFile) (const char * pFilename);                       // Opens a file in resource (sub)folder 'Text'
  void                    (* pfCloseFile)    (void *);                                       // (obvious)
  U32                     (* pfGetFileSize)  (void *);                                       // (obvious)
} APPW_FILEACCESS;

/*********************************************************************
*
*       APPW_X_FILEACCESS
*
* Purpose:
*   Bare file access
*/
typedef struct {
  void * (* pfOpen   )(const char * pFilename);
  int    (* pfSeek   )(const void * p, U32 Off);
  U32    (* pfRead   )(const void * p, void * pData, U32 NumBytes);
  void   (* pfClose  )(const void * p);
  U32    (* pfGetSize)(const void * p);
} APPW_X_FILEACCESS;

/*********************************************************************
*
*       APPW_MEMACCESS
*
* Purpose:
*   Accessing object data located in memory
*/
typedef struct {
  GUI_GET_DATA_FUNC        * pfGetDataImage;
  GUI_XBF_GET_DATA_FUNC    * pfGetDataFont;
  U32                     (* pfGetDataFile)(void * p, U8 * pData, U32 NumBytes, U32 Off);
} APPW_MEMACCESS;

/*********************************************************************
*
*       APPW_OBJECT_INFO
*
* Purpose:
*   Object info to be transferred by spy
*/
typedef struct {
  WM_WINDOW_INFO Info;
  U16            Id;
  I16            RootIndex;
} APPW_OBJECT_INFO;

/*********************************************************************
*
*       APPW_DRAW_FOCUS
*
* Purpose:
*   Defines how a focus should look like
*/
typedef struct {
  GUI_COLOR Color;  // Color of focus rect
  U32       Radius; // Radius if required
  U32       Width;  // Width  if required (minimum 1)
} APPW_DRAW_FOCUS_INFO;

/*********************************************************************
*
*       Object definitions
*
**********************************************************************
*/
/*********************************************************************
*
*       WM_OBJECT_BOX
*/
typedef struct {
  WIDGET              Widget;
  APPW_DISPOSE        Dispose;
  unsigned            Mode;
  GUI_COLOR           Color0;
  int                 NumColors;
  int                 Radius;
  GUI_GRADIENT_INFO * pGradVal;
  GUI_HMEM            hGradPix;
} WM_OBJECT_BOX;

/*********************************************************************
*
*       WM_OBJECT_BUTTON
*/
typedef struct {
  BUTTON_Obj          Widget;
  APPW_DISPOSE        Dispose;
  U32                 State;
  unsigned            Mode;
  unsigned            Delay;
  unsigned            Period;
  unsigned            Toggle;
  unsigned            AlignBm;
  int                 xOffBm;
  int                 yOffBm;
  APPW_DRAW_OBJECT    apDraw[3];
  int                 TextId;
  WM_HTIMER           hTimer;
  GUI_FONT            Font;
  GUI_XBF_DATA        FontData;
  int                 Radius;
} WM_OBJECT_BUTTON;

/*********************************************************************
*
*       WM_OBJECT_IMAGE
*/
typedef struct {
  IMAGE_OBJ           Widget;
  APPW_DISPOSE        Dispose;
  unsigned            Mode;
  GUI_COLOR           Color;
  unsigned            AlignBm;
  int                 xOffBm;
  int                 yOffBm;
  const void        * pData;
  void              * pFile;
} WM_OBJECT_IMAGE;

/*********************************************************************
*
*       WM_OBJECT_ROTARY
*/
typedef struct {
  ROTARY_OBJ          Widget;
  APPW_DISPOSE        Dispose;
  APPW_DRAW_OBJECT    apDraw[3];  // 0 = Bk, 1 = Marker, 2 = MarkerHR
  WM_HMEM             hMarkerPara;
} WM_OBJECT_ROTARY;

/*********************************************************************
*
*       WM_OBJECT_SLIDER
*/
typedef struct {
  SLIDER_Obj          Widget;
  APPW_DISPOSE        Dispose;
  unsigned            Mode;
  APPW_DRAW_OBJECT    apDraw[4];
  GUI_COLOR           aColor[2];
  GUI_COLOR         * apStretchLine[2];  // Pointer for bitmap pixels to be stretched
  U8                  HasValue;
} WM_OBJECT_SLIDER;

/*********************************************************************
*
*       WM_OBJECT_SWITCH
*/
typedef struct {
  SWITCH_Obj          Widget;
  APPW_DISPOSE        Dispose;
  U32                 State;
  APPW_DRAW_OBJECT    apDraw[6];
  int                 aTextId[2];
  GUI_FONT            Font;
  GUI_XBF_DATA        FontData;
} WM_OBJECT_SWITCH;

/*********************************************************************
*
*       WM_OBJECT_TEXT
*/
typedef struct {
  TEXT_OBJ            Widget;
  APPW_DISPOSE        Dispose;
  U32                 State;
  unsigned            Mode;
  unsigned            AlignText;
  int                 xOffText;
  int                 yOffText;
  int                 TextId;
  GUI_FONT            Font;
  GUI_XBF_DATA        FontData;
  //
  // Elements required for decimal mode
  //
  I32                 Value;
  I32                 Min;
  I32                 Max;
  //
  // Parameters for decimal mode
  //
  U8                  Len;
  U8                  Shift;
  U8                  Signed;
  U8                  Space;
} WM_OBJECT_TEXT;

/*********************************************************************
*
*       WM_OBJECT_EDIT
*/
typedef struct {
  EDIT_Obj            Widget;
  APPW_DISPOSE        Dispose;
  U32                 State;
  unsigned            Mode;
  GUI_FONT            Font;
  GUI_XBF_DATA        FontData;
  int                 FrameRadius;
  int                 FrameSize;
  GUI_COLOR           FrameColor;
  int                 IndexText;
  //
  // Elements required for decimal mode
  //
  I32                 Value;
  I32                 Min;
  I32                 Max;
  //
  // Parameters for decimal mode
  //
  U8                  Len;
  U8                  Shift;
  U8                  Signed;
  U8                  Space;
} WM_OBJECT_EDIT;

/*********************************************************************
*
*       WM_OBJECT_QRCODE
*/
typedef struct {
  QRCODE_OBJ          Widget;
  APPW_DISPOSE        Dispose;
  int                 Size;
  //
  // Elements required for setup
  //
  int                 TextId;
} WM_OBJECT_QRCODE;

/*********************************************************************
*
*       WM_OBJECT_GAUGE
*/
typedef struct {
  GAUGE_OBJ           Widget;
  APPW_DISPOSE        Dispose;
} WM_OBJECT_GAUGE;

/*********************************************************************
*
*       WM_OBJECT_KEYBOARD
*/
typedef struct {
  KEYBOARD_OBJ        Widget;
  APPW_DISPOSE        Dispose;
  GUI_FONT            aFont[2];
  GUI_XBF_DATA        aFontData[2];
  //
  // Layout
  //
  const void        * pLayout;
  U32                 FileSize;
} WM_OBJECT_KEYBOARD;

/*********************************************************************
*
*       WM_OBJECT_PROGBAR
*/
typedef struct {
  WIDGET              Widget;
  APPW_DISPOSE        Dispose;
  GUI_COLOR           aColor[2];
  APPW_DRAW_OBJECT    apDraw[2];
  U32                 Value;
  U32                 Min;
  U32                 Max;
  U32                 Radius;
  U32                 Flags;
  U8                  FrameSize;
  GUI_COLOR           FrameColor;
  U32                 AlignBm;
  I32                 xOffBm;
  I32                 yOffBm;
  U32                 Period;
  WM_HTIMER           hTimer;
  I32                 Offset;
  U32                 TimeStart;
  U8                  Tiling;
} WM_OBJECT_PROGBAR;

/*********************************************************************
*
*       WM_OBJECT_TIMER
*/
typedef struct {
  WIDGET              Widget;
  APPW_DISPOSE        Dispose;
  WM_HTIMER           hTimer;
  U32                 Period;
  unsigned            Mode;
} WM_OBJECT_TIMER;

/*********************************************************************
*
*       WM_OBJECT_MULTIEDIT
*/
typedef struct {
  MULTIEDIT_OBJ       Widget;
  APPW_DISPOSE        Dispose;
  U32                 State;
  unsigned            Mode;
  GUI_FONT            Font;
  GUI_XBF_DATA        FontData;
  int                 IndexText;
  int                 FrameRadius;
  int                 FrameSize;
  GUI_COLOR           FrameColor;
} WM_OBJECT_MULTIEDIT;

/*********************************************************************
*
*       WM_OBJECT_WINDOW
*/
typedef struct {
  WINDOW_OBJ          Widget;
  APPW_DISPOSE        Dispose;
  U32                 State;
  APPW_ROOT_INFO    * pRoot;
  APPW_SETUP_ITEM   * pSetupMotionH;
  APPW_SETUP_ITEM   * pSetupMotionV;
} WM_OBJECT_WINDOW;

/*********************************************************************
*
*       Public Data (to be supplied by project)
*
**********************************************************************
*/
extern APPW_ROOT_INFO ** APPW_ppRootList;
extern APPW_ROOT_INFO  * APPW_apRootList[];
extern APPW_VAR_OBJECT   APPW_aVarList[];

extern APPW_VAR_OBJECT * APPW_paVarList;
extern unsigned          APPW_NumScreens;
extern unsigned          APPW_NumVars;

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/
//
// Interface for callback-, create- & setup-function
//
DEFAULT_FUNC(BOX);
DEFAULT_FUNC(BUTTON);
DEFAULT_FUNC(EDIT);
DEFAULT_FUNC(GAUGE);
DEFAULT_FUNC(IMAGE);
DEFAULT_FUNC(KEYBOARD);
DEFAULT_FUNC(PROGBAR);
DEFAULT_FUNC(QRCODE);
DEFAULT_FUNC(ROTARY);
DEFAULT_FUNC(SLIDER);
DEFAULT_FUNC(SWITCH);
DEFAULT_FUNC(TEXT);
DEFAULT_FUNC(MULTIEDIT);
DEFAULT_CRCB(WINDOW);

//
// OWindow
//
int     WM_OBJECT_WINDOW_SetPara          (WM_HWIN hWin, APPW_ROOT_INFO * pRoot);
void    WM_OBJECT_WINDOW_EnableUntouchable(int OnOff);

//
// OTimer
//
WM_HWIN WM_OBJECT_TIMER_Create            (APPW_DISPOSE * pDispose, WM_HWIN hParent, int Id);

//
// AppWizard.c
//
void                    APPW__RegisterPID_Hook       (void);  // Not to be documented
APPW_DISPOSE          * APPW__GetDispose             (WM_HWIN hWin);
int                     APPW__DisposeObject          (WM_HWIN hWin, int Index);
void                    APPW_CreatePersistentScreens (void);
WM_HWIN                 APPW_CreateRoot              (APPW_ROOT_INFO * pRoot, WM_HWIN hParent);
int                     APPW_Dispose                 (WM_HWIN hWin, unsigned Invalidate);
void                    APPW_DragCoord               (APPW_DISPOSE * pDisposeOld, APPW_DISPOSE * pDisposeNew, unsigned Index, int Delta, U8 AllowSetSize);
void                    APPW_DrawFocusRect           (WM_HWIN hWin);
void                    APPW_DrawObject              (APPW_DRAW_OBJECT * pDraw, GUI_RECT * pRect, int AlignBm, int xOffBm, int yOffBm);
void                    APPW_DrawObjectAt            (APPW_DRAW_OBJECT * pDraw, int xPos, int yPos);
void                    APPW_DrawStreamedBitmapINT   (const APPW_DRAW_OBJECT * pDraw, int x, int y);
void                    APPW_DrawStreamedBitmapINT_HR(const APPW_DRAW_OBJECT * pDraw, int x, int y);
void                    APPW_Exec                    (void);
APPW_ROOT_INFO        * APPW_GetCurrentRootInfo      (WM_HWIN hWin);
const char            * APPW_GetResourcePath         (void);
WM_HWIN                 APPW_GetRoot                 (WM_HWIN hWin);
APPW_ROOT_INFO        * APPW_GetRootInfo             (WM_HWIN hRoot, int * pRootIndex);
APPW_ROOT_INFO        * APPW_GetRootInfoByHandle     (WM_HWIN hWin);
APPW_ROOT_INFO        * APPW_GetRootInfoByIndex      (int RootIndex);
APPW_ROOT_INFO        * APPW_GetRootInfoByRootId     (U16 RootId);
int                     APPW_GetOpponent             (int Index);
U8                      APPW_GetSupportFocus         (void);
void                    APPW_Init                    (const char * pResourcePath);
void                    APPW_InitResourcePath        (const char * pResourcePath);
int                     APPW_IsValid                 (WM_HWIN hWin);
int                     APPW_IsRoot                  (WM_HWIN hWin);
void                    APPW_MoveDispose             (APPW_DISPOSE * pDisposeOld, APPW_DISPOSE * pDisposeNew, int dx, int dy);
void                    APPW_MULTIBUF_Enable         (int OnOff);
void                    APPW_SetDefaultPosition      (WM_HWIN hRoot, U16 Id);
void                    APPW_SetDefaultPositionRoot  (WM_HWIN hRoot);
void                    APPW_SetFocusInfo            (APPW_DRAW_FOCUS_INFO * pInfo);
void                    APPW_SetpfInitFS             (void (* pFunc)(void));
void                    APPW_SetpfInitText           (void (* pFunc)(void));
void                    APPW_SetPos                  (WM_HWIN hItem, int xPos, int yPos);
void                    APPW_SetStreamedBitmap       (APPW_DRAW_OBJECT * pDraw, APPW_PARA_ITEM * pPara, GUI_BITMAPSTREAM_INFO * pInfo);
void                    APPW_SetStreamedBitmap_HR    (APPW_DRAW_OBJECT * pDraw, APPW_PARA_ITEM * pPara, GUI_BITMAPSTREAM_INFO * pInfo);
void                    APPW_SetSupportFocus         (U8 SupportFocus);
void                    APPW_TextInitMem             (const APPW_TEXT_INIT * pPara);
void                    APPW_TextInitExt             (const APPW_TEXT_INIT * pPara);
//
// AppWizard.c (public functions to be documented)
//
int                     APPW_GetFont                 (U16 IdScreen, U16 IdWidget, GUI_FONT * pFont, GUI_XBF_DATA * pData);
int                     APPW_GetText                 (U16 IdScreen, U16 IdWidget, char * pBuffer, U32 SizeOfBuffer);
int                     APPW_GetValue                (U16 IdScreen, U16 IdWidget, int * pError);
void                    APPW_SetCustCallback         (void (* pFunc)(void));
int                     APPW_SetText                 (U16 IdScreen, U16 IdWidget, char * pText);
int                     APPW_SetValue                (U16 IdScreen, U16 IdWidget, int Value);
int                     APPW_DoJob                   (U16 IdScreen, U16 IdWidget, int IdJob, APPW_PARA_ITEM * pPara);

//
// AppWizard_Action.c
//
int                     APPW_ACTION_SetEnable        (WM_HWIN hWinDst, WM_HWIN hWinSrc, APPW_PARA_ITEM * pPara, int RootIndex);
void                    APPW_ClearShiftScreenActive  (void);
const APPW_JOB        * APPW_GetJob                  (int Id);
const APPW_JOB        * APPW_GetJobs                 (int * pNumJobs);
const APPW_JOB        * APPW_GetJobsAnim             (int * pNumJobs);
const APPW_JOB        * APPW_GetJobsVar              (int * pNumJobs);
U8                      APPW_GetTimerActive          (void);
int                     APPW_ProcessActions          (WM_MESSAGE * pMsg, int RootIndex);
int                     APPW_ProcessMotion           (WM_MESSAGE * pMsg, APPW_SETUP_ITEM * pSetupMotion);
void                    APPW_SendNotification        (WM_HWIN hWin, int Notification);
APPW_ROOT_INFO       ** APPW_SetData                 (APPW_ROOT_INFO ** ppRootInfo, int NumScreens, APPW_VAR_OBJECT * paVarList, int NumVars);
void                    APPW_SetTimerActive          (U8 TimerActive);

//
// AppWizard_Condition.c
//
int                     APPW__CompareIsLess          (I32 v0, I32 v1);
int                     APPW__CompareIsLessOrEqual   (I32 v0, I32 v1);
int                     APPW__CompareIsEqual         (I32 v0, I32 v1);
int                     APPW__CompareIsGreaterOrEqual(I32 v0, I32 v1);
int                     APPW__CompareIsGreater       (I32 v0, I32 v1);
int                     APPW__CompareIsNotEqual      (I32 v0, I32 v1);

int                     APPW_CalcCond                (const APPW_COND * pCond, U32 * pResult, WM_HWIN hRoot);

//
// AppWizard_FileAccess.c
//
void                    APPW_DrawStreamedBitmapEXT   (const APPW_DRAW_OBJECT * pDraw, int x, int y);
void                    APPW_DrawStreamedBitmapEXT_HR(const APPW_DRAW_OBJECT * pDraw, int x, int y);
const APPW_FILEACCESS * APPW_GetFileAccess           (void);
const GUI_XBF_TYPE    * APPW_GetFileType             (U8 Type);
const APPW_MEMACCESS  * APPW_GetMemAccess            (void);
void                    APPW_GetStreamedBitmapInfoEXT(const APPW_DRAW_OBJECT * pDraw, GUI_BITMAPSTREAM_INFO * pInfo);
int                     APPW_MakeFont                (const APPW_SETUP_ITEM * pSetup, GUI_FONT * pFont, GUI_XBF_DATA * pData);
void                    APPW_SetFileAccess           (APPW_X_FILEACCESS * pFileAccess);

//
// AppWizard_Spy.c
//
int                     APPW_SPY_Process             (U8 Cmd, void * pVoid);

//
// AppWizard_Var.c
//
WM_HWIN                 APPW_GethWinVar              (void);
APPW_VAR_OBJECT       * APPW_GetVar                  (U16 Id);
int                     APPW_SetVarData              (U16 Id, I32 Data);
I32                     APPW_GetVarData              (U16 Id, int * pError);
WM_HWIN                 APPW_GetVarWin               (void);
int                     APPW_CalcTerm                (const APPW_CALC * pCalc);
//
// APPW_X_xxx.c
//
void                    APPW_X_Config                (void);
void                    APPW_X_InitText              (void);
void                    APPW_X_FS_Init               (void);
void                    APPW_X_Setup                 (void);

#endif  // GUI_WINSUPPORT

#endif  // APPWIZARD_H

/*************************** End of file ****************************/
