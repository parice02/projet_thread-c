#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include "unisex1.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int nbre_f; // Nombre de thread femme
    int nbre_h; // Nombre de thread homme

    t_bathroom bath_room = {
        .nbre_occupant = 0,
        .sex_in_bathroom = 0,
        .check_bathroom = PTHREAD_MUTEX_INITIALIZER,
        .check_condition = PTHREAD_MUTEX_INITIALIZER,
        .man_condition = PTHREAD_COND_INITIALIZER,
        .woman_condition = PTHREAD_COND_INITIALIZER,
        .max_capacity = 0
    };

    t_attente en_attente = {.nbre_man = 0, .nbre_woman = 0};

    init_nbre_thread(&nbre_f, &nbre_h, &bath_room.max_capacity, argc, argv);
    sem_init(&bath_room.sem_bathroom, 0, bath_room.max_capacity);

    t_thread thread_man[nbre_h];
    t_thread thread_woman[nbre_f];
    pthread_t id_thread_f[nbre_f], id_thread_h[nbre_h];
    int retour_f[nbre_f], retour_h[nbre_h];

    for (int i = 0; i < nbre_h; i++)
    {
        thread_man[i].id_thread = i;
        thread_man[i].sexe = HOMME;
        thread_man[i].bathroom = &bath_room;
        thread_man[i].en_attente = &en_attente;

        retour_h[i] = pthread_create(&id_thread_h[i], NULL, man_thread, (void*)&thread_man[i]);
        if (retour_h[i] == -1)
            fprintf(stderr, "Erreur lors de la création du thread homme %d!!\n", i);
    }

    for (int i = 0; i < nbre_f; i++)
    {
        thread_woman[i].id_thread = i;
        thread_woman[i].sexe = FEMME;
        thread_woman[i].bathroom = &bath_room;
        thread_woman[i].en_attente = &en_attente;

        retour_f[i] = pthread_create(&id_thread_f[i], NULL, woman_thread, (void*)&thread_woman[i]);
        if (retour_f[i] == -1)
            fprintf(stderr, "Erreur lors de la création du thread femme %d!!\n", i);
    }

    for (int i = 0; i < nbre_f; i++)
    {
        pthread_join(id_thread_f[i], NULL);
    }

   for (int i = 0; i < nbre_h; i++)
    {
        pthread_join(id_thread_h[i], NULL);
    }

    sem_destroy(&bath_room.sem_bathroom);
    pthread_cond_destroy(&bath_room.woman_condition);
    pthread_cond_destroy(&bath_room.man_condition);
    pthread_mutex_destroy(&bath_room.check_bathroom);
    pthread_mutex_destroy(&bath_room.check_condition);

    fprintf(stdout, "\n\n");
    fprintf(stdout, "Il y a %d hommes et %d femmes en attente à la fin du processus.\n", en_attente.nbre_man, en_attente.nbre_woman);
    fprintf(stdout, "Il y a %d personnes restés au toilettes à la fin du processus.\n", bath_room.nbre_occupant);
    fprintf(stdout, "sex_in_bathroom vaut %d.\n", bath_room.sex_in_bathroom);

    return 0;
}

/* 
 *  Permet d'initialiser le nombre de thread
 */
void init_nbre_thread(int *nbre_f, int *nbre_h, int *s, int argc, char const *argv[])
{
    if (argc == 4)
    {
        *nbre_h = check_entier(argv[1]);
        *nbre_f = check_entier(argv[2]);
        *s = check_entier(argv[3]);
    }
    else
    {
        *nbre_h = 15;
        *nbre_f = 16;
        *s = 3;
    }
}

/* 
 * Représente le thread est entrain de travailler
 */
void thread_at_work(int thread_id, char *gender)
{
    unsigned int duree_travail = rand()%10 + 1;

    fprintf(stdout, "%s %d est en plein travail pour %d seconde(s).\n", gender, thread_id, duree_travail);
    sleep(duree_travail);
}

/* 
 *
 */
void thread_want_use_bathroom(t_thread *thread, char *gender)
{
    enter_bathroom(thread, gender);
    leave_bathroom(thread, gender);
}

/*
 * Utilisation des toilettes
 */
void use_bathroom(t_thread *thread, unsigned int duree)
{    
    sleep(duree);
}

/*
 * Fonction thread homme
 */
void *man_thread(void *arg)
{
    t_thread *thread_man = (t_thread *) arg;
    char gender[10];
    get_gender(thread_man->sexe, gender);

    while (1)
    {
        thread_at_work(thread_man->id_thread, gender);
        thread_want_use_bathroom(thread_man, gender);
    }

    pthread_exit(NULL);
}

/* 
 * Fonction thread femme
 */
void *woman_thread(void *arg)
{
    t_thread *thread_woman = (t_thread *) arg;
    char gender[10];
    get_gender(thread_woman->sexe, gender);

    while (1)
    {
        thread_at_work(thread_woman->id_thread, gender);
        thread_want_use_bathroom(thread_woman, gender);
    }

    pthread_exit(NULL);
}

/*
 *  Gère l'entrée des toilettes
 */
void enter_bathroom(t_thread *thread, char *gender)
{
    Label :fprintf(stdout, "%s %d tente d'accéder aux toilettes.\n", gender, thread->id_thread);
    pthread_mutex_lock(&thread->bathroom->check_bathroom);
    int nbre_cle_libre;
    sem_getvalue(&thread->bathroom->sem_bathroom, &nbre_cle_libre);
    if (thread->bathroom->max_capacity == nbre_cle_libre)
    {
        fprintf(stdout, "Les toilettes sont vides.\n");
        fprintf(stdout, "%s %d entre aux toilettes.\n", gender, thread->id_thread);
        thread->bathroom->sex_in_bathroom = thread->sexe;
    }
    else if ((thread->bathroom->sex_in_bathroom == thread->sexe) && (nbre_cle_libre != 0))
    {
        fprintf(stdout, "%s %d a accès car seul(e)s les %ss ont accès aux toilettes.\n", gender, thread->id_thread, gender);
    }
    else
    {
        pthread_mutex_lock(&thread->bathroom->check_condition);
        fprintf(stdout, "%s %d est en attente.\n", gender, thread->id_thread);
        if (thread->sexe == HOMME)
        {
            thread->en_attente->nbre_man++;
            fprintf(stdout, "Il y a maintenant %d homme(s) en attente.\n", thread->en_attente->nbre_man);
            pthread_mutex_unlock(&thread->bathroom->check_bathroom);
            pthread_cond_wait(&thread->bathroom->man_condition, &thread->bathroom->check_condition);
        }
        else
        {
            thread->en_attente->nbre_woman++;
            fprintf(stdout, "Il y a maintenant %d femme(s) en attente.\n", thread->en_attente->nbre_woman);
            pthread_mutex_unlock(&thread->bathroom->check_bathroom);
            pthread_cond_wait(&thread->bathroom->woman_condition, &thread->bathroom->check_condition);
        }
        pthread_mutex_unlock(&thread->bathroom->check_condition);

        goto Label;
    }

    sem_wait(&thread->bathroom->sem_bathroom);
    thread->bathroom->nbre_occupant++;

    int duree = rand()%5 + 1;
    fprintf(stdout, "Il y a actuellement %d personne(s) dans les toilettes.\n", thread->bathroom->nbre_occupant);
    fprintf(stdout, "%s %d est aux toilettes pour une durée de %d seconde(s).\n", gender, thread->id_thread, duree);

    pthread_mutex_unlock(&thread->bathroom->check_bathroom);
    use_bathroom(thread, duree);
}

/* 
 * Gère la sortie des toilettes
 */
void leave_bathroom(t_thread *thread, char *gender)
{
    pthread_mutex_lock(&thread->bathroom->check_bathroom);
    fprintf(stdout, "%s %d sort des toilettes.\n", gender, thread->id_thread);
    thread->bathroom->nbre_occupant--;
    fprintf(stdout, "Il reste %d personne(s) dans les toilettes, %d homme(s) et %d femme(s) en attente.\n", thread->bathroom->nbre_occupant, thread->en_attente->nbre_man, thread->en_attente->nbre_woman);
    
    if (thread->bathroom->nbre_occupant != 0)
    {
        if (thread->sexe == HOMME)
        {
            if (thread->en_attente->nbre_man != 0)
            {
                pthread_mutex_lock(&thread->bathroom->check_condition);
                pthread_cond_signal(&thread->bathroom->man_condition);
                thread->en_attente->nbre_man--;
                pthread_mutex_unlock(&thread->bathroom->check_condition);
            }
        }
        else
        {
            if (thread->en_attente->nbre_woman != 0)
            {
                pthread_mutex_lock(&thread->bathroom->check_condition);
                pthread_cond_signal(&thread->bathroom->woman_condition);
                thread->en_attente->nbre_woman--;
                pthread_mutex_unlock(&thread->bathroom->check_condition);
            }
        }
    }
    else
    {
        thread->bathroom->sex_in_bathroom = 0;
        if (thread->sexe == HOMME)
        {
            if (thread->en_attente->nbre_woman != 0)
            {
                pthread_mutex_lock(&thread->bathroom->check_condition);
                pthread_cond_broadcast(&thread->bathroom->woman_condition);
                thread->en_attente->nbre_woman = 0;
                pthread_mutex_unlock(&thread->bathroom->check_condition);
            }
        }
        else
        {
            if (thread->en_attente->nbre_man != 0)
            {
                pthread_mutex_lock(&thread->bathroom->check_condition);
                pthread_cond_broadcast(&thread->bathroom->man_condition);
                thread->en_attente->nbre_man = 0;
                pthread_mutex_unlock(&thread->bathroom->check_condition);
            }
        }   
    }
    sem_post(&thread->bathroom->sem_bathroom);

    pthread_mutex_unlock(&thread->bathroom->check_bathroom);
}

/* 
 * Retoure de genre du thread courant
 * sexe (unsigned short): HOMME ou FEMME
 */
void get_gender(unsigned short sexe, char gender[])
{
    if (sexe == HOMME)
    {
        strcpy(gender, "Monsieur");
    }
    else if (sexe == FEMME)
    {
        strcpy(gender, "Madame");
    }
    else
    {
        fprintf(stderr, "%d ne correspond pas à un sexe: 1 => HOMME & 2 => FEMME.\n",sexe);
        exit(EXIT_FAILURE);
    }   
}

/* 
 *Contrôle les variables passées en argument dans le terminal (console)
 * 
 * Seuls les entiers positifs sont autorisés.
 */
int check_entier(const char *arg)
{
    for (int i = 0; i < strlen(arg); i++)
    {
        if (!isdigit(arg[i]))
        {
            fprintf(stderr, "%s n'est pas un entier positif.\n",arg);
            exit(EXIT_FAILURE);
        }
    }
    return atoi(arg);    
}