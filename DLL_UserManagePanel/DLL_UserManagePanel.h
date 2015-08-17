//==============================================================================
//
// Title:		DLL_UserManagePanel
// Purpose:		A short description of the interface.
//
// Created on:	2015/7/22 at 9:42:16 by Microsoft.
// Copyright:	Microsoft. All Rights Reserved.
//
//==============================================================================

#ifndef __DLL_UserManagePanel_H__
#define __DLL_UserManagePanel_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
#define Macro_AuthorizeSuccessFlag	0
#define Macro_AuthorizeFailureFlag	-1
		
//==============================================================================
// Types
typedef	struct UserDetailInfoStru
{
	int Id;
	int	UserType;
	char UserName[100];
	char Password[100];
	char AuthorityDetail[100];
}StruUserDetailInfo, *StruUserDetailInfoPtr;

typedef	struct UserAuthorizeResultStru
{
	StruUserDetailInfo UserDetail;
	int AuthorizeFlag;
	int ActiveLeftTimeCnt;
}StruUserAuthorizeResult, *StruUserAuthorizeResultPtr;

typedef struct UserManageMysqlDBInfoStru
{
	char hostname[50];
	char username[50];
	char password[50];
	char dbname[50];
	int	dbport;
}StruUserManageMysqlDBInfo, *StruUserManageMysqlDBInfoPtr;
//==============================================================================
// External variables

//==============================================================================
// Global functions

int Your_Functions_Here (int x);
int SetUserAuthorizationRunTime( StruUserManageMysqlDBInfoPtr pMysqlDBInfo, char* pTableName );
int CheckUserAuthorization( int UserType, StruUserAuthorizeResultPtr pUserAuthorizeResult );
int ChangeUserPassword( );


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __DLL_UserManagePanel_H__ */
