#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
             
int N;
int check = 0;
typedef struct
{
    int x, y;
} pair;
pthread_mutex_t lock;
int xMove[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int yMove[8] = {1, 2, 2, 1, -1, -2, -2, -1};
int deg[8];
 
void print_path(pair path[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d,%d|", path[i].x, path[i].y);
    }
}
 
int limits(int x, int y, int n)
{
    return x >= 0 && y >= 0 && x < n && y < n;
}
             
int accesibility(int x, int y,int** visited)
{
    int cnt = 0;
    for (int i = 0; i < 8; i++)
    {
        int nextX = x + xMove[i];
        int nextY = y + yMove[i];
        if (limits(nextX, nextY, N) && !visited[nextX][nextY])
            cnt++;
    }
    return cnt;
}
           
int isdead(int x, int y,int** visited)
{
    for (int i = 0; i < 8; ++i)
    {
        int nx = x + xMove[i];
        int ny = y + yMove[i];
        if (limits(nx, ny, N) && !visited[nx][ny])
        {
            return 0;
        }
    }
    return 1;
}
 
void tour(int x, int y,int** visited,int p,pair* path)
{
    visited[x][y] = 1;
    pair cell = {x, y};
    path[p] = cell;
    int nx = x;
    int ny = y;
    int mini = 8;
    int deadEnd = isdead(x, y,visited);
    if (deadEnd == 1)
    {
        int f = 1;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (visited[i][j] == 0)
                {
                    f = 0;
                    break;
                }
            }
            if (!f)
                break;
        }
        pthread_mutex_lock(&lock);
        if (f && check == 0)
        {
            check = 1;
            print_path(path,N*N);
            pthread_mutex_unlock(&lock);
            return;
        }
        pthread_mutex_unlock(&lock);
        return;
    }
    int st = rand()%8; 
    for(int i=0;i<8;++i)
    {
        int j = (st + i) % 8;
        int nextX = x + xMove[j];
        int nextY = y + yMove[j];
        int curr = accesibility(nextX,nextY,visited);
        if(limits(nextX,nextY,N) && !visited[nextX][nextY] && curr < mini)
        {
            mini = curr;
            nx = nextX;
            ny = nextY;
        }
    }
             
    p++;
    tour(nx, ny,visited,p,path);
}
 
void* solve(void* args)
{
    int **visited;
    int StartX = *((int *)(args + 0 * sizeof(int)));
    int StartY = *((int *)(args + 1 * sizeof(int)));
    pair path1[N * N];
    pair* path = path1;
    int visited1[N][N];
    int *rows[N];
    for (int i = 0; i < N; i++)
    {
        rows[i] = visited1[i];
    }
    visited = rows;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            visited[i][j] = 0;
        }
    }
    visited[StartX][StartY] = 1;
    tour(StartX, StartY,visited,0,path);
}
 
int main(int argc, char *argv[])
{
    pthread_mutex_init(&lock,NULL);
    if (argc != 4)
    {
        printf("Usage: ./Knight.out grid_size StartX StartY");
        exit(-1);
    }
    N = atoi(argv[1]);
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    pthread_t threads[10];
    int* arr = (int*)(malloc(2*sizeof(int)));
    arr[0] = x;
    arr[1] = y;
    for(int i=0;i<10;++i)
    {
        int p=pthread_create(&threads[i],NULL,solve,(void*)arr);
        if(p!=0)
        {
            printf("error: %d",i);
        }
    }
    for(int i=0;i<10;++i)
    {
        pthread_join(threads[i],NULL);
    }
    if(!check)
    {
        printf("No Possible Tour");
    }
    return 0;
}