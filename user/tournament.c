#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: tournament <num_processes>\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    if (n != 2 && n != 4 && n != 8 && n != 16) {
        printf("Number of processes must be a power of 2 (2, 4, 8, or 16)\n");
        exit(1);
    }

    int id = tournament_create(n);
    if (id < 0) {
        printf("Error creating tournament\n");
        exit(1);
    }

    tournament_acquire();
    printf("Process %d (PID: %d) in critical section\n", id, getpid());
    tournament_release();

    exit(0);
}
