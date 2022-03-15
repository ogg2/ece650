#include "myfunc.h"

void createtable_player(connection * C){
    string sql = "CREATE TABLE PLAYER("  \
    "PLAYER_ID SERIAL PRIMARY KEY,"\
    "TEAM_ID INT," \
    "UNIFORM_NUM INT ," \
    "FIRST_NAME           TEXT    NOT NULL," \
    "LAST_NAME           TEXT    NOT NULL," \
    "MPG         INT," \
    "PPG         INT," \
    "RPG         INT," \
    "APG         INT," \
    "SPG         REAL," \
    "BPG         REAL,"\
    "FOREIGN KEY (TEAM_ID) REFERENCES TEAM(TEAM_ID) );";

    work W(*C);
    W.exec( sql );
    W.commit();
    //cout << "Table PLAYER created successfully" << endl;
}

void createtable_team(connection * C){
    string sql = "CREATE TABLE TEAM("  \
    "TEAM_ID SERIAL PRIMARY KEY," \
    "NAME           TEXT    NOT NULL," \
    "STATE_ID INT," \
    "COLOR_ID INT," \
    "WINS INT," \
    "LOSSES INT,"\
    "FOREIGN KEY (STATE_ID) REFERENCES STATE(STATE_ID),"\
    "FOREIGN KEY (COLOR_ID) REFERENCES COLOR(COLOR_ID) );";

    work W(*C);
    W.exec( sql );
    W.commit();
    //cout << "Table TEAM created successfully" << endl;
}


void createtable_state(connection * C){
    string sql = "CREATE TABLE STATE("  \
    "STATE_ID SERIAL PRIMARY KEY," \
      "NAME           TEXT    NOT NULL );" ;

    work W(*C);
    W.exec( sql );
    W.commit();
    //cout << "Table STATE created successfully" << endl;
}

void createtable_color(connection * C){
    string sql = "CREATE TABLE COLOR("  \
     "COLOR_ID SERIAL PRIMARY KEY," \
      "NAME           TEXT    NOT NULL );" ;

    work W(*C);
    W.exec( sql );
    W.commit();
    //cout << "Table COLOR created successfully" << endl;
}

/* create ends*/

/* add player.txt, team.txt, state.txt, color.txt */
void break_add(connection * C, string s,string filename){
    vector<string>arr;
    size_t i = 0;
    string curr="";
    while(i < s.length()){
        if(s[i] == ' '){
            arr.push_back(curr);
            curr="";
        }else{
            curr+=s[i];
        }
        i++;
    }
    arr.push_back(curr);
    
    //add player
    if(filename == "player.txt"){
        add_player(C,convert<int>(arr[1]),convert<int>(arr[2]),arr[3],arr[4],convert<int>(arr[5]),convert<int>(arr[6]),convert<int>(arr[7]),convert<int>(arr[8]),convert<double>(arr[9]),convert<double>(arr[10]));
    }else if (filename == "team.txt"){
        add_team(C,arr[1],convert<int>(arr[2]),convert<int>(arr[3]),convert<int>(arr[4]),convert<int>(arr[5]));
    }else if (filename == "state.txt"){
        add_state(C,arr[1]);
    }else if(filename == "color.txt"){
        add_color(C,arr[1]);
    }
}

void readfile(connection * C,string filename){
    ifstream infile;
    infile.open(filename);
    assert(infile.is_open());

    string s;
    while(getline(infile,s)){
        break_add(C, s,filename);
    }
    infile.close();
}



/* drop table */
void droptable(connection *C){
    string sql = "DROP TABLE IF EXISTS PLAYER;";
    sql += "DROP TABLE IF EXISTS TEAM;";
    sql += "DROP TABLE IF EXISTS STATE;";
    sql += "DROP TABLE IF EXISTS COLOR;";
    work W(*C);
    W.exec( sql );
    W.commit();
    //cout << "Table droped successfully" << endl;
}
