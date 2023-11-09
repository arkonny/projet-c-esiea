#include <sqlite3.h>
#include <stdio.h>

// Create database function
int db_create() {
  char *err_msg = 0;

  sqlite3 *db;
  int rc = sqlite3_open("library.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(&db));
    sqlite3_close(db);
    return 1;
  }

  char *sql_livres = "DROP TABLE IF EXISTS Livres;"
                     "CREATE TABLE Livres("
                     "ISBN INT PRIMARY KEY,"
                     "Titre TEXT,"
                     "Auteur TEXT,"
                     "Genre TEXTE,"
                     "Date_emprunt DATE,"
                     "Id_User"
                     ");";

  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 1;
  }

  sqlite3_close(db);
  return 0;
}
