#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "fixed_op_64bit.h"

#define SHIFT_AMOUNT 30
#define EXEC_NUM 100000

int minver(int row, int col, int64_t eps);
int mmul(int  row_a, int col_a, int row_b, int col_b);

static int64_t a[3][3] = {
  {3221225472, -6442450944, 7516192768},
  {9663676416, 0, -5368709120},
  {5368709120, -8589934592, 6442450944},
};
int64_t  b[3][3], c[3][3], aa[3][3], a_i[3][3], e[3][3], det;


int64_t minver_fabs(int64_t n)
{
	int64_t f;

	if (n >= 0) f = n;
  	else f = -n;
  	return f;
}

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

int main()
{
	struct timespec start0,end0,start1,end1;
  	FILE * fp;
  	fp = fopen ("minver_fixed_results.txt","w");

	for (int k=0; k< EXEC_NUM ; k++){
		int i, j;
		int64_t eps;

		eps = 1074; //1.0e-6*2^30

		for(i = 0; i < 3; i++)
	  		for(j = 0; j < 3; j++)
	    		aa[i][j] = a[i][j];

		clock_gettime(CLOCK_MONOTONIC_RAW, &start0);
		minver(3, 3, eps);
		clock_gettime(CLOCK_MONOTONIC_RAW, &end0);

		for(i = 0; i < 3; i++)
	  		for(j = 0; j < 3; j++)
	    		a_i[i][j] = a[i][j];

		clock_gettime(CLOCK_MONOTONIC_RAW, &start1);
		mmul(3, 3, 3, 3);
		clock_gettime(CLOCK_MONOTONIC_RAW, &end1);

    	fprintf (fp, "%lld\n",(long long)(diff(start0,end0).tv_sec * pow(10,9))+(long long)diff(start0,end0).tv_nsec
    	+ (long long)(diff(start1,end1).tv_sec * pow(10,9))+(long long)diff(start1,end1).tv_nsec);
	}

	fclose (fp);
  	return 0;
}

int  mmul(int row_a, int col_a, int row_b, int col_b)
{
	int i, j, k, row_c, col_c;
	int64_t w;

	row_c = row_a;
	col_c = col_b;

	if(row_c < 1 || row_b < 1 || col_c < 1 || col_a != row_b) return(999);
	for(i = 0; i < row_c; i++)
	{
		for(j = 0; j < col_c; j++)
		{
			w = 0;
	       	for(k = 0; k < row_b; k++)
		       	w += fixed_mul_64(a[i][k],b[k][j],SHIFT_AMOUNT);
	       	c[i][j] = w;
		}
	}
	return(0);

}


int minver(int row, int col, int64_t eps)
{

	int work[500], i, j, k, r, iw, s, t, u, v;
	int64_t w, wmax, pivot, api, w1;

	if(row < 2 || row > 500 || eps <= 0) return(999);
	w1 = 1073741824; //1.0*2^30
	for(i = 0; i < row; i++)
	  	work[i] = i;
	for(k = 0; k < row; k++)
	{
		wmax = 0;
	    for(i = k; i < row; i++)
		{
			w = minver_fabs(a[i][k]);
			if(w > wmax)
		  	{
		   		wmax = w;
		    	r = i;
		  	}
		}
	    pivot = a[r][k];
	    api = minver_fabs(pivot);
	    if(api <= eps)
		{
			det = w1;
			return(1);
		}
	    w1 = fixed_mul_64(w1,pivot,SHIFT_AMOUNT);
	    u = k * col;
	    v = r * col;
	    if(r != k)
		{
			w1 = -w;
			iw = work[k];
			work[k] = work[r];
			work[r] = iw;
			for(j = 0; j < row; j++)
		  	{
		   		s = u + j;
		    	t = v + j;
		    	w = a[k][j];
		    	a[k][j] = a[r][j];
		    	a[r][j] = w;
		  	}
		}
	    for(i = 0; i < row; i++)
	      	a[k][i] = fixed_div_64(a[k][i],pivot,SHIFT_AMOUNT);
	    for(i = 0; i < row; i++)
		{
			if(i != k)
		  	{
		    	v = i * col;
		    	s = v + k;
		    	w = a[i][k];
		    	if(w != 0)
		      	{
					for(j = 0; j < row; j++)
			  		if(j != k) a[i][j] -= fixed_mul_64(w,a[k][j],SHIFT_AMOUNT);
					a[i][k] = fixed_div_64(-w,pivot,SHIFT_AMOUNT);
		      	}
		  	}
		}
		a[k][k] = fixed_div_64(1073741824,pivot,SHIFT_AMOUNT); //1.0*2^30
	}
	for(i = 0; i < row; i++)
	{
		while(1)
		{
			k = work[i];
			if(k == i) break;
			iw = work[k];
			work[k] = work[i];
			work[i] = iw;
			for(j = 0; j < row; j++)
			{
		    	u = j * col;
		    	s = u + i;
		    	t = u + k;
		    	w = a[k][i];
		    	a[k][i] = a[k][k];
		    	a[k][k] = w;
		  	}
		}
	}
	det = w1;
	return(0);

}
