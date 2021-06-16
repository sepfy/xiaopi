#ifndef UTILITY_PLOG_H_
#define UTILITY_PLOG_H_

#include <stdio.h>
#include <stdlib.h>

#define PLOGE(fmt, arg...) fprintf(stderr, "[ERROR %s:%d] " fmt"\n", __FILE__, __LINE__, ##arg)
#define PLOGW(fmt, arg...) fprintf(stdout, "[WARN %s:%d] " fmt"\n", __FILE__, __LINE__, ##arg)
#define PLOGI(fmt, arg...) fprintf(stdout, "[INFO %s:%d] " fmt"\n", __FILE__, __LINE__, ##arg)

#endif // UTILITY_PLOG_H_
