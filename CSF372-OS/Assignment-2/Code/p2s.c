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

int a, b, c;
int (*mat1)[100000];
int (*mat2)[100000];
int (*read1)[100000];
int (*read2)[100000];

long long int *prod[100000];

void *runner(void *args)
{
    int r1 = *((int *)(args + 0 * sizeof(int)));
    int r2 = *((int *)(args + 1 * sizeof(int)));
    int c1 = *((int *)(args + 2 * sizeof(int)));
    int c2 = *((int *)(args + 3 * sizeof(int)));

    for (long long int r = r1; r <= r2; ++r)
    {
        for (long long int c = c1; c <= c2; ++c)
        {
            long long int pr = 0;
            for (int u = 0; u < b; ++u)
            {
                int yy = 0;

                while (read1[r][u] == 0 || read2[c][u] == 0)
                {
                    // if(yy == 0){
                    //     printf("r = %lld and u = %d\n",r,u);
                    // }
                    yy++;

                }
                pr += (long long int)((long long int)mat1[r][u] * (long long int)mat2[c][u]);
            }
            prod[r][c] = pr;
        }
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

    key_t val1;
    key_t val2;
    val1 = ftok("/", 200);
    val2 = ftok("/", 9054);

    // vals = ftok("/", 1156);

    a = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    long long int aa[a][c];
    for (int i = 0; i < a; ++i)
    {
        prod[i] = aa[i];
    }

    // printf("a = %d , b = %d and c = %d\n", a, b, c);

    key_t key1 = ftok("/", 67590);

    key_t key2 = ftok("/", 250000);

    int sz = 10000;
    int shmid1 = shmget(key1, sizeof(int[sz][sz]), IPC_CREAT | 0666);

    int shmid2 = shmget(key2, sizeof(int[sz][sz]), IPC_CREAT | 0666);
    int shmid3 = shmget(val1, sizeof(int[sz][sz]), IPC_CREAT | 0666);
    int shmid4 = shmget(val2, sizeof(int[sz][sz]), IPC_CREAT | 0666);

    mat1 = shmat(shmid1, 0, 0);
    mat2 = shmat(shmid2, 0, 0);
    read1 = shmat(shmid3, 0, 0);
    read2 = shmat(shmid4, 0, 0);

    for (int t = 1; t <= 1; ++t)
    {

        // printf("Number of threads: %d\n",t);

        int freq[t];

        int r1[t];
        int r2[t];
        int c1[t];
        int c2[t];
        if (t <= a)
        {
            int q = a / t;
            int r = a % t;
            int cntr = 0;
            for (int j = 0; j < t; ++j)
            {
                freq[j] = q;
            }
            int pos = t - 1;
            while (r--)
            {
                freq[pos]++;
                pos--;
            }

            int start = 0;
            for (int j = 0; j < t; ++j)
            {
                r1[j] = start;
                start += freq[j] - 1;
                r2[j] = start;
                start++;
            }
            for (int j = 0; j < t; ++j)
            {
                c1[j] = 0;
                c2[j] = c - 1;
            }
        }
        else
        {
            int tpr[a];
            for (int i = 0; i < a; ++i)
            {
                tpr[i] = t / a;
            }
            int rem = t % a;
            int pos = a - 1;
            while (rem--)
            {
                tpr[pos]++;
                pos--;
            }
            int currthread = 0;
            for (int i = 0; i < a; ++i)
            {
                for (int j = 0; j < tpr[i]; ++j)
                {
                    r1[currthread] = i;
                    r2[currthread] = i;
                    currthread++;
                }
            }
            currthread = 0;
            for (int i = 0; i < a; ++i)
            {
                int colsreq = tpr[i];
                int cpt[colsreq];
                for (int j = 0; j < colsreq; ++j)
                {
                    cpt[j] = c / colsreq;
                }
                int r = c % colsreq;
                int pos = colsreq - 1;
                while (r--)
                {
                    cpt[pos]++;
                    pos--;
                }

                int cs = 0;
                for (int j = 0; j < colsreq; j++)
                {
                    c1[currthread] = cs;
                    cs += cpt[j] - 1;
                    c2[currthread] = cs;
                    cs++;
                    currthread++;
                }
            }


        }

        // create threads
        pthread_t tid[t];
        for (int i = 0; i < t; ++i)
        {
            int *arr = (int *)(malloc(4 * sizeof(int)));
            arr[0] = r1[i];
            arr[1] = r2[i];
            arr[2] = c1[i];
            arr[3] = c2[i];
            pthread_create(&tid[i], NULL, runner, (void *)arr);
        }
        for (int i = 0; i < t; ++i)
        {
            pthread_join(tid[i], NULL);
        }
    }
    FILE *fptr = fopen("out.txt", "w");
    // printf("Printing Prod matrix\n");

    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < c; ++j)
        {
            fprintf(fptr, "%lld", prod[i][j]);
            if (j < c - 1)
            {
                fprintf(fptr, " ");
            }
        }
        if (i != a - 1)
        {
            fprintf(fptr, "\n");
        }
    }
    fclose(fptr);
    end = clock();
    double rem = (double)(end - strt) / (CLOCKS_PER_SEC);
    // printf("Program2 : %f\n", rem);

    flag[1] = 1;
    // printf("flag1 : %d\n",flag[1]);
    printf("Program 2 ended\n");
}