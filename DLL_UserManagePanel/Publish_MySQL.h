#ifndef PUBLISH_MYSQL_H
#define	PUBLISH_MYSQL_H
int CheckUserAndPasswordInMysqlDB( StruUserManageMysqlDBInfoPtr pMysqlDBInfo, char* pTableName, int UserType, char* pUserName, char* pPassword  );
int UpdateNewPasswordInMysqlDB( StruUserManageMysqlDBInfoPtr pMysqlDBInfo, char* pTableName, int UserType, char* pUserName, char* pPasswordOld, char* pPasswordNew  );
#endif
