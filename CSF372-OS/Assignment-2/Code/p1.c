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
int shmid1;
int shmid2;

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
            if (c2 == k)
            {
                c2 = 0;
                c1++;
            }
            mat2[c1][c2] = y;
            c2++;
        }
        fclose(fp1);
    }
}

int main(int argc, char *argv[])
{
    // change this

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

    key_t vals;

    vals = ftok("/", 1156);
    int valsid = shmget(vals, sizeof(int[3]), IPC_CREAT | 0666);
    int *dim = shmat(valsid, 0, 0);
    dim[0] = i;
    dim[1] = j;
    dim[2] = k;

    key_t key1 = ftok("/", 67590);

    key_t key2 = ftok("/", 250000);

    int a = 10000;
    shmid1 = shmget(key1, sizeof(int[a][a]), IPC_CREAT | 0666);

    shmid2 = shmget(key2, sizeof(int[a][a]), IPC_CREAT | 0666);

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

    mat1 = shmat(shmid1, 0, 0);
    mat2 = shmat(shmid2, 0, 0);

    // Shared Memory

    int cnt = i * j;
    if (j * k < cnt)
    {
        cnt = j * k;
    }

    if (cnt > 1000)
    {
        cnt = 1000;
    }
    double time_elapsed[cnt + 1];

    for (int t = 1; t <= cnt; ++t)
    {
        clock_t strt, end;

        printf("Number of threads = %d\n", t);

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
            count1[h] = 0;
            count2[h] = 0;
        }

        int ij = i * j;
        int rr1 = ij % t;
        ij = ij / t;

        int jk = j * k;
        int rr2 = jk % t;
        jk = jk / t;
        for (int h = 0; h < t; h++)
        {
            count1[h] = ij;
            count2[h] = jk;
        }
        int pos = t - 1;
        for (int cc = 0; cc < rr1; ++cc)
        {
            count1[pos]++;
            pos--;
        }
        pos = t - 1;
        for (int cc = 0; cc < rr2; ++cc)
        {
            count2[pos]++;
            pos--;
        }

        FILE *gg = fopen("in1.txt", "r");
        FILE *gg1 = fopen("file.txt", "r");

        for (int h = 0; h < t; ++h)
        {
            int w;
            fscanf(gg, "%d", &w);
            int ff = ftell(gg);
            start1[h] = ff - nod(w);

            int temp = count1[h] - 1;

            for (int cc = 0; cc < temp; ++cc)
            {
                fscanf(gg, "%d", &w);
            }
        }

        for (int h = 0; h < t; ++h)
        {
            int w;
            fscanf(gg1, "%d", &w);
            int ff = ftell(gg1);
            start2[h] = ff - nod(w);
            // printf("%d\n",start[h]);
            int temp = count2[h] - 1;

            for (int cc = 0; cc < temp; ++cc)
            {
                fscanf(gg1, "%d", &w);
            }
        }
        fclose(gg);
        fclose(gg1);

        strt = clock();
        pthread_t tid[2 * t];

        int totread1 = 0;
        int totread2 = 0;

        for (int h = 0; h < t; ++h)
        {

            int *arr = (int *)(malloc(6 * sizeof(int)));
            int c1 = 0;
            int c2 = 0;
            arr[0] = 0;
            arr[1] = start1[h];
            arr[2] = count1[h];
            arr[3] = h;
            if (h != 0)
            {
                c2 = totread1 % j;
                c1 = totread1 / j;

                totread1 += count1[h];
            }
            arr[4] = c1;
            arr[5] = c2;

            pthread_create(&tid[h], NULL, runner, (void *)arr);

            arr[0] = 1;
            arr[1] = start2[h];
            arr[2] = count2[h];
            arr[3] = h + t;

            if (h != 0)
            {
                c2 = totread2 % k;
                c1 = totread2 / k;

                totread2 += count2[h];
            }
            arr[4] = c1;
            arr[5] = c2;
            pthread_create(&tid[h + t], NULL, runner, (void *)arr);
        }

        for (int h = 0; h < t; ++h)
        {

            pthread_join(tid[h], NULL);
            pthread_join(tid[h + t], NULL);
        }

        end = clock();
        if (end == strt)
        {
            time_elapsed[t] = 0;
        }
        else
        {
            double rem = (double)(end - strt) / (CLOCKS_PER_SEC);
            time_elapsed[t] = rem;
        }
    }


    for (int t = 1; t <= 1; ++t)
    {

        int n1 = t;

        int n2 = t;
        // printf("n1 = %d and n2 = %d\n",n1,n2);

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
            count1[h] = 0;
            count2[h] = 0;
        }

        int ij = i * j;
        int rr1 = ij % t;
        ij = ij / t;

        int jk = j * k;
        int rr2 = jk % t;
        jk = jk / t;
        for (int h = 0; h < t; h++)
        {
            count1[h] = ij;
            count2[h] = jk;
        }
        int pos = t - 1;
        for (int cc = 0; cc < rr1; ++cc)
        {
            count1[pos]++;
            pos--;
        }
        pos = t - 1;
        for (int cc = 0; cc < rr2; ++cc)
        {
            count2[pos]++;
            pos--;
        }

        FILE *gg = fopen("in1.txt", "r");
        FILE *gg1 = fopen("file.txt", "r");

        for (int h = 0; h < t; ++h)
        {
            int w;
            fscanf(gg, "%d", &w);
            int ff = ftell(gg);
            start1[h] = ff - nod(w);

            int temp = count1[h] - 1;

            for (int cc = 0; cc < temp; ++cc)
            {
                fscanf(gg, "%d", &w);
            }
        }

        for (int h = 0; h < t; ++h)
        {
            int w;
            fscanf(gg1, "%d", &w);
            int ff = ftell(gg1);
            start2[h] = ff - nod(w);
            // printf("%d\n",start[h]);
            int temp = count2[h] - 1;

            for (int cc = 0; cc < temp; ++cc)
            {
                fscanf(gg1, "%d", &w);
            }
        }
        fclose(gg);
        fclose(gg1);

        pthread_t tid[2 * t];

        int totread1 = 0;
        int totread2 = 0;

        for (int h = 0; h < 2 * t; ++h)
        {

            int *arr = (int *)(malloc(6 * sizeof(int)));

            arr[3] = h;
            if (h >= t)
            {

                arr[0] = 1;
                arr[1] = start2[h - t];
                arr[2] = count2[h - t];
            }
            else
            {
                arr[0] = 0;
                arr[1] = start1[h];
                arr[2] = count1[h];
            }
            int c1 = 0;
            int c2 = 0;
            if (h != 0 && h != t)
            {
                if (h < t)
                {
                    c2 = totread1 % j;
                    c1 = totread1 / j;

                    totread1 += count1[h];
                }
                else
                {

                    c2 = totread2 % k;
                    c1 = totread2 / k;

                    totread2 += count2[h - t];
                }
            }
            arr[4] = c1;
            arr[5] = c2;
            pthread_create(&tid[h], NULL, runner, (void *)arr);
        }

        for (int h = 0; h < 2 * t; ++h)
        {

            pthread_join(tid[h], NULL);
        }
    }

    shmdt(mat1);
    shmdt(mat2);
    for (int i = 1; i <= cnt; i++)
    {
        printf("%f\n", time_elapsed[i]);
    }
    FILE *f3 = fopen("graph_p1.txt", "w");

    for (int k = 1; k <= cnt; ++k)
    {
        char cc = ' ';
        fprintf(f3, "%f", time_elapsed[k]);
        if (k != cnt)
        {
            fprintf(f3, "%c", cc);
        }
    }
    fclose(f3);

    execlp("python3", "python3 ", "script_p1.py", NULL);

    return 0;
}
