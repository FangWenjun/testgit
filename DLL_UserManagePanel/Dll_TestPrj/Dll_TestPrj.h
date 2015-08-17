/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_CBT_MdPSWD                 2       /* control type: command, callback function: CBF_CBT_MdPSWD */
#define  PANEL_CBT_Login                  3       /* control type: command, callback function: CBF_CBT_Login */
#define  PANEL_NUMERIC_TestDll            4       /* control type: numeric, callback function: (none) */
#define  PANEL_TIMER_TestDll              5       /* control type: timer, callback function: CBF_TIMER_TestDll */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CBF_CBT_Login(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CBF_CBT_MdPSWD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CBF_TIMER_TestDll(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
