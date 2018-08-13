#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
   int left;
   int right;
   int cost;
} heapnode;

typedef struct {
   heapnode *array;
   int size;
} heap;

int *gengaps ( int n )
{
   int *d, i;

   printf("   ");
   d = (int *)malloc((n-1)*sizeof(int));
   for (i=0; i<=n-2; ++i) {
      d[i] = 5 + rand() % 16;
      printf(" %2d", d[i]);
      if (i % 25 == 24) printf("\n   ");
   }
   if (i % 25) printf("\n");
   return d;
}

heap initheap ( int *d, int n )
{
   heap H;
   int i;

   H.size = n-1;
   H.array = (heapnode *)malloc(n*sizeof(heapnode));
   for (i=0; i<=n-2; ++i) {
      H.array[i+1].left = i;
      H.array[i+1].right = i+1;
      H.array[i+1].cost = d[i];
   }
   return H;
}

void heapify ( heap H, int i )
{
   int n, l, r, m;
   heapnode t;

   n = H.size;
   while (1) {
      l = 2*i; r = 2*i + 1;
      if (l > n) return;
      if (l == n) m = l;
      else m = (H.array[l].cost <= H.array[r].cost) ? l : r;
      if (H.array[i].cost <= H.array[m].cost) return;
      t = H.array[i]; H.array[i] = H.array[m]; H.array[m] = t;
      i = m;
   }
}

void makeheap ( heap H )
{
   int i;

   for (i=H.size/2; i>=1; --i) heapify(H,i);
   return;
}

heap insert ( heap H, int l, int r, int c )
{
   int i, p, n;
   heapnode t;

   n = ++(H.size);
   H.array[n].left = l;
   H.array[n].right = r;
   H.array[n].cost = c;
   i = n;
   while (1) {
      if (i == 1) return H;
      p = i / 2;
      if (H.array[p].cost <= H.array[i].cost) return H;
      t = H.array[i]; H.array[i] = H.array[p]; H.array[p] = t;
      i = p;
   }
}

heap deletemin ( heap H )
{
   if (H.size == 0) {
      printf("*** Deletion from empty heap attempted...\n");
      return H;
   }
   H.array[1] = H.array[H.size];
   --(H.size);
   heapify(H,1);
   return H;
}

void greedy ( int *D, int N, int K )
{
   heap H;
   int k, cost, l, r, c;
   int *connectedto;

   printf("--- Adding connections\n    ");

   H = initheap(D,N);
   makeheap(H);

   connectedto = (int *)malloc(N * sizeof(int));
   for (k=0; k<N; ++k) connectedto[k] = -1;
   k = 0; cost = 0;
   while (k < K) {
      l = H.array[1].left;
      r = H.array[1].right;
      c = H.array[1].cost;
      H = deletemin(H);
      if ((connectedto[l] < 0) && (connectedto[r] < 0)) {
         connectedto[l] = r;
         connectedto[r] = l;
         ++k; cost += c;
         printf("(%2d,%2d):%2d ", l, r, c);
         if (k % 6 == 0) printf("\n    ");
      }
   }
   if (k % 6) printf("\n    ");

   printf("Total cost = %d\n", cost);

   free(H.array);
   free(connectedto);
}

void greedyec ( int *D, int N, int K )
{
   heap H;
   int k, cost, l, r, c;
   int *connectedto;     /* Array storing vertices are connected to which
                            vertices or are unconnected (-1) */

   H = initheap(D,N);
   makeheap(H);

   connectedto = (int *)malloc(N * sizeof(int));
   for (k=0; k<N; ++k) connectedto[k] = -1;
   k = 0; cost = 0;
   while (k < K) {
      l = H.array[1].left;
      r = H.array[1].right;
      c = H.array[1].cost;
      H = deletemin(H);
      if ((connectedto[l] < 0) && (connectedto[r] < 0)) { /* l and r are both free */
         connectedto[l] = r;
         connectedto[r] = l;
         ++k; cost += c;

         /* Now add the virtual connection (l-1,r+1) provided that l-1 and r+1 are
            in the valid range and are both free */
         --l; ++r;
         if ((l >= 0) && (r < N) && (connectedto[l] < 0) && (connectedto[r] < 0)) {
            c = D[l] + D[r-1] - c;
            H = insert(H,l,r,c);
         }
      }
   }
   printf("--- Total cost = %d\n", cost);

   /* Replace the virtual connections one by one */
   for (l=0; l<N; ++l) {
      r = connectedto[l];
      if ((r < 0) || (l > r)) continue;  /* unconnected vertex */
      if (r > l+1) {  /* virtual connection found */
         /* We need to add (l,l+1) and (r,r+1). This automatically deletes
            the inner connection (l+1,r-1) */
         connectedto[l] = l+1;
         connectedto[l+1] = l;
         connectedto[r] = r-1;
         connectedto[r-1] = r;
      }
   }

   printf("--- Adding connections\n    ");
   cost = 0; k = 0;
   for (l=0; l<N; ++l) {
      r = connectedto[l];
      if (r == l+1) {  /* Now there are no virtual connections */
         c = D[l];
         cost += c;
         ++k;
         printf("(%2d,%2d):%2d ", l, r, c);
         if (k % 6 == 0) printf("\n    ");
      }
   }
   if (k % 6) printf("\n    ");

   printf("Total cost = %d\n", cost);

   free(H.array);
   free(connectedto);
}

int main ( int argc, char *argv[] )
{
   int N, K;
   int *D;

   if (argc >= 3) {
      N = atoi(argv[1]);
      K = atoi(argv[2]);
   } else {
      scanf("%d%d", &N, &K);
   }
   printf("    N = %d\n    K = %d\n", N, K);

   srand((unsigned int)time(NULL));

   D = gengaps(N);

   printf("\n*** Part 1: Greedy algorithm\n");
   greedy(D,N,K);

   printf("\n*** Part 2: Greedy algorithm with error correction\n");
   greedyec(D,N,K);

   exit(0);
}
