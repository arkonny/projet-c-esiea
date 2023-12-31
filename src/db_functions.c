#include "db_functions.h"

sqlite3_stmt *suppression_stmt = NULL;
sqlite3_stmt *retour_stmt = NULL;
sqlite3_stmt *emprunt_stmt = NULL;
sqlite3_stmt *ajout_stmt = NULL;
sqlite3_stmt *recherche_stmt = NULL;
sqlite3_stmt *livres_empruntes_stmt = NULL;
sqlite3_stmt *creation_compte_stmt = NULL;
sqlite3_stmt *compte_recherche_stmt = NULL;
sqlite3_stmt *changement_mail_stmt = NULL;
sqlite3_stmt *changement_mdp_stmt = NULL;

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
	sqlite3_reset(stmt);
	return sqlite3_clear_bindings(stmt);
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

  char *sql_livres = "DROP TABLE IF EXISTS Livres;"
										 "CREATE TABLE Livres(ISBN TEXT PRIMARY KEY, Titre TEXT, Auteur TEXT, Genre TEXT, Id_User INT, Date_Emprunt TEXT);"
										 "DROP TABLE IF EXISTS Comptes;"
										 "CREATE TABLE Comptes(Id_User INTEGER PRIMARY KEY AUTOINCREMENT, Nom TEXT, Prenom TEXT, Mail TEXT, Hash TEXT, Admin INTEGER);";
                     
  int rc = sqlite3_exec(db, sql_livres, 0, 0, &err_msg);
  if (db_error_handler_err_msg(rc, err_msg, "SQL error: ")) {
    sqlite3_close(db);
  }

  return rc;
}

char *read_file(char *filename) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		debug("Cannot open file %s\n", filename);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);

	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	debug("read_file() read %ld bytes\n", fsize);
	debug("read_file() read %s\n", string);
	fclose(f);

	return string;
}

int SQL_insertion_livres() {
	char *err_msg = 0;
	// Read the file requetes.sql
	char *sql_insert = read_file("requetes.sql");
	if (sql_insert == NULL) {
		return 1;
	}
	int rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);
	if (db_error_handler_err_msg(rc, err_msg, "SQL error: ")) {
		sqlite3_close(db);
		return 2;
	}
	return 0;
}

int SQL_check_init() {
	char *err_msg = 0;
	char *sql_livres = "SELECT * FROM Comptes;";
	int rc = sqlite3_exec(db, sql_livres, 0, 0, &err_msg);
	debug("SQL_check_init() returned %d\n", rc);
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
	sqlite3_finalize(creation_compte_stmt);
	sqlite3_finalize(compte_recherche_stmt);
	sqlite3_finalize(changement_mail_stmt);
	sqlite3_finalize(changement_mdp_stmt);

	sqlite3_close(db);
	free(currentUser);
	return 0;
}


/*************************
* Fonctions de recherche *
*************************/
// Les objets de type Livre et Compte ne sont pas forcément complets
// d'où leur utilisation pour des fonctions de création de compte et de livre
int SQL_recherche(Livre *livre, listeLivre *liste) {
	char *sql_select = "SELECT * FROM Livres WHERE ISBN LIKE @ISBN OR Titre LIKE @Titre OR Auteur LIKE @Auteur OR Genre LIKE @Genre;";

	// Prepare the query
	if (recherche_stmt == NULL) {
		sqlite3_prepare_v2(db, sql_select, -1, &recherche_stmt, 0);
		debug("sqlite3_stmt : recherche_stmt initialisé\n\n");
	} else {
		sqlite3_reset(recherche_stmt);
	}

	// Bind the parameters
	sqlite3_bind_text(recherche_stmt, 1, livre->isbn, -1, SQLITE_STATIC);
	sqlite3_bind_text(recherche_stmt, 2, livre->titre, -1, SQLITE_STATIC);
	sqlite3_bind_text(recherche_stmt, 3, livre->auteur, -1, SQLITE_STATIC);
	sqlite3_bind_text(recherche_stmt, 4, livre->genre, -1, SQLITE_STATIC);

	// Launch the query
	int step = sqlite3_step(recherche_stmt);
	if (step == SQLITE_DONE) {
		debug("No result\n");
		return 0;
	}

	// If the list is null, return the first result in livre
	if (liste == NULL) {
		// Get the values
		char *isbn = (char *) sqlite3_column_text(recherche_stmt, 0);
		char *titre = (char *) sqlite3_column_text(recherche_stmt, 1);
		char *auteur = (char *) sqlite3_column_text(recherche_stmt, 2);
		char *genre = (char *) sqlite3_column_text(recherche_stmt, 3);
		int id_user = sqlite3_column_int(recherche_stmt, 4);
		char *date_emprunt = (char *) sqlite3_column_text(recherche_stmt, 5);

		// Copy the strings
		init_Livre(livre, isbn, titre, auteur, genre, id_user, date_emprunt);
		return 1;
	}

	// Get the result
	while (step == SQLITE_ROW) {
		// Create the Livre object
		Livre *livre = malloc(sizeof(Livre));
		
		// Get the values
		char *isbn = (char *) sqlite3_column_text(recherche_stmt, 0);
		char *titre = (char *) sqlite3_column_text(recherche_stmt, 1);
		char *auteur = (char *) sqlite3_column_text(recherche_stmt, 2);
		char *genre = (char *) sqlite3_column_text(recherche_stmt, 3);
		int id_user = sqlite3_column_int(recherche_stmt, 4);
		char *date_emprunt = (char *) sqlite3_column_text(recherche_stmt, 5);

		// Copy the strings
		init_Livre(livre, isbn, titre, auteur, genre, id_user, date_emprunt);

		// Add the Livre object to the listLivre
		ajouter_tete_listeLivre(liste, livre);
		debug("Got book %d:\n",liste->taille);
		debug_Livre(liste->tete->livre);
		debug("\n");

		// Get the next result
		step = sqlite3_step(recherche_stmt);
	}

	return 1;
}

// Liste de livre doit être une liste chainée
int SQL_livres_empruntes(Compte *user, listeLivre *liste) {
	char *sql_select = "SELECT * FROM Livres WHERE Id_User = @Id_User;";

	// Prepare the query
	//db_stmt_init(livres_empruntes_stmt, sql_select);
	if (livres_empruntes_stmt == NULL) {
		sqlite3_prepare_v2(db, sql_select, -1, &livres_empruntes_stmt, 0);
		debug("sqlite3_stmt : livres_empruntes_stmt initialisé\n\n");
	} else {
		sqlite3_reset(livres_empruntes_stmt);
	}

	// Bind the parameters
	sqlite3_bind_int(livres_empruntes_stmt, 1, user->id_user);

	// Launch the query
	int step = sqlite3_step(livres_empruntes_stmt);
	if (step == SQLITE_DONE) {
		debug("No result\n");
		return step;
	}

	// Get the result
	while (step == SQLITE_ROW) {
		// Create the Livre object
		Livre *livre = malloc(sizeof(Livre));

		// Get the values
		char *isbn = (char *) sqlite3_column_text(livres_empruntes_stmt, 0);
		char *titre = (char *) sqlite3_column_text(livres_empruntes_stmt, 1);
		char *auteur = (char *) sqlite3_column_text(livres_empruntes_stmt, 2);
		char *genre = (char *) sqlite3_column_text(livres_empruntes_stmt, 3);
		livre->id_user = sqlite3_column_int(livres_empruntes_stmt, 4);
		char *date_emprunt = (char *) sqlite3_column_text(livres_empruntes_stmt, 5);

		// Copy the strings
		init_Livre(livre, isbn, titre, auteur, genre, user->id_user, date_emprunt);

		// Add the Livre object to the listLivre
		ajouter_tete_listeLivre(liste, livre);
		debug("Got book %d:\n",liste->taille);
		debug_Livre(liste->tete->livre);
		debug("\n");

		// Get the next result
		step = sqlite3_step(livres_empruntes_stmt);
	}

	//db_error_handler(db, step, NULL);
	return step;
}

int SQL_livres_totaux(listeLivre *liste) {
	char *err_msg = 0;
	char *sql_select = "SELECT * FROM Livres;";

	int step = sqlite3_exec(db, sql_select, 0, 0, &err_msg);

	if (step == SQLITE_DONE) {
		debug("No result\n");
		return step;
	}

	// Get the result
	while (step == SQLITE_ROW) {
		Livre *livre = malloc(sizeof(Livre));

		// Get the values
		char *isbn = (char *) sqlite3_column_text(livres_empruntes_stmt, 0);
		char *titre = (char *) sqlite3_column_text(livres_empruntes_stmt, 1);
		char *auteur = (char *) sqlite3_column_text(livres_empruntes_stmt, 2);
		char *genre = (char *) sqlite3_column_text(livres_empruntes_stmt, 3);
		livre->id_user = sqlite3_column_int(livres_empruntes_stmt, 4);
		char *date_emprunt = (char *) sqlite3_column_text(livres_empruntes_stmt, 5);

		// Copy the strings
		init_Livre(livre, isbn, titre, auteur, genre, livre->id_user, date_emprunt);

		// Add the Livre object to the listLivre
		ajouter_tete_listeLivre(liste, livre);
		debug("Got book %d:\n",liste->taille);
		debug_Livre(liste->tete->livre);
		debug("\n");

		// Get the next result
		step = sqlite3_step(livres_empruntes_stmt);
	}

	debug("SQL_livres_totaux() returned %d\n", step);
	return step;
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
	
	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		sqlite3_bind_text(ajout_stmt, 1, livre->isbn, -1, SQLITE_STATIC);
		sqlite3_bind_text(ajout_stmt, 2, livre->titre, -1, SQLITE_STATIC);
		sqlite3_bind_text(ajout_stmt, 3, livre->auteur, -1, SQLITE_STATIC);
		sqlite3_bind_text(ajout_stmt, 4, livre->genre, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}

	rc = sqlite3_step(ajout_stmt);
  return rc;
}


int SQL_emprunt(Livre *livre) {
	debug("=>SQL_emprunt\n");
	char *sql_insert = "UPDATE Livres SET Date_emprunt = @Date_emprunt, Id_User = @Id_User WHERE ISBN = @ISBN;";

  // Prepare the query
	//int rc = db_stmt_init(emprunt_stmt, sql_insert);
	int rc;
	if (emprunt_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_insert, -1, &emprunt_stmt, 0);
	} else {
		rc = sqlite3_reset(emprunt_stmt);
	}

  if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
    // Bind the parameters
    sqlite3_bind_text(emprunt_stmt, 1, (char *) livre->date_emprunt, -1, SQLITE_STATIC);
		debug("date_emprunt = %s\n", livre->date_emprunt);
		sqlite3_bind_int(emprunt_stmt, 2, livre->id_user);
		debug("id_user = %d\n", livre->id_user);
		sqlite3_bind_text(emprunt_stmt, 3, (char *) livre->isbn, -1, SQLITE_STATIC);
		debug("isbn = %s\n", livre->isbn);
		debug("Inserted arguments\n");
  }

	rc = sqlite3_step(emprunt_stmt);
	debug("SQL_emprunt = %d\n", rc);
  return rc;
}


int SQL_retour(Livre *livre) {
	char *sql_insert = "UPDATE Livres SET Date_emprunt = NULL, Id_User = 0 WHERE ISBN = @ISBN;";

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

	rc = sqlite3_step(retour_stmt);
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

	rc = sqlite3_step(suppression_stmt);
	return rc;
}



/*********************************
* Fonctions de gestion de compte *
*********************************/

int SQL_Compte_recherche(Compte *user) {
	char *sql_select = "SELECT * FROM Comptes WHERE Mail = @Mail;";

	// Prepare the query
	// int rc = db_stmt_init(compte_recherche_stmt, sql_select);
	int rc;
	if (compte_recherche_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_select, -1, &compte_recherche_stmt, 0);
	} else {
		rc = sqlite3_reset(compte_recherche_stmt);
	}

	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		// Bind the parameters
		sqlite3_bind_text(compte_recherche_stmt, 1, user->mail, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}

	// Get the result
	int step = sqlite3_step(compte_recherche_stmt);

	if (step == SQLITE_DONE) {
		debug("No result\n");
		return 0;
	}

	// Get the values
	int id_user = sqlite3_column_int(compte_recherche_stmt, 0);
	char *nom = (char *) sqlite3_column_text(compte_recherche_stmt, 1);
	char *prenom = (char *) sqlite3_column_text(compte_recherche_stmt, 2);
	char *mail = (char *) sqlite3_column_text(compte_recherche_stmt, 3);
	char *hash = (char *) sqlite3_column_text(compte_recherche_stmt, 4);
	int admin = sqlite3_column_int(compte_recherche_stmt, 5);

	// Copy the strings
	init_Compte(user, id_user, nom, prenom, mail, hash, admin);

	return 1;
}


int SQL_insertion_compte(Compte *user) {
	char *sql_insert = "INSERT INTO Comptes(Nom, Prenom, Mail, Hash, Admin) VALUES(@Nom, @Prenom, @Mail, @Hash, @Admin);";

	// Prepare the query
	// int rc = db_stmt_init(creation_compte_stmt, sql_insert);
	int rc;
	if (creation_compte_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_insert, -1, &creation_compte_stmt, 0);
	} else {
		rc = sqlite3_reset(creation_compte_stmt);
	}

	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		// Bind the parameters
		sqlite3_bind_text(creation_compte_stmt, 1, user->nom, -1, SQLITE_STATIC);
		sqlite3_bind_text(creation_compte_stmt, 2, user->prenom, -1, SQLITE_STATIC);
		sqlite3_bind_text(creation_compte_stmt, 3, user->mail, -1, SQLITE_STATIC);
		sqlite3_bind_text(creation_compte_stmt, 4, user->hash, -1, SQLITE_STATIC);
		sqlite3_bind_int(creation_compte_stmt, 5, user->admin);
		debug("Inserted arguments\n");
	}

	rc = sqlite3_step(creation_compte_stmt);

	if (rc != SQLITE_DONE) {
		debug("Error inserting account\n");
		return rc;
	}

	// Get the id_user by searching the account
	rc = SQL_Compte_recherche(user);
	debug("SQL_Compte_recherche returned %d\n", rc);
	debug("Got id_user %d\n", user->id_user);

	return rc;
}

int SQL_changement_mdp(Compte *user, char *new_hash) {
	char *sql_update = "UPDATE Comptes SET Hash = @Hash WHERE Mail = @Mail;";

	// Prepare the query
	// int rc = db_stmt_init(changement_mail_stmt, sql_update);
	int rc;
	if (changement_mdp_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_update, -1, &changement_mdp_stmt, 0);
	} else {
		rc = sqlite3_reset(changement_mdp_stmt);
	}
	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		// Bind the parameters
		sqlite3_bind_text(changement_mdp_stmt, 1, new_hash, -1, SQLITE_STATIC);
		sqlite3_bind_text(changement_mdp_stmt, 2, user->mail, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}
	rc = sqlite3_step(changement_mdp_stmt);
	strncpy(user->hash, new_hash, 100);
	return rc;
}


int SQL_changement_mail(Compte *user, char *new_mail) {
	char *sql_update = "UPDATE Comptes SET Mail = @NewMail WHERE Mail = @Mail;";

	// Prepare the query
	// int rc = db_stmt_init(changement_mail_stmt, sql_update);
	int rc;
	if (changement_mail_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_update, -1, &changement_mail_stmt, 0);
	} else {
		rc = sqlite3_reset(changement_mail_stmt);
	}

	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		// Bind the parameters
		sqlite3_bind_text(changement_mail_stmt, 1, new_mail, -1, SQLITE_STATIC);
		sqlite3_bind_text(changement_mail_stmt, 2, user->mail, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}

	rc = sqlite3_step(changement_mail_stmt);

	// Change the mail in the variable
	strncpy(user->mail, new_mail, 100);

	return rc;
}


int SQL_suppression_compte(Compte *user) {
	char *sql_remove = "DELETE FROM Comptes WHERE Mail = @Mail;";

	// Prepare the query
	// int rc = db_stmt_init(creation_compte_stmt, sql_remove);
	int rc;
	if (creation_compte_stmt == NULL) {
		rc = sqlite3_prepare_v2(db, sql_remove, -1, &creation_compte_stmt, 0);
	} else {
		rc = sqlite3_reset(creation_compte_stmt);
	}

	if (!db_error_handler(db, rc, "Failed to execute statement: ")) {
		// Bind the parameters
		sqlite3_bind_text(creation_compte_stmt, 1, user->mail, -1, SQLITE_STATIC);
		debug("Inserted arguments\n");
	}

	rc = sqlite3_step(creation_compte_stmt);
	return rc;
}
