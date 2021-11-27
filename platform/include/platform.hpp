#ifndef __PLATFORM_HEAD
#define __PLATFORM_HEAD

#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include "../../config/include/json.hpp"

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
    virtual void info() = 0; //日志的基本信息仅供参考,查看日志的使用方法
    // virtual void write(const char* file,void* buf) = 0; //抽象读写接口
};

class init{
    public:
    std::string platform_type;
    json fp;
    virtual void init_class() = 0;          //可以访问的抽象接口  
};

/*device 作为主要接口 访问所有的设备信息*/
class device:public init,public enable{ //允许子类设备加载重构,上层封装的类设备,使能状态设置为外界不可访问;
    public:
    virtual void config(json& fp) = 0;    //可以访问的抽象接口
    virtual size_t read(int reg,void* buf) = 0; //返回从设备中读取的数据
    virtual bool write(int reg, void* buf) = 0; //向设备中写入配置数据
    virtual void info() = 0; //打印设备名称,表明设备类型
};

class open_dev{ //可能会有一些私有数据进行使用,需要用一个类进行声明; 判断设备是否可用;
    public:
    device* operator()(device& dev); //打开设备使用设备的读写接口,改变设备的指向接口;
};

class close_dev{
    public:
    device* operator()(device& dev); //release 释放正在使用的设备; 判断设备是否可以关闭,改变设备的指向接口;
};

class platform:public init,private enable,public log{
    public:
    void set_config_file(json& js);
    bool platform_register(device* dev); //在平台上注册设备
    bool platform_unregister(device* dev); //删除已经挂载的设备
    bool check_dev(const device& dev) const; //查看设备是否存在,对比完成之后查看设备是否还存在
    virtual void init_class();
    virtual void info(); //将所有的器件的信息进行收集和查找使用
    virtual ~platform(); //虚拟重载掉所有的平台
    private:
    std::list<device*> dev; //注册生成的设备进行使用
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