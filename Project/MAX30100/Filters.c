/*
 * Filters.c
 *
 * Created: 3/8/2024 10:23:26 AM
 *  Author: Afzal Hossan
 */ 


#include "Filters.h"

void FilterBuLp1_init(FilterBuLp1 *filter) {
	filter->v[0] = 0.0;
}

float FilterBuLp1_step(FilterBuLp1 *filter, float x) {
	filter->v[0] = filter->v[1];
	filter->v[1] = (2.452372752527856026e-1 * x) + (0.50952544949442879485 * filter->v[0]);
	return (filter->v[0] + filter->v[1]);
}

void DCRemover_init(DCRemover *remover, float alpha) {
	remover->alpha = alpha;
	remover->dcw = 0;
}

float DCRemover_step(DCRemover *remover, float x) {
	float olddcw = remover->dcw;
	remover->dcw = x + remover->alpha * remover->dcw;
	return remover->dcw - olddcw;
}

float DCRemover_getDCW(DCRemover *remover) {
	return remover->dcw;
}
