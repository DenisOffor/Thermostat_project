/*
 * PID_regulator.h
 *
 *  Created on: 27 окт. 2023 г.
 *      Author: denlo
 */

#ifndef PID_REGULATOR_H_
#define PID_REGULATOR_H_

#include "constant.h"

#define PID_DUTY_CYCLE_MIN 0
#define PID_DUTY_CYCLE_MAX 1000
#define iMin -30 // ћинимальное значение интегратора
#define iMax 30  // ћаксимальное значение интегратора


int pwmDutyCycle;
float errorPrevious;
float errorCurrent;
float errorIntegral;
float errorDifferential;

typedef struct Pid_Coef {
	uint16_t Kp;
	uint16_t Ki;
	uint16_t Kd;
} Pid_Coef;
Pid_Coef pid_coef;

void init_TIM3_for_PWM();
void clear_Pid_Coef();
void set_Pid_Coef(uint16_t Kp, uint16_t Ki, uint16_t Kd);

#endif /* PID_REGULATOR_H_ */
