/*
 * Filters.h
 *
 * Created: 3/8/2024 10:22:36 AM
 *  Author: Afzal Hossan
 */ 


#ifndef FILTERS_H_
#define FILTERS_H_


typedef struct {
	float v[2];
} FilterBuLp1;

void FilterBuLp1_init(FilterBuLp1 *filter);
float FilterBuLp1_step(FilterBuLp1 *filter, float x);

typedef struct {
	float alpha;
	float dcw;
} DCRemover;

void DCRemover_init(DCRemover *remover, float alpha);
float DCRemover_step(DCRemover *remover, float x);
float DCRemover_getDCW(DCRemover *remover);


#endif /* FILTERS_H_ */