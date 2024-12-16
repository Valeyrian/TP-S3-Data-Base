#include <stdio.h>
#include <stdlib.h>
#include "Settings.h"
#include "Index.h"
#include "Table.h"
#include "Menus.h"
#include "SetEntry.h"



// Réinitialisation
#define RESET "\033[0m"

// Styles
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK_SLOW "\033[5m"
#define BLINK_FAST "\033[6m"
#define INVERT "\033[7m"
#define HIDDEN "\033[8m"
#define STRIKETHROUGH "\033[9m"

// Couleurs (foreground)
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_BLUE "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"

// Couleurs brillantes (foreground bright)
#define FG_BRIGHT_BLACK "\033[90m"
#define FG_BRIGHT_RED "\033[91m"
#define FG_BRIGHT_GREEN "\033[92m"
#define FG_BRIGHT_YELLOW "\033[93m"
#define FG_BRIGHT_BLUE "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN "\033[96m"
#define FG_BRIGHT_WHITE "\033[97m"

// Couleurs de fond (background)
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

// Couleurs brillantes de fond (background bright)
#define BG_BRIGHT_BLACK "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"

// Couleurs étendues (256 couleurs)
#define FG_E TENDED(color) "\033[38;5;" #color "m"
#define BG_E TENDED(color) "\033[48;5;" #color "m"

// Couleurs TrueColor (24 bits)
#define FG_TRUECOLOR(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m"
#define BG_TRUECOLOR(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m"

// E emple de combinaison
#define STYLE_COMBINE(fg, bg, style) style fg bg



// Ecran de debut
void welcommePrint(void);

//menu de selection d'action
void selctActionPrint(void);

//menu d'affichage d'iterateur
void iterPrint(Table* table, SetEntry *result);

//menu de creation de BDD depuis un CSV
void createFromCSVPrint(FromCsv* fromCsv);

//menu de chargement de BDD
void loadDatabasePrint(dbLoad); 

//menu d'action sur une BDD
void actionOnDatabasePrint(Table *table);

// menu de recherche dans une BDD
void searchInDatabasePrint(Table* table, Filter * filter, SearchInfo *info);  

//menu pour l'insertion dans une BDD
void insertInDatabasePrint(Table* table);