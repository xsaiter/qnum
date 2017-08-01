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

#define Q_MAX(a,b) ((a) > (b) ? (a) : (b))

#define QN 100
#define QSYS 1000
#define QK 3
#define QH (QK + 1)

static int is_prime(long a)
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

static long gcd(long a, long b)
{
    while (b != 0) {
        int c = a % b;
        a = b;
        b = c;
    }
    return (a > 0) ? a : -a;
}

static int is_relatively_prime(long a, long b)
{
    return gcd(a, b) == 1;
}

typedef struct {
    int val[QN];
    int n;
} vlong;

static void str_reverse(char *s)
{
    int i, j;
    i = 0;
    j = strlen(s) - 1;
    while (i < j) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
        ++i;
        --j;
    }
}

static void vlong_read(const char *s, vlong *r)
{
    int j = 0;
    int i = strlen(s) - 1;

    char buf[QH];

    while (i >= 0) {
        memset(buf, 0, QH * sizeof (char));
        int j = 0;
        while (j < QK) {
            if (i - j < 0) {
                break;
            }
            buf[j] = s[i - j];
            ++j;
        }
        i -= j;

        str_reverse(buf);
        r->val[j] = atoi(buf);

        ++j;
    }

    r->n = j;
}

static void vlong_add(const vlong *a, const vlong *b, vlong *r)
{
    int mem = 0, i, y, rem;
    int nmax = Q_MAX(a->n, b->n);

    for (i = 0; i < nmax; ++i) {
        y = a->val[i] + b->val[i] + mem;
        rem = y % QSYS;
        mem = (y - mem) / QSYS;
        r->val[i] = rem;
    }

    if (mem > 0) {
        r->val[i++] = mem;
    }

    r->n = i;
}

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