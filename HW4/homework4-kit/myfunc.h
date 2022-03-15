#include <iostream>
#include <fstream>
#include <assert.h>
#include <pqxx/pqxx> 


#include "exerciser.h"
#include "query_funcs.h"


using namespace std;
using namespace pqxx;


//reference: https://blog.csdn.net/puppylpg/article/details/51260100
template<class out_type,class in_value>
out_type convert(const in_value& t)
{
    stringstream stream;
    out_type result;        //result of convert

    stream << t;            //convey t to stream
    stream >> result;       //write to result
    return result;
}

void createtable_player(connection * C);
void createtable_team(connection * C);
void createtable_state(connection * C);
void createtable_color(connection * C);

void break_add(connection * C, string s,string label);
void readfile(connection * C, string filename);


void droptable(connection *C);