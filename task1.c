#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int pid1,pid2;

void IntKill() {
	kill(pid1, 16);
	kill(pid2, 17);
}

void Int1() {
	printf("child process1(pid=%d ,ppid=%d)  is killed by parent!\n", getpid(), getppid());
	exit(0);
}
void Int2() {
	printf("child process2(pid=%d ,ppid=%d)  is killed by parent!\n", getpid(), getppid());
	exit(0);
}

int main() {
	int exitpid;
	if(pid1=fork()) {
		if(pid2=fork()) {
			signal(SIGINT,IntKill);  //接收到ctrl+c信号，转IntKill函数
			wait(0);  //同步
			printf("Parent(pid=%d) process is killed\n", getpid());
			exit(0);
		}else {
			signal(17,Int2);  //向Int2发软中断信号17
			pause();
		}
	}else {
		signal(16,Int1);  //向Int1发软中断信号16
		pause();
	}
}
