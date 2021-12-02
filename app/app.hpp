#ifndef __NET_APP__
#define __NET_APP__

#include "../sqlite3/sqlite3.hpp"
#include "../platform/include/platform.hpp"

static const int MSG_PAGE = 252;
static const int MEM_SHARE = 8096;
enum MSG_QUEUE_TYPE{
    WEB_DESKTOP_USE_QUEUE=0x1,
    CLI_USE_QUEUE=0x2
};

enum CACHE_TYPE_E{
    CACHE_UPDATE = 0x0,
    CACHE_NO_CHANGE = 0x1
};

enum XSI_IPC_E{
    MSG_QUEUE=0x0, //消息队列
    SEM=0x1, //信号量
    SHM=0x2 //共享内存 效率最高
};

//是否需要建立缓存队列
struct msg_buf{ //256个字节
    long msg_type;
    char text[MSG_PAGE]; //正文中最大的缓冲区
};

class xsi_ipc{ //缺点是不可以被IO口复用使用
public:
    xsi_ipc(char* kp,int id,XSI_IPC_E t):key_path(kp),key_id(id),type(t){}
    void xsi_ipc_init();
    void snd(msg_buf *buf,size_t size);
    void write(char*buf,int offset,int size);
    void read(char*buf,int offset,int size);
    void sem_p();
    void sem_v();
    void sem_del();
    void rcv(msg_buf *buf,size_t size);
    ~xsi_ipc(); //对现有的IPC 进程间使用的 IPC 键进行析构避免其他进程访问出错
private:
    XSI_IPC_E type;
    char* key_path; //key_t 所使用的需要计算的路径
    int key_id; //key_t 需要计算的key id 的值的大小
    key_t xsi_ipc_key;
    int msg_queue_id;
    int shm_id;
    char* shm_addr;
};

/*我对POSIX的标准库更有信心,前辈说,pthread库也是gnu posix 的标准库更出色*/
class cache_layer{ 
public:
    void sem_init();
    void update_cache_layer(); //更新缓冲区
    CACHE_TYPE_E cache_status(); //决定是否更新缓冲层
private:
    char* cache; //用于共享的数据区域
};

class app_layer{
public:
    app_layer(){}
    app_layer(json& f,platform_interface* ptr):fp(f),plat_ptr(ptr){}
    void operator()(json& f,platform_interface*p){
        this->fp = f;
        this->plat_ptr = p;
    }
    void app_init();
private:
    json fp;
    Sqlite3 sqlite;
    platform_interface* plat_ptr;
};

#endif