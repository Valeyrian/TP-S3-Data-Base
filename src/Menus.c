#include "Menus.h"
#include "Ui.h"


bool checkFile(char* path)
{
	FILE* test = fopen(path, "rb");
	if (test == NULL)
	{
		return false;
	}
	else
	{
		fclose(test);
		return true;
	}
}

bool checkFolder(char* path)
{
	char folderPathtest[1024] = { '\0' };
	snprintf(folderPathtest, 1024, "%s/UnFichierAvecUnNomT3llementAbsurdeQuilNePeutPasExister.txt", path);
	FILE* test = fopen(folderPathtest, "wb");

	if (test == NULL)
	{
		return false;
	}
	else
	{
		fclose(test);
		remove(folderPathtest);
		return true;
	}
}

bool checkFilter(Filter* filter, Table* table)
{
	if (filter->attributeIndex < 0 || filter->attributeIndex >= table->attributeCount)
	{
		return false;
	}
	
	if (filter->requestOp < 0 || filter->requestOp > OP_BETW)
	{
		return false;
	}
	
	if (filter->requestOp == OP_BETW)
	{
		if (strlen(filter->key1) == 0 || strlen(filter->key2) == 0)
		{
			return false;
		}
	}
	else
	{
		if (strlen(filter->key1) == 0)
		{
			return false;
		}
	}
	return true;

}

bool checkEntryFromUser(Entry* entry, Table* table)
{
	for (int i = 0; i < table->attributeCount; i++)
	{
		if (strlen(entry->values[i]) == 0)
		{
			return false;
		}
	}
	return true;
}	

bool chekTableCompatible(Table* table, Table* table2)
{
	if (table->attributeCount != table2->attributeCount) 
	{
		return false;
	}
	for (int i = 0; i < table->attributeCount; i++)
	{
		if (strcmp(table->attributes[i].name, table2->attributes[i].name) != 0)
		{
			return false;
		}
	}
	return true;
}

void getFileNameWithoutExtension(const char* filePath, char* fileName)
{
	// Trouver la dernière occurrence du separateur de chemin
	const char* lastSlash = strrchr(filePath, '\\'); // Pour Windows
	

	// Si aucun separateur, commencer au debut de filePath
	const char* baseName = lastSlash ? lastSlash + 1 : filePath;

	// Trouver la position du dernier point '.'
	const char* lastDot = strrchr(baseName, '.');

	// Copier le nom du fichier sans l'extension
	if (lastDot) {
		size_t length = lastDot - baseName;
		strncpy(fileName, baseName, length);
		fileName[length] = '\0'; // Ajouter le caractère de fin
	}
	else {
		// Pas d'extension trouvee, copier le nom complet
		strcpy(fileName, baseName);
	}
}

void getParentDirectory(const char* filePath, char* parentDir)
{
	const char* lastSlash = strrchr(filePath, '\\'); // Windows (separateur `\\`)

	if (lastSlash) {

		size_t length = lastSlash - filePath; // Longueur du chemin du repertoire parent
		strncpy(parentDir, filePath, length);
		parentDir[length] = '\0';
	}
	else {
		strcpy(parentDir, ".");// Si aucun separateur trouve, c'est un fichier dans le repertoire courant
	}
}

void pauseForUser()
{
	printf("Appuyez sur Entree pour continuer"BLINK_SLOW"...\n"RESET);
	while (getchar() != '\n'); // Nettoyer le buffer residuel
}

void clearInputBuffer() {
	int a;
	while ((a = getchar()) != '\n' && a != EOF);
}

int selectAction()
{
	int isEndending = 1, UserEntry = 0;
	while (isEndending)
	{
		welcommePrint(); 
		selctActionPrint();



		scanf("%d", &UserEntry);
		clearInputBuffer();


		switch (UserEntry)
		{
		case 1:
			return CREATE_FROM_CSV;
			isEndending = 0;
			break;
		case 2:
			return OPEN_EXISTING;
			isEndending = 0;
			break;
		case 3:
			return CREATE_MANUALLY;
			isEndending = 0;
			break;
		case 4:
			return QUIT;
			isEndending = 0;
			break;
		default:
			printf(FG_RED "Choix invalide. Veuillez entrer un nombre entre 1 et 4. Enter pour repprendre\n" RESET);
			pauseForUser();
			break;
		}

	}
}

int createFromCSV()
{
	bool isEndending = true;
	FromCsv* fromCsv = calloc(1, sizeof(FromCsv));
	if (!fromCsv) {
		fprintf(stderr, "Erreur d'allocation memoire.\n");
		return -1;
	}

	fromCsv->CSVpath = calloc(1024, 1);
	fromCsv->folderPath = calloc(1024, 1);
	if (!fromCsv->CSVpath || !fromCsv->folderPath) {
		fprintf(stderr, "Erreur d'allocation memoire pour les chemins.\n");
		free(fromCsv->CSVpath);
		free(fromCsv->folderPath);
		free(fromCsv);
		return -1;
	}

	fromCsv->validFolder = true;
	fromCsv->valideFile = true;

	int UserEntry = 0;

	while (isEndending) {
		createFromCSVPrint(fromCsv);
		UserEntry = 0;

		printf("Entrez votre choix (1-5) : ");
		scanf("%d", &UserEntry);

		clearInputBuffer(); 
		
		switch (UserEntry) {
		case 1:
			printf("Entrez le chemin du fichier CSV : ");
			scanf("%s", fromCsv->CSVpath);
			clearInputBuffer(); 
			fromCsv->valideFile = checkFile(fromCsv->CSVpath);
			break;

		case 2:
			printf("Entrez le chemin du dossier de destination : ");
			scanf("%s", fromCsv->folderPath);
			clearInputBuffer();
			fromCsv->validFolder = checkFolder(fromCsv->folderPath);
			break;

		case 3:
			fromCsv->valideFile = checkFile(fromCsv->CSVpath);
			fromCsv->validFolder = checkFolder(fromCsv->folderPath);
			if (fromCsv->valideFile && fromCsv->validFolder) {
				Table* table = Table_createFromCSV(fromCsv->CSVpath, fromCsv->folderPath);
				if (table) {
					printf(FG_GREEN "Table creee avec succès.\n" RESET);
					pauseForUser();
				}
				else {
					printf(FG_RED "Erreur lors de la creation de la table.\n" RESET);
					pauseForUser();
				}
				UserEntry = 4;
				isEndending = false;
			}
			else {
				if (!fromCsv->valideFile) {
					printf(FG_RED "Le fichier CSV est invalide ou inaccessible.\n" RESET);
				}
				if (!fromCsv->validFolder) {
					printf(FG_RED "Le dossier de destination est invalide ou inaccessible.\n" RESET);
				}
				pauseForUser();
			}
			break;

		case 4:
			isEndending = false;
			goto end;
			break;

		case 5:
			isEndending = false;
			goto end;
			break;

		default:
			printf(FG_RED "Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n" RESET);
			pauseForUser(); 
			break;
		}
		
	}
	end:
	free(fromCsv->CSVpath);
	free(fromCsv->folderPath);
	free(fromCsv);
	return UserEntry == 4 ? 1 : 0;
}

int loadDatabase()
{
	bool isEndending = true;
	DataBaseLoad* dbLoad = calloc(1, sizeof(DataBaseLoad));
	if (!dbLoad) {
		fprintf(stderr, "Erreur d'allocation memoire.\n");
		return -1;
	}

	dbLoad->tblFile = calloc(1024, 1);
	dbLoad->datFile = calloc(1024, 1);
	dbLoad->idxFolder = calloc(1024, 1);

	if (!dbLoad->tblFile || !dbLoad->datFile || !dbLoad->idxFolder) {
		fprintf(stderr, "Erreur d'allocation memoire pour les chemins.\n");
		free(dbLoad->tblFile);
		free(dbLoad->datFile);
		free(dbLoad->idxFolder);
		free(dbLoad);
		return -1;
	}

	dbLoad->tblFile[0] = '\0';
	dbLoad->datFile[0] = '\0';
	dbLoad->idxFolder[0] = '\0';
	dbLoad->validTblFile = false;
	dbLoad->validDatFile = false;
	dbLoad->validIdxFolder = false;


	int UserEntry = 0;

	while (isEndending) {
		loadDatabasePrint(dbLoad);
		UserEntry = 0;

		printf("Entrez votre choix (1-6) : ");
		scanf("%d", &UserEntry);
		clearInputBuffer();



		switch (UserEntry) {
		case 1:
			printf("Entrez le chemin du fichier .tbl : ");
			scanf("%s", dbLoad->tblFile);
			clearInputBuffer();
			break;

		case 2:
			printf("Entrez le chemin du fichier .dat : ");
			scanf("%s", dbLoad->datFile);
			clearInputBuffer();
			break;

		case 3:
			printf("Entrez le chemin du dossier des index : ");
			scanf("%s", dbLoad->idxFolder);
			clearInputBuffer();
			break;

		case 4:
			dbLoad->validTblFile = checkFile(dbLoad->tblFile);
			dbLoad->validDatFile = checkFile(dbLoad->datFile);
			dbLoad->validIdxFolder = checkFolder(dbLoad->idxFolder);

			if (dbLoad->validDatFile && dbLoad->validIdxFolder && dbLoad->validTblFile) {
				
				Table* table = Table_load(dbLoad->tblFile, dbLoad->datFile);
				if (table)
				{
					 Table_debugPrint(table);
					 int back = actionOnDatabase(table); //passe au menu des action sur la base de donnee
					 if (back)
						 continue;
					 else
						 goto end;
				}
				else
				{
					printf(FG_RED "Erreur lors du chargement de la table.\n" RESET);
					pauseForUser();
				}

			}
			else
			{
				if (!dbLoad->validTblFile)
				{
					printf(FG_RED "Le fichier .tbl est invalide ou inaccessible.\n" RESET);
				}
				if (!dbLoad->validDatFile)
				{
					printf(FG_RED "Le fichier .dat est invalide ou inaccessible.\n" RESET);
				}
				if (!dbLoad->validIdxFolder)
				{
					printf(FG_RED "Le dossier des index est invalide ou inaccessible.\n" RESET);
				}
				pauseForUser();
			}

			break;
		case 5:
			printf("Operation annulee.\n");
			isEndending = false;
			break;

		case 6:
			printf("fin de programme.\n");
			isEndending = false;
			break;

		default:
			printf(FG_RED "Choix invalide. Veuillez entrer un nombre entre 1 et 6. Enter pour repprendre\n" RESET);
			pauseForUser();
			break;
		}
	}
	end:
	free(dbLoad->tblFile);
	free(dbLoad->datFile);
	free(dbLoad->idxFolder);
	free(dbLoad);
	return UserEntry == 5 ? 1 : 0;
}

int actionOnDatabase(Table* table)
{
	int isEndending = 1, UserEntry = 0;
	while (isEndending)
	{
		actionOnDatabasePrint(table);
		printf("Entrez votre choix (1-5) : ");
		scanf("%d", &UserEntry);
		clearInputBuffer();

		switch (UserEntry)
		{
		case 1: //rechercher
			
			if (searchInDatabase(table) == 1)
				continue;
			else
			{
				UserEntry = 0; 
				goto end;
			}
			
			break;
		case 2:	
			if (insertInDatabase(table) == 1)
				continue;
			else
			{
				UserEntry = 0; 
				goto end;
			}

			break;
		case 3: //supprimer
			
			break;
		case 4: //afficher

			break;
		case 5: //retour
			
			isEndending = 0; 
			goto end ;
			break;

		case 6: //quitter
			isEndending = 0;
			goto end;
			break;

		default:
			printf(FG_RED"Choix invalide. Veuillez entrer un nombre entre 1 et 6.\n"RESET);
			pauseForUser(); 
			break;
		}
	}
end:

	return UserEntry == 5 ? 1 : 0 ;
}	

int searchInDatabase(Table* table)
{
	
	Filter* filter = calloc(1, sizeof(Filter));
	if (!filter) {
		fprintf(stderr, "Erreur d'allocation memoire.\n");
		return -1;
	}
	filter->key1 = calloc(128, sizeof(char));
	filter->key2 = calloc(128, sizeof(char));

	SearchInfo* info = calloc(1, sizeof(SearchInfo));  
	if (!info) {
		fprintf(stderr, "Erreur d'allocation memoire.\n");
		free(filter);
		return -1;
	}
	
	SetEntry* result = SetEntry_create(); 
	if (!result) {
		fprintf(stderr, "Erreur d'allocation memoire.\n");
		free(filter);
		free(info);
		return -1;
	}


	info->isFilterSelectionEnded = false;
	info->wantToDisplayResult = false;
	info->filterCount = 0;
	info->matchCount = 0;

	bool isEndending = true;
	int UserEntry = 0;
	
	while (isEndending) 
	{
		UserEntry = 0;
		
		
		while (!info->isFilterSelectionEnded)
		{
			UserEntry = 0;
			searchInDatabasePrint(table, filter, info);
			printf("Entrez votre choix (1-5) : ");
			scanf("%d", &UserEntry);
			clearInputBuffer();
			switch (UserEntry)
			{
			case 1:
				filterInsert:
				printf("Entrez l'indice de l'attribut : "); 
				scanf("%d", &filter->attributeIndex); //probleme ici si on rentre un char ca met 0 \-_-/
				clearInputBuffer(); 
				printf("Entrez le type de recherche  : ");
				scanf("%d", &filter->requestOp); //probleme ici si on rentre un char ca met 0 \-_-/
				clearInputBuffer();
				if (filter->requestOp == OP_BETW)
				{
					printf("Entrez la première clef de comparaison : ");
					scanf("%s", filter->key1);
					clearInputBuffer();
					printf("Entrez la deuxième clef de comparaison : ");
					scanf("%s", filter->key2);
					clearInputBuffer();
				}
				else
				{
					printf("Entrez la clef de comparaison : ");
					scanf("%s", filter->key1);
					clearInputBuffer();
				}
				if (checkFilter(filter, table))
				{
					info->filterCount++;
				}
				else
				{
					printf(FG_RED "Filtre invalide. Veuillez reessayer.\n" RESET);
					filter->attributeIndex = 0;
					filter->requestOp = 0;
					filter->key1[0] = '\0';
					filter->key2[0] = '\0';
					pauseForUser();
					goto filterInsert;
				}
				break;
			case 2:
				for (int i = 0; i < info->filterCount; i++)
				{
					filter->attributeIndex = 0;
					filter->requestOp = 0;
					filter->key1[0] = '\0';
					filter->key2[0] = '\0';
				}
				SetEntry_destroy(result); 
				result = SetEntry_create();

				
				info->filterCount = 0;
				break;

			case 3 : //effectue la recher
				if (info->filterCount > 0)
				{
					SetEntry_destroy(result);
					result = SetEntry_create();
					Table_search(table, filter, result); 
					info->wantToDisplayResult = true;
					info->isFilterSelectionEnded = true;
				}
				else
				{
					printf(FG_RED "Aucun filtre n'a ete trouve.\n" RESET);
					pauseForUser();
				}
				break;

			case 4:
				info->isFilterSelectionEnded = true;
				isEndending = false;
				goto end;
				break;
			case 5:
				info->isFilterSelectionEnded = true;	
				isEndending = false;	
				goto end;
				break;
			default:
				printf(FG_RED "Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n" RESET); 
				pauseForUser(); 
				break;
			}
			
			
		}

		info->matchCount = SetEntry_size(result);

		while (info->wantToDisplayResult)
		{
			UserEntry = 0;
			searchInDatabasePrint(table, filter, info);
			printf("Entrez votre choix (1-5) : ");
			scanf("%d", &UserEntry);
			clearInputBuffer();
			switch (UserEntry)
			{
			case 1:
				info->isFilterSelectionEnded = false;
				info->wantToDisplayResult = false;
				continue;
				break;
			case 2:
				if (info->matchCount == 0)
				{
					printf(FG_RED "Aucun resultat à afficher.\n" RESET); 
					pauseForUser();
				}
				if (info->matchCount > 10)
				{
					printf(FG_TRUECOLOR(165,98,0) BOLD"Il y a plus de 10 resultats. L'afficahe peut etre long. Voulez-vous les afficher ? (O/n) : "RESET BG_BLACK);
					char c = getchar();
					clearInputBuffer();
					if (c == 'O' || c == 'o' || c == '\n')
					{
						iterPrint(table,result);
						pauseForUser();
					}
					
					else
					{
						printf(FG_GREEN "Affichage annule.\n" RESET);
						pauseForUser();
						continue;
					}
				}
				else
				{
					iterPrint(table, result);
					pauseForUser();
				}
				break;

			case 3: //sauver les resultats dans un fichier
				printf("Entrez le chemin du dossier ou sera le fichier de sauvegarde : ");
				char path[1024] = { '\0' }; 
				scanf("%s", path); 
				clearInputBuffer(); 
				if (checkFolder(path))
				{
					char filePath[1024] = { '\0' };
					snprintf(filePath, 1024, "%s/Resultats.txt", path); 
					FILE* file = fopen(filePath, "w");
					SetEntryIter* it = SetEntryIter_create(result);
					if (file)
					{
						int i = 1;
						Entry* entry = Entry_create(table);

						while (SetEntryIter_isValid(it))
						{
							EntryPointer entryPtr = SetEntryIter_getValue(it);
							
							Table_readEntry(table, entry, entryPtr);


							fprintf(file, "Correspondance numero %d : \n", i);

							for (int j = 0; j < entry->attributeCount; j++)
							{
								fprintf(file,"\tT------------\n");
								fprintf(file, "\t|  - %s : %s\n", table->attributes[j].name, entry->values[j]);
							}
							fprintf(file, "\tT------------\n");

							SetEntryIter_next(it);
							i++;
						}

						printf(FG_GREEN "Resultats sauvegardes avec succès.\n" FG_WHITE); 
						fclose(file);
						SetEntryIter_destroy(it);
						pauseForUser();
					}
					else
					{
						printf(FG_RED "Il est impossible de creer un fichier dans le dossier de destination.\n" FG_WHITE); 
						pauseForUser(); 
					}
				}
				else
				{
					printf(FG_RED "Le dossier de destination est invalide ou inaccessible.\n" RESET);
					pauseForUser(); 
				}

				break;

			case 4:
				info->isFilterSelectionEnded = true;
				isEndending = false;
				goto end;
				break;
			case 5:
				info->isFilterSelectionEnded = true;
				isEndending = false;
				goto end;
				break;
			default:
				printf(FG_RED "Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n" RESET);
				pauseForUser();
				break;
			}

		
		}
		
	}	
	
end:
	SetEntry_destroy(result);
	free(filter->key1);
	free(filter->key2);
	free(filter);	
	free(info);	
	return UserEntry == 4 ? 1 : 0; 

}

int insertInDatabase(Table* table)
{
	bool isEndending = true;
	int UserEntry = 0;
	Entry* oneEntry = Entry_create(table);

	Table* tableSource = NULL; 



	while (isEndending)
	{
		insertInDatabasePrint(table);
		printf("\nEntrez votre choix (1-6) : ");
		scanf("%d", &UserEntry);
		clearInputBuffer();

		switch (UserEntry)
		{
		case 1 :
			scanEntry:
			printf("Veuillez entrer les informations concernant votre entree.\n");
			for (int i = 0; i < table->attributeCount; i++)
			{
				printf("Entrez la valeur de l'attribut "FG_YELLOW"%s"FG_WHITE" : ", table->attributes[i].name);
				memset(oneEntry->values[i], 0, table->attributes[i].size + 1); //+1 pour le dernier char 
				scanf("%s", oneEntry->values[i]); 
				clearInputBuffer(); 
			}
			if (checkEntryFromUser)
			{
				Table_insertEntry(table, oneEntry); 
				printf(FG_GREEN"Entree ajoutee avec succes.\n"FG_WHITE); 
			}
			else
			{
				printf(FG_RED"Entree invalide. Veuillez reessayer.\n"FG_WHITE);
				goto scanEntry;
			}
			pauseForUser();
			break;

		case 2 :
			insertMultipleEntryFromUser(table);	
			pauseForUser(); 
			break;

		case 3 : 
			printf("Vous allez entrer les informations concernant le fichier CSV d'entree ");
			 
			 if (mergeFromCSV(table))
				 continue;
			 else

			break;

		case 4 :
			printf(FG_CYAN"Vous allez entrer les informations concernant la table d'entree : \n"FG_WHITE); 
			
			entryInput :
			printf("Entrez le chemin du fichier .tbl : ");
			char tblFile[1024] = { '\0' };
			char datFile[1024] = { '\0' };
			scanf("%s", tblFile);	
			clearInputBuffer(); 
			printf(FG_YELLOW ITALIC "%s\n"RESET BG_BLACK, tblFile);	  
			printf("Entrez le chemin du fichier .dat : ");
			scanf("%s", datFile); 
			clearInputBuffer(); 
			printf(FG_YELLOW ITALIC "%s\n"RESET BG_BLACK, datFile);

			if (checkFile(datFile) && checkFile(tblFile) )
			{
				tableSource = Table_load(tblFile, datFile); 
				if (tableSource)
				{
					if (chekTableCompatible(table, tableSource))
					{
						if (mergeTwoDataBase(table, tableSource))
						{
						printf(FG_GREEN "Table fusionnee avec succès.\n" RESET); 
						}
						else
						{
							printf(FG_RED "Erreur lors de la fusion des tables.\n" RESET); 
						}
						pauseForUser();
					}
					else
					{
						printf(FG_RED "Les tables ne sont pas compatibles.\n" RESET);
						Table_destroy(tableSource);
					} 
				}
				else
				{
					printf(FG_RED "Erreur lors du chargement de la table.\n" RESET); 
				}

			}
			else
			{
				printf(FG_RED "Le fichier .tbl ou .dat est invalide ou inaccessible.\n" RESET);
				printf(FG_RED "Voulez-vous reessayer ? (O/n) : " RESET);
				char c = getchar();
				clearInputBuffer();
				if (c == 'O' || c == 'o' || c=='/n')
					goto entryInput;
				else
					continue;
			}

			break;

		case 5 :
			isEndending = false;
			break;
		case 6 :
			isEndending = false;
			goto end;
			break;

		default :
			printf(FG_RED"Choix invalide. Veuillez entrer un nombre entre 1 et 6.\n"RESET);
			pauseForUser();
			break;

		}
	}
end:
	Entry_destroy(oneEntry);
	return UserEntry == 5 ? 1 : 0;
}

void insertMultipleEntryFromUser(Table* table)
{
	printf(BG_BLACK);
	for (int i = 0; i < 100; i++)
	{
		printf("\n");
	}
	Entry* oneEntry = Entry_create(table);
	bool isEndending = true;
	int UserEntry = 0;
	while (isEndending)
	{
		scanEntry :
		printf("Veuillez entrer les informations concernant votre entree.\n");
		for (int i = 0; i < table->attributeCount; i++)
		{
			printf("Entrez la valeur de l'attribut "FG_YELLOW"%s"FG_WHITE" : ", table->attributes[i].name);
			memset(oneEntry->values[i], 0, table->attributes[i].size + 1);
			scanf("%s", oneEntry->values[i]); 
			clearInputBuffer();
		}
		if (checkEntryFromUser)
		{
			Table_insertEntry(table, oneEntry);
			printf(FG_GREEN"Entree ajoutee avec succes.\n");
		}
		else
		{
			printf(FG_RED"Entree invalide.\n");
			printf(FG_RED"Voulez-vous reessayer ? (O/n) : "FG_WHITE);
			char c = getchar();
			clearInputBuffer();
			if (c == 'N' || c == 'n')
			{
				isEndending = false;
				goto end;
			}
			else
				goto scanEntry;
		}
		printf(FG_TRUECOLOR(165,98,0)"Voulez-vous ajouter une autre entree ? (O/n) : "FG_WHITE);
		char c = getchar();
		clearInputBuffer();
		if (c == 'N' || c == 'n')
		{
			isEndending = false;
		}
	}
end:	
	Entry_destroy(oneEntry);

	
}

int mergeFromCSV(Table *table)
{

	bool isEndending = true;
	FromCsv* fromCsv = calloc(1, sizeof(FromCsv));
	if (!fromCsv) {
		fprintf(stderr, "Erreur d'allocation memoire.\n");
		return -1;
	}

	fromCsv->CSVpath = calloc(1024, 1);
	fromCsv->folderPath = calloc(1024, 1);
	if (!fromCsv->CSVpath || !fromCsv->folderPath) {
		fprintf(stderr, "Erreur d'allocation memoire pour les chemins.\n");
		free(fromCsv->CSVpath);
		free(fromCsv->folderPath);
		free(fromCsv);
		return -1;
	}

	fromCsv->folderPath = table->folderPath; 

	int UserEntry = 0;

	while (isEndending) {
		MergeFromCSVPrint(fromCsv);
		UserEntry = 0;

		printf("Entrez votre choix (1-5) : ");
		scanf("%d", &UserEntry);

		clearInputBuffer();

		switch (UserEntry) {
		case 1:
			printf("Entrez le chemin du fichier CSV : ");
			scanf("%s", fromCsv->CSVpath);
			clearInputBuffer();
			fromCsv->valideFile = checkFile(fromCsv->CSVpath);
			break;

		case 2:
			fromCsv->valideFile = checkFile(fromCsv->CSVpath);
			fromCsv->validFolder = checkFolder(fromCsv->folderPath);
			if (fromCsv->valideFile && fromCsv->validFolder) 
			{

				Table* tableFromCsv = Table_createFromCSV(fromCsv->CSVpath, fromCsv->folderPath);

				if (tableFromCsv) {
					//Table_debugPrint(tableFromCsv);
					printf(FG_GREEN "Table creee avec succès.\n" RESET);
					
					int back = mergeTwoDataBase(table, tableFromCsv);

					if (back == 1) 
					{
						printf(FG_GREEN "Fusion de la table avec succès.\n" RESET); 
						UserEntry = 4; 
					}
					else
					{
						printf(FG_RED"Erreur lors de la fusion du Csv avec la Table\n"FG_WHITE);
					}
					
					printf(FG_GREEN "CSV fusionnee avec succès.\n" RESET);
					pauseForUser();
					char filePath[1024] = { '\0' }; //on nettoie les traces de notre passage
					snprintf(filePath, 1024, "%s/%s.tbl", fromCsv->folderPath, tableFromCsv->name);
					remove(filePath);
					memset(filePath, 0, 1024);
					snprintf(filePath, 1024, "%s/%s.dat", fromCsv->folderPath, tableFromCsv->name);
					remove(filePath);
					tableFromCsv->dataFile = NULL;

					//Table_destroy(tableFromCsv);
					goto end; 

				}
				else {
					printf(FG_RED "Erreur lors de la creation de la table.\n" RESET);
					pauseForUser();
					goto end;
				}
				isEndending = false;
			}
			else {
				if (!fromCsv->valideFile) {
					printf(FG_RED "Le fichier CSV est invalide ou inaccessible.\n" RESET);
				}
				if (!fromCsv->validFolder) {
					printf(FG_RED "Le dossier de destination est invalide ou inaccessible.\n" RESET);
				}
				pauseForUser();
			}
			break;
		
		case 3:
			isEndending = false; 
			goto end; 
			break;
		case 4:
			isEndending = false;
			goto end;
			break;

		default:
			printf(FG_RED "Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n" RESET);
			pauseForUser();
			break;
		}
	}	
end:	

	/*free(fromCsv->CSVpath);
	free(fromCsv->folderPath);
	free(fromCsv);
	*/

	return UserEntry == 4 ? 1 : 0;
}

int mergeTwoDataBase(Table* dest, Table* source)
{
	
	if (chekTableCompatible(dest, source) == false)
	{
		printf(FG_RED "Les tables ne sont pas compatibles.\n" RESET);
		pauseForUser(); 
		return 0 ;
	}

	Entry* oneEntry = Entry_create(source);
	for (int i = 0; i < source->entryCount; i++) //on parcours les entrees de la table source 
	{

		for (int j = 0; j < source->attributeCount; j++) //pour chque entres on recupere les attributs 
		{
			memset(oneEntry->values[j], 0, source->attributes[j].size); 
		}
		Table_readEntry(source, oneEntry, i*source->entrySize);  //on insert l'entree dans la table
		printf("val 0: %s \n", oneEntry->values[0]);
		Table_insertEntry(dest, oneEntry);  //on insert l'entree dans la table
	}
	return 1;
}
