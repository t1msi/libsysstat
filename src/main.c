#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "netcat.h"

/* Maximum number of args that can be passed to saf */
#define MAX_ARGV_NR	32

#define MAX_FILE_LEN		512
#define MAX_PF_NAME		    1024
#define MAX_NAME_LEN		128

#define SAF         "saf"

/* Interval and count parameters */
long interval = -1, count = 0;

char *saf_args[MAX_ARGV_NR];

/* Close file descriptors */
#define CLOSE_ALL(_fd_)		do {			\
					close(_fd_[0]); \
					close(_fd_[1]); \
				} while (0)

#define CLOSE(_fd_)		if (_fd_ >= 0)		\
					close(_fd_)

void salloc(int i, char *ltemp)
{
	if ((saf_args[i] = (char *) malloc(strlen(ltemp) + 1)) == NULL) {
		perror("malloc");
		exit(4);
	}
	strcpy(saf_args[i], ltemp);
}

int main(int argc, char **argv)
{
    int c, protocol = SOCK_STREAM, listen = 0, family = AF_UNSPEC;

	int i, rc, opt = 1, args_idx = 1, p, q;
	char from_file[MAX_FILE_LEN], to_file[MAX_FILE_LEN];
	int fd[2];
	char ltemp[1024];

    opterr = 0;

    while ((c = getopt(argc, argv, "lu46")) != -1)
    {
        switch (c)
        {
        case 'l':
            listen = 1;
            break;
        case 'u':
            protocol = SOCK_DGRAM;
            break;
        case '4':
            family = AF_INET;
            break;
        case '6':
            family = AF_INET6;
            break;
        }
    }


    /* Reading stats from saf: */

    /* Create anonymous pipe */
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(4);
    }

    switch (fork()) {

    case -1:
        perror("fork");
        exit(4);
        break;

    case 0: /* Child */
        if (dup2(fd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(4);
        }
        CLOSE_ALL(fd);

        /*
         * Prepare options for saf.
         */
        /* Program name */
        salloc(0, SAF);

        salloc(args_idx++, "1");

        /* Count number */
        // if (count >= 0) {
        //     sprintf(ltemp, "%ld", count + 1);
        //     salloc(args_idx++, ltemp);
        // }

#ifdef TEST
        if (__unix_time) {
            sprintf(ltemp, "--unix_time=%ld", __unix_time);
            salloc(args_idx++, ltemp);
        }
#endif
        /* Flags to be passed to saf */
        // 1 -u ALL -r ALL -S -w -m BAT -n ALL -y --human
        // salloc(args_idx++, "-u");
        // salloc(args_idx++, "ALL");
        // salloc(args_idx++, "-r");
        // salloc(args_idx++, "ALL");
        // salloc(args_idx++, "-S");
        // salloc(args_idx++, "-w");
        // salloc(args_idx++, "-m");
        // salloc(args_idx++, "BAT");
        // salloc(args_idx++, "-n");
        // salloc(args_idx++, "BAT");
        // salloc(args_idx++, "-y");
        // salloc(args_idx++, "--human");

        salloc(args_idx++, "-A");
        /* Last arg is NULL */
        saf_args[args_idx] = NULL;

        /* Call now the data collector */
#ifdef DEBUG
        fprintf(stderr, "%s: 1.saf: %s\n", __FUNCTION__, SAF_PATH);
#endif

        execv(SAF_PATH, saf_args);
#ifdef DEBUG
        fprintf(stderr, "%s: 2.saf: %s\n", __FUNCTION__, SAF);
#endif
        execvp(SAF, saf_args);
        /*
         * Note: Don't use execl/execlp since we don't have a fixed number of
         * args to give to saf.
         *
         */
        fprintf(stderr, "Cannot find the data collector (%s)\n", SAF);
        perror("exec");
        exit(4);
        break;

    default: /* Parent */
        if (dup2(fd[0], STDIN_FILENO) < 0) {
            perror("dup2");
            exit(4);
        }
        CLOSE_ALL(fd);

        /* Get now the statistics */
        // read_stats();

        if (listen == 0)
        {
            if (optind + 2 != argc)
            {
                Help(argv[0]);
            }

            Client(argv[optind], argv[optind + 1], &protocol, &family);
        }
        else
        {
            char *hostname = NULL, *port;

            if (optind + 1 == argc)
            {
                port = argv[optind];
            }
            else if (optind + 2 == argc)
            {
                hostname = argv[optind];
                port = argv[optind + 1];
            }
            else
            {
                Help(argv[0]);
            }

            Server(hostname, port, &protocol, &family);
        }


        break;
    }

    return 0;
}
