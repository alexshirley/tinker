#include <dsp_ring.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif

#define NUM_ARR_ELE(x) (sizeof(x) / sizeof(x[0]))

typedef struct sig_generator sig_generator_t;
struct sig_generator {
    int i;
    double (*func)(struct sig_generator*, double);
    int sampling_frequency;
};

double crazy_signal(sig_generator_t* gen, double in) {
    const double f1        = 60.0; // Mains Line Noise that we don't want
    const double f2        = 100.0;
    const double samplingf = 1.0 / (double)gen->sampling_frequency;
    return sin(2.0 * M_PI * f1 / samplingf * in) / 2.0 + sin(2.0 * M_PI * f2 / samplingf * (double)in) / 2.0;
}

sig_generator_t create_signal_generator(void) {
    return (sig_generator_t){.i = 0, .func = crazy_signal, .sampling_frequency = 200};
}

double getNextSignal(sig_generator_t* gen) {
    const double ret = gen->func(gen, (double)gen->i++);
    return ret;
}

typedef struct {
    double* taps;
    int num_taps;
    dsp_ring_t* buffer;
} fir_filter;

double fir_get_value(fir_filter* flt, double incoming) {
    dsp_ring_push(flt->buffer, &incoming);
    double accum = 0;
    for (int i = 0; i < flt->num_taps; i++) {
        double* grab = dsp_ring_index(flt->buffer, flt->num_taps - i - 1); // Most recent
        accum += flt->taps[i] * *grab;
    }
    return accum;
}

// Fourth order moving average
static double filter_taps[] = {(1.0 / 5.0), (1.0 / 5.0), (1.0 / 5.0), (1.0 / 5.0), (1.0 / 5.0)};

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    // Initalize the generators, buffers, etc.
    sig_generator_t gen = create_signal_generator();
    // Initalize the filter
    fir_filter flt                = {.taps = filter_taps, .num_taps = NUM_ARR_ELE(filter_taps)};
    size_t ring_buffer_alloc_size = dsp_ring_required_size(sizeof(double), NUM_ARR_ELE(filter_taps));
    char* buf                     = malloc(ring_buffer_alloc_size);
    dsp_ring_init(&flt.buffer, buf, ring_buffer_alloc_size, sizeof(double));

    printf("Sample Frequency = %d\n", gen.sampling_frequency);
    printf("Frequencies at %f, %f\n", 60.0, 100.0);

    for (int i = 0; i < 10000; i++) {
        double sig     = getNextSignal(&gen);
        double flt_sig = fir_get_value(&flt, sig);
        printf("Index: %d \t Signal: %f \t Filtered %f\n", i, sig, flt_sig);
    }
}