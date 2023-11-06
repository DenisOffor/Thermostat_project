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
#define iMin -20 // ћинимальное значение интегратора
#define iMax 20  // ћаксимальное значение интегратора

#define PID_ON 1
#define PID_OFF 0
uint8_t pid_state;

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

void PID_regulation();
void PID_start();
void PID_stop();
void init_PID_regulation();
void init_TIM3_for_PWM();
void set_Pid_Coef(uint16_t Kp, uint16_t Ki, uint16_t Kd);
void init_tim14_as_delay_unit();
void PID_reset();

#endif /* PID_REGULATOR_H_ */
