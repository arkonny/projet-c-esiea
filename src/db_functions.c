#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Handles rc errors with err_msg
int db_error_handler_err_msg(int rc, char *err_msg, char *prefix) {
  if (rc != SQLITE_OK) {
    if (prefix != NULL) {
      prefix = "SQL error: ";
    }
    fprintf(stderr, "%s%s\n", prefix, err_msg);
    sqlite3_free(err_msg);
    return 1;
  }
  return 0;
}

// Handles rc errors without err_msg
int db_error_handler(sqlite3 *db, int rc, char *prefix) {
  if (rc != SQLITE_OK) {
    if (prefix != NULL) {
      prefix = "SQL error: ";
    }
    fprintf(stderr, "%s%s\n", prefix, sqlite3_errmsg(db));
    return 1;
  }
  return 0;
}

// Create the database and tables
int db_create(sqlite3 *db) {
  char *err_msg = 0;

  int rc = sqlite3_open("library.db", &db);

  if (db_error_handler(db, rc, "Cannot open database: ")) {
    sqlite3_close(db);
    return rc;
  }

  char *sql_livres = "DROP TABLE IF EXISTS Livres;"
                     "CREATE TABLE Livres("
                     "ISBN INT PRIMARY KEY,"
                     "Titre TEXT,"
                     "Auteur TEXT,"
                     "Genre TEXTE,"
                     "Date_emprunt DATE,"
                     "Id_User" 
                     ");"
                     "CREATE TABLE Users("
                     "Id_User INT PRIMARY KEY,"
                     "Nom TEXT,"
                     "Prenom TEXT,"
                     "Adresse_mail TEXT,"
                     "Droits INT,"
                     "Public_key TEXT;";

  rc = sqlite3_exec(db, sql_livres, 0, 0, &err_msg);

  if (db_error_handler_err_msg(rc, err_msg, "SQL error: ")) {
    sqlite3_close(db);
  }

  return rc;
}

// Insert parameters in a query
int db_insert_query(sqlite3 *db, char *sql_insert, int nbr_of_parameters,
                    char *parameters_names[], char *parameters[],
                    sqlite3_stmt *res) {

  // Prepare the query
  int rc = sqlite3_prepare_v2(db, sql_insert, -1, &res, 0);

  if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
    // Bind the parameters
    for (int i = 0; i < nbr_of_parameters; i++) {
      int idx = sqlite3_bind_parameter_index(res, parameters[i]);
      sqlite3_bind_text(res, idx, parameters[i], -1, SQLITE_STATIC);
    }
  }

  return rc;
}

// Insert a book
int db_insert_book(sqlite3 *db, char *ISBN, char *Titre, char *Auteur,
                   char *Genre) {
  char *err_msg = 0;

  char *sql_insert =
      "INSERT INTO Livres VALUES(@ISBN, @Titre, @Auteur, @Genre, NULL, NULL);";

  // Initialize parameters for the query
  int nbr_of_parameters = 4;
  char *parameters_names[] = {"@ISBN", "@Titre", "@Auteur", "@Genre"};

  char *parameters[] = {ISBN, Titre, Auteur, Genre};
  sqlite3_stmt *res;

  // Insert the book by calling db_insert_query
  int rc = db_insert_query(db, sql_insert, nbr_of_parameters, parameters_names,
                           parameters, res);
  sqlite3_finalize(res);

  return rc;
}

// Borrow a book
int db_borrow_book(sqlite3 *db, char *ISBN, int Id_User, char *Date_emprunt) {
  char *sql_insert = "UPDATE Livres SET Date_emprunt = @Date_emprunt, Id_User "
                     "= @Id_User WHERE ISBN = @ISBN;";

  // Prepare the query
  sqlite3_stmt *res;
  int rc = sqlite3_prepare_v2(db, sql_insert, -1, &res, 0);

  if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
    // Bind the parameters
    sqlite3_bind_text(res, 0, Date_emprunt, -1, SQLITE_STATIC);
		sqlite3_bind_int(res, 1, Id_User);
		sqlite3_bind_text(res, 2, ISBN, -1, SQLITE_STATIC);
  }

  sqlite3_finalize(res);
  return rc;
}
