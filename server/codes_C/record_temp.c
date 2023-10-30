#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include<signal.h>
#include <sqlite3.h>
#include "dht11.h"
#include "28byj-48.h"

#define SUCCESS 0
#define READ_TEMP_FAILED 1
#define OPEN_DB_FAILED 2
#define CREATE_TABLE_FAILED 3
#define INSERT_TEMP_READING_FAILED 4
#define INVALID_ARGS 5

#define MAX_TEMP_READ_ATTEMPTS 3600
#define DHT11_GPIO 14
#define MAX_ERR_COUNT 10
#define SIGINT_TIMEOUT_US 1000

sqlite3* db;

void handle_sigint(int sig) {
  printf("\nClosing database\n");
  sqlite3_close(db);
}

int open_db(char* db_path) {
  char* err_msg = 0;
  // system("mkdir -p /home/$USER/.awcs");
  // char* USER = getenv("USER");
  // char db_path[100];
  // sprintf(db_path, "/home/%s/.awcs/temp.db", USER);
  int rc = sqlite3_open(db_path, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return OPEN_DB_FAILED;
  }
  char* sql =
    "CREATE TABLE IF NOT EXISTS temp ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " temp INTEGER,"
    " average_temp INTEGER,"
    " time DATETIME DEFAULT CURRENT_TIMESTAMP"
    ");";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return CREATE_TABLE_FAILED;
  }
  return SUCCESS;
}

int read_temp(int* err) {
  dht11_data_t dht11_data;
  dht11_line_t* dht11_line = init_dht11(DHT11_GPIO);
  int _err;
  for (int i = 0; i < MAX_TEMP_READ_ATTEMPTS; i++) {
    _err = read_dht11(dht11_line, &dht11_data);
    if (_err == 0) break;
    sleep(1);
  }
  *err = _err;
  return dht11_data.temperature_int;
}

int insert_temp_reading(sqlite3* db, int temp) {
  char sql_insert[100];
  sprintf(sql_insert, "INSERT INTO temp (temp) VALUES (%d);", temp);
  char* err_msg;
  int rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return INSERT_TEMP_READING_FAILED;
  }
  return SUCCESS;
}

int main(int argc, char const* argv[]) {
  signal(SIGINT, handle_sigint);
  // get databse path from argv
  if (argc != 2) {
    printf("Usage: %s <database_path>\n", argv[0]);
    return INVALID_ARGS;
  }
  char* db_path = argv[1];
  int err_count = 0;
  int err;
  open_db(db_path);
  int temp = read_temp(&err);
  err = insert_temp_reading(db, temp);
  if (err) return err;
  return 0;
}
