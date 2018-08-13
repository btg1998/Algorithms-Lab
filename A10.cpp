#include <stdio.h>
#include <stdlib.h>

#define EVEN 0
#define ODD 1

#define OR 1
#define XOR 0

extern void registerme();
extern int rangequery ( int, int, int );
extern void verifysoln ( int [] );

/* We maintain a queue of mutually disjoint intervals with each stored
   interval guaranteed to store at leaast one solution. */
typedef struct {
   int l;       /* the left endpoint of the interval */
   int r;       /* the right endpoint of the interval */
   int parity;  /* whether the interval contains an even or an odd number of solutions */
} interval;

/* Standard binary search procudure to locate a solution in [L,R].

   If the number of solutions in [L,R] is even, we call this function with
   type = OR. The function returns the index of the leftmost one bit in the
   interval [L,R]. In this case, we make only OR range queries.

   If the number of solutions in [L,R] is odd, a solution can be found by
   making XOR range queries only, so we pass type = XOR. However it is now
   not guaranteed that the solution is leftmost or rightmost. Since the
   successive bisection of the search interval will ensure that one of
   the half-sized sub-intervals contains an odd number of solutions, we
   can pinpoint a solution using log_2 n XOR range queries.
*/
int binsearch ( int L, int R, int type )
{
   int M;

   while (L < R) {
      M = (L + R) / 2;
      if (rangequery(L,M,type)) R = M; else L = M+1;
   }
   return L;
}

/* Note: There remains a possibility that you end up with too many XOR
   queries (for example, when k = 2^t-1, and XOR-based binary search always
   gives the central solution). This problem may be solved by maintaining
   your private counts of range queries. At any point of time, an XOR-based
   binary search can be replaced by a (more powerful) OR-based binary
   search. In the following function, this is not implemented for the
   sake of simplicity. */

void findpos ( int A[], int n )
{
   interval Q[100];        /* The queue of intervals */
   int front, back, L, R;
   int i, j, k, t;

   /* Initially, we know that the interval [0,n-1] contains ten (an even
      number of) solutions. */
   Q[0].l = 0; Q[0].r = n-1; Q[0].parity = EVEN; front = back = 0; k = 0;

   /* Repeat so long as Q is not empty */
   while (front <= back) {
      L = Q[front].l; R = Q[front].r;
      if (Q[front].parity == EVEN) {  /* OR-based binary search */
         /* A[k] stores the leftmost solution */
         A[k] = binsearch(L,R,OR);

         /* To the right of A[k], there exist an odd (in particular non-zero)
            number of solutions */
         ++back; Q[back].l = A[k]+1; Q[back].r = R; Q[back].parity = ODD;  /* Enqueue */

         ++k;
      } else {                        /* XOR-based binary search */
         /* Find one solution in A[k] */
         A[k] = binsearch(L,R,XOR);

         /* To the left of A[k], there are k1 solutions, and to the right
            of A[k] there are k2 solutions. Since k1+k2 is even, k1 and k2
            must have the same parity (both even or both odd). The case
            both-even is problematic, because 0 is even. So by making
            single OR range queries, we identify which of the two sides
            of A[k] actually contain(s) solutions. */

         t = EVEN;

         if ((L <= A[k]-1) && (rangequery(L,A[k]-1,OR))) {
            /* At leat one solution to the left of A[k] */
            ++back; Q[back].l = L; Q[back].r = A[k]-1;  /* Enqueue */

            /* An XOR query reveals the parity of the number of solutions */
            t = Q[back].parity = rangequery(L,A[k]-1,XOR);
         }

         if ((t == ODD) || ((A[k]+1 <= R) && (rangequery(A[k]+1,R,OR)))) {
            /* At leat one solution to the right of A[k] */
            ++back; Q[back].l = A[k]+1; Q[back].r = R;  /* Enqueue */

            /* An XOR query is not needed now, since k1,k2 have the same parity */
            Q[back].parity = t;
         }

         ++k;   /* Next write location in the solutions array A[] */
      }

      ++front;  /* Dequeue */
   }

   /* Bubble sort the solutions array */
   for (j=9; j>0; --j) {
      for (i=0; i<j; ++i) {
         if (A[i] > A[i+1]) {
            t = A[i]; A[i] = A[i+1]; A[i+1] = t;
         }
      }
   }
}

int main ( )
{
   int A[10];

   registerme();
   findpos(A,65536);
   verifysoln(A);

   exit(0);
}
