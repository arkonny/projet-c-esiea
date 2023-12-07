DROP TABLE IF EXISTS Comptes;
CREATE TABLE Comptes(Id_User INTEGER PRIMARY KEY AUTOINCREMENT, Nom TEXT, Prenom TEXT,Mail TEXT, Admin INTEGER);
INSERT INTO Comptes(Nom,Prenom,Mail, Admin) VALUES ('Uriel','Anthony','ua@gmail.com', 1);
INSERT INTO Comptes(Nom,Prenom,Mail, Admin) VALUES ('Renard-Raguenaud','Lucien','lr@gmail.com', 0);
DROP TABLE IF EXISTS Livres;
CREATE TABLE Livres(ISBN TEXT PRIMARY KEY, Titre TEXT, Auteur TEXT, Genre TEXT, Id_User INTEGER, Date_Emprunt TEXT);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('1xxx', 'Le Seigneur des Anneaux', 'J.R.R. Tolkien', 'Fantasy', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('2xxx', '1984', 'George Orwell', 'Dystopie', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('3xxx', 'Orgueil et Préjugés', 'Jane Austen', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('4xxx', 'L''Étranger', 'Albert Camus', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('5xxx', 'Harry Potter à l''école des sorciers', 'J.K. Rowling', 'Fantasy', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('6xxx', 'Les Misérables', 'Victor Hugo', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('7xxx', 'Le Petit Prince', 'Antoine de Saint-Exupéry', 'Conte', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('8xxx', 'To Kill a Mockingbird', 'Harper Lee', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('9xxx', 'Crime and Punishment', 'Fyodor Dostoevsky', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('10xx', 'Pride and Prejudice', 'Jane Austen', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('11xx', 'The Great Gatsby', 'F. Scott Fitzgerald', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('12xx', 'One Hundred Years of Solitude', 'Gabriel García Márquez', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('13xx', 'Anna Karenina', 'Leo Tolstoy', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('14xx', 'Brave New World', 'Aldous Huxley', 'Dystopian', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('15xx', 'The Catcher in the Rye', 'J.D. Salinger', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('16xx', 'The Hobbit', 'J.R.R. Tolkien', 'Fantasy', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('17xx', 'The Chronicles of Narnia', 'C.S. Lewis', 'Fantasy', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('18xx', 'Moby-Dick', 'Herman Melville', 'Aventure', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('19xx', 'Wuthering Heights', 'Emily Brontë', 'Roman', 1);
INSERT INTO Livres (ISBN, Titre, Auteur, Genre, Id_User) Values('20xx', 'Frankenstein', 'Mary Shelley', 'Gothique', 1);
