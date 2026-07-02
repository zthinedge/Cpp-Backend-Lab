#include "ConnectionPool.h"
MySqlConnectionPool::MySqlConnectionPool(size_t pool_size){
    if(pool_size==0){
        throw std::invalid_argument("pool_size must be greater than 0");
    }
    for(size_t i=0;i<pool_size;i++){
        auto conn = std::make_shared<MySqlConnection>(i + 1);
        conn->connect();
        connections_.push(conn);
    }
}

MySqlConnectionPool::~MySqlConnectionPool(){
    {
        std::lock_guard<std::mutex>lock(mutex_);
        stopping_=true;
    }
    //唤醒所有在等待被分配连接的线程--acquire
    cv_.notify_all();
}

std::shared_ptr<MySqlConnection> MySqlConnectionPool::acquire(){
    std::unique_lock<std::mutex>lock(mutex_);
    //阻塞休眠，让出cpu
    cv_.wait(lock,[this]{
        return stopping_ || !connections_.empty();
    });

    if(stopping_)return nullptr;

    auto conn =connections_.front();
    connections_.pop();

    //外部conn离开作用域，shared_ptr时析构调用我传进去的lambda --自定义删除器
    return std::shared_ptr<MySqlConnection>(conn.get(),[this,conn](MySqlConnection*){
        std::lock_guard<std::mutex>lock(mutex_);
        connections_.push(conn);
        //唤醒一个等待连接的线程
        cv_.notify_one();
    });
}
