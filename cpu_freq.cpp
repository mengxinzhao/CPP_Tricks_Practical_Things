//#include <cpuid.h>
#include <fcntl.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#define MSR_IA32_MPERF 0x000000e7
#define MSR_IA32_APERF 0x000000e8

// The CPU can and will speculatively execute that RDTSC, though, so the
// results can be non-monotonic if compared on different CPUs.
//
static inline uint64_t rdtsc_ordered() {
  //
  // Use lfence to prevent execution of later instructions until it retires. e.g. to
  // stop _rdtsc() from reading the cycle-counter while earlier work is still pending in
  // a microbenchmark. Code reference:
  // https://elixir.bootlin.com/linux/v4.15.17/source/arch/x86/kernel/tsc.c#L1016
  // Here instead of using raw asm codes, I'm using Intel C/C++ compiler intrinsic
  _mm_lfence();
  return __rdtsc();
}

// rdmsr/wrmsr can't be executed in user space period.
#if 0
static inline uint64_t rdmsr(uint64_t msr)
{
	uint32_t low, high;
	asm volatile (
		"rdmsr"
		: "=a"(low), "=d"(high)
		: "c"(msr)
	);
	return ((uint64_t)high << 32) | low;
}

static inline void wrmsr(uint64_t msr, uint64_t val) {
    uint32_t low= (uint32_t)(val&0xffffffff), high = val>>32;
    asm volatile(
            "wrmsr"
            :
            :"c"(msr), "a"(low), "d"(high));
}
#endif

static uint64_t rdmsr(unsigned int msr, int cpu) {
  int fd;
  uint64_t val;
  char msr_file_name[64];
  sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
  std::cout << msr_file_name << std::endl;
  fd = open(msr_file_name, O_RDONLY);
  if (fd < 0) return -1;
  if (lseek(fd, msr, SEEK_CUR) == -1) goto err;
  if (read(fd, &val, sizeof(val)) != sizeof(val)) goto err;
  close(fd);
  return val;
err:
  close(fd);
  return -1;
}
/*
 * Generic CPUID function
 */
static void __cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
  /* ecx is often an input as well as an output. */
  asm volatile("cpuid"
               : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
               : "0"(*eax), "2"(*ecx)
               : "memory");
}
static void cpuid(uint32_t op, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
  *eax = op;
  *ecx = 0;  // clear ecx
  __cpuid(eax, ebx, ecx, edx);
}

static void cpuid(uint32_t cpuInfo[4], uint32_t function_id) {
  cpuInfo[0] = function_id;
  cpuInfo[2] = 0;  // clear ecx
  __cpuid(&cpuInfo[0], &cpuInfo[1], &cpuInfo[2], &cpuInfo[3]);
}

static void cpuidex(uint32_t cpuInfo[4], uint32_t function_id, uint32_t subfunction_id) {
  cpuInfo[0] = function_id;
  cpuInfo[2] = subfunction_id;
  __cpuid(&cpuInfo[0], &cpuInfo[1], &cpuInfo[2], &cpuInfo[3]);
}

static unsigned long max_cpu_khz_from_cpuid(void) {
  uint32_t eax_base_mhz, ebx_max_mhz, ecx_bus_mhz, edx;

  eax_base_mhz = ebx_max_mhz = ecx_bus_mhz = edx = 0;

  cpuid(0x16, &eax_base_mhz, &ebx_max_mhz, &ecx_bus_mhz, &edx);

  return ebx_max_mhz * 1000;
}

long long timespec_diff_us(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return (temp.tv_sec * 1000000) + (temp.tv_nsec / 1000);
}

int main() {
  std::array<uint32_t, 4> cpuInfo{0, 0, 0, 0};
  std::vector<std::array<uint32_t, 4>> data;
  std::vector<std::array<uint32_t, 4>> extdata;
  cpuid(cpuInfo.data(), 0);
  std::cout << "highest valid function Id: " << cpuInfo[0] << std::endl;
  auto nIds = cpuInfo[0];
  for (int i = 0; i <= nIds; ++i) {
    cpuidex(cpuInfo.data(), i, 0);
    data.push_back(cpuInfo);
  }
  // Capture vendor string
  char vendor[0x20];
  memset((void *)vendor, 0, sizeof(vendor));
  *reinterpret_cast<uint32_t *>(vendor) = data[0][1];
  *reinterpret_cast<uint32_t *>(vendor + 4) = data[0][3];
  *reinterpret_cast<uint32_t *>(vendor + 8) = data[0][2];
  std::cout << vendor << std::endl;

  // feature information Table 3-8
  auto f_1_ECX = data[1][2];
  auto f_1_EDX = data[1][3];
  // EDX[4]: Time Stamp Counter. The RDTSC instruction is supported, including CR4.TSD for
  // controlling privilege
  std::cout << "TSC supported: " << std::boolalpha << bool(f_1_EDX & (0x1 << 4)) << std::endl;
  //  Thermal and Power Management Features. vol2 Table 3-8
  auto f_b_ECX = data[6][2];
  // ECX[0]:Hardware Coordination Feedback Capability (Presence of IA32_MPERF and IA32_APERF). The
  // capability to provide a measure of delivered processor performance (since last reset of the
  // counters), as a percentage of the expected processor performance when running at the TSC
  // frequency.
  //
  // The max frequency mperf is ticking at (in C0), either retrieved via:
  // 1) calculated after measurements if we know TSC ticks at mperf/P0 frequency
  // 2) cpufreq /sys/devices/.../cpu0/cpufreq/cpuinfo_max_freq at init time
  // 1) Is preferred as it also works without cpufreq subsystem
  //
  std::cout << "IA32_MPERF/IA32_APERF supported: " << std::boolalpha << bool(f_b_ECX) << std::endl;

  // extended function
  // Calling cpuid with 0x80000000 as the function_id argument
  // gets the number of the highest valid extended ID.
  cpuid(cpuInfo.data(), 0x80000000);
  auto nExIds = cpuInfo[0];
  std::cout << "extended function number: " << std::hex << nExIds - 0x80000000 << std::endl;

  for (int i = 0x80000000; i <= nExIds; ++i) {
    cpuidex(cpuInfo.data(), i, 0);
    extdata.push_back(cpuInfo);
  }

  auto f_7_EDX = extdata[7][3];
  std::cout << "invariant TSC supported: " << std::boolalpha << bool(f_7_EDX & (0x1 << 8))
            << std::endl;

  // only for display purpose not an actual read of max freq
  auto cpu_khz = max_cpu_khz_from_cpuid();
  std::cout << "max cpu_khz: " << cpu_khz << std::endl;
  uint64_t tsc = 0;
  auto cpu = sched_getcpu();
  std::cout << "running on cpu " << cpu << std::endl;
  uint64_t begin_tick;
  struct timespec start_time, end_time;

  // Mperf register is defined to tick at P0 (maximum) frequency. Use TSC counter if it reliably
  // ticks at P0/mperf frequency*/
  clock_gettime(CLOCK_REALTIME, &start_time);
  begin_tick = rdtsc_ordered();
  uint64_t aperf = rdmsr(MSR_IA32_APERF, cpu);
  uint64_t mperf = rdmsr(MSR_IA32_MPERF, cpu);

  for (int i = 0; i < 1000; i++)
    for (int j = std::numeric_limits<int>::max(); j > 1;) j = (int)sqrt(j);

  uint64_t aaperf = rdmsr(MSR_IA32_APERF, cpu);
  uint64_t mmperf = rdmsr(MSR_IA32_MPERF, cpu);
  uint64_t end_tick = rdtsc_ordered();
  clock_gettime(CLOCK_REALTIME, &end_time);
  auto diff_time = timespec_diff_us(start_time, end_time);
  auto tick_delta = end_tick - begin_tick;
  auto mperf_diff = mmperf - mperf;
  auto aperf_diff = aaperf - aperf;
  std::cout << "mperf_diff : " << mperf_diff << std::endl;
  std::cout << "aperf_diff : " << aperf_diff << std::endl;
  std::cout << "diff in us: " << diff_time << std::endl;
  auto max_frequency = (double)tick_delta / diff_time / 1000.0;
  std::cout << "max_frequency: " << max_frequency << " GHz" << std::endl;
  // running freq includiung freq in turbo
  auto running_freq = max_frequency * ((double)aperf_diff / mperf_diff);
  std::cout << "average running_freq: " << running_freq << " GHz" << std::endl;
  auto percent = mperf_diff * 100.0 / tick_delta;
  std::cout << "C0 percent: " << percent << std::endl;
}
