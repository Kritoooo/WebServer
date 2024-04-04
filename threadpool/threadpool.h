#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>
#include "../http/http_conn.h"
#include <exception>
#include <memory>
#include "../locker/locker.h"
#include <cstdio>

// 线程池类，定义成模板类，为了代码复用，模板参数T是任务类
template<typename T>
class threadpool {
public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    bool append(T* request);
private:
    static void* worker(void* arg);
    void run();

private:
    int m_thread_number;
    pthread_t* m_threads;
    // 请求队列最多的任务数
    int m_max_requests;
    // 请求队列
    std::list<T*> m_workqueue;
    // 互斥锁
    locker m_queuelocker;
    // 判断是否有任务需要处理
    sem m_queuestat;
    // 是否结束线程
    bool m_stop;

};
#endif
