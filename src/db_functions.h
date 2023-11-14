// Handles rc errors with err_msg
int db_error_handler_err_msg(int rc, char *err_msg, char *prefix)

// Handles rc errors without err_msg
int db_error_handler(sqlite3 *db, int rc, char *prefix)

// Create the database and tables
int db_create(sqlite3 *db);

// Insert parameters in a query
int db_insert_query(sqlite3 *db, char *sql_insert, int nbr_of_parameters,
                    char *parameters_names[], char *parameters[],
                    sqlite3_stmt *res);

// Insert a book
int db_insert_book(sqlite3 *db, int ISBN, char *Titre, char *Auteur,
                   char *Genre);
