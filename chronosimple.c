#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#define DELAI 500000

/*Mezouar Mikael 
 * Peinzi Ferdyl
 * Groupe 6*/
 
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




int main(void){


	struct timeval temps_debut;
	struct timeval temps_fin;

	int temps;


	gettimeofday(&temps_debut, NULL);

	while(1){

		gettimeofday(&temps_fin, NULL);

		temps = (((temps_fin.tv_sec - temps_debut.tv_sec) * 1000000 + temps_fin.tv_usec) - temps_debut.tv_usec);

		temps = temps / 1000;

		usleep(DELAI);

		printf("%d : %d : %d : %d \n", nb_ms_vers_heures(temps), nb_ms_vers_minutes(temps), nb_ms_vers_secondes(temps), nb_ms_vers_centiemes(temps));
		
	}

	return 0;
}