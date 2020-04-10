#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(gettime, struct timespec64 __user *, ts)
{
  ktime_get_ts64(ts);
  return 0;
}
