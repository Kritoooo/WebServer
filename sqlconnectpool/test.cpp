#include <chrono>
#include <iostream>
#include <memory>
#include "sql_conn.h"
#include "conn_pool.h"
// 1.单线程：使用/不使用连接池
// 2.多线程：使用/不适用连接池

void op1(int begin, int end) {
    for(int i = begin; i < end; i++) {
        MysqlConn conn;
        conn.connect("root","12345678","testdb","localhost",3306);
        char sql[1024] = {0};
        sprintf(sql, "insert into person values(%d,26,'man','Jerry')", i);
        conn.update(sql);
    }
} 

void op2(ConnectionPool* pool, int begin, int end) {
    for(int i = begin; i < end; i++) {
        std::shared_ptr<MysqlConn> conn = pool->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into person values(%d,26,'man','Jerry')", i);
        conn->update(sql);
    }
} 

int query() {
    MysqlConn conn;
    std::cout << conn.connect("root","12345678","testdb","localhost",3306) << "\n";
    std::string sql = "insert into person values(5,26,'man','Jerry')";
    bool flag = conn.update(sql);
    std::cout << "flag value : " << flag << std::endl;
    sql = "select * from person";
    conn.query(sql);
    while(conn.next()) {
        std::cout << conn.value(0) << ", "
                <<  conn.value(1) << ", "
                << conn.value(2) << ", "
                << conn.value(3) << "\n";
    }
    return 0;
}

void test1() {
#if 1
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    op1(0, 5000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    std::cout << "非连接池 单线程 用时：" << length.count() << "纳秒, "
        << length.count() / 1000000 << " 毫秒" << std::endl;
#else
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    op2(pool, 0, 5000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    std::cout << "连接池 单线程 用时：" << length.count() << "纳秒, "
        << length.count() / 1000000 << " 毫秒" << std::endl;
#endif
}

int main () {
    // query();
    test1();
    return 0;
}