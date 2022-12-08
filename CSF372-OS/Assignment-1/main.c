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
     
    int i,j,k;
    int** Matrix_A;
    int** Matrix_B;
    int** Matrix_C;
    
    int nod(int w){
        int cnt = 0;
        int ff = w;
        while(ff){
            ff /= 10;
            cnt ++ ;
        }
        return cnt;
    }
    int** read_matrix_from_file(int rows,int cols,char* filename)
    {
        clock_t start = clock();
        int** matrix = (int**)malloc(rows*sizeof(int*));
        for(int a = 0; a < rows; a++)
        {
            matrix[a] = (int*)malloc(cols*sizeof(int));
        }
        
        FILE *fp;
        fp = fopen(filename,"r");
        if(fp == NULL) exit(-1);
        for(int a = 0; a < i; a++)
        {
            for(int b = 0; b < j; b++)
            {
                fscanf(fp,"%d",&matrix[a][b]);
            }
        }
        clock_t end = clock();
        double exec_time = ((double)(end - start))/CLOCKS_PER_SEC;
        // printf("%f\n",exec_time);
        return matrix;
    }
    void* runner(void* args){
 
        int mat = *((int*)(args + 0*sizeof(int)));
        int st = *((int*)(args + 1*sizeof(int)));
        int c = *((int*)(args + 2*sizeof(int)));
        int id = *((int*)(args + 3*sizeof(int)));
        printf("mat = %d , st = %d and c = %d and thread id = %d\n",mat,st,c,id);
        FILE* fp = fopen("in1.txt","r");
        FILE* fp1 = fopen("in2.txt","r");
        int s = 0;
        fseek(fp,st,SEEK_SET);
 
        if(mat == 0){
            while(c--){
                int y;
                fscanf(fp,"%d",&y);
                if(id < 3){
                   printf("%d\n",y); 
                }
                
     
            }
     
        }else{
     
        }
 
    }
     
    int main(int argc, char *argv[])
    {
        if (argc != 7)
        {
            printf("Insufficient Arguments");
            exit(-1);
        }
        i = atoi(argv[1]);
        j = atoi(argv[2]);
        k = atoi(argv[3]);
        int cnt = i*j + j*k;
        int f1 = i*j;
        int f2 = j*k;
        // printf("i = %d , j = %d and k=%d",i,j,k);
        
        for(int t=4;t <= 4;++t)
        {
            // printf("f1 = %d , t=%d and cnt = %d",f,t,cnt);
            int n1 = 3;
            int n2 = t - n1 ;
            pthread_t tid[t];
            
            int start[t];
            for(int h=0;h<t;++h){
                start[h] = 0;
            }
            int count[t];
             for(int h=0;h<t;++h){
                count[h] = 0;
            }
            
            int k1 = i*j;
            int k2 = n1;
            int k3 = k1/k2 ;
            int k4 = k1%k2 ;
            
            for(int e = 0;e<n1;e++){
                count[e] = k3;
            }
            count[n1-1] += k4 ;
            
            k1 = j*k;
            k2 = n2;
            k3 = k1/k2;
            k4 = k1%k2;
            
            for(int e=n1;e<t;++e){
                count[e] = k3;
                
            }
            count[t-1] += k4;
            
            FILE* gg= fopen("in1.txt","r");
 
            // FILE* lmao = fopen("in1.txt","r");
            // fseek(lmao,12,SEEK_SET);
            // char ch = fgetc(lmao);
            // printf("Here -> %c\n",ch);
 
 
            start[0] = 0;
            for(int h = 0;h<n1;++h){
                int w;
                fscanf(gg,"%d",&w);
                int ff = ftell(gg);
                start[h] = ff - nod(w);
                int temp = count[h] - 1;
                
                while(temp--){
                    fscanf(gg,"%d",&w);
                }
            }
          
            for(int h=0;h<t;++h){
                // printf("%d\n",start[h]);
                int* arr = (int*)(malloc(4*sizeof(int)));
       
                arr[0] = 0;
 
                // if(h >= n1){
                //     arr[0] = 1;
                // }
 
                arr[1] = start[h];
                arr[2] = count[h];
                arr[3] = h;
                // printf("%d %d %d %d\n",arr[0],arr[1],arr[2],arr[3]);
                pthread_create(&tid[h],NULL,runner,(void*) arr);
 
            }
            for(int h=0;h<t;++h){
                
                pthread_join(tid[h],NULL);
 
            }
            
            
          
            
        }
        
        
    }