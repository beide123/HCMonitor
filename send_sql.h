#ifndef _SENDSQL_H
#define _SENDSQL_H

/*
#include <mysql/mysql.h>

#define TEST_X86 1
#define OP_MYSQL 1
#define CMD_BUF_SIZE 256
#define SQLPORT 3306
MYSQL g_conn;
MYSQL g_conn_10;
MYSQL g_conn_init; 
int insert_tag;
int timecnt;

int init_mysql(void) ;
void uninitializeMysql(void) ;
int executesql(const char * sql) ;
void print_mysql_error(const char *msg);
#ifdef TEST_X86
int insert_mysql(float x,float y,int i) ;
#else
int insert_mysql_hy(float x,float y,int i);
#endif
int insert_mysql_init(int i);

int insert_mysql_inter(float delay,unsigned int conn);

#define DEBUG
*/

uint32_t traffic;
uint32_t traffic_mid;


#endif

