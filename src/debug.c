#include "debug.h"
#include "db_functions.h"

int err() {
	printf("Last result was not expected\n\n");
	SQL_close();
	return 1;
}
