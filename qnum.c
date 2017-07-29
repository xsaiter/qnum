#define _XOPEN_SOURCE

#include "postgres.h"

#include <ctype.h>
#include <string.h>
#include <stddef.h>

#include "fmgr.h"

#include "utils/builtins.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

int is_prime(long a)
{
    if (a < 2) {
        return 0;
    }

    for (long i = 2; i * i <= a; ++i) {
        if (a % i == 0) {
            return 0;
        }
    }
    return 1;
}

long gcd(long a, long b) {
  while (b != 0) {
    int c = a % b;
    a = b;
    b = c;
  }
  return (a > 0) ? a : -a;
}

int is_relatively_prime(long a, long b) { return gcd(a, b) == 1; }

PG_FUNCTION_INFO_V1(q_num_is_prime);
PG_FUNCTION_INFO_V1(q_num_gcd);
PG_FUNCTION_INFO_V1(q_num_is_relatively_prime);

Datum
q_num_is_prime(PG_FUNCTION_ARGS)
{
    long n = PG_GETARG_INT64(0);
    int res = is_prime(n);
    PG_RETURN_BOOL(res);
}

Datum
q_num_gcd(PG_FUNCTION_ARGS)
{
    long a = PG_GETARG_INT64(0);
    long b = PG_GETARG_INT64(1);
    int res = gcd(a, b);
    PG_RETURN_INT64(res);
}

Datum
q_num_is_relatively_prime(PG_FUNCTION_ARGS)
{
    long a = PG_GETARG_INT64(0);
    long b = PG_GETARG_INT64(1);
    int res = is_relatively_prime(a, b);
    PG_RETURN_BOOL(res);
}