#include <stdio.h>
#include <pthread.h>

void *my_thread_function(void *arg) {
    char *message = (char*) arg;
    printf("Thread siger: %s\n", message);
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

    return 0;
}
