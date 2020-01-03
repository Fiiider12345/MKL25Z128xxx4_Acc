/*
 * Engine.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: marek
 */

#include <elevator/Engine.h>

Engine::Engine() {
	speed = DEFAULT_ENGINE_SPEED;
}

Engine::Engine(Timer* timer_p) {
	speed = DEFAULT_ENGINE_SPEED;
	timer = timer_p;
}

Engine::~Engine() {
}

void Engine::stop() {
	comunicator.sendCommand(ELEVATOR_ENGINE, engine_stop);
}

void Engine::goDown() {
	uint8_t down[] = { engine_movement, 0xFF, 0xFF, 0xFF, ~(speed - 1) };
	comunicator.sendMessage(ELEVATOR_ENGINE, down, 5);
}

void Engine::goUp() {
	uint8_t up[] = { engine_movement, 0x00, 0x00, 0x00, speed };
	comunicator.sendMessage(ELEVATOR_ENGINE, up, 5);
}

void Engine::set_speed(uint8_t newSpeed) {
	speed = newSpeed;
}

double Engine::getPosition() {

	uint8_t sender;
	uint8_t data[MESSAGE_BUFFER_SIZE];
	uint8_t lenght;
	comunicator.sendCommand(ELEVATOR_ENGINE, engine_sendMotorEncoderCount);
	timer->setChannelTimer(timerChannel_checkPositionACK, 300);
	do {
		comunicator.receivedMessage(data, &sender, &lenght);
	} while (sender != ELEVATOR_ENGINE);

	double* d = (double *) data;
	double position = (*d) * (0.004057) + 4.05; //0.004057
	if (position > 4)
		position = 4;
	if (position < 0)
		position = 0;
	return position;
}

