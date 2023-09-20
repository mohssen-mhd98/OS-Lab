#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) 
{ 
    // Process names in here are P0, P1, P2, P3, P4
  
    int n, m, i, j, k; 
    n = 5; // Number of processes 
    m = 3; // Number of resources
    int available[3];
    for(i=0;i<3;i++){
        available[i] = atoi(argv[i+1]); // Available Resources 
    }
    int allocation[5][3] = { { 0, 1, 0 }, // P0    // Allocation Matrix 
                        { 2, 0, 0 }, // P1 
                        { 3, 0, 2 }, // P2 
                        { 2, 1, 1 }, // P3 
                        { 0, 0, 2 } }; // P4 
  
    int maximum[5][3] = { { 7, 5, 3 }, // P0    // Maximum Matrix 
                      { 3, 2, 2 }, // P1 
                      { 9, 0, 2 }, // P2 
                      { 2, 2, 2 }, // P3 
                      { 4, 3, 3 } }; // P4 
  
  
    int f[n], ans[n], ind = 0; 
    for (k = 0; k < n; k++) { 
        f[k] = 0; 
    } 
    int need[n][m]; 
    for (i = 0; i < n; i++) { 
        for (j = 0; j < m; j++) 
            need[i][j] = maximum[i][j] - allocation[i][j]; 
    } 
    int y = 0; 
    for (k = 0; k < 5; k++) { 
        for (i = 0; i < n; i++) { 
            if (f[i] == 0) { 
                int flag = 0; 
                for (j = 0; j < m; j++) { 
                    if (need[i][j] > available[j]){ 
                        printf("Process %d needs %d of resourse %d and available is %d, So Process will Not get the resource \n", i, need[i][j], j, available[j]);
                        flag = 1; 
                         break; 
                    } 
                    printf("Process %d needs %d of resourse %d and available is %d, So process is gonna get the resource \n", i, need[i][j], j, available[j]);

                } 
  
                if (flag == 0) { 
                    ans[ind++] = i; 
                    for (y = 0; y < m; y++) 
                        available[y] += allocation[i][y]; 
                    f[i] = 1;        

                } 
            } 
        } 
    } 
  
    printf("Following is the SAFE Sequence\n"); 
    for (i = 0; i < n - 1; i++) 
        printf(" P%d ->", ans[i]); 
    printf(" P%d \n", ans[n - 1]); 
  
    return 0; 
  
} 
