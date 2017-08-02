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

/*begin vlong*/

#define N 100
#define SYS 1000
#define K 3
#define H (K + 1)

typedef struct {
    int val[N];
    int n;
} vlong;

static void _str_reverse(char *s)
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

void vlong_read(const char *s, vlong *res)
{
    int index = 0;
    int i = strlen(s) - 1;
    int j;

    char buf[H];

    while (i >= 0) {
        memset(buf, 0, H * sizeof (char));
        j = 0;
        while (j < K) {
            if (i - j < 0) {
                break;
            }
            buf[j] = s[i - j];
            ++j;
        }
        i -= j;

        _str_reverse(buf);
        res->val[index] = atoi(buf);

        ++index;
    }

    res->n = index;
}

void vlong_add(const vlong *a, const vlong *b, vlong *r)
{
    int mem = 0;
    int nmax = Q_MAX(a->n, b->n);
    int i;

    for (i = 0; i < nmax; ++i) {
        int y = a->val[i] + b->val[i] + mem;
        int rem = y % SYS;
        mem = (y - mem) / SYS;
        r->val[i] = rem;
    }

    if (mem > 0) {
        r->val[i++] = mem;
    }

    r->n = i;
}

static void _str_prepend(char *s, const char *t)
{
    int len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    strncpy(s, t, len);
}

void vlong_str(const vlong *v, char *res)
{
    char s[H], t[H];
    int i, j, len;

    for (i = 0; i < v->n; ++i) {
        memset(s, 0, H * sizeof (char));
        sprintf(s, "%i", v->val[i]);

        if (i != v->n - 1) {
            len = strlen(s);
            if (len < K) {
                memset(t, 0, H * sizeof (char));

                for (j = 0; j < K - len; ++j) {
                    t[j] = '0';
                }

                strcat(t, s);
                strcpy(s, t);
            }
        }

        _str_prepend(res, s);
    }
}

/*end vlong*/

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