
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
   int idx;
   int sum;
   int inc;
} htnode;

typedef struct {
   int size;
   htnode *table;
} hashtable;

#define EMPTY (htnode){-1,-1,-1}

int *geninput ( int N )
{
   int *A, i;

   A = (int *)malloc(N * sizeof(int));
   for (i=0; i<N; ++i) {
      if (i % 12 == 0) printf("   ");
      A[i] = 10000 + rand() % 90000;
      printf(" %5d", A[i]);
      if (i % 12 == 11) printf("\n");
   }
   if (i % 12) printf("\n");
   return A;
}

int exhs ( int *A, int N, int T, int sum, int i )
{
   int val0, val1;

   if (sum > T) return -1;
   if (i >= N) return sum;
   val0 = exhs(A,N,T,sum,i+1);
   val1 = (sum+A[i]<=T) ? exhs(A,N,T,sum+A[i],i+1) : -1;
   return (val0 >= val1) ? val0 : val1;
}

int h ( int i, int sum, int s )
{
   return (100003 * i + 103 * sum) % s;
}

hashtable htinit ( int s )
{
   hashtable H;
   int i;

   H.size = s;
   H.table = (htnode *)malloc(s * sizeof(htnode));
   for (i=0; i<s; ++i) H.table[i] = EMPTY;
   return H;
}

int isempty ( hashtable H, int k )
{
   return (H.table[k].idx == -1);
}

int htsearch ( hashtable H, int i, int sum )
{
   int k, j, s;

   s = H.size;
   k = h(i,sum,s);
   for (j=0; j<s; ++j) {
      if (isempty(H,k)) return 0;
      if ((H.table[k].idx == i) && (H.table[k].sum == sum)) return k+1;
      if (++k == s) k = 0;
   }
   return 0;
}

void htinsert ( hashtable H, int i, int sum, int included )
{
   int k, j, s;

   s = H.size;
   k = h(i,sum,s);
   for (j=0; j<s; ++j) {
      if (isempty(H,k)) {
         H.table[k].idx = i;
         H.table[k].sum = sum;
         H.table[k].inc = included;
         break;
      }
      if (++k == s) k = 0;
   }
   return;
}

int hashs ( int *A, int N, int T, int sum, int i, hashtable H )
{
   int val0, val1;

   if (sum > T) return -1;
   if (i >= N) return sum;
   if (htsearch(H,i,sum)) val0 = -1; else {
      htinsert(H,i,sum,0);
      val0 = hashs(A,N,T,sum,i+1,H);
   }
   if ((sum+A[i]>T) || (htsearch(H,i,sum+A[i]))) val1 = -1; else {
      htinsert(H,i,sum+A[i],1);
      val1 = hashs(A,N,T,sum+A[i],i+1,H);
   }
   return (val0 >= val1) ? val0 : val1;
}

void findsol ( int *A, int N, int P, hashtable H )
{
   int *include, i, k, firstprn;

   include = (int *)malloc(N * sizeof(int));
   for (i=0; i<N; ++i) include[i] = 0;
   for (i=N-1; i>=0; --i) {
      if (htsearch(H,i,P)) break;
   }
   while (P>0) {
      k = htsearch(H,i,P) - 1;
      if (H.table[k].inc) {
         include[i] = 1;
         P -= A[i];
      }
      --i;
   }
   firstprn = 1;
   printf("    Solution: ");
   for (i=0; i<N; ++i) {
      if (include[i]) {
         if (!firstprn) printf(" + ");
         printf("%d", A[i]);
         firstprn = 0;
      }
   }
   printf("\n");
   free(include);
}

int main ( int argc, char *argv[] )
{
   int N;
   int *A, i;
   int S, T, P;
   hashtable H;

   srand((unsigned int)time(NULL));

   if (argc > 1) N = atoi(argv[1]); else scanf("%d", &N);
   printf("    N = %d\n", N);

   A = geninput(N);
   S = 0; for (i=0; i<N; ++i) S += A[i]; T = S / 2;
   printf("\n    S = %d\n    T = %d\n", S, T);

   printf("\n+++ Exhaustive search\n");
   P = exhs(A,N,T,0,0);
   printf("    Maximum P = %d\n", P);

   H = htinit(T*N);

   printf("\n+++ Search with a hash table\n");
   P = hashs(A,N,T,0,0,H);
   printf("    Maximum P = %d\n", P);
   findsol(A,N,P,H);

   free(H.table);

   exit(0);
}
