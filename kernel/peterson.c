#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "peterson.h"

// #define MAX_PETERSON_LOCKS 15

// struct peterson_lock {
//   int active; // 1 if active, 0 if destroyed
//   int flag[2]; // role 0 and role 1 flag
//   int turn;
// };

struct peterson_lock peterson_locks[MAX_PETERSON_LOCKS];

void peterson_locks_init(void) {
  for (int i = 0; i < MAX_PETERSON_LOCKS; i++) {
    peterson_locks[i].active = 0;
    peterson_locks[i].flag[0] = 0;
    peterson_locks[i].flag[1] = 0;
    peterson_locks[i].turn = 0;
  }
}




