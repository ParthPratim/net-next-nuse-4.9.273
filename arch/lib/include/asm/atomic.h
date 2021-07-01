#ifndef _ASM_SIM_ATOMIC_H
#define _ASM_SIM_ATOMIC_H

#include <linux/types.h>
#include <asm-generic/cmpxchg.h>

#if !defined(CONFIG_64BIT)
typedef struct {
	volatile long long counter;
} atomic64_t;
#endif

#define ATOMIC64_INIT(i) { (i) }

#define atomic64_read(v)        (*(volatile long *)&(v)->counter)
static inline void atomic64_add(long i, atomic64_t *v)
{
	v->counter += i;
}
static inline void atomic64_sub(long i, atomic64_t *v)
{
	v->counter -= i;
}
static inline void atomic64_inc(atomic64_t *v)
{
	v->counter++;
}
int atomic64_sub_and_test(long i, atomic64_t *v);
#define atomic64_dec(v)			atomic64_sub(1LL, (v))
int atomic64_dec_and_test(atomic64_t *v);
int atomic64_inc_and_test(atomic64_t *v);
int atomic64_add_negative(long i, atomic64_t *v);
static inline long atomic64_add_return(long i, atomic64_t *v)
{
	v->counter += i;
	return v->counter;
}
static inline void atomic64_set(atomic64_t *v, long i)
{
	v->counter = i;
}
static void atomic64_and(long long i, atomic64_t *v)
{
	v->counter &= i;
}

static void atomic64_or(long i, atomic64_t *v)
{
	v->counter |= i;
}

static void atomic64_xor(long i, atomic64_t *v)
{
	v->counter ^= i;
}

long atomic64_sub_return(long i, atomic64_t *v);
#define atomic64_inc_return(v)  (atomic64_add_return(1, (v)))
#define atomic64_dec_return(v)  (atomic64_sub_return(1, (v)))
static inline long atomic64_cmpxchg(atomic64_t *v, long old, long new)
{
	long long val;

	val = v->counter;
	if (val == old)
		v->counter = new;
	return val;
}
long atomic64_xchg(atomic64_t *v, long new);
int atomic64_add_unless(atomic64_t *v, long a, long u);
int atomic64_inc_is_not_zero(atomic64_t *v);
#define atomic64_inc_not_zero(v) 	atomic64_add_unless((v), 1LL, 0LL)

#define ATOMIC64_OP(op, c_op)						\
static inline void atomic64_##op(long long i, atomic64_t *v)		\
{									\
	long long old, c = 0;						\
	while ((old = atomic64_cmpxchg(v, c, c c_op i)) != c)		\
		c = old;						\
}

#define ATOMIC64_FETCH_OP(op, c_op)					\
static inline long long atomic64_fetch_##op(long long i, atomic64_t *v)	\
{									\
	long long old, c = 0;						\
	while ((old = atomic64_cmpxchg(v, c, c c_op i)) != c)		\
		c = old;						\
	return old;							\
}

ATOMIC64_FETCH_OP(add, +)

#define atomic64_fetch_sub(i, v)	atomic64_fetch_add(-(i), (v))

#define ATOMIC64_OPS(op, c_op)						\
	ATOMIC64_FETCH_OP(op, c_op)

ATOMIC64_OPS(and, &)
ATOMIC64_OPS(or, |)
ATOMIC64_OPS(xor, ^)

#undef ATOMIC64_OPS
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_OP

#include <asm-generic/atomic.h>

#endif /* _ASM_SIM_ATOMIC_H */
