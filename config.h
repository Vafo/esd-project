#ifndef CONFIG_H
#define CONFIG_H

// CPU Frequency
#define F_CPU 16000000UL

#define USB_UBRR 0x67
#define ESP_UBRR 0x08

#define JOYSTICK_X 4
#define JOYSTICK_Y 3

#define SECOND_JOYSTICK 1

#define LCD_HEIGHT 64
#define LCD_WIDTH 128

#define EDGE_SAMPLE_LENGTH 0.8

#define BORDER_EDGE_RADIUS 4
#define PANEL_EDGE_RADIUS 6
#define GATE_EDGE_RADIUS 3

#define BALL_RADIUS 2

#define DRAG_COEFF 0.1

#define PANEL_LENGTH 10
#define PANEL_NO_HIT_CNT 0

#define BALL_SPEED 4
#define PANEL_SPEED 4

#define BALL_WEIGHT 1
#define PANEL_WEIGHT 10000

#define GATE_WIDTH 8
#define GATE_HEIGHT 20

#endif