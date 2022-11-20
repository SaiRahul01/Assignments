#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
 
// initialise the parameters
 
int num, len, idx = 0, *path, states;
int i, j, k; // Iterators
char *memptr, *s, *ptr;
// File pointers for reading and writing into the input and output files
FILE *fileinp, *fileout1, *fileout2;
 
// Function to check if a point is well within the box limit
 
int chk(int a, int b, int cnt)
{
    return ((a >= 0 && a < cnt) && (b >= 0 && b < cnt));
}
 
// Traversal Code
 
void solve(int x, int y)
{
    if (memptr[0] == 'y')
    {
        exit(1);
    }
    if (idx == len)
    {
        // String is completely read
 
        if (x == num && y == num)
        {
            // Target Node is reached, hence set the memory
 
            int memkey = shmget(0204, 1, 0666 | IPC_CREAT);
            if (memkey == -1)
            {
                exit(1);
            }
 
            memptr = (char *)shmat(memkey, 0, 0);
            memptr[0] = 'y';
            // Set the memorty to yes, that means the target point can be reached in a particular path using the given string
 
            // Print success to output file
 
            fprintf(fileout1, "%s", "Accepted! Followed Path: ");
 
            for (i = 0; i < idx; ++i)
            {
                fprintf(fileout1, "%d ", path[i]);
            }
            fprintf(fileout1, "%d ", x + y * (num + 1));
            fprintf(fileout1, "%s ", "\n");
            fflush(fileout1);
            fclose(fileout1);
        }
        else
        {
            // Print Failure to output File
 
            fprintf(fileout1, "%s ", "Failed at Path: ");
 
            for (i = 0; i < idx; ++i)
            {
                // Print the Path
                fprintf(fileout1, "%d ", path[i]);
            }
            fprintf(fileout1, "%d ", x + y * num + 1);
            fprintf(fileout1, "%s ", "\n");
            fflush(fileout1);
            // Flush the output being printed into output.txt
        }
 
        exit(1);
        // No need to solve any further
    }
    else
    {
        path[idx] = x + (num + 1) * y; // Fill the path array with the encoded value of the current point
 
        // Print the ids of the current and Parent Processes
 
        fprintf(fileout1, "%s", "[");
        // Current Process id
        fprintf(fileout1, "%d", getpid());
        fprintf(fileout1, "%s", ":");
        // Parent Process id
        fprintf(fileout1, "%d", getppid());
        fprintf(fileout1, "%s", "]");
        fprintf(fileout1, "%s ", "Exploring Path: ");
        for (i = 0; i <= idx; ++i)
        {
            fprintf(fileout1, "%d ", path[i]);
        }
        fprintf(fileout1, "%s ", "\n");
        fflush(fileout1);
    }
 
    char ch = s[idx]; // Read the current character of the input string
    int xx = x;
    int yy = y;
    if (ch == '1') // Move vertically
    {
        yy++; // check upper Point
 
        if (chk(xx, yy, num + 1) && !fork())
        {
            idx++;
            solve(xx, yy); // Go further
            exit(1);       // End the Process
        }
        else
        {
            yy -= 2; // Check lower Point
 
            if (chk(xx, yy, num + 1) && !fork())
            {
                idx++;
                solve(xx, yy); // Go further
                exit(1);       // End the Process
            }
            else
            {
                while (wait(NULL) > 0)
                {
                    // Wait for the child/ childs Processes to complete their execution and then return
                }
                return;
            }
        }
    }
    else // Move horizontally
 
    {
        xx++;
        if (chk(xx, yy, num + 1) && !fork()) // Check the right Point
        {
            idx++;
            solve(xx, yy); // Go further
            exit(1);       // End the Process
        }
        else
        {
            xx -= 2;
            if (chk(xx, yy, num + 1) && !fork()) // Check the left Point
            {
                idx++;
                solve(xx, yy); // Go further
                exit(1);       // End the Process
            }
            else
            {
                while (wait(NULL) > 0)
                {
                    // Wait for the child/ childs Processes to complete their execution and then return
                }
                return;
            }
        }
    }
 
    // wait for all children
}
 
void code1() // Code for solving Task1
{
    char inputstring[100];
    fgets(inputstring, 100, fileinp);
    s = inputstring;
    len = strlen(s);
    int helper[len];
    path = helper;
    int start1 = 0, start2 = 0;
 
    int memkey = shmget(0204, 1, 0666 | IPC_CREAT);
 
    if (memkey == -1)
    {
        exit(1);
    }
 
    memptr = shmat(memkey, 0, 0);
 
    memptr[0] = 'n';
 
    solve(start1, start2);
 
    if (memptr[0] == 'y')
    {
        printf("YES\n");
    }
    else
    {
        printf("NO\n");
    }
    fprintf(fileout1, "%s", "\nDestroyed block: shm.bin");
    fflush(fileout1);
}
 
// For checking the necessary conditions
 
int helper(int *presentstates, int n, int ns[], int totstates)
{
    for (i = 0; i < totstates; ++i)
    {
        int f = 1;
        for (j = 0; j < n * n; ++j)
        {
            if (presentstates[i * n * n + j] != ns[j])
            {
                f = 0;
                break;
            }
        }
        if (f)
        {
            return i;
        }
    }
    return -1;
}
 
// Code for converting the NFA to DFA
 
int dfa(int n, int transition0[], int transition1[], int finalstate[])
{
    int totstates = 1;
    int presentstates[n * n][n * n];
    for (i = 0; i <= (n * n) - 1; ++i)
    {
        presentstates[0][i] = 0;
    }
    presentstates[0][0] = 1;
 
    for (k = 0; k < totstates; ++k)
    {
        int s0[n * n];
        int s1[n * n];
        for (i = 0; i < n * n; ++i)
        {
            s0[i] = 0;
            s1[i] = 0;
        }
        for (i = 0; i < n * n; ++i)
        {
            if (presentstates[k][i] == 1){
                int y = i % n;
                int x = i / n;
 
                if (chk(x, y + 1, n)){
                    s0[x * n + y + 1] = 1;
                }
                if (chk(x, y - 1, n)){
                    s0[x * n + y - 1] = 1;
                }
                if (chk(x + 1, y, n)){
                    s1[(x + 1) * n + y] = 1;
                }
                if (chk(x - 1, y, n)){
                    s1[(x - 1) * n + y] = 1;
                }
            }
        }
 
        if ((transition0[k] = helper((int *)presentstates, n, s0, totstates)) == -1)
        {
            for (i = 0; i < n * n; ++i)
            {
                presentstates[totstates][i] = s0[i];
            }
            totstates++;
 
            transition0[k] = totstates - 1;
        }
        if ((transition1[k] = helper((int *)presentstates, n, s1, totstates)) == -1)
        {
            for (i = 0; i < n * n; ++i)
            {
                presentstates[totstates][i] = s1[i];
            }
 
            totstates++;
 
            transition1[k] = totstates - 1;
        }
    }
 
    for (i = 0; i < totstates; ++i)
    {
        finalstate[i] = 0;
        if (presentstates[i][n * n - 1] == 1)
        {
            finalstate[i] = 1;
        }
    }
    return totstates;
}
 
void code2() // Code for solving Task2
{
 
    states = (num + 1) * (num + 1);
    int finalstate[states];
 
    int transition0[states];
 
    int transition1[states];
 
    int totstates = dfa(num + 1, transition0, transition1, finalstate);
    // printf("states= %d  and totstates = %d",states,totstates);
    int *minfinalstate = finalstate;
 
    int *mintransition0 = transition0;
 
    int *mintransition1 = transition1;
 
    int minstates = totstates; // Here Total States are min states as per the question
 
    for (i = 0; i < minstates; ++i)
    {
        fprintf(fileout2, "%d ", minfinalstate[i]);
    }
 
    fprintf(fileout2, "%s", "\n");
    fflush(fileout2);
 
    for (i = 0; i < minstates; ++i)
    {
        for (j = 0; j < minstates; ++j)
        {
            fprintf(fileout2, "%d ", j == mintransition0[i]);
        }
 
        fprintf(fileout2, "%s", "\n");
        fflush(fileout2);
    }
    for (i = 0; i < minstates; ++i)
    {
        for (j = 0; j < minstates; ++j)
        {
            fprintf(fileout2, "%d ", j == mintransition1[i]);
        }
 
        fprintf(fileout2, "%s", "\n");
        fflush(fileout2);
    }
}
 
int main()
{
    fileinp = fopen("input.txt", "r");
    fileout1 = fopen("2020A7PS0204H_t1.txt", "w");
    fileout2 = fopen("2020A7PS0204H_t2.txt", "w");
    char numchar[100];
    fgets(numchar, 100, fileinp);
    num = atoi(numchar);
    code1();
    code2();
    return 0;
}