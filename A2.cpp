#include <stdio.h>
#include <stdlib.h>

extern int registerme ( int );
extern void startsort ( );
extern int compareballs ( int, int );
extern void verifysort ( int * );
extern void startmatch ( int );
extern int fitsin ( int i, int j );
extern void verifymatch ( int * );

void mergesort ( int *S, int n, int *ncall )
{
   int m, i, j, k;
   int *T;

   if (n <= 1) return;
   m = n/2;
   mergesort(S,m,ncall);
   mergesort(S+m,n-m,ncall);
   T = (int *)malloc(n * sizeof(int));
   i = 0; j = m; k = 0;
   while ((i < m) || (j < n)) {
      if (i >= m) T[k++] = S[j++];
      else if (j >= n) T[k++] = S[i++];
      else {
         if (compareballs(S[i],S[j]) < 0) T[k++] = S[i++];
         else T[k++] = S[j++];
         ++(*ncall);
      }
   }
   for (k=0; k<n; ++k) S[k] = T[k];
   free(T);
}

int *sortballs ( int n, int *ncall )
{
   int i, *S;

   S = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; ++i) S[i] = i;
   mergesort(S,n,ncall);
   return S;
}

void quicksort1 ( int S[], int T[], int n, int *ncall )
{
   int *SL, *SG, *TL, *TG;
   int i, l, g, P, cmpres;

   if (n <= 1) return;

   /* Additional arrays used for partitioning */

   SL = (int *)malloc(n * sizeof(int));
   SG = (int *)malloc(n * sizeof(int));
   TL = (int *)malloc(n * sizeof(int));
   TG = (int *)malloc(n * sizeof(int));

   /* We can take any element of S or T as pivot. Here, S[0] is taken as
      the pivot. The array T is partitioned using this pivot. In the process,
      the exact match T[P] of S[0] is discovered. */

   l = g = 0;
   for (i=0; i<n; ++i) {
      cmpres = fitsin(S[0],T[i]); ++(*ncall);
      if (cmpres == 0) P = i;
      else if (cmpres > 0) TG[g++] = T[i];
      else TL[l++] = T[i];
   }

   /* Now, partition S usiing T[P] as pivot */

   l = g = 0;
   for (i=1; i<n; ++i) {
      cmpres = fitsin(S[i],T[P]); ++(*ncall);
      if (cmpres > 0) SL[l++] = S[i];
      else SG[g++] = S[i];
   }

   /* Write back the temporary arrays to the original arrays */

   /* The pivots are in position */
   T[l] = T[P]; S[l] = S[0];

   /* The smaller items are placed before the pivot */
   for (i=0; i<l; ++i) { T[i] = TL[i]; S[i] = SL[i]; }

   /* The larger items are placed after the pivot */
   for (i=0; i<g; ++i) { T[l+1+i] = TG[i]; S[l+1+i] = SG[i]; }

   free(TL); free(TG); free(SL); free(SG);

   /* Recursive calls */

   quicksort1(S,T,l,ncall);
   quicksort1(S+l+1,T+l+1,g,ncall);
}

/* This function does not use the solution of merge sort (Part 1) */
int *findmatch1 ( int n, int *ncall )
{
   int *S, *T, *M, i;

   /* Sort the balls (S) and boxes (T) together */
   S = (int *)malloc(n * sizeof(int));
   T = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; ++i) S[i] = T[i] = i;
   quicksort1(S,T,n,ncall);

   /* Prepare the match array M */
   M = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; ++i) M[S[i]] = T[i];

   free(S); free(T);

   return M;
}

void quicksort2 ( int S[], int T[], int n, int *ncall )
{
   int m, i, l, g, *TL, *TG, cmpres, P;

   if (n <= 1) return;

   /* Additional arrays used for partitioning. S is already sorted, so there
      is no need to sort it again. */

   TL = (int *)malloc(n * sizeof(int));
   TG = (int *)malloc(n * sizeof(int));

   /* Use the middle element of S as the pivot. This will force the
      subproblem sizes to n /2, not only for S but also for T. */

   m = n / 2; l = g = 0;

   /* Partition T using S[m] as pivot. The process discovers T[P] as
      the exact match of S[m]. */

   for (i=0; i<n; ++i) {
      cmpres = fitsin(S[m],T[i]); ++(*ncall);
      if (cmpres == 0) P = i;
      else if (cmpres < 0) TL[l++] = T[i];
      else TG[g++] = T[i];
   }

   /* Write back to T */

   /* Place the pivot in its correct position */
   T[m] = T[P];

   /* The smaller items are placed before the pivot */
   for (i=0; i<m; ++i) T[i] = TL[i];

   /* The larger items are placed after the pivot */
   for (i=0; i<n-m-1; ++i) T[m+1+i] = TG[i];

   /* Recursive calls */

   quicksort2(S,T,m,ncall);
   quicksort2(S+m+1,T+m+1,n-m-1,ncall);
}

/* This function uses the array S available from merge sort (Part 1).
   Since S is already sorted, we can force quick sort to exhibit its
   best-case performance. */
int *findmatch2 ( int S[], int n, int *ncall )
{
   int *T, *M, i;

   /* Now, we need only one array (T) for the boxes */
   T = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; ++i) T[i] = i;

   /* Sort T using S as a guide for effective partitioning */
   quicksort2(S,T,n,ncall);

   /* Finally, prepare the matching array M */
   M = (int *)malloc(n * sizeof(int));
   for (i=0; i<n; ++i) M[S[i]] = T[i];

   free(T);

   return M;
}

int main ()
{
   int n = 1000000;
   int *S;
   int *M;
   int ncall;

   registerme(n);

   printf("\n+++ Sorting the balls...\n");
   startsort(); ncall = 0;
   S = sortballs(n,&ncall);
   printf("    Number of black-box queries = %d\n", ncall);
   verifysort(S);

   printf("\n+++ Finding the matching boxes: Method 1...\n");
   startmatch(0); ncall = 0;
   M = findmatch1(n,&ncall);
   printf("    Number of black-box queries = %d\n", ncall);
   verifymatch(M);
   free(M);

   printf("\n+++ Finding the matching boxes: Method 2...\n");
   startmatch(1); ncall = 0;
   M = findmatch2(S,n,&ncall);
   printf("    Number of black-box queries = %d\n", ncall);
   verifymatch(M);
   free(M);

   free(S);

   exit(0);
}
