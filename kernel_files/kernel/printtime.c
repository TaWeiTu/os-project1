#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE3(printtime, pid_t, pid, struct timespec64 __user *, start, struct timespec64 __user *, end)
{
  printk("[Project 1] %d %lld.%09lld %lld.%09lld\n", (int)pid, (long long)start->tv_sec, (long long)start->tv_nsec, (long long)end->tv_sec, (long long)end->tv_nsec);
  return 0;
}
