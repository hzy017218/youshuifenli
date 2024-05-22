#include "kalman.h"


#define RAD_TO_DEG 57.295780
#define DEG_TO_RAD 0.017453
#define ACCEL_CORRECTOR 0.000061
#define GYRO_CORRECTOR 0.007634

uint32_t kalman_timer;

Kalman_t KalmanX = {
    .Q_angle = 0.001f,
    .Q_gyro = 0.003f,
    .R_measure = 0.03f,
	.P = {{1, 0}, {0, 1}}
};

Kalman_t KalmanY = {
	.Q_angle = 0.001f,
	.Q_gyro = 0.003f,
	.R_measure = 0.03f,
	.P = {{1, 0}, {0, 1}}
};


void KalmanAHRSupdateIMU(short ax_raw, short ay_raw, short az_raw, short gx_raw, short gy_raw, short gz_raw, double *roll, double *pitch)
{
    double ax, ay, az;
    double gx, gy, gz;

    ax = (double)ax_raw  * ACCEL_CORRECTOR;
    ay = (double)ay_raw  * ACCEL_CORRECTOR;
    az = (double)az_raw  * ACCEL_CORRECTOR;

    gx = (double)gx_raw * GYRO_CORRECTOR;
    gy = (double)gy_raw * GYRO_CORRECTOR;
    gz = (double)gz_raw * GYRO_CORRECTOR;

    // Kalman angle solve
    double dt = (double)(HAL_GetTick() - kalman_timer) / 1000;
    kalman_timer = HAL_GetTick();

    double acc_roll = atan2(ay, az) * RAD_TO_DEG;
    double acc_pitch = -atan2(ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;

    *roll = Kalman_getAngle(&KalmanX, acc_roll, gx, dt);
    *pitch = Kalman_getAngle(&KalmanY, acc_pitch, gy, dt);
}

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt)
{
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_gyro * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};
