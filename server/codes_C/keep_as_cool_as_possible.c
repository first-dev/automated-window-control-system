// c program that periodically check temperature using dht11 and calculate the average
// temperature of every n readings, if the temperature is rising then it will turn on the
// motor to close the window, if the temperature is falling then it will turn on the motor
// to open the window, if the temperature is stable then it will turn off the motor.

#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
#include "dht11.h"
#include "28byj-48.h"

#define SUCCESS 0
#define READ_TEMP_FAILED 1
#define OPEN_DB_FAILED 2
#define CREATE_TABLE_FAILED 3
#define INSERT_TEMP_READING_FAILED 4

// sqlite3* db;

// function that reads temperature
int read_temp() {
  dht11_data_t dht11_data;
  dht11_line_t* dht11_line = init_dht11(14);
  int err;
  for (int i = 0; i < 10; i++) {
    err = read_dht11(dht11_line, &dht11_data);
    if (err == 0) break;
    sleep(1);
  }
  if (err) return NULL;
  return dht11_data.temperature_int;
}

// function that opens the database
int open_db(sqlite3* db) {
  char* err_msg = 0;
  system("mkdir -p /home/$USER/.awcs");
  char* USER = getenv("USER");
  char db_path[100];
  sprintf(db_path, "/home/%s/.awcs/temp.db", USER);
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

// function that inserts temperature reading into database
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

// calculate average temperature of last n readings
int calculate_average(int sample_size) {
  sqlite3* db;
  int err = open_db(db);
  if (err) return err;
  sqlite3_stmt* res;
  char sql_select[100];
  sprintf(sql_select, "SELECT temp FROM temp ORDER BY time DESC LIMIT %d;", sample_size);
  err = sqlite3_prepare_v2(db, sql_select, -1, &res, 0);
  if (err != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    return 1;
  }
  int sum = 0;
  int count = 0;
  while (sqlite3_step(res) == SQLITE_ROW) {
    sum += sqlite3_column_int(res, 0);
    count++;
  }
  sqlite3_finalize(res);
  sqlite3_close(db);
  return sum / count;
}

// function that insert average temperature into database
int insert_average_temp(sqlite3* db, int average_temp) {
  char sql_insert[100];
  sprintf(sql_insert, "INSERT INTO temp (average_temp) VALUES (%d);", average_temp);
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
  sqlite3* db;
  int err = open_db(db);
  if (err) return err;
  while (1) {
    int temp = read_temp();
    insert_temp_reading(db, temp);
    int average = calculate_average(10);
    insert_average_temp(db, average);
    sleep(1);
  }

  return 0;
}
