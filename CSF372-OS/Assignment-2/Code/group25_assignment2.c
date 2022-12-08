#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Insufficient args\n");
        exit(1);
    }

    key_t k1 = ftok("hh", 996);

    int vid = shmget(k1, sizeof(int[2]), IPC_CREAT | 0666);
    int *flag = shmat(vid, 0, 0);
    flag[0] = 0;
    flag[1] = 0;

    int(*read1)[100000];
    int(*read2)[100000];
    int(*mat1)[100000];
    int(*mat2)[100000];
    int shmid1;
    int shmid2;
    int shmid3;
    int shmid4;
    key_t vals;
    key_t val1;
    key_t val2;

    val1 = ftok("/", 200);
    val2 = ftok("/", 9054);
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = atoi(argv[3]);

    key_t key1 = ftok("/", 67590);

    key_t key2 = ftok("/", 250000);

    int aa = 10000;
    shmid1 = shmget(key1, sizeof(int[aa][aa]), IPC_CREAT | 0666);
    shmid2 = shmget(key2, sizeof(int[aa][aa]), IPC_CREAT | 0666);
    shmid3 = shmget(val1, sizeof(int[aa][aa]), IPC_CREAT | 0666);
    shmid4 = shmget(val2, sizeof(int[aa][aa]), IPC_CREAT | 0666);

    if (shmid1 == -1)
    {
        perror("shmid1 error\n");
        exit(1);
    }

    if (shmid2 == -1)
    {
        perror("shmid2 error\n");
        exit(1);
    }

    if (shmid3 == -1)
    {
        perror("shmid3 error in scheduler\n");
        exit(1);
    }

    if (shmid4 == -1)
    {
        perror("shmid4 error\n");
        exit(1);
    }

    mat1 = shmat(shmid1, 0, 0);
    mat2 = shmat(shmid2, 0, 0);
    read1 = shmat(shmid3, 0, 0);
    read2 = shmat(shmid4, 0, 0);

    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            mat1[i][j] = 0;
        }
    }
    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < b; j++)
        {
            mat2[i][j] = 0;
        }
    }
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            read1[i][j] = 0;
        }
    }
    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < b; j++)
        {
            read2[i][j] = 0;
        }
    }

    pid_t child_1, child_2, parent;
    double q = 1;
    child_1 = fork();

    struct timespec beginp1, beginp2, watp1, watp2, loopstart;

    if (!child_1)
    {

        child_1 = getpid();
        kill(child_1, SIGSTOP);
        // usleep(100);

        execlp("./p1.out", "p1.out", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
    }
    else
    {
        parent = getpid();
        child_2 = fork();
        if (!child_2)
        {
            child_2 = getpid();
            kill(child_2, SIGSTOP);

            // usleep(200);

            execlp("./p2.out", "p2.out", argv[1], argv[2], argv[3], NULL);
        }
        else
        {
            double tat1 = 0.0;
            double tat2 = 0.0;
            double wat1 = 0.0;
            double wat2 = 0.0;

            int curr = child_2;

            int ff1 = 0, f1 = 0;
            int ff2 = 0, f2 = 0;
            int p1cnt = 0;
            int p2cnt = 0;
            printf("Here\n");
            double soh = 0.0;
            double tottime = 0.0;

            clock_gettime(CLOCK_REALTIME, &loopstart);

            clock_gettime(CLOCK_REALTIME, &watp1);
            clock_gettime(CLOCK_REALTIME, &watp2);

            while (flag[0] == 0 && flag[1] == 0)
            {

                if (curr == child_2)
                {
                    curr = child_1;
                    if (f1 > 0)
                    {
                        struct timespec currtime;
                        clock_gettime(CLOCK_REALTIME, &currtime);
                        double temptime = ((double)currtime.tv_sec * 1e9 + currtime.tv_nsec) - ((double)watp1.tv_sec * 1e9 + watp1.tv_nsec);
                        temptime = temptime / 1000000;
                        wat1 += temptime;
                    }

                    if (ff1 == 0)
                    {
                        clock_gettime(CLOCK_REALTIME, &beginp1);
                        ff1++;
                    }
                    // printf("P1 has started.\n");

                    kill(child_1, SIGCONT);
                    kill(child_2, SIGSTOP);

                    clock_gettime(CLOCK_REALTIME, &watp2);
                    f2 = 1;
                    p1cnt++;
                }
                else
                {
                    curr = child_2;

                    if (f2 > 0)
                    {
                        struct timespec currtime;
                        clock_gettime(CLOCK_REALTIME, &currtime);
                        double temptime = ((double)currtime.tv_sec * 1e9 + currtime.tv_nsec) - ((double)watp2.tv_sec * 1e9 + watp2.tv_nsec);
                        temptime = temptime / 1000000;
                        wat2 += temptime;
                    }

                    if (ff2 == 0)
                    {
                        clock_gettime(CLOCK_REALTIME, &beginp2);
                        ff2++;
                    }
                    // printf("P2 has started.\n");

                    kill(child_2, SIGCONT);

                    kill(child_1, SIGSTOP);

                    clock_gettime(CLOCK_REALTIME, &watp1);

                    f1 = 1;
                    p2cnt++;
                }
                sleep(q / 1000);
            }

            int ended = -1;
            if (flag[0] == 0)
            {
                struct timespec p2ended;
                clock_gettime(CLOCK_REALTIME, &p2ended);

                tat2 = ((double)p2ended.tv_sec * 1e9 + p2ended.tv_nsec) - ((double)beginp2.tv_sec * 1e9 + beginp2.tv_nsec);
                tat2 = tat2 / 1000000;

                kill(child_1, SIGCONT);
                ended = 2;
            }
            if (flag[1] == 0)
            {
                struct timespec p1ended;
                clock_gettime(CLOCK_REALTIME, &p1ended);

                tat1 = ((double)p1ended.tv_sec * 1e9 + p1ended.tv_nsec) - ((double)beginp1.tv_sec * 1e9 + beginp1.tv_nsec);
                tat1 = tat1 / 1000000;

                kill(child_2, SIGCONT);
                ended = 1;
            }
            while (wait(NULL) > 0)
                ;
            struct timespec lasttime;
            clock_gettime(CLOCK_REALTIME, &lasttime);

            tottime = ((double)lasttime.tv_sec * 1e9 + lasttime.tv_nsec) - ((double)loopstart.tv_sec * 1e9 + loopstart.tv_nsec);
            tottime = tottime / 1000000;

            if (ended == 1)
            {
                tat2 = ((double)lasttime.tv_sec * 1e9 + lasttime.tv_nsec) - ((double)beginp2.tv_sec * 1e9 + beginp2.tv_nsec);
                tat2 = tat2 / 1000000;
            }
            else
            {

                tat1 = ((double)lasttime.tv_sec * 1e9 + lasttime.tv_nsec) - ((double)beginp1.tv_sec * 1e9 + beginp1.tv_nsec);
                tat1 = tat1 / 1000000;
            }

            soh = tottime - (wat2 + wat1);
            printf("Turnaround time of P1: %fms and Turnaround time of P2: %fms\n", tat1, tat2);
            printf("Waiting time of P1: %fms and Waiting time of P2: %fms\n", wat1, wat2);

            printf("\nP1 = %d and P2 = %d\n", p1cnt, p2cnt);

            tat1 /= 1000;
            tat2 /= 1000;
            wat1 /= 1000;
            wat2 /= 1000;
            soh /= 1000;

            FILE *g1 = fopen("./data/tat1q1.csv", "a");
            fprintf(g1, "%f,", tat1);
            FILE *g2 = fopen("./data/tat2q1.csv", "a");
            fprintf(g2, "%f,", tat2);
            FILE *g3 = fopen("./data/wat1q1.csv", "a");
            fprintf(g3, "%f,", wat1);
            FILE *g4 = fopen("./data/wat2q1.csv", "a");
            fprintf(g4, "%f,", wat2);
            FILE *g5 = fopen("./data/sohq1.csv", "a");
            fprintf(g5, "%lf,", soh);
        }
    }

    shmdt(mat1);
    shmdt(mat2);
    shmdt(read1);
    shmdt(read2);

    return 0;
}