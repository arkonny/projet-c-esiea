#include "db_functions.h"

sqlite3_stmt *suppression_stmt = NULL;
sqlite3_stmt *retour_stmt = NULL;
sqlite3_stmt *emprunt_stmt = NULL;
sqlite3_stmt *ajout_stmt = NULL;
sqlite3_stmt *recherche_stmt = NULL;
sqlite3_stmt *livres_empruntes_stmt = NULL;

/************************
* Fonctions d'usage SQL *
************************/

// Handles rc errors with err_msg
int db_error_handler_err_msg(int rc, char *err_msg, char *prefix) {
  if (rc != SQLITE_OK) {
    if (prefix == NULL) {
      prefix = "SQL error: ";
    }
    debug("%s%s\n", prefix, err_msg);
    sqlite3_free(err_msg);
    return 1;
  }
  return 0;
}

// Handles rc errors without err_msg
int db_error_handler(sqlite3 *db, int rc, char *prefix) {
  if (rc != SQLITE_OK) {
    if (prefix == NULL) {
      prefix = "SQL error: ";
    }
    debug("%s%s\n", prefix, sqlite3_errmsg(db));
    return 1;
  }
  return 0;
}

/*
// Handles sqlite3_stmt variables
int db_stmt_init(sqlite3_stmt *stmt, char *sql_request) {
	if (stmt == NULL) {
		debug("stmt == NULL\n");
		return sqlite3_prepare_v2(db, sql_request, -1, &stmt, 0);
	}
	return sqlite3_reset(stmt);
}

int db_stmt_finalize(sqlite3_stmt *stmt) {
	//debug("Finalizing statement\n");
	if (stmt != NULL) {
		debug("stmt != NULL\n");
		return sqlite3_finalize(stmt);
	}
	return 0;
}
*/

/*********************************************
* Fonctions de gestion de la base de données *
*********************************************/
// La base de donnée est ouverte une seule fois au lancement du programme
// et fermée à la fin de celui-ci
// La variable db est globale et est utilisée par toutes les fonctions

int SQL_init() {
  char *err_msg = 0;
  int rc = sqlite3_open("library.db", &db);
  if (db_error_handler(db, rc, "Cannot open database: ")) {
    sqlite3_close(db);
    return rc;
  }

  char *sql_livres = "DROP TABLE IF EXISTS Livres;"
										 "CREATE TABLE Livres(ISBN TEXT PRIMARY KEY, Titre TEXT, Auteur TEXT, Genre TEXT, Id_User INT, Date_Emprunt TEXT);";
                     
  rc = sqlite3_exec(db, sql_livres, 0, 0, &err_msg);
  if (db_error_handler_err_msg(rc, err_msg, "SQL error: ")) {
    sqlite3_close(db);
  }

  return rc;
}


int SQL_open() {
	int rc = sqlite3_open("library.db", &db);
	if (db_error_handler(db, rc, "Cannot open database: ")) {
		sqlite3_close(db);
	}
	return rc;
}


int SQL_close() {
	sqlite3_finalize(suppression_stmt);
	sqlite3_finalize(retour_stmt);
	sqlite3_finalize(emprunt_stmt);
	sqlite3_finalize(ajout_stmt);
	sqlite3_finalize(recherche_stmt);
	sqlite3_finalize(livres_empruntes_stmt);

	sqlite3_close(db);
	return 0;
}


/*************************
* Fonctions de recherche *
*************************/
// Les objets de type Livre et Compte ne sont pas forcément complets
// d'où leur utilisation pour des fonctions de création de compte et de livre
int SQL_recherche(Livre *livre, listeLivre *liste) {
	return 0;
}

// Liste de livre doit être une liste chainée
int SQL_livres_empruntes(Compte *user, listeLivre *liste) {
	char *sql_select = "SELECT * FROM Livres WHERE Id_User = @Id_User;";

	// Prepare the query
	//db_stmt_init(livres_empruntes_stmt, sql_select);
	int rc;
	if (livres_empruntes_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_select, -1, &livres_empruntes_stmt, 0);
	} else {
		rc = sqlite3_reset(livres_empruntes_stmt);
	}
	debug("rc = %d\n", rc);

	// Bind the parameters
	sqlite3_bind_int(livres_empruntes_stmt, 1, user->id_user);

	// Launch the query
	int step = sqlite3_step(livres_empruntes_stmt);
	debug("step = %d\n", step);
	if (step == SQLITE_DONE) {
		debug("No result\n");
		return step;
	}

	// Get the result
	while (step == SQLITE_ROW) {
		// Create the Livre object
		Livre *livre = malloc(sizeof(Livre));

		// Get the values
		livre->isbn = sqlite3_column_text(livres_empruntes_stmt, 0);
		livre->titre = sqlite3_column_text(livres_empruntes_stmt, 1);
		livre->auteur = sqlite3_column_text(livres_empruntes_stmt, 2);
		livre->genre = sqlite3_column_text(livres_empruntes_stmt, 3);
		livre->id_user = sqlite3_column_int(livres_empruntes_stmt, 4);
		livre->date_emprunt = sqlite3_column_text(livres_empruntes_stmt, 5);
		print_Livre(livre);

		// Add the Livre object to the listLivre
		ajouter_tete_listeLivre(liste, livre);
		print_Livre(liste->tete->livre);
		printf("titre = %s\n", liste->tete->livre->titre);
		celluleLivre *cel = liste->tete;
		printf("titre = %s\n", cel->livre->titre);

		// Get the next result
		step = sqlite3_step(livres_empruntes_stmt);
		debug("step = %d\n", step);
	}

	db_error_handler(db, step, NULL);
	return step;
}


// Complète un objet Livre avec les informations de la base de données
int SQL_disponibilite(Livre *livre) {
	char *sql_select = "SELECT Date_Emprunt FROM Livres WHERE ISBN = @ISBN;";

	// Prepare the query
	//db_stmt_init(recherche_stmt, sql_select);
	int rc;
	if (recherche_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_select, -1, &recherche_stmt, 0);
	} else {
		rc = sqlite3_reset(recherche_stmt);
	}
	debug("rc = %d\n", rc);
	

	// Bind the parameters
	sqlite3_bind_text(recherche_stmt, 1, (char *)livre->isbn, -1, SQLITE_STATIC);
	debug("Inserted arguments\n");

	// Get the result
	int step = sqlite3_step(recherche_stmt);
	if (step == SQLITE_ROW) {
		// Get the values
		livre->date_emprunt = sqlite3_column_text(recherche_stmt, 0);
		debug("date_emprunt = %s\n", livre->date_emprunt);
		return 0;
	} else if (step == SQLITE_DONE) {
		debug("No result\n");
		return 1;
	} else {
		debug("Error\n");
		return step;
	}
}



/**********************************
* Fonctions de gestion des livres *
**********************************/

int SQL_ajout(Livre *livre) {
  char *sql_insert = "INSERT INTO Livres VALUES(@ISBN, @Titre, @Auteur, @Genre, NULL, NULL);";

	//int rc = db_stmt_init(ajout_stmt, sql_insert);
	int rc;
	if (ajout_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_insert, -1, &ajout_stmt, 0);
	} else {
		rc = sqlite3_reset(ajout_stmt);
	}
	debug("rc = %d\n", rc);
	
	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		sqlite3_bind_text(ajout_stmt, 1, (char *) livre->isbn, -1, SQLITE_STATIC);
		sqlite3_bind_text(ajout_stmt, 2, (char *) livre->titre, -1, SQLITE_STATIC);
		sqlite3_bind_text(ajout_stmt, 3, (char *) livre->auteur, -1, SQLITE_STATIC);
		sqlite3_bind_text(ajout_stmt, 4, (char *) livre->genre, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}

	int step = sqlite3_step(ajout_stmt);
	debug("step = %d\n", step);
  return step;
}


int SQL_emprunt(Livre *livre, Compte *user) {
	char *sql_insert = "UPDATE Livres SET Date_emprunt = @Date_emprunt, Id_User = @Id_User WHERE ISBN = @ISBN;";

  // Prepare the query
	//int rc = db_stmt_init(emprunt_stmt, sql_insert);
	int rc;
	if (emprunt_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_insert, -1, &emprunt_stmt, 0);
	} else {
		rc = sqlite3_reset(emprunt_stmt);
	}
	debug("rc = %d\n", rc);

  if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
    // Bind the parameters
    sqlite3_bind_text(emprunt_stmt, 1, (char *) livre->date_emprunt, -1, SQLITE_STATIC);
		sqlite3_bind_int(emprunt_stmt, 2, user->id_user);
		sqlite3_bind_text(emprunt_stmt, 3, (char *) livre->isbn, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
  }

	sqlite3_step(emprunt_stmt);
  return rc;
}


int SQL_retour(Livre *livre) {
	char *sql_insert = "UPDATE Livres SET Date_emprunt = NULL, Id_User = NULL WHERE ISBN = @ISBN;";

  // Prepare the query
	//int rc = db_stmt_init(retour_stmt, sql_insert);
	int rc;
	if (retour_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_insert, -1, &retour_stmt, 0);
	} else {
		rc = sqlite3_reset(retour_stmt);
	}

  if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
    // Bind the parameters
		sqlite3_bind_text(retour_stmt, 1, (char *) livre->isbn, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
  }

	sqlite3_step(retour_stmt);
  return rc;

}


int SQL_suppression(Livre *livre) {
	char *sql_remove = "DELETE FROM Livres WHERE ISBN = @ISBN;";

	// Prepare the query
	//int rc = db_stmt_init(suppression_stmt, sql_remove);
	int rc;
	if (suppression_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_remove, -1, &suppression_stmt, 0);
	} else {
		rc = sqlite3_reset(suppression_stmt);
	}

	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		// Bind the parameters
		sqlite3_bind_text(suppression_stmt, 1, (char *) livre->isbn, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}

	sqlite3_step(suppression_stmt);
	return rc;
}



/*********************************
* Fonctions de gestion de compte *
*********************************/

int SQL_creation_compte(Compte *user);
int SQL_connexion(Compte *user);
int SQL_deconnexion(Compte *user);
int SQL_suppression_compte(Compte *user);
