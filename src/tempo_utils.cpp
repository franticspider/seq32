#include "tempo_utils.h"

double
pulse_length_us(double bpm, int ppqn, int beat_width)
{
    return 60000000.0 / ppqn / bpm * (double(beat_width) / 4.0);
}
