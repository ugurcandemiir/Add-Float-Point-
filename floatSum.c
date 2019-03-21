
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "floatInt.h"

float floatSum(float a,float b);
void floatPrint(char * pref,float a);

int main(int argc,char **argv) {
	if (argc!=3) {
		printf("Please invoke as %s <a> <b>\n",argv[0]);
		printf("   where <a> and <b> are floating point numbers.\n");
		return 1;
	}
	float a=atof(argv[1]);
	floatPrint("a             :",a);
	float b=atof(argv[2]);
	floatPrint("b             :",b);
	floatPrint("floatSum(a,b) :",floatSum(a,b));
	floatPrint("a + b         :",a+b);
	return 0;
}

float floatSum(float a,float b) {
	/* Add your code to add a and b with doing a+b here */
	int sign_bit_value = 0;
	int sign_bit_value_a;
	int sign_bit_value_b;
	int exponent_sum;
	int fractional_sum;
	int fractional_a;
	int fractional_b;
	float result;
	if (float2Exp(a) > float2Exp(b)){
		exponent_sum =float2Exp(a);
	}else{
		exponent_sum = float2Exp(b);
	}
	if (float2Sign(b)== 0){
		sign_bit_value_b= 0;
	}
	if (float2Sign(b)== 1){
		sign_bit_value_b = 1;
	}
	if (float2Sign(a)== 0){
		sign_bit_value_a = 0;
	}
	if(float2Sign(a)== 1){
		sign_bit_value_a = 1;
	}
	
	fractional_a = float2Frac(a)>> (exponent_sum - float2Exp(a));
	fractional_b = float2Frac(b)>> (exponent_sum - float2Exp(b));
	fractional_sum = pow(-1,sign_bit_value_b)* fractional_b + pow(-1, sign_bit_value_a)*fractional_a;

	if (fractional_sum < 0){
		sign_bit_value = 1;
		fractional_sum = -fractional_sum ;
	}else{
		sign_bit_value = 0;
	}
	while (fractional_sum > (1<<24)&& exponent_sum<129){
		fractional_sum = fractional_sum >> 1;
		exponent_sum++;
	}
	while (fractional_sum < (1<<23) && exponent_sum>-126){
		fractional_sum = fractional_sum << 1;
		exponent_sum--;
	}
	if (exponent_sum > 127){
		fractional_sum = 1<<23;
		exponent_sum = 128;
	}
	while (exponent_sum <= -127){
		fractional_sum = fractional_sum>>1;
		exponent_sum++;
		if (fractional_sum == 0 ){
			return 0.0;
		}
	}
	if (exponent_sum== -127 && fractional_sum>0){
		fractional_sum&= ~(1<<23);
		exponent_sum = -126;
	}
      if (isnanf(a) || isnanf(b)) return NAN;
      if (isinff(a) && b==0.0) {
           if(a<0){
                 return -INFINITY;
           }
           else{
                 return INFINITY;
           }
          
      }
	 
      if (isinff(b) && a==0.0) {
                if(b<0){
                        return -INFINITY;
                }
                else{
                        return INFINITY;
                }
        }
     
      if (isinff(a) && b!=0.0) {
           if(a<0){
                 return -INFINITY;
           }
           else{
                 return INFINITY;
           }
      }
      if (isinff(b) && a!=0.0) {
                if(b<0){
                        return -INFINITY;
                }
                else{
                        return INFINITY;
                }
        }
 
 
      if (isinff(a) && isinff(b)) {
           if(a<0 && b>0) return 0.0;
           if(a>0 && b<0) return 0.0;
           if(a>0 && b>0) return INFINITY;
           if(a<0 && b<0) return -INFINITY;
      }
      if (a==0.0 && b==0.0) return 0.0;
	result = makeFloat(sign_bit_value, exponent_sum, fractional_sum);

	return result;
}
void floatPrint(char * pref,float a) {
	int norm=isNormal(a);
	int frac=float2Frac(a);
	int exp=float2Exp(a);
	if (isNormal(a)) {
		frac &= ~(1<<23); // Turn of 1. for print
	} else exp=-126;
	printf("%s %g = -1^%d x %d.%06x x 2^%d\n",pref,a,
		float2Sign(a),norm,frac,exp);
}
