#include <iostream>
#include <sqlite3.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <cstring>
using std::uint8_t;
using namespace std;
sqlite3 *db;
int rc = sqlite3_open("parking.db", &db);
char *zErrMsg = 0;
const char* data = "Callback function called";
int conn();
int createTable();
void insertData();
void menu();
void view(int id);
void updateVehicle(int id);
void viewAll();
void deleteVehicle(int id);
static void doBasics(string str);

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


int createTable(){
    char *sql;
    sql = "CREATE TABLE PARK(" \
 "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
 "OWNERNAME TEXT NOT NULL," \
 "VEHICLENUMBER TEXT NOT NULL," \
 "PAID INT NOT NULL );";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK){
        fprintf(stderr, "\nSQL ERROR: %s \n",zErrMsg);
        sqlite3_free(zErrMsg);
        return 1;
    } else{
        fprintf(stdout,"\nTABLE created successfully!!!");
    }
}
void insertData(){
    char* query;
    std::string sql;
    string ownerName;
    string vehicleNumber;
    int paid;
    std::cout<<"Enter User Data:\n";
    std::cout<<"Owner Name:\t";
    std::cin>>ownerName;
    std::cout<<"\nVehicle Number:\t";
    std::cin>>vehicleNumber;
    std::cout<<"\nPaid Money:\t";
    std::cin>>paid;
    std::stringstream ss;
    ss << "INSERT INTO PARK (OWNERNAME, VEHICLENUMBER, PAID)"\
 "VALUES('"<<ownerName<<"', '"<<vehicleNumber<<"', "<<paid<<");";
    sql = ss.str();
    strcpy(query, sql.c_str());
    rc = sqlite3_exec(db, query,callback, 0, &zErrMsg);
    if (rc != SQLITE_OK){
        fprintf(stderr, "\nSQL ERROR: %s \n",zErrMsg);
        sqlite3_free(zErrMsg);
    } else{
        fprintf(stdout,"\nData Inserted successfully!!!");
    }
}
void view(int id){
    char *sql;
    std::string query;
    std::stringstream ss;
    ss<<"SELECT * from PARK WHERE ID = "<<id<<";";
    query = ss.str();
    strcpy(sql, query.c_str());
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void* )data, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout<<stderr<<" SQL Error: "<<zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        std::cout<<"USER DATA!!!"<<std::endl;
    }
}

void updateVehicle(int id){
    std::string own;
    std::string vnum;
    std::string paid;
    char *sql;
    std::string query;
    std::stringstream ss;
    cout<<"Enter Data \n";
    cout<<"Owner Name:\t";
    cin>>own;
    cout<<"Vehicle Number:\t";
    cin>>vnum;
    cout<<"Paid Money:\t";
    cin>>paid;
    ss <<"UPDATE PARK set OWNERNAME = '"<<own<<"', VEHICLENUMBER = '"<<vnum<<"',PAID="<<paid<<" WHERE ID="<<id<<"; " \
 "SELECT * from PARK";
    query = ss.str();
    strcpy(sql, query.c_str());
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
}
void deleteVehicle(int id){
    char *sql;
    std::string query;
    std::stringstream ss;
    ss<<"DELETE from PARK where ID="<<id<<"; " \
 "SELECT * from PARK";
    query = ss.str();
    strcpy(sql, query.c_str());
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
}
void drop(){
    char* sql;
    sql = "DELETE FROM PARK";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
}
void viewAll(){
    char* sql;
    sql = "SELECT * FROM PARK";
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
    }
}
int main(int argc, char* argv[]) {
    conn();
    menu();
    sqlite3_close(db);
    return 0;
}
void menu(){
    std::cout<<"******Vehicle Type******\n";
    int choice;
    while (true){
        std::cout<<"[1]\tCreate Table\n[2]\tINSERT\n[3]\tVIEW\n[4]\tDELETE\n[5]\tUpdate\n[6]"
                   "\tView All\n[7]\tTruncate Data\n[8]\tEXIT\n";
        std::cin>>choice;
        switch(choice){
            case 1:{
                createTable();
            }
                break;
            case 2:{
                insertData();
            }
                break;
            case 3:{
                int id;
                std::cout<<"Enter ID:\t";
                std::cin>>id;
                view(id);
            }
                break;
            case 4:{
                int id;
                cout<<"Enter ID to Delete\n";
                cin>>id;
                deleteVehicle(id);
            }
                break;
            case 5:{
                int id;
                cout<<"Enter ID to Update\n";
                cin>>id;
                updateVehicle(id);
            }
                break;
            case 6:
                viewAll();
                break;
            case 7:{
                char ch = 'n';
                cout<<"WARNING!";
                cout<<"Are You Sure to delete whole data!!\n";
                cin>>ch;
                if(ch == 'Y' || ch == 'y'){
                    drop();
                }
            }
                break;
            case 8:{
                exit(0);
            }
            default:{
                cout<<"Invalid Input Entered!!!";
            }
        }
    }
}
int conn(){
    if(rc){
        fprintf(stderr,"Can't open database; %s \n",sqlite3_errmsg(db));
        return(0);
    } else{
        fprintf(stderr,"Database Successfully connected");
    }
}
