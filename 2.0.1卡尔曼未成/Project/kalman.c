#include "kalman.h"

void Kalman_Init(Kalman_t *kf, float Q, float R) {
    kf->Q = Q;
    kf->R = R;
    kf->P[0][0] = 1.0f;
    kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f;
    kf->P[1][1] = 1.0f;
    kf->angle = 0.0f;
    kf->bias = 0.0f;
}

float Kalman_Update(Kalman_t *kf, float newValue, float newRate, float dt) {
    float rate = newRate - kf->bias;
    kf->angle += dt * rate;

    kf->P[0][0] += dt * (dt * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->Q);
    kf->P[0][1] -= dt * kf->P[1][1];
    kf->P[1][0] -= dt * kf->P[1][1];
    kf->P[1][1] += kf->Q * dt;

    float S = kf->P[0][0] + kf->R;
    kf->K[0] = kf->P[0][0] / S;
    kf->K[1] = kf->P[1][0] / S;

    float y = newValue - kf->angle;
    kf->angle += kf->K[0] * y;
    kf->bias += kf->K[1] * y;

    float P00_temp = kf->P[0][0];
    float P01_temp = kf->P[0][1];

    kf->P[0][0] -= kf->K[0] * P00_temp;
    kf->P[0][1] -= kf->K[0] * P01_temp;
    kf->P[1][0] -= kf->K[1] * P00_temp;
    kf->P[1][1] -= kf->K[1] * P01_temp;

    return kf->angle;
}
