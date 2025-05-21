#include "user.h"
#include "kernel/peterson.h"

#define MAX_PROCESSES 16
#define MAX_LOCKS (MAX_PROCESSES - 1)
#define MAX_LEVELS 4 

// struct peterson_lock *locks;
int locks[MAX_LOCKS];
int num_processes;
int process_index;
int levels;
int roles[MAX_LEVELS];
int lock_indices[MAX_LEVELS];

int tournament_create(int processes) {
    if (processes <= 0 || processes > MAX_PROCESSES || (processes & (processes - 1)) != 0)
        return -1; // Not a power of 2 or too large

    num_processes = processes;
    levels = 0;
    for (int i = processes; i > 1; i >>= 1)
        levels++;


    // locks = malloc(sizeof(struct peterson_lock) * (processes - 1));
    // if (!locks) return -1;

    for (int i = 0; i < processes - 1; i++) {
        // peterson_init(&locks[i]);
        locks[i] = peterson_create();
    }

    for (int i = 0; i < processes; i++) {
        int pid = fork();
        if (pid < 0) return -1;
        if (pid == 0) {
            process_index = i;

            
            for (int l = 0; l < levels; l++) {
                int bit_pos = levels - l - 1;
                roles[l] = (i & (1 << bit_pos)) >> bit_pos;
                int lock_id = i >> (levels - l);
                lock_indices[l] = (1 << l) - 1 + lock_id;
            }

            return i; // Tournament ID
        }
    }


    for (int i = 0; i < processes; i++)
        wait(0);

    exit(0);
    return -1; 
}

int tournament_acquire(void) {
    for (int l = 0; l < levels; l++) {
        int idx = lock_indices[l];
        int role = roles[l];
        // peterson_acquire(&locks[idx], role);
        peterson_acquire(locks[idx], role);
    }
    return 0;
}

int tournament_release(void) {
    for (int l = levels - 1; l >= 0; l--) {
        int idx = lock_indices[l];
        int role = roles[l];
        // peterson_release(&locks[idx], role);
        peterson_release(locks[idx], role);
    }
    return 0;
}
