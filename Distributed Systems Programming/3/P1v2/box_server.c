/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "box.h"

 /* some worker procedures */
float surface(float l, float w, float h)
{
	return 2 * (l * w + l * h + w * h);
}

float volume(float l, float w, float h)
{
	return l * w * h;
}

box_results*
box_calc_1_svc(dimensions* argp, struct svc_req* rqstp)
{
	static box_results  result;

	result.surface = surface(argp->length, argp->width, argp->height);
	result.volume = volume(argp->length, argp->width, argp->height);

	return &result;
}
