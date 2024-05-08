#include <stdio.h>
#include <x86intrin.h>
#include <sys/time.h>

#define N 0xfffffff
#define SEED 0x1234

float *a, *b, *c;

#if defined(USE_AVX)
void gen_data() {
  srand(SEED);
  a = (float*) _mm_malloc(N * sizeof(float), 32);
  b = (float*) _mm_malloc(N * sizeof(float), 32);
  c = (float*) _mm_malloc(N * sizeof(float), 32);
  for(int i = 0; i < N; i++) {
    a[i] = b[i] = (float)(rand() % N);
  }
}

void free_data(void) {
  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}

void multiply(void) {
  printf("AVX multiply...\n");
  __m256 A, B, C;
  int i = 0;
  for(; i < (N & ((~(unsigned)0x7))); i += 8) {
    A = _mm256_load_ps(&a[i]);
    B = _mm256_load_ps(&b[i]);
    C = _mm256_mul_ps(A, B);
    _mm256_store_ps(&c[i], C);
  }
  for(; i < N; i++) {
    c[i] = a[i] * b[i];
  }
}
#elif defined(USE_SSE)
void gen_data() {
  srand(SEED);
  a = (float*) _mm_malloc(N * sizeof(float), 16);
  b = (float*) _mm_malloc(N * sizeof(float), 16);
  c = (float*) _mm_malloc(N * sizeof(float), 16);
  for(int i = 0; i < N; i++) {
    a[i] = b[i] = (float)(rand() % N);
  }
}

void multiply() {
  printf("SSE multiply...\n");
  __m128 A, B, C;
  int i = 0;
  for(; i < (N & ((~(unsigned)0x3))); i += 4) {
    A = _mm_load_ps(&a[i]);
    B = _mm_load_ps(&b[i]);
    C = _mm_mul_ps(A, B);
    _mm_store_ps(&c[i], C);
  }
  for(; i < N; i++) {
    c[i] = a[i] * b[i];
  }
}

void free_data() {
  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}
#else
void gen_data() {
  srand(SEED);
  a = (float*) malloc(N * sizeof(float));
  b = (float*) malloc(N * sizeof(float));
  c = (float*) malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    a[i] = b[i] = (float)(rand() % N);
  }
}

void multiply() {
  printf("Normal multiply...\n");
  for (int i = 0; i < N; i++) {
    c[i] = a[i] * b[i];
  }
}

void free_data() {
  free(a);
  free(b);
  free(c);
}
#endif

void print_data() {
  printf("%f, %f, %f, %f\n", c[0], c[1], c[N-2], c[N-1]);
}

int main() {
  double start = 0.0, stop = 0.0, msecs;
  gen_data();

  struct timeval before, after;
  gettimeofday(&before, NULL);
  multiply();
  gettimeofday(&after, NULL);

  msecs = (after.tv_sec - before.tv_sec) * 1000.0 + (after.tv_usec - before.tv_usec) / 1000.0;
  print_data();
  printf("Execution time = %2.3lf ms\n", msecs);

  free_data();
  return 0;
}
