//Иванчук Евгения 9894

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

bool flag = true;
int pipefd;

void *func(void *arg)
{
	int buf;
	while (flag == 1)
	{
		buf = 0;
		read(pipefd, &buf, sizeof(int));
		if (buf != 0)
		{
			cout << buf << " ";
			cout.flush();
		}
		sleep(1);
	}
	return NULL;
}

int main()
{
	mkfifo("/tmp/pipefd", 0644);
	pipefd = open("/tmp/pipefd", O_RDONLY|O_NONBLOCK);
	pthread_t th;
	pthread_create(&th, NULL, func, NULL);
	getchar();
	flag = false;
	pthread_join(th, NULL);
	close(pipefd);
	unlink("/tmp/pipefd");
	return 0;
}
