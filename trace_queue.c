#include <stdint.h>
#include <stdio.h>
#include "dis.h"


#define MAX_TRACE_QUEUE 65536
static int trace_queue_count = 0;
static addr_t trace_queue_val [MAX_TRACE_QUEUE];


void init_trace_queue (void)
{
  trace_queue_count = 0;
}


int trace_queue_empty (void)
{
  return (trace_queue_count == 0);
}


void push_trace_queue (addr_t addr)
{
  if (trace_queue_count >= MAX_TRACE_QUEUE)
    crash ("trace queue overflow");
  trace_queue_val [trace_queue_count++] = addr;
}


addr_t pop_trace_queue (void)
{
  if (trace_queue_count == 0)
    crash ("trace queue empty");
  return (trace_queue_val [--trace_queue_count]);
}


