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
On *SSE*, call XMM0-XMM15 (four 32-bit single-precision floating point numbers) while on *AVX*, call YMM0-YMM15, each include 8 32-bit single-precsion or 4 46-bit double-precsision floating point numbers . Both of them have different version of instruction sets.	
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
		+ -mavx
		+ -mavx2
		+ -mfma  
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

+ float-based implementation

+ int-based implementation

## Conclusion
