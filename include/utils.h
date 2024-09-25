#ifndef __UTILS_H
#define __UTILS_H

#include "dartboard.h"

const char* utils_zone_to_str(dartboard_zone_t zone);
int utils_mult_from_zone(int zone);
bool utils_valid_shot(dartboard_shot_t* ds);

#endif // __UTILS_H
