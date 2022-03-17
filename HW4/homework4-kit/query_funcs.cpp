#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);

    string sql;
    stringstream ss;
    ss << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) ";
    ss << "VALUES (" << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", " << W.quote(last_name) << ", ";
    ss << mpg << ", " << ppg << ", " << rpg << ", " << apg << ", " << spg << ", " << bpg << " );";
    sql = ss.str();
    //cout << sql << endl;

    W.exec( sql );
    W.commit();
    //cout << "PLAYER Records created successfully" << endl;
}



void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    string sql;
    stringstream ss;

    ss << "INSERT INTO TEAM ( NAME, STATE_ID, COLOR_ID, WINS, LOSSES)";
    ss << "VALUES (" << W.quote(name) << ", " << state_id << ", " << color_id << ", ";
    ss << wins << ", " << losses << " );";
    sql = ss.str();
    //cout << sql << endl;

    W.exec( sql );
    W.commit();
    //cout << "TEAM Records created successfully" << endl;
}


void add_state(connection *C, string name)
{
    work W(*C);
    string sql;
    stringstream ss;

    ss << "INSERT INTO STATE (NAME)";
    ss << "VALUES (" << W.quote(name) <<  " );";
    sql = ss.str();
    //cout << sql << endl;

    W.exec( sql );
    W.commit();
    //cout << "STATE Records created successfully" << endl;
}


void add_color(connection *C, string name)
{
    work W(*C);
    string sql;
    stringstream ss;

    ss << "INSERT INTO COLOR (NAME)";
    ss << "VALUES (" << W.quote(name) <<  " );";
    sql = ss.str();
    //cout << sql << endl;

    W.exec( sql );
    W.commit();
    //cout << "COLOR Records created successfully" << endl;
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    string sql = "SELECT * FROM PLAYER WHERE MPG BETWEEN 35 AND 40;";
    stringstream ss;
    ss << "SELECT * FROM PLAYER";
    bool has_condition = false;
    if(use_mpg){
        if(!has_condition){
            ss << " WHERE ";
            has_condition = true;
        }else{
            ss << " AND ";
        }
        ss<< " MPG BETWEEN " << min_mpg << " AND " << max_mpg;
    }
    if(use_ppg){
        if(!has_condition){
            ss << " WHERE ";
            has_condition = true;
        }else{
            ss << " AND ";
        }
        ss<< " PPG BETWEEN " << min_ppg << " AND " << max_ppg;
    }
    if(use_rpg){
        if(!has_condition){
            ss << " WHERE ";
            has_condition = true;
        }else{
            ss << " AND ";
        }
        ss<< " RPG BETWEEN " << min_rpg << " AND " << max_rpg;
    }
    if(use_apg){
        if(!has_condition){
            ss << " WHERE ";
            has_condition = true;
        }else{
            ss << " AND ";
        }
        ss<< " APG BETWEEN " << min_apg << " AND " << max_apg;
    }
    if(use_spg){
        if(!has_condition){
            ss << " WHERE ";
            has_condition = true;
        }else{
            ss << " AND ";
        }
        ss<< " SPG BETWEEN " << min_spg << " AND " << max_spg;
    }
    if(use_bpg){
        if(!has_condition){
            ss << " WHERE ";
            has_condition = true;
        }else{
            ss << " AND ";
        }
        ss<< " BPG BETWEEN " << min_bpg << " AND " << max_bpg;
    }
    ss << ";";

    sql = ss.str();
    nontransaction N(*C);      
    result R( N.exec( sql ));
    

    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;

    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<int>() << " " << c[1].as<int>() << " " <<  c[2].as<int>() << " ";
      cout << c[3].as<string>() << " " << c[4].as<string>() << " " << c[5].as<int>() << " ";
      cout << c[6].as<int>() << " " << c[7].as<int>() << " " << c[8].as<int>() << " ";
      cout << c[9].as<double>() << " " << c[10].as<double>() << endl;
    }
    //cout << "QUERY 1 Operation done successfully" << endl;
}


void query2(connection *C, string team_color)
{
    work W(*C);
    cout << "NAME" << endl;
    string sql = "SELECT NAME FROM TEAM WHERE COLOR_ID.NAME = team_color";
    stringstream ss;
    ss << "SELECT TEAM.NAME FROM TEAM INNER JOIN COLOR ON TEAM.COLOR_ID = COLOR.COLOR_ID ";
    ss << "WHERE COLOR.NAME = " << W.quote(team_color);
    ss << ";";
    W.commit();

    sql = ss.str();
    nontransaction N(*C);      
    result R( N.exec( sql ));

    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<string>()<<endl;
    }
    //cout << "QUERY 2 Operation done successfully" << endl;
}


void query3(connection *C, string team_name)
{
    work W(*C);
    cout<<"FIRST_NAME LAST_NAME"<<endl;
    //order by
    string sql = "SELECT FIRST_NAME, LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID ";
    stringstream ss;
    ss << sql;
    ss << " AND TEAM.NAME = " << W.quote(team_name);
    ss << " ORDER BY PLAYER.PPG DESC";
    ss << ";";
    W.commit();

    sql = ss.str();
    nontransaction N(*C);      
    result R( N.exec( sql ));

    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<string>()<<" "<< c[1].as<string>()<<endl;
    }
    //cout << "QUERY 3 Operation done successfully" << endl;
}


void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    //cout<<"FIRST_NAME LAST_NAME UNIFORM_NUM"<<endl;
    cout<<"UNIFORM_NUM FIRST_NAME LAST_NAME"<<endl;

    //order by
    string sql = "SELECT FIRST_NAME, LAST_NAME, UNIFORM_NUM FROM PLAYER, TEAM, STATE, COLOR WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID ";
    stringstream ss;
    ss << sql;
    ss <<" AND TEAM.STATE_ID = STATE.STATE_ID ";
    ss <<" AND TEAM.COLOR_ID = COLOR.COLOR_ID";
    ss << " AND STATE.NAME = " << W.quote(team_state);
    ss << " AND COLOR.NAME = " << W.quote(team_color);
    ss << ";";
    W.commit();

    sql = ss.str();
    nontransaction N(*C);      
    result R( N.exec( sql ));

    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout <<  c[2].as<int>()<<" "<< c[0].as<string>() <<" "<< c[1].as<string>() << endl;
    }
    //cout << "QUERY 4 Operation done successfully" << endl;
}


void query5(connection *C, int num_wins)
{
    work W(*C);
    cout<<"FIRST_NAME LAST_NAME NAME WINS"<<endl;
    //order by
    string sql = "SELECT FIRST_NAME, LAST_NAME, TEAM.NAME,TEAM.WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID ";
    stringstream ss;
    ss << sql;
    ss << " AND TEAM.WINS > " << num_wins;
    ss << ";";
    W.commit();

    sql = ss.str();
    nontransaction N(*C);      
    result R( N.exec( sql ));

    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<string>()<<" "<< c[1].as<string>() <<" "<< c[2].as<string>() << " " << c[3].as<int>() << endl;
    }
    //cout << "QUERY 5 Operation done successfully" << endl;
}
