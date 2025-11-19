#include <stdio.h>
#include <pthread.h>

#include "deadlock.h"

// Det her var første trådfunktion vi arbejdede med
void *my_thread_function(void *arg) {
    // Stack-framen for trådfunktionen bliver frigjort når tråden stopper
    // Men hukommelsen som tråden har adgang til er delt med resten af processen

    int i = 42;
    // struct foo foo = { asdf asdf asdf a };
    char *message = (char*) arg;
    printf("Thread siger: %s\n", message);
    return NULL;
}

struct payload {
    char *name;
    int count;
};

int counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// Det her var anden trådfunktion vi arbejdede med
// Den var ikke så heldig, fordi der opstod race conditions
void *counter_thread(void *arg) {
    struct payload *payload = (struct payload *) arg;

    for (int i = 0; i < payload->count; i++) {
        counter++;
        // printf("Traad %s loop iteration %d\n", payload->name, i);
    }
}

// Det her var tredje trådfunktion vi arbejdede med
// Den fikser race conditions med en mutex (mutual exclusion / lås)
void *safe_counter_thread(void *arg) {
    struct payload *payload = (struct payload *) arg;

    for (int i = 0; i < payload->count; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

int main(void) {
    // Sjov med void-pointere:
    int foo = 5;
    int* foop = &foo;
    void* vp = foop; // implicit cast til void*

    printf("foo er %d\n", * (int*) vp); // eksplicit cast tilbage til int*

    pthread_t my_thread;
    char *m = "Hello, World!";
    pthread_create(&my_thread, NULL, my_thread_function, m);
    pthread_join(my_thread, NULL);

    pthread_t a_traad;
    struct payload a_payload = { "A", 1'000'000 };
    pthread_t b_traad;
    struct payload b_payload = { "B", 1'000'000 };

    // Det her er ikke særlig sikkert, fordi de begynder at overskrive
    // counter via koden 'counter++' fordi det sker oven i hinanden.
    // Resultatet er at counter ikke slutter som 2'000'000' men noget meget mindre.
    // pthread_create(&a_traad, NULL, counter_thread, &a_payload);
    // pthread_create(&b_traad, NULL, counter_thread, &b_payload);

    pthread_create(&a_traad, NULL, safe_counter_thread, &a_payload);
    pthread_create(&b_traad, NULL, safe_counter_thread, &b_payload);

    pthread_join(a_traad, NULL);
    pthread_join(b_traad, NULL);

    pthread_mutex_destroy(&counter_mutex);
    printf("Done! counter = %d\n", counter);

    // Gul bil-eksperimentet
    pthread_t c_traad;
    pthread_t d_traad;

    printf("Gul bil eksperiment starter nu!\n");

    pthread_create(&c_traad, NULL, gul_bil_thread_c, "gul bil");
    pthread_create(&d_traad, NULL, gul_bil_thread_d, "gul bil");

    printf("Spillerne har observeret gule biler!\n");

    pthread_join(c_traad, NULL);
    pthread_join(d_traad, NULL);

    printf("Vinderen var %c...\n", get_winner());

    printf("Så, nu er den gule bil kørt væk, stop med at larme!\n");

    return 0;
}
