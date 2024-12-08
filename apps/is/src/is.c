/* This version has the last one who updated shared_keyden
   to zero out it. */
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <jia.h>
#include <unistd.h>

double aint(double x)
{
  if (x > 0.0)
    return floor(x);
  else
    return ceil(x);
}


/*
 ****** Parameters of the program 
 */
#define A       1220703125.0
#define S       314159265.0

#define MAXPROC 8

unsigned long N;
/* Determine problem size */
#ifdef  LARGE
/*#define N       (1 << 23) */
#define MAXKEY  (1 << 15)
#define NUMREPS 10
#endif

#ifdef  MEDIUM
/*#define N       (1 << 23)*/
#define MAXKEY  (1 << 10)
#define NUMREPS 10
#endif

#ifdef  SAMPLE
/*#define N       (1 << 23)*/
#define MAXKEY  (1 << 7)
#define NUMREPS 10
#endif

#define m       MAXKEY/4

/*
 ****** Function prototype
 */
void    init_rand();
double  randlc(double *x, double a);
void    bucksort(int key[], int rank[], int shared_keyden[]);
void    partsmall(int rank[], int iter);
void    partbig(int rank[], int iter);
void    main (int argc, char **argv);

/*
 ****** Global variables
 */
double  half23, half46, two23, two46;

int     local_keyden[MAXKEY];
int     start, finish, count;
int    *shared_keyden;

struct  shared {
  int key;
  int rank;
  int misplaced;
  int first[MAXPROC];
  int last[MAXPROC];
} *global;
	    

void    init_rand()
{
  int i;

  half23 = half46 = two23 = two46 = 1.0;
  for (i = 23; i > 0; i--)
    half23 *= 0.5, half46 *= 0.5 * 0.5, two23 *= 2.0, two46 *= 2.0 * 2.0;
}  


double  randlc(double *x, double a)
{
  double a1, a2, b1, b2, t1, t2, t3, t4, t5;

  a1 = aint(half23 * a);
  a2 = a - two23 * a1;
  b1 = aint(half23 * *x);
  b2 = *x - two23 * b1;
  t1 = a1 * b2 + a2 * b1;
  t2 = aint(half23 * t1); 
  t3 = t1 - two23 * t2;
  t4 = two23 * t3 + a2 * b2;
  t5 = aint(half46 * t4);
  *x = t4 - two46 * t5;

  return (half46 * *x);
}


void  bucksort(int key[], int rank[], int shared_keyden[])
{
  int i;

  bzero(local_keyden, MAXKEY * sizeof(int));

  for (i = 0; i < count; i++)
    {
      local_keyden[key[i]]++;
    }

  for (i = 1; i < MAXKEY; i++)
    local_keyden[i] += local_keyden[i-1];
   if (jiapid == 0) bzero(shared_keyden, MAXKEY * sizeof(int));

  if (jiahosts > 1)
    {
	jia_barrier();
      jia_lock(0);
      for (i=0; i < MAXKEY; i++)
	shared_keyden[i] += local_keyden[i];
      /* copy intermediate shared_keyden into local keyden */
      memcpy(local_keyden, shared_keyden, MAXKEY * sizeof(int));


      jia_unlock(0);
  
      jia_barrier();

      for (i = MAXKEY-1; i > 0; i--)
	local_keyden[i] += shared_keyden[i-1] - local_keyden[i-1];
    }

  for (i = 0; i < count; i++) 
    {
      rank[i] = --local_keyden[key[i]];
    }
    if (jiahosts > 1) jia_barrier();
}


void    partsmall(int rank[], int i)
{
  if (start <= 48427 && 48427 < finish)
    {
      if (rank[48427-start] != (0+i))
	fprintf(stderr, "FAILED partial verification test\n");
    }
  if (start <= 17148 && 17148 < finish)
    {
      if (rank[17148-start] != (18+i))
	fprintf(stderr, "FAILED partial verification test\n");
    }
  if (start <= 23627 && 23627 < finish)
    {
      if (rank[23627-start] != (346+i))
	fprintf(stderr, "FAILED partial verification test\n");
    }
  if (start <= 62548 && 62548 < finish)
    {
      if (rank[62548-start] != (64917-i))
	fprintf(stderr, "FAILED partial verification test\n");
    }
  if (start <= 4431 && 4431 < finish)
    {
      if (rank[4431-start] != (65463-i))
	fprintf(stderr, "FAILED partial verification test\n");
    }
}


void   partbig(int rank[], int i)
{
  if (rank[2112377] != (104+i))
    fprintf(stderr, "FAILED partial verification test\n");
  else if (rank[662041] != (17523+i))
    fprintf(stderr, "FAILED partial verification test\n");
  else if (rank[5336171] != (123928+i))
    fprintf(stderr, "FAILED partial verification test\n");
  else if (rank[3642833] != (8288932-i))
    fprintf(stderr, "FAILED partial verification test\n");
  else if (rank[4250760] != (8388264-i))
    fprintf(stderr, "FAILED partial verification test\n");
}


void    main (int argc, char **argv)
{
  int            *key, *rank;
  int             i,j;
  double          x, seed;
  struct timeval  time1, time2;
  int             c;
  int            *sorted_key;

  while ((c = getopt(argc, argv, "n:")) != -1)
         switch (c) {
          case 'n':
                  N = 1 << atoi(optarg);
                  break;
          }

  if (!N) N=1<<14;
  
  jia_init(argc, argv);
  /* Initialize shared memory */
      
  shared_keyden = (int *) jia_alloc((MAXKEY) * sizeof(int));
  
  jia_barrier(); 

  /* Find start and finish of each processor's section */
  start = jiapid * (N / jiahosts);
  finish   = (jiapid + 1) * (N / jiahosts);
  if (finish > N) finish = N;
  count = finish - start;

  /* alocate local key array */
  key  = (int *) malloc(3000+count * sizeof(int));
  rank = (int *) malloc(count * sizeof(int));
  printf("key %x, rank %x\n", key, rank);
  
  /* Initialize seed for random number generator */
  init_rand(); seed = S;

  /* Initialize keys with a Gaussian distribution in key densities */
  for (i = 0; i < N; i++) 
    {
      x  = randlc(&seed, A);
      x += randlc(&seed, A);
      x += randlc(&seed, A);
      x += randlc(&seed, A);
      if (i >= start && i < finish)
	key[i-start] = m * x;
    }

  jia_barrier();
  gettimeofday (&time1, NULL);

  for (i=1; i <= NUMREPS; i++) 
    {
      /* Assume that elems i,i+NUMREPS always belong to the processor 0 */
      if (jiapid == 0) 
	{
	  key[i] = i;
	  key[i+NUMREPS] = MAXKEY - i;
	}

      bucksort(key, rank, shared_keyden);

    }

  gettimeofday (&time2, NULL);

  if (jiapid == 0)
    {
      fprintf(stderr, "Performed %d rankings in time: %6.3f\n", NUMREPS,
	      ((time2.tv_sec - time1.tv_sec) * 1.0e6 + 
	       (time2.tv_usec - time1.tv_usec)) * 1.0e-6);
    }

#define FULL_VERIFICATION
/*
  Full verification is horrendously slow and should never be done
  once the correctness of the program has been established.
*/
#ifdef FULL_VERIFICATION

      global = (struct shared *) jia_alloc(sizeof(struct shared));

    /*  bzero(global, sizeof(struct shared));
      TMK_DISTRIBUTE(global);*/

  jia_barrier();

  /* allocate local array to store sorted keys */
  sorted_key = (int *) malloc(count * sizeof(int));

  /* sort the array */
  for (i = 0; i < N; i++)
    {
      /* if I own this index (i), then I assign to global rank and key */
      if (i >= start && i < finish)
	{
	  global->rank = rank[i-start];
	  global->key  = key[i-start];
	}

      jia_barrier();

      /* Now, if I own global rank index, I assign to it global key */
      if (global->rank >= start && global->rank < finish)
	{
	  sorted_key[global->rank-start] = global->key;
	}

      jia_barrier();
    }

  for (i = 0; i < count-1; i++)
    if (sorted_key[i+1] < sorted_key[i])
      {
	fprintf(stderr,"key[%d]%d, key[%d]%d\n", 
		start+i+1, sorted_key[i+1], start+i, sorted_key[i]);
	jia_lock(0);
	global->misplaced++;
	jia_unlock(0);
      }
	
  if (jiapid > 0)
    {
      global->first[jiapid] = sorted_key[0];
    }

  if (jiapid < jiahosts-1)
    {
      global->last[jiapid] = sorted_key[count-1];
    }
      
  jia_barrier();

  if (jiapid == 0)
    {
      for (i = 0; i < jiahosts-1; i++)
	{
	  if (global->last[i] > global->first[i+1])
	    {
                jprintf("failed key\n");
	      fprintf(stderr,"key[%d]%d, key[%d]%d\n", 
		      count*(i+1), global->first[i+1], 
		      count*(i+1)-1, global->last[i]);
	      global->misplaced++;
	    }
	}
      if (global->misplaced == 0)
	fprintf(stderr, "PASSED:   0 out of place.\n");
      else
	fprintf(stderr, "FAILED:  %d out of place.\n",global->misplaced);
    }

#endif
      
  jia_exit ();
}
