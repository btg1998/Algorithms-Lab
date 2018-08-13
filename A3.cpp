#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLUS_INFTY 2000000000

int *genarray ( int n, int M, char prompt[], int multiplier )
{
   int i, X;
   int *A;

   printf("+++ %s :", prompt);
   X = (multiplier * M) / n;
   A = (int *)malloc((n+1) * sizeof(int));
   for (i=1; i<=n; ++i) {
      A[i] = 1 + rand() % X;
      if (A[i] > M) { --i; continue; }
      printf("%4d", A[i]);
   }
   printf("\n");
   return A;
}

void mergesort ( int E[], int n )
{
   int m, i, j, k;
   int *A;

   if (n <= 1) return;
   m = (n+1) / 2;
   mergesort(E,m);
   mergesort(E+m,n-m);
   A = (int *)malloc(n * sizeof(int));
   i = 0; j = m; k = 0;
   while ((i < m) || (j < n)) {
      if (j == n) A[k] = E[i++];
      else if (i == m) A[k] = E[j++];
      else if (E[i] <= E[j]) A[k] = E[i++];
      else A[k] = E[j++];
      ++k;
   }
   for (k=0; k<n; ++k) E[k] = A[k];
   free(A);
}

void solveGRD ( int E[], int n, int M )
{
   int i, c, effort, neweffort;

   mergesort(E+1,n);
   effort = 0; c = 0;
   for (i=1; i<=n; ++i) {
      neweffort = effort + E[i];
      if (neweffort > M) break;
      ++c; effort = neweffort;
   }
   printf("    Minimum effort = %5d = ", effort);
   if (effort) {
      printf("%2d", E[1]);
      for (i=2; i<=c; ++i) printf(" + %2d", E[i]);
   }
   printf("\n");
}

void solveDP ( int E[], int P[], int n, int M )
{
   int **T, **S;
   int MAXP;
   int i, p, c;
   int e0, e1;
   int *sol;

   MAXP = 0;
   for (i=1; i<=n; ++i) MAXP += P[i];

   /* T is the objective-function array such that T[i][p] stores the
      minimum effort needed to achive total point p from assignments
      1,2,..,i. If exactly p is not achievable, then T[i][p] = INFINITY.

      S is an additional array used for finding the choice of the
      assignments achieving the optimal objective-function value.
      S[i][p] stores the decision: 0 means exclude assignment i to
      attempt total point p (exactly), 1 means solve assignment i to
      attempt total point p. If T[i][p] = INFINITY, we take S[i][p] = 0.
   */
   T = (int **)malloc((n+1) * sizeof(int *));
   S = (int **)malloc((n+1) * sizeof(int *));
   for (i=0; i<=n; ++i) {
      T[i] = (int *)malloc((1 + MAXP) * sizeof(int));
      S[i] = (int *)malloc((1 + MAXP) * sizeof(int));
      T[i][0] = S[i][0] = 0;
   }
   for (p=1; p<=MAXP; ++p) T[0][p] = PLUS_INFTY;
   for (i=1; i<=n; ++i) {
      for (p=1; p<=MAXP; ++p) {
         e0 = T[i-1][p];
         e1 = ((p < P[i]) || (T[i-1][p-P[i]] == PLUS_INFTY)) ? PLUS_INFTY : E[i] + T[i-1][p-P[i]];
         if ((e0 > M) && (e1 > M)) {
            T[i][p] = PLUS_INFTY;
            S[i][p] = 0;  /* Do not include Assignment i */
         } else if (e0 <= e1) {
            T[i][p] = e0;
            S[i][p] = 0;  /* Do not include Assignment i */
         } else {
            T[i][p] = e1;
            S[i][p] = 1;  /* Include Assignment i */
         }
      }
   }

   /* Search the last row for locating the maximum achievable point */
   p = MAXP;
   while ((p >= 0) && (T[n][p] == PLUS_INFTY)) --p;

   /* MAXP now stores the maximum achievable point */
   MAXP = p;

   /* Work out an optimal choice of the assignments */
   if (p > 0) {
      sol = (int *)malloc(n * sizeof(int));
      c = 0; i = n;
      while (p > 0) {         /* So long as all assigments are not identified */
         if (S[i][p] == 1) {  /* Include Assignment i */
            sol[c++] = i;     /* Store i in the solution array sol[] */
            p -= P[i];
         }
         --i;
      }

      /* Print the solution backward as they are stored in sol[] */
      printf("    Maximum points = %5d = ", MAXP);
      printf("%2d", P[sol[c-1]]);
      for (i=c-2; i>=0; --i) printf(" + %2d", P[sol[i]]);
      printf("\n");
      printf("    Minimum effort = %5d = ", T[n][MAXP]);
      printf("%2d", E[sol[c-1]]);
      for (i=c-2; i>=0; --i) printf(" + %2d", E[sol[i]]);
      printf("\n");
      free(sol);
   } else {
      printf("    Maximum points = 0\n");
      printf("    Minimum effort = 0\n");
   }
   for (i=0; i<=n; ++i) { free(T[i]); free(S[i]); }
   free(T); free(S);
}

/* This is another variant, more commonly available to solve the 0-1 Knapsack
   problem. */
void solveDP2 ( int E[], int P[], int n, int M )
{
   int **T, **S;
   int i, m, c, *sol;
   int p0, p1;

   /* Here, T[i][m] stores the maximum point that can be attempted from
      Assignments 1,2,...,i if the available time limit is M. The implementation
      does not assume that exactly m minutes must be spent to achieve the
      maximum point.

      T[i][m], as in solveDP(), stores the decision whether to excluce (0)
      or include (1) Assignment i in the optimal solution. */
   T = (int **)malloc((n + 1) * sizeof(int *));
   S = (int **)malloc((n + 1) * sizeof(int *));
   for (i=0; i<=n; ++i) {
      T[i] = (int *)malloc((M + 1) * sizeof(int));
      S[i] = (int *)malloc((M + 1) * sizeof(int));
      T[i][0] = 0; S[i][0] = 0;  /* In zero minutes, no assignment can be solved */
   }
   for (m=1; m<=M; ++m) { T[0][m] = 0; S[0][m] = 0; } /* No assignments, so zero points */
   for (i=1; i<=n; ++i) {
      for (m=1; m<=M; ++m) {
         if (m < E[i]) {  /* The i-th assignment is too long to be done in m minutes */
            T[i][m] = T[i-1][m];
            S[i][m] = 0; 
         } else {
            p0 = T[i-1][m];              /* Include Assignment i */
            p1 = P[i] + T[i-1][m-E[i]];  /* Include Assignment i */
            if (p0 >= p1) {  /* Take the better of the two choices */
               T[i][m] = p0;
               S[i][m] = 0;
            } else {
               T[i][m] = p1;
               S[i][m] = 1;
            }
         }
      }
   }
   if (T[n][M] == 0) {
      printf("    Maximum points = 0\n");
      printf("    Minimum effort = 0\n");
   } else {
      /* Here T[n][M] > 0. This does not mean that a total point T[n][M]
         can be achieved in exactly M hours. We have to first find the
         minimum m such that T[n][m] = T[n][M]. For this m, the maximum
         point m can be achieved in exactly m hours. */
      m = M;
      while (m >= 0) {
         if (T[n][m] < T[n][M]) { ++m; break; }
         --m;
      }
      M = m;

      /* Now, construct the solution backward */
      sol = (int *)malloc(n * sizeof(int)); c = 0; i = n;
      while (m > 0) {
         if (S[i][m] == 1) {  /* Include Assignment i in the solution */
            sol[c++] = i;
            m -= E[i];
         }
         --i;
      }
      printf("    Maximum points = %5d = ", T[n][M]);
      printf("%2d", P[sol[c-1]]);
      for (i=c-2; i>=0; --i) printf(" + %2d", P[sol[i]]);
      printf("\n");
      printf("    Minimum effort = %5d = ", M);
      printf("%2d", E[sol[c-1]]);
      for (i=c-2; i>=0; --i) printf(" + %2d", E[sol[i]]);
      printf("\n");
      free(sol);
   }
   for (i=0; i<=n; ++i) { free(T[i]); free(S[i]); }
   free(T); free(S);
}

int main ( int argc, char *argv[] )
{
   int n, M;
   int *E, *P;

   srand((unsigned int)time(NULL));
   if (argc >= 3) {
      n = atoi(argv[1]);
      M = atoi(argv[2]);
   } else scanf("%d", &n);
   printf("n = %d\nM = %d\n\n", n, M);

   E = genarray(n,M,"Efforts",4);

   printf("\n+++ Part 1 (Greedy)\n");
   solveGRD(E,n,M);

   printf("\n");
   P = genarray(n,M,"Points ",1);
   E = genarray(n,M,"Efforts",4);

   printf("\n+++ Part 2 (DP)\n");
   solveDP(E,P,n,M);

   printf("\n+++ Part 2 (DP, alternative)\n");
   solveDP2(E,P,n,M);

   exit(0);
}
