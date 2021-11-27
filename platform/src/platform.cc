#include "platform.hpp"

void platform_config(device* ptr){ //配置文件进行传递
   ptr->config(ptr->fp);
}

void platform_init(device* ptr){
    ptr->init_class();
}

void platform_info(device* ptr){
    ptr->info();
}

void platform::init_class(){//平台的配置方法
    std::for_each(dev.begin(),dev.end(),platform_init);
}

bool platform::platform_register(device* dev){
    if(dev == nullptr)
        return false;
    dev->fp = this->fp;     //赋值配置文件的所有属性
    dev->set_true();        //代表设备正在被使用
    this->dev.push_back(dev);
    return true;
}

bool platform::platform_unregister(device *dev){
    if(dev == nullptr)
        return false;
    dev->set_false();
    this->dev.remove(dev);
    return true;
}

void enable::set_true(void){ //单独对数据进行保护
    this->status = true;
}

void enable::set_false(void){ //单独对数据进行保护
    this->status = false;
}

bool enable::check_status(void){ //返回使能使用的状态值
    return this->status;
}

void platform_del_dev(device* ptr){
    std::cout << "clear dev start: "<< std::endl;
    ptr->info();
}

platform::~platform(){
    // std::cout << "platform del dev start \n";
    std::for_each(dev.begin(),dev.end(),platform_del_dev); //释放开辟的内存空间,释放内存的使用
    dev.clear(); //清空平台所有挂载的设备
    // std::cout << "platform del dev ok\n";
}

void platform::info(){ //还有日志类型的相关的开关
    std::cout << "platform:" <<this->platform_type << std::endl;
    std::for_each(dev.begin(),dev.end(),platform_info);
}

std::string device_status_check(bool it){
    return (it)? "on" : "off";
}