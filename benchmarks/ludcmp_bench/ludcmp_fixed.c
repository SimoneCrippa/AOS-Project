#include <stdint.h>
#include <math.h>
#include "fixed_op_64bit.h"

#define SHIFT_AMOUNT 30

int64_t a[50][50], b[50], x[50];

int ludcmp( /* int nmax, */ int n, int64_t eps);

static int64_t fab(int64_t n)
{
	int64_t f;

	if (n >= 0)
		f = n;
	else
		f = -n;
	return f;
}

int main(void)
{

	int i, j, chkerr;
  	int n = 5; /*, nmax = 50*/
	int64_t  eps;
  	int64_t w;
	eps = 1074; //1.0 * 10^-6 * 2^30

	for (i = 0; i <= n; i++) {
		w = 0;
		for (j = 0; j <= n; j++) {
			a[i][j] = ((int64_t)(i + j + 2)) << SHIFT_AMOUNT;
			if (i == j)
				a[i][j] = fixed_mul_64(a[i][j],10737418240,SHIFT_AMOUNT);		//10 shifted by SHIFT_AMOUNT
			w += a[i][j];
		}
		b[i] = w;
	}
	chkerr = ludcmp( /* nmax, */ n, eps);
	return 0;

}

int ludcmp( /* int nmax, */ int n, int64_t eps)
{

	int i, j, k;
	int64_t w, y[100];

	if (n > 99 || eps <= 0)
		return (999);
	for (i = 0; i < n; i++) {
		if (fab(a[i][i]) <= eps)
			return (1);
		for (j = i + 1; j <= n; j++) {
			w = a[j][i];
			if (i != 0)
				for (k = 0; k < i; k++)
          			w -= fixed_mul_64(a[j][k],a[k][i],SHIFT_AMOUNT);
				a[j][i] = fixed_div_64(w,a[i][i],SHIFT_AMOUNT);
		}
		for (j = i + 1; j <= n; j++) {
			w = a[i + 1][j];
			for (k = 0; k <= i; k++)
				w -= fixed_mul_64(a[i + 1][k],a[k][j],SHIFT_AMOUNT);
			a[i + 1][j] = w;
		}
	}
	y[0] = b[0];
	for (i = 1; i <= n; i++) {
		w = b[i];
		for (j = 0; j < i; j++)
			w -= fixed_mul_64(a[i][j],y[j],SHIFT_AMOUNT);
		y[i] = w;
	}
	x[n] = fixed_div_64(y[n],a[n][n],SHIFT_AMOUNT);
	for (i = n - 1; i >= 0; i--) {
		w = y[i];
		for (j = i + 1; j <= n; j++)
			w -= fixed_mul_64(a[i][j],x[j],SHIFT_AMOUNT);
		x[i] = fixed_div_64(w,a[i][i],SHIFT_AMOUNT);
	}
	return (0);

}
