#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

static volatile uint32 *rtc_low  = (uint32*)RTC_LOW;
static volatile uint32 *rtc_high = (uint32*)RTC_HIGH;

uint64
read_rtc(void) {
  uint32 hi, lo;

  lo = *rtc_low;
  hi = *rtc_high;

  return ((uint64)hi << 32) | lo;
}