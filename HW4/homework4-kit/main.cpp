#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"
#include "myfunc.h"

using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  
  
  //droptable
  droptable(C);

  //createtable
  createtable_state(C);
  createtable_color(C);
  createtable_team(C);
  createtable_player(C);

  //load txt
  readfile(C,"state.txt");
  readfile(C,"color.txt");
  readfile(C,"team.txt");
  readfile(C,"player.txt");

  //test
  exercise(C);



  //Close database connection
  C->disconnect();

  return 0;
}


