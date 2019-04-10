#include <stdio.h>
#include <immintrin.h>
#include <nmmintrin.h>
#include <sys/time.h>
#include <unistd.h>

#define attr_num 24

__m256i mask_256 = _mm256_set_epi32(1<<31, 1<<31, 1<<31, 1<<31, 1<<31, 1<<31, 1<<31, 1<<31);
double add_baseline(const int *a, const int *b){
	int c[attr_num] = {0};
	for (int i  = 0; i < attr_num; i++){
		c[i] = a[i] + b[i];
	}
	return c[attr_num - 1]/1000000.0;
}

double addmul_baseline(const int *a, const int *b){
	int c[attr_num] = {0};
	for (int i  = 0; i < attr_num; i++){
		c[i] = a[i] * i + b[i];
	}
	return c[attr_num - 1]/1000000.0;
}

double add_avx(const int *a, const int *b){
	__attribute__ ((aligned (32))) int c[attr_num] = {0};
	for (int i = 0; i < attr_num / 8; i ++){
		__m256i x = _mm256_maskload_epi32(a + 8 * i, mask_256);
		__m256i y = _mm256_maskload_epi32(b + 8 * i, mask_256);
		__m256i ret = _mm256_add_epi32(x, y);
		_mm256_maskstore_epi32(c + 8 * i, mask_256, ret);
	}
	for (int i = attr_num - attr_num % 8; i < attr_num; i ++){
		c[i] = a[i] + b[i];
	} 
	return c[attr_num -1]/1000000.0;
}

double addmul_avx(const int *a, const int *b){
	__attribute__ ((aligned (32))) int c[attr_num] = {0};
	for (int i = 0; i < attr_num / 8; i ++){
		__m256i x = _mm256_maskload_epi32(a + 8 *i, mask_256);
		__m256i y = _mm256_set_epi32(7+i*8, 6+i*8, 5+i*8, 4+i*8, 3+i*8, 2+i*8, 1+i*8, i*8);
		__m256i z = _mm256_mullo_epi32(x, y); 
		__m256i t = _mm256_maskload_epi32(b + 8 *i, mask_256);
		__m256i ret =  _mm256_add_epi32(z, t);
		_mm256_maskstore_epi32(c + 8 * i, mask_256, ret);
	}
	for (int i = attr_num - attr_num % 8; i < attr_num; i ++){
		c[i] = a[i] * i + b[i];
	} 
	return c[attr_num -1]/1000000.0;
}

int main()
{
	__attribute__ ((aligned (32))) int a[attr_num] = {0};
	__attribute__ ((aligned (32))) int b[attr_num] = {0};
	double base_time = 0;
	double used_time = 0;

	for (int i = 0 ; i < attr_num; i++){
		a[i] = 1000000 / (i + 1);
		b[i] = 2000000 / (i + 3);
	}
	struct timeval start_time, end_time;

	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 1; i ++){
		//add_baseline(a, b);
		printf("add result %lf\n", add_baseline(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	base_time = used_time;
	//printf("add baseline used time: %lf \n", used_time);

	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 1; i ++){
		//add_avx(a, b);
		printf("add_avx result %lf\n", add_avx(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	//printf("add_avx used time: %lf speedup %lf \n", used_time, base_time / used_time);

	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 1; i ++){
		//addmul_baseline(a, b);
		printf("addmul_baseline result %lf\n", addmul_baseline(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	base_time = used_time;
	//printf("addmul baseline used time: %lf \n", used_time);
	
	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 1; i ++){
		//addmul_avx(a, b);
		printf("addmul_avx result %lf\n", addmul_avx(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);

	return 0;
}
