/*
 * This code is generated by rpcgen
 * and modified by Zarif Karim.
 */

#include "box_2.h"

/* print out the box details */
void print_box(float l, float w, float h, float sa, float v, float m, float p)
{
	printf("D(%.2f x %.2f x %.2f)\t",l,w,h);
	printf("SA: %.2f\t",sa);
	printf("V: %.2f\t",v);
	printf("M: %.2f\t",m);
	printf("P: %.2f\n",p);
	return;
}

void
rpc_box_2(char *host, float l, float w, float h, float m)
{
	CLIENT *clnt;
	box_results  *result_1;
	dimensions  box_calc_2_arg;
	float  *result_2;
	mail_dims  mail_calc_2_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_BOX, BOXVERSION2, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	box_calc_2_arg.length = l;
	box_calc_2_arg.width = w;	
	box_calc_2_arg.height = h;
#endif	/* DEBUG */

	result_1 = box_calc_2(&box_calc_2_arg, clnt);
	if (result_1 == (box_results *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
	mail_calc_2_arg.volume = result_1->volume;
	mail_calc_2_arg.mass = m;
	result_2 = mail_calc_2(&mail_calc_2_arg, clnt);
	if (result_2 == (float*)NULL) {
		clnt_perror(clnt, "call failed");
	}

	/* print out the results */
	print_box(l, w, h, result_1->surface, result_1->volume, m, *result_2);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}



int
main (int argc, char *argv[])
{
	char *host;
	float l, w, h, m;

	if (argc != 6) 
	{
		printf ("usage: %s server_host length width height mass\n", argv[0]);
		exit (1);
	}

	host = argv[1];

	if ((l = atof(argv[2])) <= 0 && *argv[2] == '0') 
	{
		fprintf(stderr, "invalid length: %s\n", argv[2]);
		exit(1);
	}

	if ((w = atof(argv[3])) <= 0 && *argv[3] == '0') 
	{
		fprintf(stderr, "invalid width: %s\n", argv[3]);
		exit(1);
	}
	
	if ((h = atof(argv[4])) <= 0 && *argv[3] != '0') 
	{
		fprintf(stderr, "invalid height: %s\n", argv[3]);
		exit(1);
	}
	
	if ((m = atof(argv[5])) <= 0 && *argv[4] != '0') 
	{
		fprintf(stderr, "invalid mass: %s\n", argv[4]);
		exit(1);
	}
	rpc_box_2 (host, l, w, h, m);

	exit (0);
}
