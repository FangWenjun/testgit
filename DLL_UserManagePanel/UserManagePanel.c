#include <formatio.h>
#include "pwctrl.h"
#include "toolbox.h"
#include <userint.h>

//==============================================================================
//
// Title:		DLL_UserManagePanel
// Purpose:		A short description of the library.
//
// Created on:	2015/7/22 at 9:42:16 by Microsoft.
// Copyright:	Microsoft. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <utility.h>
#include "UserManagePanel.h"
#include "DLL_UserManagePanel.h"
#include "e:\Repos\UserManager\DLL_UserManagePanel\Publish_MySQL.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
static int gs_LoginPanelHandle;
static StruUserManageMysqlDBInfo gs_MysqlDBInfo;
static char gs_TableName[100];
static int gs_RunTimeInitFlag;
static int gs_password_ctrl;
static int gs_CheckUserType;
static int gs_MdPswdProcessFlag;
//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int Your_Functions_Here (int x)
{
	return x;
}
static void DiscardUIObjectsForDLL (void)
{
	/* Discard the panels loaded in InitUIForDLL */
	/* Call this function from the appropriate place in your code */
	/* to discard UI objects.                                     */
	if (gs_LoginPanelHandle > 0)
	{
		DiscardPanel (gs_LoginPanelHandle);
		gs_LoginPanelHandle = 0;
	}
}
static int Init_LoginVarible( void )
{
	//if ( (panelHandle = LoadPanel (0, "UserManagePanel.uir", PANEL_User)) < 0 )
	gs_RunTimeInitFlag = 0;
	gs_password_ctrl = 0;
	gs_LoginPanelHandle = 0;
	return 0;
}

static int Init_LoginPanel( void )
{
	//if ( (panelHandle = LoadPanel (0, "UserManagePanel.uir", PANEL_User)) < 0 )
	if ((gs_LoginPanelHandle = LoadPanelEx (0, "UserManagePanel.uir", PL_Login, __CVIUserHInst)) < 0)
	{
		return -1;
	}
	else
	{
		//gs_RunTimeInitFlag = 1;
		return 0;
	}
}
static int CheckUserAndPassword( int UserType, char* pUserName, char* pPassword )
{
	return CheckUserAndPasswordInMysqlDB( &gs_MysqlDBInfo, gs_TableName, UserType, pUserName, pPassword );
}
int SetUserAuthorizationRunTime( StruUserManageMysqlDBInfoPtr pMysqlDBInfo, char* pTableName )
{
	gs_MysqlDBInfo = *pMysqlDBInfo;
	StringCopyMax( gs_TableName, pTableName, 100-1 );
	gs_RunTimeInitFlag = 1;
	return 0;
}
int CheckUserAuthorization( int UserType, StruUserAuthorizeResultPtr pUserAuthorizeResult )
{
	int retCode;
	int whichPanel, whichControl;			char mUserName[100]; 		char mPassword[100];
	int userDoneFlag;
	pUserAuthorizeResult->AuthorizeFlag = Macro_AuthorizeFailureFlag;
	if ( UserType<0 || UserType>2 || 0==gs_RunTimeInitFlag )
	{
		return -1;
	}	
	Init_LoginPanel();
	if ( gs_LoginPanelHandle< 0 )
	{
		return -1;
	}

	gs_CheckUserType = UserType;
	
	SetCtrlVal( gs_LoginPanelHandle, PL_Login_TEXTMSG_LoginPrompt, "" );
	InstallPopup (gs_LoginPanelHandle);
	gs_password_ctrl = 0;
	if ( gs_password_ctrl <= 0 )
	{
		gs_password_ctrl = PasswordCtrl_ConvertFromString ( gs_LoginPanelHandle, PL_Login_STRING_Password );
		PasswordCtrl_SetAttribute ( gs_LoginPanelHandle, gs_password_ctrl, ATTR_PASSWORD_MASK_CHARACTER, '*');
	}
	
	retCode = -1; 	userDoneFlag = 0;
	while ( userDoneFlag == FALSE ) 
	{
		/* Wait for a commit event to occur on one of the user interface controls */
		GetUserEvent (1, &whichPanel, &whichControl);
		/* Run code corresponding to what button the user clicked */
		switch (whichControl)
		{
			case PL_Login_CBT_UserLogin: 
				GetCtrlVal( gs_LoginPanelHandle, PL_Login_STRING_UserName, mUserName );
				PasswordCtrl_GetAttribute ( gs_LoginPanelHandle, gs_password_ctrl, ATTR_PASSWORD_VAL, mPassword );
				retCode = CheckUserAndPassword( gs_CheckUserType, mUserName, mPassword );
				if ( retCode>0 )
				{	
					pUserAuthorizeResult->AuthorizeFlag = Macro_AuthorizeSuccessFlag;
					DiscardPanel ( gs_LoginPanelHandle );
					gs_LoginPanelHandle = 0;	userDoneFlag = TRUE;
				}
				else
				{
					SetCtrlVal( gs_LoginPanelHandle, PL_Login_TEXTMSG_LoginPrompt, "用户名和密码不匹配" );
				}
				break;
			case PL_Login_CBT_Cancel:
				DiscardPanel ( gs_LoginPanelHandle );
				gs_LoginPanelHandle = 0;  userDoneFlag = TRUE;
				break;
			default:
				break;
		}
	} 

	return retCode;
}

int ChangeUserPassword(  )
{
	int retCode;	int mMdHandlePanel;		int mPasswordOld_ctrl, mPasswordNew1_ctrl, mPasswordNew2_ctrl;
	int whichPanel, whichControl;			char mUserName[100]; 		
	char mPasswordOldStr[100];	char mPasswordNew1Str[100]; 	char mPasswordNew2Str[100];
	int userDoneFlag;
	
	if ( 1==gs_MdPswdProcessFlag )
	{
		return -1;
	}
	
	if ( 0==gs_RunTimeInitFlag )
	{
		return -1;
	}	
	if ((mMdHandlePanel = LoadPanelEx (0, "UserManagePanel.uir", PL_MdPSWD, __CVIUserHInst)) < 0)
	{
		return -1;
	}

	gs_MdPswdProcessFlag = 1;
	SetCtrlVal( mMdHandlePanel, PL_MdPSWD_TEXTMSG_PSWDPrompt, "" );
	
	InstallPopup ( mMdHandlePanel );
	mPasswordOld_ctrl = PasswordCtrl_ConvertFromString ( mMdHandlePanel, PL_MdPSWD_STRING_PasswordOld );
	PasswordCtrl_SetAttribute ( mMdHandlePanel, mPasswordOld_ctrl, ATTR_PASSWORD_MASK_CHARACTER, '*');
	mPasswordNew1_ctrl = PasswordCtrl_ConvertFromString ( mMdHandlePanel, PL_MdPSWD_STRING_PasswordNew1 );
	PasswordCtrl_SetAttribute ( mMdHandlePanel, mPasswordNew1_ctrl, ATTR_PASSWORD_MASK_CHARACTER, '*');
	mPasswordNew2_ctrl = PasswordCtrl_ConvertFromString ( mMdHandlePanel, PL_MdPSWD_STRING_PasswordNew2 );
	PasswordCtrl_SetAttribute ( mMdHandlePanel, mPasswordNew2_ctrl, ATTR_PASSWORD_MASK_CHARACTER, '*');

	retCode = -1; 	userDoneFlag = 0;
	while ( userDoneFlag == FALSE ) 
	{
		/* Wait for a commit event to occur on one of the user interface controls */
		GetUserEvent (1, &whichPanel, &whichControl);
		/* Run code corresponding to what button the user clicked */
		switch (whichControl)
		{
			case PL_MdPSWD_CBT_ChangePSWD: 
				GetCtrlVal( mMdHandlePanel, PL_MdPSWD_STRING_UserName, mUserName );
				PasswordCtrl_GetAttribute ( mMdHandlePanel, mPasswordOld_ctrl, ATTR_PASSWORD_VAL, mPasswordOldStr );
				PasswordCtrl_GetAttribute ( mMdHandlePanel, mPasswordNew1_ctrl, ATTR_PASSWORD_VAL, mPasswordNew1Str );
				PasswordCtrl_GetAttribute ( mMdHandlePanel, mPasswordNew2_ctrl, ATTR_PASSWORD_VAL, mPasswordNew2Str );
				retCode = -2 ;
				
				if ( 0!=CompareBytes( mPasswordNew1Str, 0, mPasswordNew2Str, 0, StringLength( mPasswordNew1Str ), 1 ) )
				{
					SetCtrlVal( mMdHandlePanel, PL_MdPSWD_TEXTMSG_PSWDPrompt, "新密码不一致！" );
					break;
				}
				else
				{
					if ( 0 == StringLength( mPasswordNew1Str ) )
					{
						SetCtrlVal( mMdHandlePanel, PL_MdPSWD_TEXTMSG_PSWDPrompt, "操作失败，新密码不能为空！" );
						break;
					}
					if ( 0>=CheckUserAndPasswordInMysqlDB( &gs_MysqlDBInfo, gs_TableName, 2, mUserName, mPasswordOldStr  ) )
					{
						SetCtrlVal( mMdHandlePanel, PL_MdPSWD_TEXTMSG_PSWDPrompt, "找不到用户，或旧密码错误！" );
						break;
					}
					else
					{
						if ( 0>UpdateNewPasswordInMysqlDB( &gs_MysqlDBInfo, gs_TableName, 2, mUserName, mPasswordOldStr, mPasswordNew1Str ) )
						{
							SetCtrlVal( mMdHandlePanel, PL_MdPSWD_TEXTMSG_PSWDPrompt, "修改密码失败！" );
							break;
						}
						else
						{
							SetCtrlVal( mMdHandlePanel, PL_MdPSWD_TEXTMSG_PSWDPrompt, "修改密码成功！" );
							retCode = 0;
							break;
						}
						
					}
				}
			/*	
				DiscardPanel ( mMdHandlePanel );
				mMdHandlePanel = 0;	userDoneFlag = TRUE;
				break;
			*/
			case PL_Login_CBT_Cancel:
				DiscardPanel ( mMdHandlePanel );
				mMdHandlePanel = 0;  userDoneFlag = TRUE;
				break;
			default:
				break;
		}
	} 

	gs_MdPswdProcessFlag = 0;
	return retCode;
}


int CVICALLBACK CBF_CBT_UserLogin (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	char mUserName[100]; 		char mPassword[100];
	int rTmp1;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal( gs_LoginPanelHandle, PL_Login_STRING_UserName, mUserName );
			PasswordCtrl_GetAttribute ( gs_LoginPanelHandle, gs_password_ctrl, ATTR_PASSWORD_VAL, mPassword );
			rTmp1 = CheckUserAndPassword( gs_CheckUserType, mUserName, mPassword ); 
			break;
	}
	return 0;
}

//==============================================================================
// DLL main entry-point functions

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
			{
				return 0;	  /* out of memory */
			}
			else
			{
				if ( -1 == Init_LoginVarible() )
				{
					return 0;
				}
			}
			break;
		case DLL_PROCESS_DETACH:
			DiscardUIObjectsForDLL ();
			CloseCVIRTE ();
			break;
	}
	
	return 1;
}

int __stdcall DllEntryPoint (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	/* Included for compatibility with Borland */

	return DllMain (hinstDLL, fdwReason, lpvReserved);
}

int CVICALLBACK CBF_CBT_Cancel (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel( gs_LoginPanelHandle );
			break;
	}
	return 0;
}
