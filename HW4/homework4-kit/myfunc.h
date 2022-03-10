#include <iostream>
#include <fstream>
#include <assert.h>
#include <pqxx/pqxx> 


#include "exerciser.h"
#include "query_funcs.h"


using namespace std;
using namespace pqxx;


template<class out_type,class in_value>
out_type convert(const in_value& t)
{
    stringstream stream;
    out_type result;        //这里存储转换结果

    stream << t;            //向流中传值
    stream >> result;       //向result中写入值
    return result;
}

void createtable_player(connection * C);
void createtable_team(connection * C);
void createtable_state(connection * C);
void createtable_color(connection * C);

void break_add(connection * C, string s,string label);
void readfile(connection * C, string filename);


void droptable(connection *C);