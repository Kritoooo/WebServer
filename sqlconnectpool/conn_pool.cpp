#include "conn_pool.h"
#include <memory>
#include <mutex>

ConnectionPool* ConnectionPool::getConnectPool()
{
    static ConnectionPool pool;
    return &pool;
}

void ConnectionPool::parseJsonFile() {
    // 使用Jsoncpp进行解析
    m_ip = "localhost";
    m_port = 3306;
    m_user = "root";
    m_passwd = "12345678";
    m_timeout = 1000;
    m_maxIdleTime = 5000;
    m_maxSize = 1024;
    m_minSize = 100;
    m_dbName = "testdb";
}

void ConnectionPool::produceConnection() {
    while(true) {
        std::unique_lock<std::mutex> locker(m_mutexQ);
        while(m_connectionQ.size() >= m_minSize) {
            m_cond.wait(locker);
        }
        addConnection();
        m_cond.notify_all();
    }
}

void ConnectionPool::recycleConnection() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> locker(m_mutexQ);
        while(m_connectionQ.size() > m_minSize) {
            MysqlConn* conn = m_connectionQ.front();
            if (conn->getAliveTime() >= m_maxIdleTime) {
                m_connectionQ.pop();
                delete conn;
            } else {
                break;
            }
        }
    }
}

void ConnectionPool::addConnection() {
    MysqlConn* conn = new MysqlConn;
    conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

std::shared_ptr<MysqlConn> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> locker(m_mutexQ);
    while (m_connectionQ.empty()) {
        if(std::cv_status::timeout == m_cond.wait_for(locker, std::chrono::milliseconds(m_timeout))) {
            if(m_connectionQ.empty()) {
                continue;
            }
        }
    }  

    std::shared_ptr<MysqlConn> connptr(m_connectionQ.front(), [this](MysqlConn* conn) {
        std::lock_guard<std::mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
    });
 
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}

ConnectionPool::ConnectionPool() {
    parseJsonFile();

    for (int i = 0; i < m_minSize; i ++) {
        addConnection();
    }

    std::thread producer(&ConnectionPool::produceConnection, this);
    std::thread recyclcer(&ConnectionPool::recycleConnection,this);

    producer.detach();
    recyclcer.detach();
}

ConnectionPool::~ConnectionPool() {
    while(!m_connectionQ.empty()) {
        MysqlConn* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}