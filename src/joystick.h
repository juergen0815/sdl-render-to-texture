/*
 * joystick.h
 *
 *  Created on: Jan 26, 2013
 *      Author: Jurgen
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// PS3/XBox
// 1 ... X
// 2 ... O
// 3 ... [] Square/X
// 4 ... ^ Triangle/Y
// 5 ... L1
// 6 ... R1
// 7 ... Select
// 8 ... Start
// 9 ... L3
// 10... R3
// L2/R2 - Z-Axis!

enum JOY_BUTTONS {
    X = 0,

    CIRCLE,
    SQUARE,
    TRIANGLE,

    L1,
    R1,
    SELECT,
    START,
    L3,
    R3
};

enum JOY_AXIS {
    X_MOTION = 0,
    Y_MOTION,
    Z_TRIGGER,
    X_ROTATION,
    Y_ROTATION
};

#define JOY_AXIS_SCALE 0.0002f
#define JOY_AXIS_THRESHOLD 5.0f
#define JOY_AXIS_DEADZONE  1.0f

#endif /* JOYSTICK_H_ */
