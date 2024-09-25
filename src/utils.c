#include <assert.h>

#include "utils.h"
#include "log.h"

/* Global variables ***********************************************************/
/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

const char* utils_zone_to_str(dartboard_zone_t zone)
{
	const char* str_zone;
	if (zone == 0) {
		str_zone = "triple";
	} else if (zone == 1) {
		str_zone = "double";
	} else {
		str_zone = "single";
	}
	return str_zone;
}

int utils_mult_from_zone(int zone)
{
	if (zone == ZONE_TRIPLE) {
		return 3;
	} else if (zone == ZONE_DOUBLE) {
		return 2;
	}
	return 1;
}

// This function check if a shot is valid. This is:
// - Number must be between [0, 20]
// - Zone must be between [0, 4]
bool utils_valid_shot(dartboard_shot_t* ds)
{
	LOG_DEBUG("NUMBER: %d, ZONE: %d", ds->number, ds->zone);
	bool valid = ds->number >= 0 && ds->number <= 20 && ds->zone >= 0 && ds->zone <= 4;
	if (!valid) {
		LOG_ERROR("Invalid shot!");
	}
	return valid;
}
