//==============================================================================
//
// Title:		Dll_TestPrj
// Purpose:		A short description of the application.
//
// Created on:	2015/7/22 at 9:44:39 by Microsoft.
// Copyright:	Microsoft. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Dll_TestPrj.h"
#include "toolbox.h"
#include "DLL_UserManagePanel.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;

//==============================================================================
// Static functions
static int	InitMySQLDBInfoStru( StruUserManageMysqlDBInfoPtr pMysql )
{
	Fmt( pMysql->hostname,"%s", "127.0.0.1" );
	//Fmt( pMysql->hostname,"%s", "192.168.1.3" );
	//Fmt( pMysql->hostname,"%s", "localhost" ); 
	Fmt( pMysql->username,"%s", "root" );
	Fmt( pMysql->password,"%s", "Hlxm123456" );
	Fmt( pMysql->dbname,"%s", "hljk" );
	pMysql->dbport = 3306;  
	return 0;
}

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Dll_TestPrj.uir", PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

int CVICALLBACK CBF_CBT_Login (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int rTmp1;
	StruUserAuthorizeResult mUserAuthorizeResult;
	StruUserManageMysqlDBInfo mUserManageMysqlDBInfo;
	switch (event)
	{
		case EVENT_COMMIT:
			InitMySQLDBInfoStru( &mUserManageMysqlDBInfo );
			SetUserAuthorizationRunTime( &mUserManageMysqlDBInfo, "user_setting_table" );
			rTmp1 = CheckUserAuthorization( 2, &mUserAuthorizeResult );
			if ( rTmp1> 0 )
			{
				DebugPrintf( "√‹¬Î’˝»∑\n" );
			}
			else
			{
				DebugPrintf( "√‹¬Î¥ÌŒÛ\n" );
			}
			break;
	}
	return 0;
}

int CVICALLBACK CBF_TIMER_TestDll (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	static int sEnterCnt = 0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			sEnterCnt ++;
			SetCtrlVal( panelHandle, PANEL_NUMERIC_TestDll, sEnterCnt );
			break;
	}
	return 0;
}

int CVICALLBACK CBF_CBT_MdPSWD (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	StruUserManageMysqlDBInfo mUserManageMysqlDBInfo;
	switch (event)
	{
		case EVENT_COMMIT:
			InitMySQLDBInfoStru( &mUserManageMysqlDBInfo );
			SetUserAuthorizationRunTime( &mUserManageMysqlDBInfo, "user_setting_table" );
			ChangeUserPassword( );
			break;
	}
	return 0;
}
