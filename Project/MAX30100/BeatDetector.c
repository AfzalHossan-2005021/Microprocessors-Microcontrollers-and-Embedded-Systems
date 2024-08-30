/*
 * BeatDetector.c
 *
 * Created: 3/8/2024 10:34:29 AM
 *  Author: Afzal Hossan
 */ 


#include "BeatDetector.h"

#ifndef min
#define min(a,b) \
({ __typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })
#endif

void BeatDetector_init(BeatDetector *detector)
{
	detector->state = BEATDETECTOR_STATE_INIT;
	detector->threshold = BEATDETECTOR_MIN_THRESHOLD;
	detector->beatPeriod = 0;
	detector->lastMaxValue = 0;
	detector->tsLastBeat = 0;
}

bool BeatDetector_addSample(BeatDetector *detector, float sample)
{
	return checkForBeat(detector, sample);
}

float BeatDetector_getRate(BeatDetector *detector)
{
	if (detector->beatPeriod != 0) {
		return 1 / detector->beatPeriod * 1000 * 60;
		} else {
		return 0;
	}
}

float BeatDetector_getCurrentThreshold(BeatDetector *detector)
{
	return detector->threshold;
}

bool checkForBeat(BeatDetector *detector, float sample)
{
	bool beatDetected = false;

	switch (detector->state) {
		case BEATDETECTOR_STATE_INIT:
		if (millis() > BEATDETECTOR_INIT_HOLDOFF) {
			detector->state = BEATDETECTOR_STATE_WAITING;
		}
		break;

		case BEATDETECTOR_STATE_WAITING:
		if (sample > detector->threshold) {
			detector->threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
			detector->state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
		}

		// Tracking lost, resetting
		if (millis() - detector->tsLastBeat > BEATDETECTOR_INVALID_READOUT_DELAY) {
			detector->beatPeriod = 0;
			detector->lastMaxValue = 0;
		}

		decreaseThreshold(detector);
		break;

		case BEATDETECTOR_STATE_FOLLOWING_SLOPE:
		if (sample < detector->threshold) {
			detector->state = BEATDETECTOR_STATE_MAYBE_DETECTED;
			} else {
			detector->threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
		}
		break;

		case BEATDETECTOR_STATE_MAYBE_DETECTED:
		if (sample + BEATDETECTOR_STEP_RESILIENCY < detector->threshold) {
			// Found a beat
			beatDetected = true;
			detector->lastMaxValue = sample;
			detector->state = BEATDETECTOR_STATE_MASKING;
			float delta = millis() - detector->tsLastBeat;
			if (delta) {
				detector->beatPeriod = BEATDETECTOR_BPFILTER_ALPHA * delta +
				(1 - BEATDETECTOR_BPFILTER_ALPHA) * detector->beatPeriod;
			}

			detector->tsLastBeat = millis();
			} else {
			detector->state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
		}
		break;

		case BEATDETECTOR_STATE_MASKING:
		if (millis() - detector->tsLastBeat > BEATDETECTOR_MASKING_HOLDOFF) {
			detector->state = BEATDETECTOR_STATE_WAITING;
		}
		decreaseThreshold(detector);
		break;
	}

	return beatDetected;
}

void decreaseThreshold(BeatDetector *detector)
{
	// When a valid beat rate readout is present, target the
	if (detector->lastMaxValue > 0 && detector->beatPeriod > 0) {
		detector->threshold -= detector->lastMaxValue * (1 - BEATDETECTOR_THRESHOLD_FALLOFF_TARGET) /
		(detector->beatPeriod / BEATDETECTOR_SAMPLES_PERIOD);
		} else {
		// Asymptotic decay
		detector->threshold *= BEATDETECTOR_THRESHOLD_DECAY_FACTOR;
	}

	if (detector->threshold < BEATDETECTOR_MIN_THRESHOLD) {
		detector->threshold = BEATDETECTOR_MIN_THRESHOLD;
	}
}
