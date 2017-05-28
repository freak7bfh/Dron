/*
 * motors.h
 *
 *  Created on: May 7, 2017
 *      Author: Jarek
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#define MOTORS_GPIO GPIOA
#define MOTOR1 GPIO_Pin_8
#define MOTOR2 GPIO_Pin_9
#define MOTOR3 GPIO_Pin_10
#define MOTOR4 GPIO_Pin_11

void MotorsPWMInit();

#endif /* MOTORS_H_ */
