#include <iostream>
#include "config.hpp"
#include "platform.hpp"
#include <sys/stat.h>
#include <unistd.h>

extern void register_device(platform* plat,json& fp);

namespace PLATFORM{
    xilinx xilinx_platform;
    altera altera_platform;
};

int main(int argc,char** argv){ //主函数中调用动态库,分别进行调试

    using namespace PLATFORM; //已经实例化的平台对象列表,以供平台查询使用

    init* platform_init; //平台初始化接口
    std::string plat; // 平台版本的
    log* query_info; //平台信息查询接口

    try{
        if(argc != 2) //必须初始化参数为两个参数
            throw 1;
        
        if(access(argv[1],F_OK) < 0) // 检测文件是否 真的存在
            throw 2;

        json config_platform(argv[1]); //获取json 控制对象
        config_platform.find("platform",plat);

        if(!plat.find("xilinx")){//开始配置xilinx 平台
            platform_init = &xilinx_platform;                       // xilinx 设备初始化和 注册方式
            register_device(&xilinx_platform,config_platform);      // 选择注册平台的加载方式
            query_info = &xilinx_platform;
        }else if(!plat.find("altera")){                             //开始配置altera 平台;
            platform_init = &altera_platform;                       // altera 设备初始化和 注册方式
            register_device(&altera_platform,config_platform);
            query_info = &altera_platform;
        }else{
            throw 3;
        }

        platform_init->init_class(); //平台的初始化方式
        
        query_info->info();

        // std::cout << "******************************************************" << std::endl;
        // std::cout << platform_version << std::endl; //打印由Makefile 生成的platform 版本信息

    }catch(MAIN_THREAD_ERROR tmp){
        switch(tmp){
            case CONFIG_FILE_NONE:
                std::cout << "platform thread need config file" << std::endl; return -1;
            case ARGC_ARGV_ERROR:
                std::cout << "config file not exit" << std::endl; return -2;
            case DONT_MATCH:
                std::cout << "can't find match platform,please check your config file format" << std::endl; return -3;
            case CONFIG_ERROR:
                std::cout << "platform config error，please look /var/log/platform.log" << std::endl ; return -4; //查看平台错误日志
            case INIT_ERROR:
                std::cout << "platform init error,please look /var/log/platform.log" << std::endl; return -5; //查看平台错误日志
        }
    }

    return 0;
}