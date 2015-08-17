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
    mysql_init(&mysql);// ��ʼ��mysql�ṹ
	
	if (!mysql_real_connect(&mysql, m_MySQLDBInfo.hostname, m_MySQLDBInfo.username, m_MySQLDBInfo.password, m_MySQLDBInfo.dbname, m_MySQLDBInfo.dbport, NULL, 0))
    {
        FmtOut("\n�û���¼ʱMySQL���ݿ�����ʧ��!\n");
        mysql_error(&mysql);
		RetCode = -1;						 
		return RetCode;
    }
    else
    {
		res = mysql_query(&mysql, "SET character_set_connection = GB2312");
		res = mysql_query(&mysql, "SET character_set_results = GB2312");
		res = mysql_query(&mysql, "SET character_set_client = GB2312"); 
       	if(res) 	{	FmtOut("SET character_set����ʧ��!\n");	}
		else
		{
			Fmt( SQLCmdBuf, "select * from %s where user='\%s'\ and password='\%s'\ and UserType=%d", pTableName, pUserName, pPassword, UserType  );
			res = mysql_query(&mysql, SQLCmdBuf );
			ResultSet = mysql_store_result( &mysql );
			if(ResultSet == NULL)  
			{  
				error_code = mysql_errno( &mysql );                    //��ȡ������
				strncpy( error_info, mysql_error( &mysql ),1024);           //��ȡ������Ϣ 
				return -1;	//���ش���    
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
		mysql_close(&mysql); // �ͷ����ݿ�����
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
    mysql_init(&mysql);// ��ʼ��mysql�ṹ
	
	if (!mysql_real_connect(&mysql, m_MySQLDBInfo.hostname, m_MySQLDBInfo.username, m_MySQLDBInfo.password, m_MySQLDBInfo.dbname, m_MySQLDBInfo.dbport, NULL, 0))
    {
        FmtOut("\n�û���¼ʱMySQL���ݿ�����ʧ��!\n");
        mysql_error(&mysql);
		RetCode = -1;						 
		return RetCode;
    }
    else
    {
		res = mysql_query(&mysql, "SET character_set_connection = GB2312");
		res = mysql_query(&mysql, "SET character_set_results = GB2312");
		res = mysql_query(&mysql, "SET character_set_client = GB2312"); 
       	if(res) 	{	FmtOut("SET character_set����ʧ��!\n");	}
		else
		{
			//UPDATE `eig_alarm_setting_table` SET `FddiSensitivity`=%d,
			//Fmt( SQLCmdBuf, "select * from %s where user='\%s'\ and password='\%s'\ and UserType=%d", pTableName, pUserName, pPassword, UserType  );
			Fmt( SQLCmdBuf, "UPDATE %s SET password='\%s'\ where user='\%s'\ and password='\%s'\ and UserType=%d", pTableName, pPasswordNew, pUserName, pPasswordOld, UserType  );
			res = mysql_query(&mysql, SQLCmdBuf );
			if(res)
	        {
				error_code = mysql_errno( &mysql );                    //��ȡ������
				strncpy( error_info, mysql_error( &mysql ),1024);           //��ȡ������Ϣ 
				DebugPrintf( "%s ��������ʧ��!\n", pTableName );
				RetCode = -1;
	        }
	        else   
			{	   
				RetCode = 0;
			}
		}  
		mysql_close(&mysql); // �ͷ����ݿ�����
    }  			   
	return RetCode;  
}
