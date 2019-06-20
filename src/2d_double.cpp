#include <stdio.h>
#include <immintrin.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define attr_num 100

__attribute__ ((aligned (32))) double a[attr_num][attr_num] = {0.0};
__attribute__ ((aligned (32))) double b[attr_num][attr_num] = {0.0};

double matrix_add_baseline() {

	double c[attr_num][attr_num] = {0.0};
    for (int i = 0; i < attr_num; i++){
	    for (int j = 0; j < attr_num; j++){
		    c[i][j] = a[i][j] + b[i][j];
        }
	}

    for ( int i = 90; i < attr_num; i++){
        for (int j = 90; j < attr_num; j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    //cout << a[1][1] << " " << b[1][1] << " " << c[1][1] << endl;

	return c[1][1];
}

double matrix_add_avx() {

	__attribute__ ((aligned (32))) double c[attr_num][attr_num] = {0.0};
    for (int i = 0; i < attr_num; i++) {
	    for (int j = 0; j < attr_num / 4; j ++) {
	    	__m256d x = _mm256_load_pd(a[i] + 4 * j);
	    	__m256d y = _mm256_load_pd(b[i] + 4 * j);
	    	__m256d ret = _mm256_add_pd(x, y);
	    	_mm256_store_pd(c[i] + 4 * j, ret);
        }
	}

    for (int i = 0; i < attr_num; i ++) {
	    for (int j = attr_num - attr_num % 4; j < attr_num; j ++){
		    c[i][j] = a[i][j] + b[i][j];
        }
	}
    for ( int i = 90; i < attr_num; i++){
        for (int j = 90; j < attr_num; j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    //cout << a[1][1] << " " << b[1][1] << " " << c[1][1] << endl;
	return c[1][1];
}

int main()
{
	double base_time = 0;
	double used_time = 0;

    for (int i = 0 ; i < attr_num; i++) { 
	    for (int j = 0 ; j < attr_num; j++){
		    a[i][j] = i * 1.0 / (j + 1.0);
		    b[i][j] = i * 2.0 / (j + 2.0);
        }
	}
	struct timeval start_time, end_time;

    //printf("add base is %lf and avx base is %lf\n", matrix_add_baseline(), matrix_add_avx());

    matrix_add_baseline();
    matrix_add_avx();

	//gettimeofday(&start_time, NULL); 
	//for (int i = 1; i <= 100000000; i ++){
	//	matrix_add_baseline(a, b);
	//	printf("add result %lf\n", add_baseline(a, b));
	//}
	//gettimeofday(&end_time, NULL);
	//used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	//base_time = used_time;
	//printf("add baseline used time: %lf \n", used_time);

	//gettimeofday(&start_time, NULL); 
	//for (int i = 1; i <= 100000000; i ++){
	//	matrix_add_avx(a, b);
	//	//printf("add_avx result %lf\n", add_avx(a, b));
	//}
	//gettimeofday(&end_time, NULL);
	//used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	//printf("add_avx used time: %lf speedup %lf \n", used_time, base_time / used_time);
	
    return 0;
}
