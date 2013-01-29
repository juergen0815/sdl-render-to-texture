/*
 * worker.h
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#ifndef WORKER_H_
#define WORKER_H_

class Worker
{
public:
	virtual ~Worker() {}

	virtual void Terminate() = 0;

	virtual void Run() = 0;
};

#endif /* WORKER_H_ */
