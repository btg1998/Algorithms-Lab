#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int stackystrhelper ( char *A, int n, int nplus, int nminus, int cnt )
{
   if ((nplus == n) && (nminus == n)) {
      ++cnt;
      printf("    Stacky string %5d: %s\n", cnt, A);
      return cnt;
   }

   if (nminus < nplus) {
      A[nplus+nminus] = '-';
      cnt = stackystrhelper(A,n,nplus,nminus+1,cnt);
   }

   if (nplus < n) {
      A[nplus+nminus] = '+';
      cnt = stackystrhelper(A,n,nplus+1,nminus,cnt);
   }

   return cnt;
}

int stackystr ( int n )
{
   int cnt;
   char *A;

   A = (char *)malloc((2*n+1)*sizeof(char));
   A[2*n] = '\0';
   cnt = stackystrhelper(A,n,0,0,0);
   free(A);
   return cnt;
}

unsigned long long stackystrformula ( int n )
{
   unsigned long long cnt;
   int i;

   cnt = 1;
   for (i=1; i<=n; ++i) {
      cnt *= (4*i - 2);
      cnt /= (i + 1);
   }
   return cnt;
}

int stackypermhelper ( char *A, int n, int nplus, int nminus, int cnt )
{
   int *S, top;
   int i, a, npr;

   if ((nplus == n) && (nminus == n)) {
      ++cnt;
      S = (int *)malloc(n * sizeof(int));
      top = -1; a = 1; npr = 0;
      printf("    Stacky permutation %5d: ", cnt);
      for (i=0; i<2*n; ++i) {
         if (A[i] == '+') {
            S[++top] = a++;
         } else {
            printf("%d", S[top--]);
            ++npr;
            if (npr < n) printf(","); else printf("\n");
         }
      }
      free(S);
      return cnt;
   }

   if (nminus < nplus) {
      A[nplus+nminus] = '-';
      cnt = stackypermhelper(A,n,nplus,nminus+1,cnt);
   }

   if (nplus < n) {
      A[nplus+nminus] = '+';
      cnt = stackypermhelper(A,n,nplus+1,nminus,cnt);
   }

   return cnt;
}

int stackyperm ( int n )
{
   int cnt;
   char *A;

   A = (char *)malloc((2*n+1)*sizeof(char));
   A[2*n] = '\0';
   cnt = stackypermhelper(A,n,0,0,0);
   free(A);
   return cnt;
}

int isstacky ( int *A, int n )
{
   int *S, top, a, i;

   S = (int *)malloc(n * sizeof(int));
   top = -1; a = 1; i = 0;
   while (1) {
      if ((top >= 0) && (S[top] == A[i])) {
         --top; ++i;
      } else if (a <= n) {
         S[++top] = a++;
      } else {
         break;
      }
   }
   free(S);
   return (i == n) ? 1 : 0;
}

void genperm ( int *A, int n )
{
   int i, j, k, t;

   for (i=0; i<n; ++i) A[i] = i+1;
   for (i=0; i<n; ++i) {
      j = rand() % n;
      k = rand() % n;
      t = A[j]; A[j] = A[k]; A[k] = t;
   }
   for (i=0; i<n; ++i) printf("%d ", A[i]);
}

int main ( int argc, char *argv[] )
{
   int n, cnt, *A;

   srand((unsigned int)time(NULL));
   if (argc > 1) n = atoi(argv[1]); else scanf("%d", &n);
   printf("n = %d\n\n", n);

   printf("+++ All stacky strings of parameter %d are:\n", n);
   cnt = stackystr(n);
   printf("--- There are %d of them\n\n", cnt);

   printf("+++ Stacky string counts by formula\n");
   printf("    C[%d] = %Lu\n", n, stackystrformula(n));
   printf("    C[16] = %Lu\n", stackystrformula(16));
   printf("    C[32] = %Lu\n\n", stackystrformula(32));

   printf("+++ All stacky permutations of length %d are:\n", n);
   cnt = stackyperm(n);
   printf("--- There are %d of them\n\n", cnt);

   A = (int *)malloc(n * sizeof(int));
   printf("+++ Check for stacky permutations:\n");
   printf("    Permutation "); genperm(A,n);
   printf("is%sstacky\n", isstacky(A,n) ? " " : " not ");
   printf("    Permutation "); genperm(A,n);
   printf("is%sstacky\n", isstacky(A,n) ? " " : " not ");
   free(A);

   exit(0);
}
