#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INFINITY 1000000000

int **gencosts ( int n )
{
   int **C, i, j;

   printf("\n+++ Flyover building costs\n");
   C = (int **)malloc((n-1)*sizeof(int *));
   for (i=0; i<=n-2; ++i) {
      printf("  ");
      C[i] = (int *)malloc(n * sizeof(int));
      for (j=0; j<=n-1; ++j) {
         C[i][j] = 5 + rand() % 16;
         printf("  %2d", C[i][j]);
      }
      printf("\n");
   }
   return C;
}

int greedy ( int **C, int n )
{
   int TC, i, j, k, h, kmin, TCnew, TCmin;

   TC = 0;
   i = j = h = 0;
   while (i < n-1) {
      kmin = j; TCmin = TC + C[i][j];
      for (k=j+1; k<=n-1; ++k) {
         if (i) TC += 5;
         TCnew = TC + C[i][k];
         if (TCnew < TCmin) {
            kmin = k;
            TCmin = TCnew;
         }
      }
      if (i) h += (kmin - j);
      ++i; j = kmin; TC = TCmin;
      printf("    Building flyover from (%d,%d) to (%d,%d): Cost = %2d\n",
             i-1, j, i, j, C[i-1][j]);
   }
   printf("    Total cost of the horizontal segments      = %2d\n", 5*h);
   return TC;
}

int DP ( int **C, int n )
{
   int **T;
   int i, j;
   int TC, TC1, TC2;

   T = (int **)malloc(n * sizeof(int *));
   for (i=0; i<=n-1; ++i) {
      T[i] = (int *)malloc(n * sizeof(int));
      if (i == 0) {
         for (j=0; j<=n-1; ++j) T[i][j] = 0;
      } else {
         for (j=0; j<=n-1; ++j) {
            TC1 = (j == 0) ? INFINITY : T[i][j-1] + 5;
            TC2 = T[i-1][j] + C[i-1][j];
            T[i][j] = (TC1 <= TC2) ? TC1 : TC2;
         }
      }
   }

   TC = T[n-1][0];
   for (j=1; j<=n-1; ++j) {
      if (T[n-1][j] < TC) TC = T[n-1][j];
   }

   for (i=0; i<=n-1; ++i) free(T[i]);
   free(T);

   return TC;
}

void DPsol ( int **C, int n )
{
   int **T;
   char **D;
   int *S;
   int i, j, k;
   int TC, TC1, TC2;

   T = (int **)malloc(n * sizeof(int *));
   D = (char **)malloc(n * sizeof(char *));
   for (i=0; i<=n-1; ++i) {
      T[i] = (int *)malloc(n * sizeof(int));
      D[i] = (char *)malloc(n * sizeof(char));
      if (i == 0) {
         for (j=0; j<=n-1; ++j) {
            T[i][j] = 0;
            D[i][j] = 'S';
         }
      } else {
         for (j=0; j<=n-1; ++j) {
            TC1 = (j == 0) ? INFINITY : T[i][j-1] + 5;
            TC2 = T[i-1][j] + C[i-1][j];
            if (TC1 <= TC2) {
               T[i][j] = TC1;
               D[i][j] = 'H';
            } else {
               T[i][j] = TC2;
               D[i][j] = 'V';
            }
         }
      }
   }

   TC = T[n-1][0]; j = 0;
   for (i=1; i<=n-1; ++i) {
      if (T[n-1][i] < TC) {
         TC = T[n-1][i];
         j = i;
      }
   }

   S = (int *)malloc((n-1)*sizeof(int));
   i = n-1; k = 0;
   while (i > 0) {
      if (D[i][j] == 'H') {
         ++k; --j;
      } else {
         --i; S[i] = j;
      }
   }

   for (i=0; i<=n-2; ++i) {
      printf("    Building flyover from (%d,%d) to (%d,%d): Cost = %2d\n",
             i, S[i], i+1, S[i], C[i][S[i]]);
   }
   printf("    Total cost of the horizontal segments      = %2d\n", 5*k);

   for (i=0; i<=n-1; ++i) { free(T[i]); free(D[i]); }
   free(T); free(D); free(S);
}

int main ( int argc, char *argv[] )
{
   int n;
   int **C, TC;

   if (argc > 1) n = atoi(argv[1]); else scanf("%d", &n);
   printf("n = %d\n", n);
   srand((unsigned int)time(NULL));

   C = gencosts(n);

   printf("\n+++ Part 1: Greedy algorithm\n");
   TC = greedy(C,n);
   printf("--- Total cost = %d\n", TC);

   printf("\n+++ Part 2: Dynamic programming algorithm\n");
   TC = DP(C,n);
   printf("--- Total cost = %d\n", TC);

   printf("\n+++ Part 3: Dynamic programming algorithm with solution\n");
   DPsol(C,n);

   exit(0);
}
