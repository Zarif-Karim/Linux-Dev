#include <stdio.h>
#include <math.h>
/* some worker procedures */
float surfaceArea(float l, float w, float h)
{
	l = ceilf(l/3) * 3;
	w = ceilf(w/3) * 3;
	h = ceilf(h/3) * 3;
	
	return 2 * (l * w + l * h + w * h);
}

float volume(float l, float w, float h)
{
	l = ceilf(l/3) * 3;
	w = ceilf(w/3) * 3;
	h = ceilf(h/3) * 3;
	
	return l * w * h;
}

float postage (float v, float m)
{
	float result;
	
	if(v < m*9) 
	{
		result = v * 3;
	}
	else 
	{
		result = m * 9 * 2;
	}
	return result;
}

int main (/* int argc, char * argv[] */)
{
	float limit = 12;
	float v;
	float m = 10;
	for(float l = 3; l <= limit; l+=3)
	{
		for(float w = 3; w <= limit; w+=3)
		{
			for(float h = 3; h <= limit; h+=3)
			{
				//for(float m = 1; m <= limit; m++)
				//{
					v = volume(l,w,h);
					
					printf("D(%.2f x %.2f x %.2f)\t",l,w,h);
					printf("SA: %.2f\t", surfaceArea(l,w,h));
					printf("V: %.2f\t",v);
					printf("M: %.2f\t",m);
					printf("P: %.2f\n",postage(v,m));
				//}
			}
		}
	}
	
	return 0;
}