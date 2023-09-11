#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <string.h>

pid_t temp;

void err_quit(const char *msg) {
    perror(msg);
    exit(1);
}

void start_daemon(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    
    // child process
    pid = fork();
    temp = pid;
    if (pid < 0)
        err_quit("can't fork");
    else if (pid != 0) // parent
        exit(0);

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("can't get file limit");


    setsid();

    // Changing the current working directory to the root.
    if (chdir("/") < 0)
        err_quit("can't change directory to /");

    // Closing all open file descriptors.
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    // Attach file descriptors 0, 1, and 2 to /dev/null.
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    //Initializing log file.
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d",fd0, fd1, fd2);
        exit(1);
    }

    // message of successful of daemon creation in syslog file.
    syslog(LOG_INFO, "Daemon is successfully created.");

    while (1)
    {
        pid = fork();
        if(pid<0)
        {
            fprintf(stderr,"Fork Failed");
            return 1;
        }
        else if(pid ==0)
        {
            char* sequence;
            int n = getpid(),id=getpid();
            syslog(LOG_NOTICE,"child started, n=%d\n", n);
            while(n!=1)
            {
                syslog(LOG_NOTICE, "sequence(%d): %d",id, n);
                printf("%d ", n);
                if(n%2==0)
                    n=n/2;
                else
                    n=3*n+1;
            }
            exit(0);
        }
        else
        {
            wait(NULL);
        }

        // next child in 20seconds
        sleep(20);
    }
}

void stop_daemon()
{
    //message to syslog about stopping of daemon
    syslog(LOG_INFO, "got SIGTERM; exiting");
    closelog();
    //pid of daemon process got from global variable
    kill(temp, SIGTERM);
}

int main(int argc,char* argv[])
{
    // to view logs
    //  sudo tail -f /var/log/syslog
    // printf("in main");
    if (argc != 2)
    {
        printf("Invalid program call");
    }
    else if(strcmp(argv[1],"start")==0)
    {
        printf("Daemon started\n");
        start_daemon("started daemon");
    }
    else if(strcmp(argv[1],"stop")==0)
    {
        stop_daemon();
    }

    closelog();

    return EXIT_SUCCESS;
}