#pragma once

typedef struct ranctx {
    unsigned long a;
    unsigned long b;
    unsigned long c;
    unsigned long d;
} ranctx;

#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
unsigned long ranval(ranctx *x) {
    unsigned long e = x->a - rot(x->b, 27);
    x->a = x->b ^ rot(x->c, 17);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}
#undef rot

void raninit(ranctx *x, unsigned long seed) {
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (unsigned long i = 0; i < 20; ++i) {
        (void)ranval(x);
    }
}
