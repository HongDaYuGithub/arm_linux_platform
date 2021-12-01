#include "sqlite3.hpp"
#include "../platform/include/platform.hpp"

int sqlite_common_callback(void* data,int argc,char** argv,char** Col){ //简单打印查询信息
    for(int i = 0 ; i < argc ; i++){
        std::cout << argv[i] << std::endl;
    }
}

void Sqlite3::init(){
    try{
        int ret = sqlite3_open(db_path,&database);
        if(ret < 0)
            throw sqlite3_errstr(ret);
        
    }catch(const char* error){
        std::cout << "db name: " << db_path <<" error: " << error << std::endl;
    }
}

void Sqlite3::sql(const char* _sql){
    try{
        char* errmsg;
        int ret = sqlite3_exec(database,_sql,NULL,NULL,&errmsg);
        if(ret < 0)
            throw errmsg;
    }catch(char* e){
        std::cout << e << std::endl;
    }
}

Sqlite3::~Sqlite3(){
    sqlite3_close(database); //关闭数据库
}

void Sqlite3::find(const char* _sql){
    try{
        char* errMsg;
        int ret = sqlite3_exec(database,_sql,callback,NULL,&errMsg);
        if(ret < 0)
            throw errMsg;
    }catch(char* e){
        std::cout << e << std::endl;
    }
}

void Sqlite3::update_callback(sqlite3_callback call){
    this->callback = call; //重新更新callback的调用操作
}

void Sqlite3::operator()(const char* p,const char* n){
    this->db_path = p;
    this->db_table = n;
}

static int check_tab_callback(void *buf,int argc,char** argv,char** Col)
{
    if(argc&0x1){
        int st = strtol(argv[0],NULL,16);
        if(!buf)
            *((int*)buf) = st; //返回相应的查询状态
    }
}

bool Sqlite3::check_tab(std::string tab){
    std::string check_sql = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name =";
    check_sql += std::string(this->db_table);

    std::cout << check_sql << std::endl;
    int st; //sqlite3 中查询数据表是否存在
    char* errMsg;
    int ret = sqlite3_exec(database,check_sql.c_str(),check_tab_callback,(int*)&st,&errMsg);
    if(ret < 0){
        std::cout << errMsg << std::endl;
        exit(1); //程序退出
    }

    return st? true : false;
}

void Sqlite3::config(std::string file){
    file_attr attr(file.c_str());
    std::vector<std::string> buf;
    char tmp[256]; //1行的数据一般情况下不会超过256个字节
    std::cout << attr.size() << std::endl;
    bool tb_head = false;
    bool tb_first = false;
    std::string sql_in;
    std::exception error;
    try
    {
       FILE* fp = fopen(file.c_str(),"r");
       if(!fp)
            throw error;

        while(!feof(fp)){
            fgets(tmp,sizeof(tmp),fp);
            if(tb_head){
                sql_in = "CREATE TABLE ";
                sql_in += std::string(db_table);
                sql_in += "(";
                sql_in += std::string(tmp);
                sql_in += ");";
                sql(sql_in.c_str());
                tb_head = true;
                sql_in.clear();
            }
            // TODO: 之前做的sqlit的高效率写入代码,移植到C++
            // if (insert_flag == 0) {
            //         sprintf(buffer_loop,
            //                 "SELECT \'%#x\' AS ID,\'%s\' AS NAME,\'%s\' AS VALUE,\'%s\' AS TYPE,%d AS MODE,"
            //                 "%d AS ATTR",
            //                 search->index, search->name, search->value, search->value_type, search->mode,
            //                 search->select);
            //     } else {
            //         memset(tmp, 0, sizeof(tmp));
            //         sprintf(tmp, " UNION SELECT \'%#x\',\'%s\',\'%s\',\'%s\',\'%d\',\'%d\'",
            //                 search->index, search->name, search->value, search->value_type, search->mode,
            //                 search->select);
            //         strcat(buffer_loop, tmp);
            //     }

        }       
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
        
}