#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <opngcore.h>
#include <opngtrans>
typedef struct opng_options dem_options;
int pngopt(FILE *f2){
	dem_options d;
	d.optim_level=2;
	opng_optimizer_t op1=opng_create_optimizer();
	opng_set_options(op1,d);
	opng_optimize_file(op1,f2);
	return 0;
}