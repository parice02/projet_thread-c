#ifndef UNISEX_H_INCLUDED
#define UNISEX_H_INCLUDED

#include <stdlib.h>
#include <pthread.h>

#define HOMME 1 // Représente le genre masculin
#define FEMME 2 // Représente le genre féminin

/*
 * Modélisation (contextuelle) des toilettes
 * 
 *  * nbre_occupant (unsigned int) : Nombre de personnes (hommes ou femmes) dans les toilettes
 *  * sex_in_bathroom (unsigned int) : sexe des personnes dans les toilettes
 *  * check_bathroom (pthread_mutex_t): mutex pour vérifier les toilettes avant un éventuel accès
 *  * check_condition (pthread_mutex_t) : mutex pour réveiller les thread en sommeil
 *  * condition (pthread_cond_t) : Condition de réveil d'un thread en sommeil
 */
typedef struct s_bathroom 
{
    unsigned int nbre_occupant;
    unsigned short sex_in_bathroom;
    pthread_mutex_t check_bathroom;
    pthread_mutex_t check_condition;
    pthread_cond_t condition;
} t_bathroom;

/*
 * Modélisation d'un thread
 * 
 *  * id_thread (int): Identité du thread. Un entier octroyé pendant sa création
 *  * sexe (unsigned short): Sexe du thread
 *  * bathroom (t_bathroom): Les toilettes
 */
typedef struct s_thread
{
    int id_thread;
    unsigned short sexe;
    t_bathroom *bathroom;
} t_thread;

void *man_thread(void *arg);
void *woman_thread(void *arg);
void use_bathroom(t_thread *thread, unsigned int duree);
void enter_bathroom(t_thread *thread, char *gender);
void leave_bathroom(t_thread *thread, char *gender);
void thread_at_work(int thread_id, char *gender);
void thread_want_use_bathroom(t_thread *thread, char *gender);
void init_nbre_thread(int *nbre_f, int *nbre_h, int argc, char const *argv[]);
void get_gender(unsigned short sexe, char gender[]);
int check_entier(const char *arg);

#endif // UNISEX_H_INCLUDED
