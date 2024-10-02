#ifndef CNOVEL_DEF_H_SENTRY
#define CNOVEL_DEF_H_SENTRY

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(*(arr))

typedef enum m_bool {
    cn_false = 0,
    cn_true = 1
} cn_bool;

#endif
