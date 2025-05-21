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
    
    // Calculate levels (log2(processes))
    levels = 0;
    for (int i = processes; i > 1; i >>= 1)
        levels++;

    // Create locks
    for (int i = 0; i < processes - 1; i++) {
        int id = peterson_create();
        if (id < 0) return -1;
        locks[i] = id;
    }

    // Fork children
    for (int i = 0; i < processes; i++) {
        int pid = fork();
        if (pid < 0) return -1; // fork failed
        if (pid == 0) {
            // Child process logic
            process_index = i;

            for (int l = 0; l < levels; l++) {
                int bit_pos = levels - l - 1;
                roles[l] = (i & (1 << bit_pos)) >> bit_pos;
                int lock_l = i >> (levels - l);
                lock_indices[l] = lock_l + ((1 << l) - 1);
            }
            return i; // Child returns its tournament ID
        }
        if (pid > 0) {
            for (int i = 0; i < processes; i++){
                wait(0);  
                // printf("Parent process (PID: %d) created child (PID: %d)\n", getpid(), pid);
            }
            // Only parent cleans up
            // for (int i = 0; i < processes - 1; i++) {
            //     peterson_destroy(locks[i]);
            // }
            // tournament 
        }
    }

    // printf("Parent process (PID: %d) child %d exited\n", getpid(), process_index);
    return -10;
}


int tournament_acquire(void) {
    for (int l = 0; l < levels; l++) {
        int lock_id = locks[lock_indices[l]];
        int role = roles[l];
        if(peterson_acquire(lock_id, role) < 0) {
            return -1; // Failed to acquire lock
        }   
    }
    return 0;
}

int tournament_release(void) {
  for (int l = levels - 1; l >= 0; l--) {
    int lock_id = locks[lock_indices[l]];
    if (peterson_release(lock_id, roles[l]) < 0)
      return -1;
  }
  return 0;
}

