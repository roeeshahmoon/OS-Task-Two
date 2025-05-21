#ifndef PETERSON_H
#define PETERSON_H
#define MAX_PETERSON_LOCKS 15

struct peterson_lock {
  int active;
  int flag[2];
  int turn;
};

extern struct peterson_lock peterson_locks[MAX_PETERSON_LOCKS];

void peterson_locks_init(void);

#endif