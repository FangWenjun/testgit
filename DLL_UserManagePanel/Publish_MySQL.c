#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "toolbox.h"
#include <userint.h>
#include <formatio.h>
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "libmysql")
#include "sql_include\mysql.h"
#include "Dll_UserManagePanel.h"

int CheckUserAndPasswordInMysqlDB( StruUserManageMysqlDBInfoPtr pMysqlDBInfo, char* pTableName, int UserType, char* pUserName, char* pPassword  )
{
    MYSQL mysql;
    MYSQL_RES *ResultSet;	
	int res;
	int RetCode;
	char SQLCmdBuf[1000];
	StruUserManageMysqlDBInfo m_MySQLDBInfo;
	char error_info[2048];
	int  error_code;
	int	num_rows;

	m_MySQLDBInfo = *pMysqlDBInfo;  

	
	ResultSet = NULL;  
	RetCode = -1;
    mysql_init(&mysql);// 初始化mysql结构
	
	if (!mysql_real_connect(&mysql, m_MySQLDBInfo.hostname, m_MySQLDBInfo.username, m_MySQLDBInfo.password, m_MySQLDBInfo.dbname, m_MySQLDBInfo.dbport, NULL, 0))
    {
        FmtOut("\n用户登录时MySQL数据库连接失败!\n");
        mysql_error(&mysql);
		RetCode = -1;						 
		return RetCode;
    }
    else
    {
		res = mysql_query(&mysql, "SET character_set_connection = GB2312");
		res = mysql_query(&mysql, "SET character_set_results = GB2312");
		res = mysql_query(&mysql, "SET character_set_client = GB2312"); 
       	if(res) 	{	FmtOut("SET character_set数据失败!\n");	}
		else
		{
			Fmt( SQLCmdBuf, "select * from %s where user='\%s'\ and password='\%s'\ and UserType=%d", pTableName, pUserName, pPassword, UserType  );
			res = mysql_query(&mysql, SQLCmdBuf );
			ResultSet = mysql_store_result( &mysql );
			if(ResultSet == NULL)  
			{  
				error_code = mysql_errno( &mysql );                    //获取错误码
				strncpy( error_info, mysql_error( &mysql ),1024);           //获取错误信息 
				return -1;	//返回错误    
			}
			else
			{
				num_rows = (int)( mysql_num_rows( ResultSet ) );
				if ( 0<num_rows )
				{
					RetCode = num_rows;
				}
			}
		}
		
		if(ResultSet != NULL) 
		{			
			mysql_free_result( ResultSet );  
			ResultSet  = NULL;	 
		}  
		mysql_close(&mysql); // 释放数据库连接
    }  			   
	return RetCode;  
}

int UpdateNewPasswordInMysqlDB( StruUserManageMysqlDBInfoPtr pMysqlDBInfo, char* pTableName, int UserType, char* pUserName, char* pPasswordOld, char* pPasswordNew  )
{
    MYSQL mysql;
    MYSQL_RES *ResultSet;	
	int res;
	int RetCode;
	char SQLCmdBuf[1000];
	StruUserManageMysqlDBInfo m_MySQLDBInfo;
	char error_info[2048];
	int  error_code;
	
	m_MySQLDBInfo = *pMysqlDBInfo;  	 
	
	ResultSet = NULL;  
	RetCode = -1;
    mysql_init(&mysql);// 初始化mysql结构
	
	if (!mysql_real_connect(&mysql, m_MySQLDBInfo.hostname, m_MySQLDBInfo.username, m_MySQLDBInfo.password, m_MySQLDBInfo.dbname, m_MySQLDBInfo.dbport, NULL, 0))
    {
        FmtOut("\n用户登录时MySQL数据库连接失败!\n");
        mysql_error(&mysql);
		RetCode = -1;						 
		return RetCode;
    }
    else
    {
		res = mysql_query(&mysql, "SET character_set_connection = GB2312");
		res = mysql_query(&mysql, "SET character_set_results = GB2312");
		res = mysql_query(&mysql, "SET character_set_client = GB2312"); 
       	if(res) 	{	FmtOut("SET character_set数据失败!\n");	}
		else
		{
			//UPDATE `eig_alarm_setting_table` SET `FddiSensitivity`=%d,
			//Fmt( SQLCmdBuf, "select * from %s where user='\%s'\ and password='\%s'\ and UserType=%d", pTableName, pUserName, pPassword, UserType  );
			Fmt( SQLCmdBuf, "UPDATE %s SET password='\%s'\ where user='\%s'\ and password='\%s'\ and UserType=%d", pTableName, pPasswordNew, pUserName, pPasswordOld, UserType  );
			res = mysql_query(&mysql, SQLCmdBuf );
			if(res)
	        {
				error_code = mysql_errno( &mysql );                    //获取错误码
				strncpy( error_info, mysql_error( &mysql ),1024);           //获取错误信息 
				DebugPrintf( "%s 更新密码失败!\n", pTableName );
				RetCode = -1;
	        }
	        else   
			{	   
				RetCode = 0;
			}
		}  
		mysql_close(&mysql); // 释放数据库连接
    }  			   
	return RetCode;  
}
