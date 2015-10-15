#include "kernel/common.h"
#include "kernel/paging.h"
#include "kernel/kfs.h"


int kfs_init()
{
	//paging_automap(0xE0000000, PG_P | PG_R);
	return 0;
}
