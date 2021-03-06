#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;
#define M 7
#define NSTACK 50
#define SHIFT_AMOUNT 14
#define EXEC_NUM 100000

int32_t arr[20];

void fillarray()
{
	for(int i = 0; i < 20; i++)
		arr[i] = (int32_t)(((rand() % 100000) / 100) * pow(2,SHIFT_AMOUNT));
}

int istack[100];

void sort(unsigned long n)
{
	unsigned long i,ir=n,j,k,l=1;
	int jstack=0;
	int flag;
	int32_t a,temp;

	flag = 0;
	while (1) {
		if (ir-l < M) {
			for (j=l+1;j<=ir;j++) {
				a=arr[j];
				for (i=j-1;i>=l;i--) {
					if (arr[i] <= a) break;
					arr[i+1]=arr[i];
				}
				arr[i+1]=a;
			}
			if (jstack == 0) break;
			ir=istack[jstack--];
			l=istack[jstack--];
		} else {
			k=(l+ir) >> 1;
			SWAP(arr[k],arr[l+1])
			if (arr[l] > arr[ir]) {
				SWAP(arr[l],arr[ir])
			}
			if (arr[l+1] > arr[ir]) {
				SWAP(arr[l+1],arr[ir])
			}
			if (arr[l] > arr[l+1]) {
				SWAP(arr[l],arr[l+1])
			}
			i=l+1;
			j=ir;
			a=arr[l+1];
			for (;;) {
				i++; while (arr[i] < a) i++;
				j--; while (arr[j] > a) j--;
				if (j < i) break;
				SWAP(arr[i],arr[j]);
			}
			arr[l+1]=arr[j];
			arr[j]=a;
			jstack += 2;

			if (ir-i+1 >= j-l) {
				istack[jstack]=ir;
				istack[jstack-1]=i;
				ir=j-1;
			} else {
				istack[jstack]=j-1;
				istack[jstack-1]=l;
				l=i;
			}
		}
	}
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
	struct timespec start,end;
	FILE * fp;
	fp = fopen ("qsort_exam_fixed_results.txt","w");
	srand(5);
	for (int i=0; i< EXEC_NUM ; i++)
  	{
    		fillarray();
    		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    		sort(19);
    		clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    		fprintf (fp, "%lld\n",(long long)(diff(start,end).tv_sec * pow(10,9))+(long long)diff(start,end).tv_nsec);
  	}
  	fclose(fp);
  	return 0;
}
