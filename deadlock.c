#include "deadlock.h"
#include <pthread.h>
#include <stdio.h>

char gul_bil_captured_by = 'X';
pthread_mutex_t gul_bil_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gul_bil_stop_mutex = PTHREAD_MUTEX_INITIALIZER;

char get_winner() {
    return gul_bil_captured_by;
}

void *gul_bil_thread_c(void *arg) {
    char *str = (char *) arg;
    if (gul_bil_captured_by == 'X') {
        pthread_mutex_lock(&gul_bil_mutex);
        printf("[C] %s!\n", str);
        pthread_mutex_lock(&gul_bil_stop_mutex);
        printf("[C] Stop!\n");
        gul_bil_captured_by = 'C';
        pthread_mutex_unlock(&gul_bil_mutex);
        pthread_mutex_unlock(&gul_bil_stop_mutex);
    } else {
        printf("[C] :(\n");
    }
}

void *gul_bil_thread_d(void *arg) {
    char *str = (char *) arg;
    if (gul_bil_captured_by == 'X') {
        pthread_mutex_lock(&gul_bil_mutex);
        printf("[D] %s!\n", str);
        pthread_mutex_lock(&gul_bil_stop_mutex);
        printf("[D] Stop!\n");
        gul_bil_captured_by = 'D';
        pthread_mutex_unlock(&gul_bil_mutex);
        pthread_mutex_unlock(&gul_bil_stop_mutex);
    } else {
        printf("[D] :(\n");
    }
}
