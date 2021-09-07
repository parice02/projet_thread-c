#ifndef UNISEX2_H_INCLUDED
#define UNISEX2_H_INCLUDED

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define HOMME 1 // Représente le genre masculin
#define FEMME 2 // Représente le genre féminin

/*
 * Modélisation (contextuelle) des toilettes
 * 
 * max_capacity (unsigned int) : capacité maximal des toilettes
 * nbre_occupant (unsigned int) : Nombre de personnes (hommes ou femmes) dans les toilettes
 * sex_in_bathroom (unsigned int) : sexe des personnes dans les toilettes
 * check_bathroom (pthread_mutex_t): mutex pour vérifier les toilettes avant un éventuel accès
 * check_condition (pthread_mutex_t) : mutex pour réveiller les thread en sommeil
 * man_condition (pthread_cond_t) : Condition de réveil d'un thread homme en sommeil
 * woma_condition (pthread_cond_t) : Condition de réveil d'un thread femme en sommeil
 * sem_bathroom (sem_t): Semaphore cotrolant l'accès aux toiletes
 */
typedef struct s_bathroom 
{
    unsigned int max_capacity;
    unsigned int nbre_occupant;
    unsigned short sex_in_bathroom;
    pthread_mutex_t check_condition;
    pthread_cond_t man_condition;
    pthread_cond_t woman_condition;
    pthread_mutex_t check_bathroom;
    sem_t sem_bathroom;
} t_bathroom;

/*
 * Structure représentant les hommes et femmes en attente pour l'utilisation des toilettes
 * 
 * nbre_man (unsigned int) : Nombre d'homme en attente
 * nbre_woman (unsigned int) : Nombre de femme en attente
 */
typedef struct s_attente
{
    unsigned int nbre_man;
    unsigned int nbre_woman;
}t_attente;

/*
 * Modélisation d'un thread
 * 
 * id_thread (int): Identité du thread. Un entier octroyé pendant sa création
 * sexe (unsigned short): Sexe du thread
 * bathroom (t_bathroom): Les toilettes
 * en_attente (t_attente): Ensemble de thread en attente
 */
typedef struct s_thread
{
    int id_thread;
    unsigned short sexe;
    t_bathroom *bathroom;
    t_attente *en_attente;
} t_thread;


void *man_thread(void *arg);
void *woman_thread(void *arg);
void use_bathroom(t_thread *thread, unsigned int duree);
void enter_bathroom(t_thread *thread, char *gender);
void leave_bathroom(t_thread *thread, char *gender);
void thread_at_work(int thread_id, char *gender);
void thread_want_use_bathroom(t_thread *thread, char *gender);
void init_nbre_thread(int *nbre_f, int *nbre_h, int *s, int argc, char const *argv[]);
void get_gender(unsigned short sexe, char gender[]);
int check_entier(const char *arg);

#endif // UNISEX2_H_INCLUDED
