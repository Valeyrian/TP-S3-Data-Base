#ifndef MENUS_H
#define MENUS_H


#include <stdio.h>
#include <stdlib.h>
#include "Settings.h"
#include "Index.h"
#include "Table.h"
#include "SetEntry.h"

typedef struct FromCsv FromCsv;
typedef struct DataBaseLoad DataBaseLoad;
typedef struct SearchInfo SearchInfo; 

enum Action
{
	CREATE_FROM_CSV = 1,
	OPEN_EXISTING,
	CREATE_MANUALLY,
	QUIT
};

 struct FromCsv
 {
	 char* CSVpath;	
	 char* folderPath;
	 bool validFolder;
	 bool valideFile;

 };

 struct DataBaseLoad
 {
	 char* tblFile;
	 char* datFile;
	 char* idxFolder;
	 bool validTblFile;
	 bool validDatFile;
	 bool validIdxFolder;
 };

 struct SearchInfo
 {
	 bool isFilterSelectionEnded;
	 bool wantToDisplayResult;
	 int filterCount;
	 int matchCount;
 };


//verrification de l'existance du fichier
bool checkFile(char* path);

//verrification de l'existance du dossier
bool checkFolder(char* path);

//verrifie un filtre
bool checkFilter(Filter* filter, Table* table);

//verrifie une entrée
bool checkEntryFromUser(Entry* entry, Table* table);

//trouve le nom d'un fichier
void getFileNameWithoutExtension(const char* filePath, char* fileName);


//fait une pause dans le programme et attend que l'utilisateur appuie sur une touche
void pauseForUser();

//nettoie le buffer d'entrée
void clearInputBuffer();

//choix de l'action
int selectAction();

//affichage du menu create from csv
int createFromCSV();

// affiche un menu pour charger une base de données
int loadDatabase();

//action on database load
int actionOnDatabase(Table *table);

//menu de recherche dans une BDD
int searchInDatabase(Table* table);

//menu pour l'insertion dans une BDD
int insertInDatabase(Table* table);

//menu pour l'insertion multiple dans une BDD
void insertMultipleEntryFromUser(Table* table);

//menu pour merge depuis un csv
int mergeFromCSV(Table *table);


#endif // !MENUS_H