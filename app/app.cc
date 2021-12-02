#include "app.hpp"

void app_layer::app_init(){
    
    std::string path,db,config;

    this->fp.find("sqlite3","path",path);
    this->fp.find("sqlite3","db",db);
    this->fp.find("sqlite3","config",config);

    std::cout << "app init start ... ...\n";
    std::cout << path << std::endl;
    std::cout << db << std::endl;
    std::cout << config << std::endl;
    sqlite(path.c_str(),db.c_str());
    sqlite.init(); //开始初始化数据库,获取数据库的句柄
    if(!sqlite.check_tab(db)) //如果数据表不存在,则创建数据表
        sqlite.config(config);
}

//缓冲区可以被共享到多个进程当中,大概就是32 位系统的 1页的大小 4096Kb,但是只能通过缓冲区操作,数据库
//将这个缓冲层做多进程的内存共享,只要发现缓冲区发生了切实可行的变化,就马上更新数据库,只有写操作

void cache_layer::sem_init(){ 
   
}

void xsi_ipc::xsi_ipc_init(){
    std::exception error;
    try
    {
        xsi_ipc_key = ftok(key_path,key_id); //使用的路径必须为全名称路径
        if(xsi_ipc_key < 0)
            throw error; //查看系统中错误的原因,不知道会不会生效这部分

        switch(type){
            case SEM:
            semun sem_union;
            sem_union.val = 0;
            if(semctl(xsi_ipc_key,0,SETVAL,&sem_union) < 0) //设置信号量,设置单个信号量,用于多进程之间进行互斥
                throw error;
            
            break;
            case MSG_QUEUE: //进程之间同行使用的消息队列
               if((msg_queue_id = msgget(xsi_ipc_key,IPC_CREAT|0666)) < 0 )
                    throw error;
                else
                    break;
            case SHM:
                if((shm_id = shmget(xsi_ipc_key,MEM_SHARE,0666))< 0)
                    throw error;
                
                if((shm_addr = (char*)shmat(shm_id,0,0)) == NULL)
                    throw error;
                
                break;
            default:
                throw error;//可以自定义错误类型
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

/*
* 
*/
void xsi_ipc::snd(msg_buf* buf,size_t size){
    std::exception error;
    try
    {
       switch (type)
       {
       case MSG_QUEUE:
           if(msgsnd(msg_queue_id,(char*)buf,size,IPC_NOWAIT) < 0) //不阻塞直接发送
            throw error;     
       default:
           break;
       }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void xsi_ipc::rcv(msg_buf* buf,size_t size){
    std::exception error;
    try
    {
       switch (type)
       {
       case MSG_QUEUE:
           if(msgrcv(msg_queue_id,(char*)buf,size,0,IPC_NOWAIT) < 0) //不阻塞IPC通道,应该立即返回
                throw error;
       default:
           break;
       }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void xsi_ipc::sem_p(){
   std::exception error;
   sembuf tmp;
   tmp.sem_flg = SEM_UNDO;
   tmp.sem_num = 0;
   tmp.sem_op = -1;

   try
   {
       if(semop(xsi_ipc_key,&tmp,1) < 0)
            throw error;
   }
   catch(const std::exception& e)
   {
       std::cerr << e.what() << '\n';
   }
   
}

void xsi_ipc::sem_v(){
   std::exception error;
   sembuf tmp;
   tmp.sem_flg = SEM_UNDO;
   tmp.sem_num = 0;
   tmp.sem_op = 1;

   try
   {
       if(semop(xsi_ipc_key,&tmp,1) < 0)
            throw error;
   }
   catch(const std::exception& e)
   {
       std::cerr << e.what() << '\n';
   }
}

void xsi_ipc::sem_del(){
    std::exception error;
    try
    {   
        semun tmp;
        if(semctl(xsi_ipc_key,0,IPC_RMID,tmp) < 0)
            throw error;
    }
    catch(const std::exception& e)
    {
            std::cerr << e.what() << '\n';
    }    
}

xsi_ipc::~xsi_ipc(){
    switch (type)
    {
    case SEM:
        sem_del();
        break;
    case MSG_QUEUE:
        msgctl(msg_queue_id,IPC_RMID,NULL);
        break;
    case SHM:
        shmctl(shm_id,IPC_RMID,NULL); //删除共享内存区间
        break;
    default:
        break;
    }
}

void xsi_ipc::write(char* buf,int offset,int size){
    memcpy(shm_addr+offset,buf,size);
}

void xsi_ipc::read(char*buf,int offset,int size){
    memcpy(buf,shm_addr+offset,size);
}