/* Program for generating random instances of SAT problems.   */
/* This program is written by Bart Selman. It is property     */
/* of AT&T. For research use only.                            */
/*     Copyright 1992 by AT&T Bell Laboratories               */
/*     All Rights Reserved                                    */
/* AT&T disclaims all warranties with regard to this software,    */
/* including all implied warranties of merchantability and        */
/* fitness.  In no event shall AT&T be liable for any special,    */
/* indirect or consequential damages or any damages whatsoever    */
/* resulting from loss of use, data or profits, whether in an     */
/* action of contract, negligence or other tortious action,       */
/* arising out of or in connection with the use or performance of */
/* this software.                                                 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_CLEN 1000
#define CNF_style 0
#define F_style 1
#define KF_style 2

int nvars, nclauses;
int clen;
int style;
int seed;


#define random lrand48
#define srandom srand48

long int random();         /*used functions                                 */
void srandom(long int);

int 
newseed()
{
  struct timeval tv;
  struct timezone tzp;
  int seed;
  unsigned sec, usec;

  gettimeofday(&tv,&tzp);
  usec = (int)tv.tv_usec;
  sec = (int)tv.tv_sec;

  seed =  (((sec&3)<<20) | (usec<<22) | usec) & 0x7FFFFFFF;

  return seed;
}

void
help()
{
    fprintf(stderr, "Bad arguments\n");
    fprintf(stderr, "Use: makecnf [-seed N] [-cnf] [-f] [-kf] clen nvars nclauses\n");
    exit(-1);
}


int
flip()
{
    if (random() % 1000 < 500) return 0;
    return 1;
}

scanone(int argc, char *argv[], int i, int *varptr)
{
    if (i >= argc) help();
    if (sscanf(argv[i], " %d", varptr)!=1) help();
}

void
generate_wff()
{
    int i, j, k;
    int cl[MAX_CLEN];
    int lit;
    int dup;

    if (style == CNF_style){
	printf("c seed= %d\np cnf %d %d\n", seed, nvars, nclauses); 
    }
    else if (style == KF_style){
	printf("%d %d\n", nvars, nclauses);
    }

    for (i=1; i<=nclauses; i++){
	for (j=0; j<clen; j++){
	    do {
		lit = random()%nvars + 1;
		dup = FALSE;
		for (k=0; k<j; k++)
		  if (lit == cl[k]) dup = TRUE;
	    } while(dup);
	    cl[j] = lit;
	}
	for (j=0; j<clen; j++)
	  if (flip()) cl[j] *= -1;
	
	if (style == CNF_style){
	    for (j=0; j<clen; j++)
	      printf("%d ", cl[j]);
	    printf("0\n");
	}
	else if (style == F_style){
	    printf("%d", cl[0]);
	    for (j=1; j<clen; j++)
	      printf(" %d", cl[j]);
	    printf("\n");
	}
	else if (style == KF_style){
	    printf("%d", j);
	    for (j=0; j<clen; j++)
	      printf(" %d", cl[j]);
	    printf("\n");
	}
    }

    if (style == F_style){
	printf("%%\n0\n");
    }
}


int 
main(int argc, char *argv[])
{
    int i;
    static int * argptrs[] = { &clen, &nvars, &nclauses };
    int currarg;

    seed = newseed();
    currarg = 0;
    style = F_style;

    for (i=1;i < argc;i++){
	if (strcmp(argv[i],"-seed") == 0)
	  scanone(argc, argv, ++i, &seed);
	else if (strcmp(argv[i],"-cnf") == 0)
	  style = CNF_style;
	else if (strcmp(argv[i],"-f") == 0)
	  style = F_style;
	else if (strcmp(argv[i],"-kf") == 0)
	  style = KF_style;
	else if (currarg>=3 || sscanf(argv[i], " %d", argptrs[currarg++])!=1)
	  help();
    }
    if (currarg!=3) help();

    /* fprintf(stderr, "    makewff: seed is %d\n", seed); */

    srandom(seed);

    generate_wff();

    return 0;
}
