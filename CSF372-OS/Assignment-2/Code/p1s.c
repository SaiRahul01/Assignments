#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <time.h>

int i, j, k;
int (*mat1)[100000];
int (*mat2)[100000];
int (*read1)[100000];
int (*read2)[100000];
int shmid1;
int shmid2;
int shmid3;
int shmid4;

int nod(int w)
{
    int cnt = 0;
    int ff = w;
    while (ff > 0)
    {
        ff /= 10;
        cnt++;
    }
    return cnt;
}

void *runner(void *args)
{

    int mat = *((int *)(args + 0 * sizeof(int)));
    int st = *((int *)(args + 1 * sizeof(int)));
    int c = *((int *)(args + 2 * sizeof(int)));
    int id = *((int *)(args + 3 * sizeof(int)));
    int c1 = *((int *)(args + 4 * sizeof(int)));
    int c2 = *((int *)(args + 5 * sizeof(int)));

    if (mat == 0)
    {
        FILE *fp = fopen("in1.txt", "r");

        fseek(fp, 0, SEEK_END);
        fseek(fp, st, SEEK_SET);

        for (int cc = 0; cc < c; ++cc)
        {
            int y;
            fscanf(fp, "%d", &y);



            if (c2 == j)
            {
                c2 = 0;
                c1++;
            }
            read1[c1][c2] = 1;
            // printf("c1=%d,c2=%d\n",c1,c2);
            mat1[c1][c2] = y;
            c2++;
        }
        fclose(fp);
    }
    else if (mat == 1)
    {
        FILE *fp1 = fopen("file.txt", "r");
        fseek(fp1, st, SEEK_SET);
        for (int cc = 0; cc < c; ++cc)
        {
            int y;
            fscanf(fp1, "%d", &y);

            // printf("%d ",y);

            if (c2 == j)
            {
                c2 = 0;
                c1++;
                // printf("\n");
            }
            read2[c1][c2] = 1;
            // printf("c1=%d,c2=%d from mat2\n",c1,c2);
            mat2[c1][c2] = y;
            c2++;
        }
        fclose(fp1);
    }
}

int main(int argc, char *argv[])
{
    key_t k1 = ftok("hh", 996);

    int vid = shmget(k1, sizeof(int[2]), IPC_CREAT | 0666);
    int *flag = shmat(vid, 0, 0);
    flag[0] = 0;

    clock_t strt, end;
    strt = clock();
    if (argc != 7)
    {
        printf("Insufficient Arguments");
        exit(-1);
    }
    i = atoi(argv[1]);
    j = atoi(argv[2]);
    k = atoi(argv[3]);

    // Transposing Matrix
    if (!fork())
    {
        execlp("python3", "python3 ", "transpose.py", NULL);
    }
    else
    {
        wait(NULL);
    }

    // Shared Memory

    key_t val1;
    key_t val2;
    val1 = ftok("/", 200);
    val2 = ftok("/", 9054);

    // printf("i=%d , j=%d and k=%d from p1\n",i,j,k);

    key_t key1 = ftok("/", 67590);

    key_t key2 = ftok("/", 250000);

    int a = 10000;
    shmid1 = shmget(key1, sizeof(int[a][a]), IPC_CREAT | 0666);

    shmid2 = shmget(key2, sizeof(int[a][a]), IPC_CREAT | 0666);

    shmid3 = shmget(val1, sizeof(int[a][a]), IPC_CREAT | 0666);
    shmid4 = shmget(val2, sizeof(int[a][a]), IPC_CREAT | 0666);

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
        perror("shmid3 error in P1\n");
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



    int t = 1;


    int start1[t];
    int start2[t];
    for (int h = 0; h < t; ++h)
    {
        start1[h] = 0;
        start2[h] = 0;
    }
    int count1[t];
    int count2[t];
    for (int h = 0; h < t; ++h)
    {
        count1[h] = i * j;
        count2[h] = j * k;
    }

    pthread_t tid[2 * t];

    for (int h = 0; h < t; ++h)
    {

        int *arr = (int *)(malloc(6 * sizeof(int)));
        int c1 = 0;
        int c2 = 0;
        arr[0] = 0;
        arr[1] = start1[h];
        arr[2] = count1[h];
        arr[3] = h;
        arr[4] = c1;
        arr[5] = c2;
        // printf("Before thread creation :%d\n",arr[0]);
        pthread_create(&tid[h], NULL, runner, (void *)arr);
        int *arr1 = (int *)(malloc(6 * sizeof(int)));
        arr1[0] = 1;
        arr1[1] = start2[h];
        arr1[2] = count2[h];
        arr1[3] = h + t;

        arr1[4] = c1;
        arr1[5] = c2;
        pthread_create(&tid[h + t], NULL, runner, (void *)arr1);
    }



    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);


    shmdt(mat1);
    shmdt(mat2);
    end = clock();
    double rem = (double)(end - strt) / (CLOCKS_PER_SEC);


    flag[0] = 1;
    printf("Program 1 ended\n");


    return 0;
}