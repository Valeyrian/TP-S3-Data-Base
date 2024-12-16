#include "Ui.h"
#


void welcommePrint(void)
{
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
    printf(BG_BLACK FG_BLUE "\n-<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>-\n" RESET BG_BLACK);
    
    /*printf(BG_BLACK BOLD FG_MAGENTA  "\n ________  ________  _________  ________                 ________  ________  ________  _______      ");
    printf("\n|\\   ___ \\|\\   __  \\|\\___   ___\\\\   __  \\               |\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\                   ");
    printf("\n\\ \\  \\_|\\ \\ \\  \\|\\  \\|___ \\  \\_\\ \\  \\|\\  \\  ____________\\ \\  \\|\\ /\\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|     ");
    printf("\n \\ \\  \\ \\ \\ \\   __  \\   \\ \\  \\ \\ \\   __  \\|\\____________\\ \\   __  \\ \\   __  \\ \\_____  \\ \\  \\_|/__          ");
    printf("\n  \\ \\  \\_\\ \\ \\  \\ \\  \\   \\ \\  \\ \\ \\  \\ \\  \\|____________|\\ \\  \\|\\  \\ \\  \\ \\  \\|____|\\  \\ \\  \\_|\\ \\");
    printf("\n   \\ \\_______\\ \\__\\ \\__\\   \\ \\__\\ \\ \\__\\ \\__\\              \\ \\_______\\ \\__\\ \\__\\____\\_\\  \\ \\_______\\   ");
    printf("\n    \\|_______|\\|__|\\|__|    \\|__|  \\|__|\\|__|               \\|_______|\\|__|\\|__|\\_________\\|_______|                   ");
    printf("\n                                                                               \\|_________|                                      " RESET);
    */

    printf(FG_MAGENTA BOLD );
	printf(" ______   _______ _________ _______         ______   _______  _______  _______"); printf("\n");
    printf("(  __  \\ (  ___  )\\__   __ /( ___  )       (  ___ \\ (  ___  )(  ____ \(  ____ \\ "); printf("\n");
	printf("| (  \\  )| (   ) |   ) (   | (   ) |       | (   ) )| (   ) || (    \\/| (    \/ "); printf("\n");
	printf("| |   ) || (___) |   | |   | (___) | _____ | (__/ / | (___) || (_____ | (__ "); printf("\n");
	printf("| |   | ||  ___  |   | |   |  ___  |(_____)|  __ (  |  ___  |(_____  )|  __) "); printf("\n");
	printf("| |   ) || (   ) |   | |   | (   ) |       | (  \\ \\ | (   ) |      ) || ( "); printf("\n");
	printf("| (__/  )| )   ( |   | |   | )   ( |       | )___) )| )   ( |/\____) || (____/\\ "); printf("\n");
	printf("(______/ |/     \\|   )_(   |/     \\|       |/ \\___/ |/     \\|\\_______)(_______/ "); printf("\n");
	printf("\n"RESET BG_BLACK);

    /*
    
         ______   _______ _________ _______         ______   _______  _______  _______ 
        (  __  \ (  ___  )\__   __/(  ___  )       (  ___ \ (  ___  )(  ____ \(  ____ \
        | (  \  )| (   ) |   ) (   | (   ) |       | (   ) )| (   ) || (    \/| (    \/
        | |   ) || (___) |   | |   | (___) | _____ | (__/ / | (___) || (_____ | (__    
        | |   | ||  ___  |   | |   |  ___  |(_____)|  __ (  |  ___  |(_____  )|  __)   
        | |   ) || (   ) |   | |   | (   ) |       | (  \ \ | (   ) |      ) || (      
        | (__/  )| )   ( |   | |   | )   ( |       | )___) )| )   ( |/\____) || (____/\
        (______/ |/     \|   )_(   |/     \|       |/ \___/ |/     \|\_______)(_______/
                                                                               
                                                                         
    
    */


    printf(BG_BLACK  FG_BLUE "<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>--<>-\n" RESET BG_BLACK);
    printf(BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "X" RESET BG_BLACK "                                                    " FG_WHITE FG_WHITE "X" BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "X" RESET BG_BLACK "                     " UNDERLINE FG_RED "Made by :" RESET "                    " RESET BG_BLACK "  " FG_WHITE FG_WHITE "X" BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "X" RESET BG_BLACK " " FG_RED BOLD "          Alan RIVEY - Noann MAZON-MAZA           " RESET BG_BLACK " " FG_WHITE FG_WHITE "X" BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "X" BG_BLACK "                  " UNDERLINE FG_RED "Supervised by :" RESET "                " RESET BG_BLACK "   " FG_WHITE FG_WHITE "X" BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "X" RESET FG_RED BOLD BG_BLACK "  Arnaud BANNIER - Nicolas BODIN - Mathieu LEBERRE " RESET BG_BLACK " " FG_WHITE FG_WHITE "X" BG_BLACK "\n");
    printf(BG_BLACK "                         " FG_WHITE FG_WHITE "X" RESET BG_BLACK "                                                    "RESET FG_WHITE FG_WHITE"X"RESET BG_BLACK);
    printf(BG_BLACK "\n                         " RESET FG_WHITE FG_WHITE  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" RESET);
    printf(BG_BLACK "\n\n\n");
}

void selctActionPrint(void)
{

    printf(RESET BG_BLACK FG_WHITE);
    printf("\n");
    printf("    "BOLD FG_WHITE UNDERLINE"Que souhaitez-vous faire ?\n" RESET);
    printf("\n");
    printf(FG_WHITE ITALIC"   1. Creer une nouvelle BDD depuis un CSV\n");
    printf("   2. Ouvrir une BDD existante\n");
    printf("   3. Creer manuellement une nouvelle BBD\n");
    printf("   4. Quitter\n");
	printf("\n  ");


}

void createFromCSVPrint(FromCsv* fromCsv)
{
    printf(RESET BG_BLACK FG_WHITE);
        for (int i = 0; i < 100; i++)
        {
            printf("\n");
        }
        printf("\n");
        //printf("        "BOLD FG_WHITE UNDERLINE"Creer une nouvelle BDD depuis un CSV\n\n" RESET);//

		printf(FG_TRUECOLOR(255,151,0)   "");
        
        
        printf(" ______                __          ______                        ____________    __\n");
		printf("/ ____/_______  ____ _/ /____     / ____/________  ____ ___     / ____/ ___/ |  / /\n");
		printf("/ /   / ___/ _ \\/ __ `/ __/ _ \\   / /_  / ___/ __ \\/ __ `__ \\   / /    \\__ \\| | / /\n");
		printf("/ /___/ /  /  __/ /_/ / /_/  __/  / __/ / /  / /_/ / / / / / /  / /___ ___/ /| |/ /\n");
		printf("\\____/_/   \\___/\\__,_/\\__/\\___/  /_/   /_/   \\____/_/ /_/ /_/   \\____//____/ |___/\n\n\n" RESET);


        /*
        * 

           ______                __          ______                        ____________    __
          / ____/_______  ____ _/ /____     / ____/________  ____ ___     / ____/ ___/ |  / /
         / /   / ___/ _ \/ __ `/ __/ _ \   / /_  / ___/ __ \/ __ `__ \   / /    \__ \| | / /
        / /___/ /  /  __/ /_/ / /_/  __/  / __/ / /  / /_/ / / / / / /  / /___ ___/ /| |/ /
        \____/_/   \___/\__,_/\__/\___/  /_/   /_/   \____/_/ /_/ /_/   \____//____/ |___/



        */

        printf(FG_WHITE ITALIC"");
        printf("\n\n");

		printf("    "BOLD UNDERLINE FG_WHITE"Options :"RESET"\n\n ");

		printf("   1. Modifier le chemin du fichier CSV : ");
		printf(FG_YELLOW ITALIC"%s\n" RESET, fromCsv->CSVpath);  
		
		printf("    2. Modifier le chemin du dossier de destination : ");
		printf(FG_YELLOW ITALIC"%s\n" RESET, fromCsv->folderPath);
		
	
		printf("    3. Valier et continuer\n");
		printf("    4. Retour\n"); 
		printf("    5. Quiter\n\n"); 
}

void loadDatabasePrint(DataBaseLoad* dataBaseLoad)
{
	printf(RESET BG_BLACK FG_WHITE);
	for (int i = 0; i < 100; i++)
	{
		printf("\n");
	}
	printf("\n");
	//printf("        "BOLD FG_WHITE UNDERLINE"Creer une nouvelle BDD depuis un CSV\n\n" RESET);//

	/*
    
            __                    __   ____        __              ____                
           / /   ____  ____ _____/ /  / __ \____ _/ /_____ _      / __ )____ _________ 
          / /   / __ \/ __ `/ __  /  / / / / __ `/ __/ __ `/_____/ __  / __ `/ ___/ _ \
         / /___/ /_/ / /_/ / /_/ /  / /_/ / /_/ / /_/ /_/ /_____/ /_/ / /_/ (__  )  __/
        /_____/\____/\__,_/\__,_/  /_____/\__,_/\__/\__,_/     /_____/\__,_/____/\___/ 
                                                                               

    */

	printf(FG_TRUECOLOR(255, 151, 0)""); 
    printf(" __                     __   ____        __              ____                \n");
	printf("/ /    ____  ____ _____/ /  / __ \\____ _/ /_____ _      / __ )____ _________ \n");
	printf("/ /   / __ \\/ __ `/ __  /  / / / / __ `/ __/ __ `/_____/ __  / __ `/ ___/ _ \\\n");
	printf("/ /___/ /_/ / /_/ / /_/ /  / /_/ / /_/ / /_/ /_/ /_____/ /_/ / /_/ (__  )  __/\n");
	printf("/_____/\____/\\__,_/\\__,_/  /_____/\\__,_/\\__/\\__,_/     /_____/\\__,_/____/\\___/ \n\n\n" RESET);

	printf(FG_WHITE ITALIC"");

    printf("    "BOLD UNDERLINE FG_WHITE"Options :"RESET"\n\n ");

    printf("   1. Modifier le chemin du fichier table : ");
    printf(FG_YELLOW ITALIC"%s\n" RESET, dataBaseLoad->tblFile);

    printf("    2. Modifier le chemin du fichier dat : ");
    printf(FG_YELLOW ITALIC"%s\n" RESET, dataBaseLoad->datFile);

    printf("    3. Modifier le chemin du dossier contenant les index : ");
    printf(FG_YELLOW ITALIC"%s\n" RESET, dataBaseLoad->idxFolder);

    printf("    4. Valier et continuer\n");
    printf("    5. Retour\n");
    printf("    6. Quiter\n\n");

}

void iterPrint(Table* table, SetEntry *result)
{
    int i = 1;
    Entry* entry = Entry_create(table); 
    SetEntryIter* it = SetEntryIter_create(result); 


    while (SetEntryIter_isValid(it))
    { 
    EntryPointer entryPtr = SetEntryIter_getValue(it);  
	
    Table_readEntry(table, entry, entryPtr);    


	printf("\n\n    ""Correspondance numero "FG_GREEN"%d"FG_WHITE" : \n", i);

    for (int j = 0; j < entry->attributeCount; j++)
    {
        printf(FG_WHITE "    T------------\n" );
        printf(FG_CYAN "    |  - "FG_MAGENTA"%s : " FG_YELLOW "%s\n" ,table->attributes[j].name, entry->values[j]);
    }
    printf(FG_WHITE "    L------------\n" );

    SetEntryIter_next(it);
    i++; 
    }
    SetEntryIter_destroy(it);

}

void actionOnDatabasePrint(Table *table)
{
	printf(RESET BG_BLACK FG_WHITE);
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
    printf(FG_TRUECOLOR(255, 151, 0)"");
    printf("        ___        __  _                               ____        __              ____                \n");
	printf("       /   | _____/ /_(_)___  ____     ____  ____     / __ \\____ _/ /_____ _      / __ )____ _________ \n");
	printf("      / /| |/ ___/ __/ / __ \\/ __ \\   / __ \\/ __ \\   / / / / __ `/ __/ __ `/_____/ __  / __ `/ ___/ _ \\\n");
	printf("     / ___ / /__/ /_/ / /_/ / / / /  / /_/ / / / /  / /_/ / /_/ / /_/ /_/ /_____/ /_/ / /_/ (__  )  __/\n");
	printf("    /_/  |_/\\___/\\__/_/\\____/_/ /_/   \\____/_/ /_/  /_____/\\__,_/\\__/\\__,_/     /_____/\\__,_/____/\\___/ \n");
	printf("\n\n\n" RESET);
   
    /*
    
        ___        __  _                               ____        __              ____                
       /   | _____/ /_(_)___  ____     ____  ____     / __ \____ _/ /_____ _      / __ )____ _________ 
      / /| |/ ___/ __/ / __ \/ __ \   / __ \/ __ \   / / / / __ `/ __/ __ `/_____/ __  / __ `/ ___/ _ \
     / ___ / /__/ /_/ / /_/ / / / /  / /_/ / / / /  / /_/ / /_/ / /_/ /_/ /_____/ /_/ / /_/ (__  )  __/
    /_/  |_\___/\__/_/\____/_/ /_/   \____/_/ /_/  /_____/\__,_/\__/\__,_/     /_____/\__,_/____/\___/ 
                                                                                                   
    */

	printf(FG_GREEN ITALIC"La BDD %s a correctement etait ouverte\n\n"RESET,table->name);
    printf("    "BOLD UNDERLINE FG_WHITE"Que souhaitez vous faire :"RESET"\n\n ");

    printf("   1. Rechercher dans la BDD.\n");

    printf("    2. Ajouter des entrees dans la BDD.\n");

    printf("    3. Supprimer une entree dans la BDD.\n");

    printf("    4. Afficher la BDD "FG_RED"/!\\ Attention cela peut etre long. /!\\ \n"RESET);
    
    printf("    5. Retour\n");
    
    printf("    6. Quiter\n\n");

}

void searchInDatabasePrint(Table* table, Filter* filter, SearchInfo* info)  
{
	printf(RESET BG_BLACK FG_WHITE);
	for (int i = 0; i < 100; i++)
	{
		printf("\n");
	}
	printf(FG_TRUECOLOR(255, 151, 0)"");
    /*
       _____                      __  
      / ___/___  ____ ___________/ /_ 
      \__ \/ _ \/ __ `/ ___/ ___/ __ \
     ___/ /  __/ /_/ / /  / /__/ / / /
    /____/\___/\__,_/_/   \___/_/ /_/ 
                                  
    */
	printf(" _____                      __  \n");
	printf("/ ___/___  ____ ___________/ /_ \n");
	printf("\\__ \\/ _ \\/ __ `/ ___/ ___/ __ \\\n");
	printf("___/ /  __/ /_/ / /  / /__/ / / /\n");
	printf("/____/\\___/\\__,_/_/   \\___/_/ /_/ \n\n\n" RESET);

	if (!info->isFilterSelectionEnded)
    {
        printf(FG_WHITE ITALIC"");
        printf("    "BOLD UNDERLINE FG_WHITE"Recherche dans la BDD "FG_BRIGHT_GREEN"%s\n\n"RESET, table->name);
        printf(" Listes des Attributs : ");
		for (int i = 0; i < table->attributeCount; i++)
		{
			printf("| "  FG_YELLOW "%d"FG_WHITE ": "FG_CYAN"%s "FG_WHITE, i, table->attributes[i].name);
		}
        printf("\n Liste des comparateurs : ");
			printf(FG_WHITE"| " FG_YELLOW "0"FG_WHITE": " FG_MAGENTA "Infferieur ");
			printf(FG_WHITE"| "FG_YELLOW "1"FG_WHITE": " FG_MAGENTA "Infferieur ou egale ");
            printf(FG_WHITE"| "FG_YELLOW "2"FG_WHITE": " FG_MAGENTA "egale ");
            printf(FG_WHITE"| "FG_YELLOW "3"FG_WHITE": " FG_MAGENTA "supperieur ou egale ");
            printf(FG_WHITE"| "FG_YELLOW "4"FG_WHITE": " FG_MAGENTA "supperieur ");
            printf(FG_WHITE"| "FG_YELLOW "5"FG_WHITE": " FG_MAGENTA "situe entre\n\n"FG_WHITE);

        printf("    "BOLD UNDERLINE FG_WHITE"Options :"RESET"\n\n");

        if (info->filterCount > 0)
        {
			printf("Voici vos filtres actuels : \n");
			for (int i = 0; i < info->filterCount; i++)
			{
				printf("\t- Filtre"FG_BRIGHT_BLUE" %d "FG_WHITE":" , i);
				if (filter->requestOp == OP_BETW)
                {
                    printf(FG_WHITE "elements de l'attribut " FG_CYAN "%s" FG_WHITE ", ", table->attributes[filter->attributeIndex].name);
                    printf("de la table " FG_YELLOW "%s" FG_WHITE ", ", table->name);
                    printf("compris entre " FG_GREEN "%s " FG_WHITE "et " FG_GREEN "%s\n" FG_WHITE, filter->key1, filter->key2);
				}
                else
                {
                    printf("Elements de l'attribut"FG_CYAN" ""%s"" " FG_WHITE, table->attributes[filter->attributeIndex].name);
                    printf("de la table"FG_YELLOW" ""%s"" "FG_WHITE, table->name);
                    
                    switch (filter->requestOp)
					{
					case OP_LT:
						printf("qui sont "FG_MAGENTA"inferieur "FG_WHITE"a"FG_GREEN" %s.\n"FG_WHITE, filter->key1);
						break; 
					case OP_LEQ:
						printf("qui sont "FG_MAGENTA"inferieur ou egale"FG_WHITE" a"FG_GREEN" %s.\n"FG_WHITE, filter->key1);
						break;
					case OP_EQ:
						printf("qui sont "FG_MAGENTA"egale"FG_WHITE" a"FG_GREEN" %s.\n"FG_WHITE, filter->key1);
						break;
					case OP_GEQ:
						printf("qui sont "FG_MAGENTA" supperieur ou egale"FG_WHITE" a"FG_GREEN" %s.\n"FG_WHITE, filter->key1);
						break;
					case OP_GT:
						printf("qui sont "FG_MAGENTA"supperieur"FG_WHITE" a"FG_GREEN" %s.\n"FG_WHITE, filter->key1);
						break;
					}

                    
				}
			}
			printf("\n");
        }

        printf("    1. Ajouter un filtre\n");

        printf("    2. Reinitialiser les filtres\n");

        printf("    3. Valider et lancer la recherche\n");

        printf("    4. Retour\n");

        printf("    5. Quiter\n\n");
	}
    else
    {
        printf(FG_WHITE ITALIC"");
        printf("    "BOLD UNDERLINE FG_WHITE"Resultat de la recherche :"RESET"\n "); 
		printf("    "FG_GREEN"Nombre de correspondances trouve : "FG_WHITE"%d\n\n", info->matchCount);
        printf("    "BOLD UNDERLINE FG_WHITE"Options :"RESET"\n\n");

        printf("    1. Modifier les parametres de la recherche\n");
            
		printf("    2. Afficher les correspondances \n"RESET);

        printf("    3. Sauvegarder les resultats dans un fichier\n");

        printf("    4. Retour\n");

        printf("    5. Quiter\n\n");


	}




}

void insertInDatabasePrint(Table* table)
{
	printf(RESET BG_BLACK FG_WHITE);
	for (int i = 0; i < 100; i++)
	{
		printf("\n");
	}
	printf(FG_TRUECOLOR(255, 151, 0)"");
	
	printf("____                     __     _          ____        __              ____                \n");
	printf("/  _/___  ________  _____/ /_   (_)___     / __ \\____ _/ /_____ _      / __ )____ _________ \n");
	printf("/ // __ \\/ ___/ _ \\/ ___/ __/  / / __ \\   / / / / __ `/ __/ __ `/_____/ __  / __ `/ ___/ _ \\\n");
	printf("_/ // / / (__  )  __/ /  / /_   / / / / /  / /_/ / /_/ / /_/ /_/ /_____/ /_/ / /_/ (__  )  __/\n");
	printf("/___/_/ /_/____/\\___/_/   \\__/  /_/_/ /_/  /_____/\\__,_/\\__/\\__,_/     /_____/\\__,_/____/\\___/ \n\n\n" RESET);

    /*
    ____                     __     _          ____        __              ____                
   /  _/___  ________  _____/ /_   (_)___     / __ \____ _/ /_____ _      / __ )____ _________ 
   / // __ \/ ___/ _ \/ ___/ __/  / / __ \   / / / / __ `/ __/ __ `/_____/ __  / __ `/ ___/ _ \
 _/ // / / (__  )  __/ /  / /_   / / / / /  / /_/ / /_/ / /_/ /_/ /_____/ /_/ / /_/ (__  )  __/
/___/_/ /_/____/\___/_/   \__/  /_/_/ /_/  /_____/\__,_/\__/\__,_/     /_____/\__,_/____/\___/ 
                                                                                               
*/

	
    printf("    "BOLD UNDERLINE FG_WHITE"Que souhaitez vous faire :"RESET"\n\n ");

	printf("   1. Ajouter une entree manuellement\n");

	printf("    2. Ajouter plusieurs entree manuellement\n");
	
    printf("    3. Ajouter plusieurs entree depuis un fichier CSV\n");

	printf("    4. Ajouter plusieurs entree depuis une autre table\n");

	printf("    4. Retour\n");

	printf("    5. Quiter\n");



}

void MergeFromCSVPrint(FromCsv* fromCsv)
{
    printf(RESET BG_BLACK FG_WHITE);
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
    printf("\n");
    printf("        "BOLD FG_WHITE UNDERLINE"Ajouter des donnes depuis un CSV\n\n" RESET);//

  
    printf(FG_WHITE ITALIC"");
    printf("\n\n");

    printf("    "BOLD UNDERLINE FG_WHITE"Options :"RESET"\n\n ");

    printf("   1. Modifier le chemin du fichier CSV : ");
    printf(FG_YELLOW ITALIC"%s\n" RESET, fromCsv->CSVpath);

    printf("    2. Valier et continuer\n");
    printf("    3. Retour\n");
    printf("    4. Quiter\n\n");
}