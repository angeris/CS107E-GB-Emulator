#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct _command_struct {
  char *name;
  char *description;
  int (*fn)(int argc, const char *argv[]);
} command_t;

int cmd_echo(int argc, const char *argv[]);
int cmd_help(int argc, const char *argv[]);
int cmd_clear(int argc, const char *argv[]);
int cmd_reboot(int argc, const char *argv[]);
int cmd_peek(int argc, const char *argv[]);
int cmd_poke(int argc, const char *argv[]);

#endif
