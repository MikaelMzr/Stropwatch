#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <ncurses.h>
#include <unistd.h>


/*Mezouar Mikael 
 * Peinzi Ferdyl
 * Groupe 6*/
 
 
 /*
 int intervalle_ms(struct timeval temps_debut, struct timeval temps_fin){

	int temps;
	

	gettimeofday(&temps_debut, NULL);


	/* instuction 


	gettimeofday(&temps_fin, NULL);	

	temps = (((temps_fin.tv_sec - temps_debut.tv_sec) * 1000000 + temps_fin.tv_usec) - temps_debut.tv_usec);

	temps = temps / 1000; /* temps total en millisecondes 

	return temps;

} 
*/
/* Renvoie la valeur entiere de l'intervalle entre 'debut' et 'fin' en milliseconde */
int intervalle_ms(struct timeval debut, struct timeval fin) {
    return ((fin.tv_sec - debut.tv_sec) * 1000 + (fin.tv_usec - debut.tv_usec) / 1000);
}

int nb_ms_vers_centiemes(int nb_ms){ 


	int temps;

	temps = (nb_ms / 10) % 100;

	return temps;


}

int nb_ms_vers_secondes(int nb_ms){

	int temps;

	temps = (nb_ms / 1000) % 60;

	return temps;
}


int nb_ms_vers_minutes(int nb_ms){


	int temps;

	temps = ((nb_ms / 1000) / 60) % 60;

	return temps;
}


int nb_ms_vers_heures(nb_ms){

	int temps;

	temps = ((nb_ms / 1000) / 3600) % 100;

	return temps;
}

/*Affiche le chronometre*/
void affiche(int duree_tot){
	int i;
	int cent_sec;
	int sec;
	int min;
	int h;
	
	assert(duree_tot >=0);
	h = nb_ms_vers_heures(duree_tot);
	assert (h < 100);
	min = nb_ms_vers_minutes(duree_tot);
	sec = nb_ms_vers_secondes(duree_tot);
	cent_sec = nb_ms_vers_centiemes(duree_tot);
	mvprintw(0 , COLS / 2 - 8, "==Chronometre==");
	mvprintw(LINES / 2, COLS / 2 -6, "%2d : %2d : %2d : %2d", h, min, sec,cent_sec);
	
	for(i = 0; i < COLS ; i++)
		mvaddch(LINES - 8, i, '-');
	mvprintw(LINES - 7, 0, "Espace = start/pause");
	
	}
	
/* Affiche le chronometre en fonction d'un état et d'une durée */
void menu_chrono(void) {
    struct timeval temps_debut;
    struct timeval temps_fin;
    int touche;
    int duree_totale;
    int etat;

    duree_totale = 0;
    etat = 0;  /*0=pause et 1=en marche*/
    while (1) {
        touche = getch();
        if (touche == ' ') {
            if (etat == 0) {
                etat = 1;
                gettimeofday(&temps_debut, NULL);
            }
            else
                etat = 0;
        }
        usleep(1000);
        if (etat == 1) {
            gettimeofday(&temps_fin, NULL);
            duree_totale += intervalle_ms(temps_debut, temps_fin);
            gettimeofday(&temps_debut, NULL);
        }
        affiche(duree_totale);
        refresh();
    }
}

int main(int argc, char *argv[]) {
    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);
    menu_chrono();
    endwin();
    return 0;
}
