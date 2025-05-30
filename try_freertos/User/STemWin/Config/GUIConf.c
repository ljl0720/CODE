/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software  has been  licensed to  STMicroelectronics  International
N.V. whose  registered office  is situated at Plan-les-Ouates, Geneva,
39 Chemin du Champ des Filles,  Switzerland solely for the purposes of
creating libraries for  STMicroelectronics  ARM Cortex�-M-based 32-bit
microcontroller    products,    sublicensed    and    distributed   by
STMicroelectronics  under  the  terms  and  conditions of the End User
License Agreement supplied with  the software. The use of the software
on a stand-alone basis  or for any purpose other  than to operate with
the specified  microcontroller is prohibited and subject to a separate
license agreement.

Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
//
// Define the available number of bytes available for the GUI
//
#if USE_EXTMEMHEAP
  #define GUI_NUMBYTES   (1024 * 35)    // x Byte
  #define GUI_EXTBUFADD  (0xD2000000-GUI_NUMBYTES)//32MB SDRAM�����8MB��ΪSTemWIN��̬�ڴ�
#else
  #define GUI_NUMBYTES  (1024 * 110)    // x KByte
#endif


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#if USE_EXTMEMHEAP
  static U32 HeapMem[GUI_NUMBYTES / 4] __attribute__((at(GUI_EXTBUFADD)));
#else
  static U32 extMem[GUI_NUMBYTES / 4];
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       Get_ExtMemHeap``
*
* Purpose:
*   Allocate heap from external memory
*/
#if USE_EXTMEMHEAP
U32* Get_ExtMemHeap (void)
{
  return HeapMem;
}
#endif

/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void)
{
#ifdef USE_EXTMEMHEAP
	static U32 aMemory[GUI_NUMBYTES / 4];
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
	GUI_SetDefaultFont(GUI_FONT_6X8);
	#else	
  GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);	
	#endif
}


/*************************** End of file ****************************/
