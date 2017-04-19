#include "Robot_IMU_Control.h"

const float Robot_IMU_Control::num_lp_10Hz[3] = { 0.355, 0.355, 0.0 };
const float Robot_IMU_Control::num_lp_25Hz[3] = { 0.755, 0.755, 0.0 };
const float Robot_IMU_Control::den_lp_10Hz[3] = { 1.0, -0.2905, 0.0 };
const float Robot_IMU_Control::den_lp_25Hz[3] = { 1.0, 0.5095, 0.0 };

const double Robot_IMU_Control::comp_headset[3][3] = { { 0.000168679716274983, -5.17710741425967e-06, -8.77884394210979e-07 },
													   { -5.17710741425967e-06, 0.000159571222440490, -3.27611632378113e-06 },
													   { -8.77884394210986e-07, -3.27611632378113e-06, 0.000166456977421752 } };

const double Robot_IMU_Control::hardiron_headset[3] = { -725.100317519760, 491.703440889862, -53.3108814506022 };