#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H
#include <chrono>
#include <iostream>
#include <mysql/mysql.h>

class MysqlConn
{
public:

    MysqlConn();
    ~MysqlConn();
    bool connect(std::string user, std::string passwd, std::string dbName, std::string ip, unsigned short port = 3306);
    bool update(std::string sql);
    bool query(std::string sql);
    bool next();
    std::string value(int index);
    bool transaction();
    bool commit();
    bool rollback();
    void refreshAliveTime();
    long long getAliveTime();
private:
    void freeResult();
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row = nullptr;
    std::chrono::steady_clock::time_point m_alivetime;
};

#endif