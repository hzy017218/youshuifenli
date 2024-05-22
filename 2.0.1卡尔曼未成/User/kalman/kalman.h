#ifndef HARDWARE_KALMAN_H_
#define HARDWARE_KALMAN_H_

#include <math.h>
#include "stm32f1xx_hal.h"

// Kalman structure
typedef struct
{
    double Q_angle;   //???????,????????
    double Q_gyro;    //????????,?????????
    double R_measure; //?????????
    double angle;     //??
    double bias;      //?????
    double P[2][2];   //???????P
} Kalman_t;

void KalmanAHRSupdateIMU(short ax_raw, short ay_raw, short az_raw, short gx_raw, short gy_raw, short gz_raw, double *roll, double *pitch);
double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);

#endif /* HARDWARE_KALMAN_H_ */
