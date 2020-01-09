#include <sched.h>
#include <iostream>
#include <thread>

class NumberOfCPUs {
 public:
  // number of cpu cores.
  // hard coded here to avoid API calls for fast bit operation
  static int value;
};

static inline __cpu_mask BITMAP_LAST_WORD_MASK(size_t nbits) {
  return (~0UL >> (-(nbits) & (__NCPUBITS - 1)));
}
bool IsSubset(cpu_set_t const &set1, cpu_set_t const &set2, size_t const bits) {
  const __cpu_mask *bitmap1 = set1.__bits;
  const __cpu_mask *bitmap2 = set2.__bits;
  unsigned int k, lim = bits / __NCPUBITS;
  for (k = 0; k < lim; ++k)
    if (bitmap1[k] & ~bitmap2[k]) return 0;

  if (bits % __NCPUBITS)
    if ((bitmap1[k] & ~bitmap2[k]) & BITMAP_LAST_WORD_MASK(bits)) return 0;
  return 1;
}

int NumberOfCPUs::value = std::thread::hardware_concurrency();
int main() {
  cpu_set_t myset;
  CPU_ZERO_S(NumberOfCPUs::value, &myset);
  CPU_SET(NumberOfCPUs::value - 1, &myset);
  sched_setaffinity(0, sizeof(cpu_set_t), &myset);

  cpu_set_t set1;
  cpu_set_t set2;
  CPU_ZERO_S(96, &set1);
  CPU_ZERO_S(96, &set2);
  for (int cpu = 0; cpu < NumberOfCPUs::value; cpu++) {
    CPU_SET(cpu, &set1);
    if (cpu % 2) CPU_SET(cpu, &set2);
  }

  std::cout << "logical core: " << NumberOfCPUs::value << std::endl;
  auto ret = IsSubset(set1, set2, 96);
  std::cout << ret << std::endl;
  ret = IsSubset(set2, set1, 96);
  std::cout << ret << std::endl;
  std::cout << "running on " << sched_getcpu() << std::endl;
  return 0;
}
