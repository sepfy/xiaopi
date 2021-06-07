#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <sys/types.h> 
#include <sys/wait.h>

#define MAX_ARGUMENT_NUM 10

typedef struct watchdog_service_t {

  char *cmd;
  char *argv[MAX_ARGUMENT_NUM];
  struct watchdog_service_t *next;
  pid_t pid;

} watchdog_service_t;

watchdog_service_t *watchdog_service_list = NULL;

watchdog_service_t* create_watchdog_service() {
  watchdog_service_t *watchdog_service = (watchdog_service_t*)malloc(sizeof(*watchdog_service));

  if(watchdog_service == NULL)
    return NULL;
  watchdog_service->cmd = NULL;

  int i;
  for(i = 0; i < MAX_ARGUMENT_NUM; ++i) {
    watchdog_service->argv[i] = NULL;
  }
  watchdog_service->pid = -1;
  watchdog_service->next = NULL;
  return watchdog_service;
}

pid_t watchdog_dispatch(char *cmd, char *argv[]) {

  pid_t pid = fork();

  switch(pid) {
    case -1:
      perror("fork");
      sync();
      reboot(RB_AUTOBOOT);
        
    case 0:
      execvp(cmd, argv);
      break;
        
    default:
//      printf("Fork successfully. Get pid = %d\n", pid);
      return pid; 
  }

  return 0;

}

void watchdog_parse(const char *path) {

  watchdog_service_t *prev_watchdog_service = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *fp = NULL;

  fp = fopen(path, "r");
  if(fp == NULL)
    return;

  while((read = getline(&line, &len, fp)) != -1) {
    line[strlen(line) - 1] = '\0';

    if(strstr(line, "service") == NULL )
      continue;

    watchdog_service_t *watchdog_service = create_watchdog_service();
    char *p = strtok(line + 8, " ");
    int i = -1;
    while (p != NULL) {
      if(i == -1) {
        watchdog_service->cmd = strdup(p);
      }
      else {
        watchdog_service->argv[i] = strdup(p);
      }
      i++;
      p = strtok(NULL, " ");
    }

    if(prev_watchdog_service == NULL) {
      watchdog_service_list = watchdog_service;
      prev_watchdog_service = watchdog_service;
    }
    else {
      prev_watchdog_service->next = watchdog_service;
      prev_watchdog_service = watchdog_service;
    }
  }
  fclose(fp);
}

void watchdog_init() {

  watchdog_service_t *watchdog_service = watchdog_service_list;
  while(watchdog_service != NULL) {
    printf("watchdog service: %s\n", watchdog_service->argv[0]);
    //int i = 0;
    //for(i = 0; i < MAX_ARGUMENT_NUM; ++i)
    //  if(watchdog_service->argv[i] != NULL)
    //    printf("%s ", watchdog_service->argv[i]);
    //printf("\n");
    watchdog_service->pid = watchdog_dispatch(watchdog_service->cmd, watchdog_service->argv);
    sleep(1);
    watchdog_service = watchdog_service->next;
  }

}

int watchdog_check_status(pid_t pid) {
  int status  =-1;
  waitpid(pid, &status, WNOHANG);
  if(WIFEXITED(status))
    //printf("child exited normal exit status=%d\n", WEXITSTATUS(status));
    return -1;
  else if(WIFSIGNALED(status))
    //printf("child exited abnormal signal number=%d\n", WTERMSIG(status));
    return -1;
  else if(WIFSTOPPED(status))
    //printf("child stoped signal number=%d\n", WSTOPSIG(status));
    return -1;
  return 0;
}

void watchdog_monitor() {

  while(1) {

    watchdog_service_t *watchdog_service = watchdog_service_list;
    while(watchdog_service != NULL) {

      if(watchdog_check_status(watchdog_service->pid) < 0) {    
        watchdog_service->pid = watchdog_dispatch(watchdog_service->cmd, watchdog_service->argv);
      }
      watchdog_service = watchdog_service->next;
    }
    sleep(2);

  }
}

int main(int argc, char *argv[]) {


  if(argc < 2) {
    printf("Usage: ./%s <config file>\n", argv[0]);
    return 0;
  }

  watchdog_parse(argv[1]);
  watchdog_init();
  watchdog_monitor();

  return 0;

}
