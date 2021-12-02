#include "platform.hpp"

void platform_config(device* ptr){ //配置文件进行传递
   ptr->config(*(ptr->fp));
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

/*FIXME: 安全隐患重复设备注册,需要特征码进行区分*/
bool platform::platform_register(device* dev){
    if(dev == nullptr)
        return false;
    dev->fp = this->fp;     
    dev->set_true();        
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

device* platform::search_dev(std::string name){
    std::list<device*>::iterator itr;
    std::list<device*>::iterator itr_start = this->dev.begin();
    std::list<device*>::iterator itr_end = this->dev.end();
    for( itr = itr_start ; itr != itr_end ; itr++){
        if(name == (*itr)->get_dev_type()){
            return (*itr);
        }
    }
    return nullptr;
}

void enable::set_true(void){
    this->status = true;
}

void enable::set_false(void){
    this->status = false;
}

bool enable::check_status(void){
    return this->status;
}

/*FIXME: 存在安全隐患,没有将在register 函数模块中注册的归还内存给系统*/

platform::~platform(){
    dev.clear();
}

void platform::info(){ //还有日志类型的相关的开关
    std::cout << "platform:" <<this->platform_type << std::endl;
    std::for_each(dev.begin(),dev.end(),platform_info);
}

std::string device_status_check(bool it){
    return (it)? "on" : "off";
}

void platform::set_config_file(json& fp){
    this->fp = &fp;
}