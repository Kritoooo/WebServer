#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H
#include <condition_variable>
#include <queue>
#include "sql_conn.h"
#include "../locker/locker.h"
#include <thread>
#include <mutex>

class ConnectionPool {
public:
    static ConnectionPool* getConnectPool();
    ConnectionPool(const ConnectionPool& obj) = delete;
    ConnectionPool& operator=(const ConnectionPool&obj) = delete;
    std::shared_ptr<MysqlConn> getConnection();
    ~ConnectionPool();
private:
    ConnectionPool();
    void parseJsonFile();
    void produceConnection();
    void recycleConnection();
    void addConnection();

    std::string m_ip;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbName;
    unsigned short m_port;
    int m_minSize;
    int m_maxSize;
    int m_timeout;
    int m_maxIdleTime;
    std::queue<MysqlConn*> m_connectionQ;
    std::mutex m_mutexQ;
    std::condition_variable m_cond;
};

#endif
