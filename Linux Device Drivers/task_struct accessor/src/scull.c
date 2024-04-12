#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <pthread.h>

#include "scull.h"

#define CDEV_NAME "/dev/scull"

/* Quantum command line option */
static int g_quantum;

static void usage(const char *cmd)
{
	printf("Usage: %s <command>\n"
	       "Commands:\n"
	       "  R          Reset quantum\n"
	       "  S <int>    Set quantum\n"
	       "  T <int>    Tell quantum\n"
	       "  G          Get quantum\n"
	       "  Q          Query quantum\n"
	       "  X <int>    Exchange quantum\n"
	       "  H <int>    Shift quantum\n"
	       "  h          Print this message\n"
	       "  i          Get Info\n"
	       "  p          run processes concurrently\n"
	       "  t          run threads concurrently\n",
	       cmd);
}

typedef int cmd_t;

void* start_routine(void* arg) { // start routine for thread
    struct task_info t_info;
    int fd = *((int*)arg);
    for(int i = 0; i < 2; i++) { // call ioctl twice then exit
	ioctl(fd, SCULL_IOCIQUANTUM, &t_info);
	printf("state %u, cpu %u, prio %d, pid %d, tgid %d, nv %lu, niv %lu\n", t_info.__state, t_info.cpu, t_info.prio, t_info.pid, t_info.tgid, t_info.nvcsw, t_info.nivcsw);
    }
    pthread_exit(NULL);
}

static cmd_t parse_arguments(int argc, const char **argv)
{
	cmd_t cmd;

	if (argc < 2) {
		fprintf(stderr, "%s: Invalid number of arguments\n", argv[0]);
		cmd = -1;
		goto ret;
	}

	/* Parse command and optional int argument */
	cmd = argv[1][0];
	switch (cmd) {
	case 'S':
	case 'T':
	case 'H':
	case 'X':
		if (argc < 3) {
			fprintf(stderr, "%s: Missing quantum\n", argv[0]);
			cmd = -1;
			break;
		}
		g_quantum = atoi(argv[2]);
		break;
	case 'R':
	case 'G':
	case 'Q':
	case 'h':
		break;
	case 'i':
	    break;
	case 'p':
	    break;
	case 't':
	    break;
	default:
		fprintf(stderr, "%s: Invalid command\n", argv[0]);
		cmd = -1;
	}

ret:
	if (cmd < 0 || cmd == 'h') {
		usage(argv[0]);
		exit((cmd == 'h')? EXIT_SUCCESS : EXIT_FAILURE);
	}
	return cmd;
}

static int do_op(int fd, cmd_t cmd)
{
	int ret, q;
	struct task_info t_info;
	switch (cmd) {
	case 'i':
	    ret = ioctl(fd, SCULL_IOCIQUANTUM, &t_info);
	    printf("state %u, cpu %u, prio %d, pid %d, tgid %d, nv %lu, niv %lu\n", t_info.__state, t_info.cpu, t_info.prio, t_info.pid, t_info.tgid, t_info.nvcsw, t_info.nivcsw);
	    break;
	case 'p': ; //semi-colon is not a mistype, declarations are not allowed after labels so this is making an empty statement to get rid of that error
	    int status = 0; // status of child processes
	    int wpid;
	    int child_pid = -1;
	    for(int i = 0; i < 4; i++) {
		if((child_pid = fork()) == -1) {
		    printf("error creating child\n");
		} else if(child_pid == 0) { // child processes created
		    for(int j = 0; j < 2; j++) { // child processes call ioctl function
			ret = ioctl(fd, SCULL_IOCIQUANTUM, &t_info);
			printf("state %u, cpu %u, prio %d, pid %d, tgid %d, nv %lu, niv %lu\n", t_info.__state, t_info.cpu, t_info.prio, t_info.pid, t_info.tgid, t_info.nvcsw, t_info.nivcsw);
		    }
		    exit(0);
		}
	    }
	    while((wpid = wait(&status)) > 0); // parent waits for all children to be finished
	    ret = 0;
	    break;
	case 't': ; // semi-colon is not a mistype, declarations are not allowed after labels so this is making an empty statement to get rid of that error
	    pthread_t threads[4];
	    for(int i = 0; i < 4; i++) { // create threads
		pthread_create(&threads[i], NULL, &start_routine, (void*)&fd);
	    }
	    for(int i = 0; i < 4; i++) { // join threads
		pthread_join(threads[i], NULL);
	    }
	    ret = 0;
	    break;
	case 'R':
		ret = ioctl(fd, SCULL_IOCRESET);
		if (ret == 0)
			printf("Quantum reset\n");
		break;
	case 'Q':
		q = ioctl(fd, SCULL_IOCQQUANTUM);
		printf("Quantum: %d\n", q);
		ret = 0;
		break;
	case 'G':
		ret = ioctl(fd, SCULL_IOCGQUANTUM, &q);
		if (ret == 0)
			printf("Quantum: %d\n", q);
		break;
	case 'T':
		ret = ioctl(fd, SCULL_IOCTQUANTUM, g_quantum);
		if (ret == 0)
			printf("Quantum set\n");
		break;
	case 'S':
		q = g_quantum;
		ret = ioctl(fd, SCULL_IOCSQUANTUM, &q);
		if (ret == 0)
			printf("Quantum set\n");
		break;
	case 'X':
		q = g_quantum;
		ret = ioctl(fd, SCULL_IOCXQUANTUM, &q);
		if (ret == 0)
			printf("Quantum exchanged, old quantum: %d\n", q);
		break;
	case 'H':
		q = ioctl(fd, SCULL_IOCHQUANTUM, g_quantum);
		printf("Quantum shifted, old quantum: %d\n", q);
		ret = 0;
		break;
	default:
		/* Should never occur */
		abort();
		ret = -1; /* Keep the compiler happy */
	}

	if(ret != 0) {
	    perror("ioctl");
	}
	return ret;
}

int main(int argc, const char **argv)
{
	int fd, ret;
	cmd_t cmd;

	cmd = parse_arguments(argc, argv);

	fd = open(CDEV_NAME, O_RDONLY);
	if (fd < 0) {
		perror("cdev open");
		return EXIT_FAILURE;
	}

	printf("Device (%s) opened\n", CDEV_NAME);

	ret = do_op(fd, cmd);

	if (close(fd) != 0) {
		perror("cdev close");
		return EXIT_FAILURE;
	}

	printf("Device (%s) closed\n", CDEV_NAME);

	return (ret != 0)? EXIT_FAILURE : EXIT_SUCCESS;
}
