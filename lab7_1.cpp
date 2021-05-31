//Иванчук Евгения 9894

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

bool flag = true;
int pipefd;
pthread_t th2;

void signalhandle(int signum)
{
	cout << "Pipe is closed!" << endl;
}

void *func(void *arg)
{
	int buf;
	while (flag == 1)
	{
		buf = 9894;
		write(pipefd, &buf, sizeof(int));
		signal(SIGPIPE, signalhandle);
		sleep(1);
	}
	return NULL;
}

void *openpipe(void *arg)
{
	while (flag == 1)
	{
		pipefd = open("/tmp/pipefd", O_WRONLY|O_NONBLOCK);
		if (pipefd == -1)
		{
			cout << "Can't open pipe." << endl;
			sleep(1);
		}
		else
		{
			pthread_create(&th2, NULL, func, NULL);
			return NULL;
		}
	}
	return NULL;
}

int main()
{
	mkfifo("/tmp/pipefd", 0644);
	pthread_t th1;
	pthread_create(&th1, NULL, openpipe, NULL);
	getchar();
	flag = false;
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	close(pipefd);
	unlink("/tmp/pipefd");
	return 0;
}
