#ifndef _MODE_H_
#define _MODE_H_

typedef enum
{
    MODE_MANUAL = 0,
    MODE_AUTOMATIC,
    MODE_MAX
} mode_t;

mode_t mode = MODE_MANUAL;

static inline mode_t getMode(void){ return mode; }
static inline void   setMode(mode_t m ) { if (m < MODE_MAX) mode = m; } 

#endif // !_MODE_H_