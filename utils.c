#include <stdint.h>
#include <stdlib.h>

#include "utils.h"

int rand_sign() 
{
  return (float)rand() / ((float)INT32_MAX / 2.0f) <= 0.5 ? -1 : 1;
}
