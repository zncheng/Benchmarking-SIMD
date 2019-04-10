# Benchmarking-SIMD

This is a simple example to show SIMD gains.
I try to use SIMD to improve the performance of the system I am currently working on.
However, I find there are many different instruction sets in my platform. 
As a freshman, I was unable to catch the right one.
I decide to simply benchmark these different instruction sets based on the workload I need so I can have the best choices.

## SIMD basic
You can find the SIMD concept [here](https://en.m.wikipedia.org/wiki/SIMD) and [here](http://www.cs.uu.nl/docs/vakken/magr/2017-2018/files/SIMD%20Tutorial.pdf).
Basically, SIMD means Single Instruction Multiple Data, which means you can use a single instruction to manipulate multiple pairs of data. 
It is suitable for workloads that needs *for* or *while* loop and have no dependency between the target data.
SIMD is hardware related, here, we only focus on Intel CPU.

### Existing instruction sets
Most modern CPU includes SIMD instructions, we can use them in program languages. These instructions operate on registers. In Intel CPU, there are two series instruction sets, namely *SSE* and *AVX*, both have 16 registers.
On *SSE*, call XMM0-XMM15 (four 32-bit single-precision floating point numbers) while on *AVX*, call YMM0-YMM15, each include 8 32-bit single-precsion or 4 64-bit double-precsision floating point numbers . Both of them have different version of instruction sets.	
+ SSE 
	+ single precision vectors
	+ with 60 instructions
	+ release in 1999
+ SSE2
	+ double precision vectors
		+ extend SSE to support 6/16/32/64/128 bit vector integer
	+ 144 instructions
	+ release in 2000
+ SSE3
	+ 32 instruction
	+ support complex data type
	+ release in 2004
+ SSE4.1
	+ support video/graphic building block
	+ release in 2007
+ SSE4.2
	+ support string processing
	+ release in 2008
+ AVX
	+ support 256 bit vector integer
	+ release in 2010
+ AVX2	
	+ introduce fuse multiply-acumulate (FMA) opeations
	+ release in 2011
	+ All CPUs since AMD Carrizo or Intel Haswell support AVX2.
+ AVX-512 series
	+ expand avx to 512-bit support
	+ release in 2013

### Check your CPU support
+ To check what SIMD instruction sets you cpu supports, check the CPU flag
+ by `lscpu` or `cat /proc/cpuinfo`
	+ check whether the flag includes the key works sse, sse2, sse3, sse4_1 sse4_2, avx, avx2, or avx-512 prefix

### Lookup the APIs that can be called
+ For all APIs you can use, check [here](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=288,130,170,2516,5563,4873,3871,3514,91)
+ Understand the APIs
	+ `_mm256_add_epi32(__m256i a, __m256i b)`
		+ means use the 256 bit vector, for addition, where the content in the vector will be intercepted as 32-bit integer.
		+ other APIs have similar meaning, you may check the usage of each APIs in the above link.
### Inside your program
+ head file
	+ SSE series you only needs `#include <nmmintrin.h>`
	+ AVX series you only needs `#include <immintrin.h>`
+ compiler
	+ add the compiler flage
		+ before avx needs not flag
		+ `-mavx` for avx
		+ `-mavx2` for avx2
		+ `-mfma` for fma
## Benchmark
+ in our system, we only care two types of Workload
	+ array add: simple addition of each elements between two array
		+ that is `a[i] + b[i]`
	+ arrary multiply and add, muliplication of each elements between two array, then do the addition between each elements between the multiplied results and another arrary
		+ that is `a[i]*b[i] + c[i]`
+ Source code
	+ *test_double.c* implements the baseline and SIMD version based on *double* type
	+ *test_float.c* implements the baseline and SIMD version based on *float* type
	+ *test_int.c* implements the baseline and SIMD version based on *integer* type
+ inside each cpp file
	+ `add_baseline()`
		+ normal processing for array add
	+ `add_avx()`
		+ using avx instructions for array add
	+ `addmul_baseline()`
		+ normal processing for arrary multiply and add
	+ `addmul_avx()`
		+ using avx instructions for array multiply and array add
	+ `addmul_fma()`
		+ using fma instructions for arrary multiply and array add
## Report
We repeat each function 10000000 times, vary the array size, and report the performance result.
+ double-based implementation
	+ array size = 10
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |2.83s	|1.0  |
		  |add_avx	|2.22s  |1.27 |
		  |addmul_base	|3.10s	|1.0  |	
		  |addmul_avx	|3.09s	|1.004|
		  |addmul_fma	|2.84s	|1.09 |
	+ array size = 24
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |7.13s	|1.0  |
		  |add_avx	|4.55s  |1.56 |
		  |addmul_base	|6.97s	|1.0  |	
		  |addmul_avx	|7.07s	|0.98 |
		  |addmul_fma	|6.20s	|1.12 |
	+ array size = 100
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |24.9s	|1.0  |
		  |add_avx	|13.6s  |1.82 |
		  |addmul_base	|25.4s	|1.0  |	
		  |addmul_avx	|23.2s	|1.1  |
		  |addmul_fma	|19.9s	|1.27 |
+ float-based implementation
	+ array size = 10
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |2.37s	|1.0  |
		  |add_avx	|1.27s  |1.86 |
		  |addmul_base	|2.87s	|1.0  |	
		  |addmul_avx	|2.02s	|1.42 |
		  |addmul_fma	|1.87s	|1.53 |
	+ array size = 24
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |7.07s	|1.0  |
		  |add_avx	|2.23s  |3.18 |
		  |addmul_base	|7.69s	|1.0  |	
		  |addmul_avx	|4.64s	|1.65 |
		  |addmul_fma	|4.32s	|1.78 |
	+ array size = 100
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |24.8s	|1.0  |
		  |add_avx	|7.98s  |3.11 |
		  |addmul_base	|26.3s	|1.0  |	
		  |addmul_avx	|17.6s	|1.49 |
		  |addmul_fma	|15.9s	|1.64 |

+ int-based implementation
	+ array size = 10
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |2.42s	|1.0  |
		  |add_avx	|1.36s  |1.77 |
		  |addmul_base	|2.67s	|1.0  |	
		  |addmul_avx	|1.87s	|1.42 |
	+ array size = 24
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |6.96s	|1.0  |
		  |add_avx	|2.83s  |2.45 |
		  |addmul_base	|7.02s	|1.0  |	
		  |addmul_avx	|4.48s	|1.57 |
	+ array size = 100
		+ | methods     | used time | speedup|
		  |  ---        |    ---    |  ---   |
		  |add_base     |24.7s	|1.0  |
		  |add_avx	|9.21s  |1.69 |
		  |addmul_base	|24.8s	|1.0  |	
		  |addmul_avx	|15.24s	|1.62 |

## Conclusion
Based on result, we draw the following conclusions.
+ float-based and inter-based implementatin achieve almost same speedup, which means the speedup actually depends on number of vector bits
+ double-based implementation improves limited performances, by theorey we can improve up to 4x, while in practice only 10-20%
+ FMA indeed outperform avx2 for fuse operations
+ float-based is the best choice
	+ improve 2.18x in vector addtion
	+ improve 78% using FMA for mul_and_add operations
