//
// Created by Moritz Maywald on 30.11.24.
//
#include <stdio.h>
#include <unistd.h>
#include <time.h>


#define ITERATIONS 10000000

int main() {
    struct timespec start, end;
    long iterations = ITERATIONS;
    double total_time_ns = 0;

    // Measure time for `getpid()` system call
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime start");
        return 1;
    }

    for (long i = 0; i < iterations; i++) {
        (void)getpid();
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime end");
        return 1;
    }

    // Calculate elapsed time in nanoseconds
    total_time_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    // Calculate the average time per system call
    double avg_time_per_call = total_time_ns / iterations;

    printf("Time for %ld calls to getpid(): %.2f ns\n", iterations, total_time_ns);
    printf("Average time per getpid() call: %.2f ns\n", avg_time_per_call);

    // Uncomment the following sections for read() and write() system call variants:


    // Measure time for `read()` system call (e.g., reading from stdin)
    char buf;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime start");
        return 1;
    }

    for (long i = 0; i < iterations; i++) {
        read(STDIN_FILENO, &buf, 0);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime end");
        return 1;
    }

    total_time_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    avg_time_per_call = total_time_ns / iterations;

    printf("Time for %ld calls to read(): %.2f ns\n", iterations, total_time_ns);
    printf("Average time per read() call: %.2f ns\n", avg_time_per_call);


    // Measure time for `write()` system call (e.g., writing to stdout)
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime start");
        return 1;
    }

    for (long i = 0; i < iterations; i++) {
        write(STDOUT_FILENO, NULL, 0);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime end");
        return 1;
    }

    total_time_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    avg_time_per_call = total_time_ns / iterations;

    printf("Time for %ld calls to write(): %.2f ns\n", iterations, total_time_ns);
    printf("Average time per write() call: %.2f ns\n", avg_time_per_call);

    return 0;
}
