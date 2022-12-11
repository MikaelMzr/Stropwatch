#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <ncurses.h>
#define DELAI 1000
#define N_TOUR 5

/*Mezouar Mikael 
 * Peinzi Ferdyl
 * Groupe 6*/

typedef struct chrono {
    int etat;
    int duree_tot;
    int avert;
    int der_tour;
    int nb_tour;
    int tps_par_tour[N_TOUR];
} Chronometre;

/* Renvoie la valeur en int de l'intervalle entre 'debut' et 'fin' en milliseconde  */
int intervalle_ms(struct timeval debut, struct timeval fin) {
    return ((fin.tv_sec - debut.tv_sec) * 1000 + (fin.tv_usec - debut.tv_usec) / 1000);
}

/* renvoie la convertion de ms en centieme de seconde*/
int nb_ms_vers_centiemes(int nb_ms){


	int temps;

	temps = (nb_ms / 10) % 100;

	return temps;


}

/* renvoie la convertion de ms en seconde */
int nb_ms_vers_secondes(int nb_ms){

	int temps;

	temps = (nb_ms / 1000) % 60;

	return temps;
}

/* renvoie la convertion de ms en minute */
int nb_ms_vers_minutes(int nb_ms){


	int temps;

	temps = ((nb_ms / 1000) / 60) % 60;

	return temps;
}

/* renvoie la convertion de ms en heure */
int nb_ms_vers_heures(nb_ms){

	int temps;

	temps = ((nb_ms / 1000) / 3600) % 100;

	return temps;
}


/* Affiche la duree 'nb_ms' dans le forma demandé par le TP par rapport à une ordonnee 'y' et 
*  une abscisse 'x' de la fenetre*/
void afficher_duree(int y, int x, int nb_ms) {


    int cent;
    int sec;
    int min;
    int heures;
    
    assert(y >= 0);
    assert(y < LINES);
    assert(x >= 0);
    assert(x < COLS);
    assert(nb_ms >= 0);

    cent = nb_ms_vers_centiemes(nb_ms);
    sec = nb_ms_vers_secondes(nb_ms);
    min = nb_ms_vers_minutes(nb_ms);
    heures = nb_ms_vers_heures(nb_ms);

    mvprintw(y, x, "%2d", heures);
    printw(" : ");
    printw("%2d", min);
    printw(" : ");
    printw("%2d", sec);
    printw(" : ");
    printw("%2d", cent);
}

/* Affiche un rectangle alternant du noir au blanc de la taille de la fenetre */
void afficher_flash(void) {

    int col;
    int i;
    int j;

    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    attron(COLOR_PAIR(1));
    col = 0;
    while (col < 200) {
        if ((col % 2) == 1) {
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(2));
        }
        else {
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(1));
        }
        for (i = 0; i < LINES; i++){
            for (j = 0; j < COLS; j++)
                mvaddch(i, j, '*');
        }
        refresh();
        usleep(DELAI * 10);
        col++;
    }
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
    clear();
}

/* Fonction qui affiche l'interface du chronometre 'chrono' ainsi que son menu */
void affiche_interface(Chronometre chrono) {
    int i;

    assert(chrono.duree_tot >= 0);
    mvprintw(0, COLS / 2 - 8, "==Chronometre==");
    for (i = 0 ; i < chrono.nb_tour ; i++) {
        mvprintw(LINES / 2 - (i + 1), COLS / 2 - 17, "Tour %3d : ",
                 chrono.der_tour - i);
        afficher_duree(LINES / 2 - (i + 1), COLS / 2 - 6,
                       chrono.tps_par_tour[i]);
    }
    afficher_duree(LINES / 2, COLS / 2 - 6, chrono.duree_tot);
    mvprintw(LINES / 2 + 1, COLS / 2 - 22, "Avertissement :");
    afficher_duree(LINES / 2 + 1, COLS / 2 - 6, chrono.avert);
    for (i = 0 ; i < COLS ; i++)
        mvaddch(LINES - 8, i, '-');
    mvprintw(LINES - 7, 0, "Espace : Lancer / Pause");
    mvprintw(LINES - 6, 0, "r      : Reinitialiser");
    mvprintw(LINES - 5, 0, "t      : Tour");
    mvprintw(LINES - 4, 0,
             "F1/F2  : Incrementer / Decrementer les heures avertissement");
    mvprintw(LINES - 3, 0,
             "F3/F4  : Incrementer / Decrementer les minutes avertissement");
    mvprintw(LINES - 2, 0,
             "F5/F6  : Incrementer / Decrementer les secondes avertissement");
    mvprintw(LINES - 1, 0, "q      : Quitter");
}

/* Fonction qui renvoie une variable de type struct chrono initialiser */
Chronometre initialiser_chrono(void) {
    Chronometre chrono;
    int i;

    chrono.etat = 0;
    chrono.duree_tot = 0;
    chrono.avert = 20000;
    chrono.nb_tour = 0;
    chrono.der_tour = 0;
    for (i = 0 ; i < N_TOUR ; i++)
        chrono.tps_par_tour[i] = 0;
    return (chrono);
}

/* Reinitialise le chrono */
void reinitialiser_chrono(Chronometre *chrono) {
    int i;

    assert(chrono != NULL);
    chrono->etat = 0;
    chrono->nb_tour = 0;
    chrono->der_tour = 0;
    chrono->duree_tot = 0;
    for (i = 0 ; i < N_TOUR ; i++)
        chrono->tps_par_tour[i] = 0;
}

/* Fonction qui Effectue un decalage a droite dans le champ tableau tps_par_tour et affecte a l'indexe 0 la valeur du champ duree_tot */
void ajoute_tour(Chronometre *chrono) {
    int i;

    assert(chrono != NULL);
    chrono->der_tour++;
    if (chrono->nb_tour < N_TOUR)
        chrono->nb_tour++;
    for (i = chrono->nb_tour - 1 ; i > 0 ; i--)
        chrono->tps_par_tour[i] = chrono->tps_par_tour[i - 1];
    chrono->tps_par_tour[0] = chrono->duree_tot;
}

/* Fonction qui intervertit les valeurs 0 et 1 du champ etat du pointeur 'chrono' et
*  appelle la fonction gettimeofday() avec le pointeur 'temps_debut' en argument 
*  si la valeur du champ etat etait 0 */
void change_etat(Chronometre *chrono, struct timeval *temps_debut) {
    assert(chrono != NULL);
    assert(temps_debut != NULL);
    if (chrono->etat == 0) {
        chrono->etat = 1;
        gettimeofday(temps_debut, NULL);
    }
    else
        chrono->etat = 0;
}

/* Fonction qui incremente, si la valeur de 'm' est differente de 0 ou decremente 
*  sinon, la valeur du champ avert du pointeur 'chrono' de l'equivalent d'une
*  heure en ms */
void heure_avert(Chronometre *chrono, int m) {
    assert(m >= 0);
    assert(chrono != NULL);
    if (m) {
        assert(chrono->avert / (3600 * 1000) + 1 < 100);
        chrono->avert += (3600 * 1000);
    }
    else {
        assert(chrono->avert / (3600 * 1000) - 1 >= 0);
        chrono->avert -= (3600 * 1000);
    }
}


void minute_avert(Chronometre *chrono, int m) {
    assert(m >= 0);
    assert(m <= 1);
    assert(chrono != NULL);
    if (m)
        chrono->avert += (60 * 1000);
    else
        chrono->avert -= (60 * 1000);
}


void seconde_avert(Chronometre *chrono, int m) {
    assert(m >= 0);
    assert(m <= 1);
    assert(chrono != NULL);
    if (m)
        chrono->avert += 1000;
    else
        chrono->avert -= 1000;
}

/* Affiche un chronometre qui incrmente sa duree et en gérant les fonctionnalites (touche)*/
void menu(void) {
    struct timeval temps_debut;
    struct timeval temps_fin;
    int touche;
    int ligne_test;
    int colonne_test;
    Chronometre chrono;

    chrono = initialiser_chrono();
    ligne_test = LINES;
    colonne_test = COLS;
    while (1) {
        switch (touche) {
            case ' ' : change_etat(&chrono, &temps_debut); break;
            case 't' : ajoute_tour(&chrono); break;
            case 'r' : reinitialiser_chrono(&chrono);
                       clear(); break;
            case 'q' : return ;
            case KEY_F(1) : heure_avert(&chrono, 1); break; 
            case KEY_F(2) : heure_avert(&chrono, 0); break;
            case KEY_F(3) : minute_avert(&chrono, 1); break;
            case KEY_F(4) : minute_avert(&chrono, 0); break;
            case KEY_F(5) : seconde_avert(&chrono, 1); break;
            case KEY_F(6) : seconde_avert(&chrono, 0); break;
            default : break;
        }
        usleep(DELAI);
        if (chrono.etat == 1) {
            gettimeofday(&temps_fin, NULL);
            chrono.duree_tot += intervalle_ms(temps_debut, temps_fin);
            gettimeofday(&temps_debut, NULL);
        }
        if (chrono.avert == chrono.duree_tot) {
            afficher_flash();
        }
        if (LINES != ligne_test || colonne_test != COLS) {
            clear();
            ligne_test = LINES;
            colonne_test = COLS;
        }
        touche = getch();
        affiche_interface(chrono);
        refresh();
    }
}

int main(void) {
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    noecho();
    menu();
    endwin(); 
    return 0;
}    
