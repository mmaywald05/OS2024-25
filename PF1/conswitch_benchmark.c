//
// Created by Moritz Maywald on 30.11.24.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 1000000

pthread_mutex_t mutex;
pthread_cond_t cond;
int turn = 0;  // Thread turn (0 or 1)
int switches = 0;  // Context switch count

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    struct timespec start, end;
    double total_time = 0;

    // Benchmark context switch times
    for (int i = 0; i < ITERATIONS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Lock the mutex and wait for the turn
        pthread_mutex_lock(&mutex);
        while (turn != thread_id) {
            pthread_cond_wait(&cond, &mutex);
        }

        // Update turn and signal the other thread
        turn = 1 - thread_id;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate time taken for this context switch
        double time_taken = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        total_time += time_taken;
        switches++;
    }

    // Print average context switch time for this thread
    printf("Thread %d: Average context switch time = %.2f ns\n", thread_id, total_time / ITERATIONS);
    return NULL;
}

int main() {
    pthread_t threads[2];
    int thread_ids[2] = {0, 1};

    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create two threads
    pthread_create(&threads[0], NULL, thread_function, &thread_ids[0]);
    pthread_create(&threads[1], NULL, thread_function, &thread_ids[1]);

    // Wait for both threads to complete
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    // Print total context switches
    printf("Total context switches: %d\n", switches);
    return 0;
}