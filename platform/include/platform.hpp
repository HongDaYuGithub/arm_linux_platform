#ifndef __PLATFORM_HEAD
#define __PLATFORM_HEAD

#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include "../../config/include/json.hpp"
#include "../../sqlite3/sqlite3.hpp"
#include <semaphore.h> //用户信号量POSIX gnu 库 性能更值得信赖
#include <pthread.h> //用户信号量POSIX gnu 库性能更值得信赖
#include <sys/sem.h>
#include <sys/types.h> // XSI IPC 存在相关的数据类型
#include <sys/ipc.h> // IPC ipc 创建所使用的键位
#include <sys/msg.h> // IPC msg 消息队列
#include <sys/shm.h>

#define struct  struct __attribute__((packed))

union semun
{
   int val;
   semid_ds *buf;
   unsigned short* array;
};


enum MAIN_THREAD_ERROR{
    CONFIG_FILE_NONE = 1,
    ARGC_ARGV_ERROR = 2,
    DONT_MATCH = 3,
    CONFIG_ERROR = 4,
    INIT_ERROR = 5
};

class enable{
  public:
  enable(){}
  int index;
  void set_true();
  void set_false();
  bool check_status();
  private:
  bool status;
};

class log{
public:
    virtual void info() = 0;
};

class init{
    public:
    json* fp;
    virtual void init_class() = 0;    
};

/*device 作为主要接口 访问所有的设备信息*/
class device:public init,public enable{ //设备的竞态访问,可能会做到内核中,还有可能会做到用户层的软件底层中
    public:
    virtual void config(json& fp) = 0;    
    virtual size_t read(int reg,void* buf) = 0; 
    virtual bool write(int reg, void* buf) = 0; 
    virtual void info() = 0; 
    virtual std::string get_dev_type() = 0;
};

class open_dev{
    public:
    device* operator()(device& dev);
};

class close_dev{
    public:
    device* operator()(device& dev);
};

class platform_interface:public init{
public:
    virtual device* search_dev(std::string name) = 0;
};

//实际可以挂载的设备没有很多,所以使用list 结构完全够用,不需要使用AVL做查找
class platform:public platform_interface,private enable,public log{ 
    public:
    std::string platform_type;
    virtual device* search_dev(std::string name);
    void set_config_file(json& js);
    bool platform_register(device* dev); 
    bool platform_unregister(device* dev); 
    bool check_dev(const device& dev) const; 
    virtual void init_class();
    virtual void info(); 
    virtual ~platform(); 
    private:
    std::list<device*> dev;
};

class xilinx:public platform{
public:
    xilinx(){
        this->platform_type = "xilinx";
    }
};

class altera:public platform{
public:
    altera(){
        this->platform_type = "altera";
    }
};

std::string device_status_check(bool it);

#endif /* PLATFORM HEAD */