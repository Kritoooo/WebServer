#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "locker.h"
#include <sys/uio.h>

class http_conn
{
public:
    static inline int m_epollfd = -1;          // 所有的socket上的事件都能被注册到同一个epoll上
    static inline int m_user_count = 0;        // 统计用户的数量
    static const int READ_BUFFER_SIZE = 2048;  // 读缓冲区大小
    static const int WRITE_BUFFER_SIZE = 2048; // 写缓冲区大小

    // HTTP请求方法
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    /*
        主状态机状态
        CHECK_STATE_REQUESTLINE:当前正在分析请求行
        CHECK_STATE_HEADER:当前正在分析头部字段
        CHECK_STATE_CONTENT:当前正在解析请求体
    */
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    
    // 从状态机状态
    // 1.读取到一个完整的行 2.行出错 3.行数据不完整
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };

    
    // 服务器处理HTTP请求的可能结果
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };

    http_conn() {}
    ~http_conn() {}

    void init(int sockfd, const sockaddr_in &addr);
    void process();    // 解析http请求、处理客户端请求
    void close_conn(); // 关闭连接
    bool read();       // 非阻塞读
    bool write();      // 非阻塞写

    HTTP_CODE process_read(); // 解析HTTP请求
    HTTP_CODE parse_request_line(char * text); // 解析请求首行
    HTTP_CODE parse_headers(char * text); // 解析请求头
    HTTP_CODE parse_content(char * text);  // 解析请求体

    LINE_STATUS parse_line();

private:
    int m_sockfd;                      // 该HTTP连接的socket
    sockaddr_in m_address;             // 通信的socket地址
    char m_read_buf[READ_BUFFER_SIZE]; // 读缓冲区
    int m_read_idx;
    ; // 标识读缓冲区中已经读入的客户端数据的最后一个字节的下一个字节
};

#endif