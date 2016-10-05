#include <stdio.h>
#include <conio.h>
#include <mem.h>

#include "aga3.h"
#include "aga8.h"
#include "units.h"


#define PRECISION 8

aga3param       parameters;
aga3measuredval measuredvalues;
aga3calcval     calculatedvalues;


aga8param       aga8parameters;
aga8calcval     aga8calculated;



void printParameters(aga3param *param){
	printf("*** Parameters Values *** \n");
	printf("a1   = %.*f  \t  a2 = %.*f\n",PRECISION,param->a1,PRECISION,param->a2);
	printf("dr   = %.*f  \t  Dr = %.*f\n",PRECISION,param->dr,PRECISION,param->Dr);
	printf("Tr   = %.*f  \t          \n",PRECISION,param->Tr);

}

void printMeasured(aga3measuredval *measured) {
	printf("*** Measured Values *** \n");
	printf("Tf    = %.*f  \t  DP    = %.*f\n",PRECISION,measured->Tf,PRECISION,measured->DP);
	printf("Pf1   = %.*f  \t  Pf2   = %.*f\n",PRECISION,measured->Pf1,PRECISION,measured->Pf2);

}

void printCalculated(aga3calcval *calculated) {
	printf("*** Calculated Values *** \n");
	printf("df   = %.*f  \t  Df   = %.*f\n",PRECISION,calculated->df,PRECISION,calculated->Df);
	printf("Bf   = %.*f  \t  Ev   = %.*f\n",PRECISION,calculated->Bf,PRECISION,calculated->Ev);
	printf("Bf2  = %.*f  \t  Bf4  = %.*f\n",PRECISION,calculated->Bf2,PRECISION,calculated->Bf4);
	printf("Bf07 = %.*f  \t  Bf08 = %.*f\n",PRECISION,calculated->Bf07,PRECISION,calculated->Bf08);
	printf("L1   = %.*f  \t  L2   = %.*f\n",PRECISION,calculated->L1,PRECISION,calculated->L2);
	printf("k    = %.*f  \t  x    = %.*f\n",PRECISION,calculated->k,PRECISION,calculated->x);
	printf("Yp   = %.*f  \t  Y    = %.*f\n",PRECISION,calculated->Yp,PRECISION,calculated->Y);
	printf("FIc  = %.*f  \t  FIp  = %.*f\n",PRECISION,calculated->FIc,PRECISION,calculated->FIp);
	printf("FI   = %.*f  \t  M2   = %.*f\n",PRECISION,calculated->FI,PRECISION,calculated->M2);
	printf("Tu   = %.*f  \t  Td   = %.*f\n",PRECISION,calculated->Tu,PRECISION,calculated->Td);
	printf("Ts   = %.*f  \t  Cd0  = %.*f\n",PRECISION,calculated->Ts,PRECISION,calculated->Cd0);
	printf("Cd1  = %.*f  \t  Cd2  = %.*f\n",PRECISION,calculated->Cd1,PRECISION,calculated->Cd2);
	printf("Cd3  = %.*f  \t  Cd4  = %.*f\n",PRECISION,calculated->Cd3,PRECISION,calculated->Cd4);

	printf("Cd   = %.*f  \t  X    = %.*f\n",PRECISION,calculated->Cd,PRECISION,calculated->X);
	printf("Fc   = %.*f  \t  Dc   = %.*f\n",PRECISION,calculated->Fc,PRECISION,calculated->Dc);
	printf("delCd= %.*f  \t  Fmass= %.*f\n",PRECISION,calculated->deltaCd,PRECISION,calculated->Fmass);

	printf("qm   = %.*f  \t  Qb   = %.*f\n",PRECISION,calculated->qm,PRECISION,calculated->Qb);

}



void setUnits_US(aga3param *param){
	param->R   = (REAL)10.7316;
	param->Mr  = (REAL)28.9625;
	param->Nc  = (REAL)323.279;
	param->NIc = (REAL)6.23582e-4;
	param->N3  = (REAL)27.7070;
	param->N4  = (REAL)1.0;
	param->N5  = (REAL)459.67;
}

void setUnits_IP(aga3param *param){
	param->R   = (REAL)10.7316;
	param->Mr  = (REAL)28.9625;
	param->Nc  = (REAL)46552.1;
	param->NIc = (REAL)0.0773327;
	param->N3  = (REAL)27.7070;
	param->N4  = (REAL)0.08333333;
	param->N5  = (REAL)459.67;
}

void setUnits_METRIC(aga3param *param){
	param->R   = (REAL)0.0831451;
	param->Mr  = (REAL)28.9625;
	param->Nc  = (REAL)0.036;
	param->NIc = (REAL)0.1;
	param->N3  = (REAL)1000;
	param->N4  = (REAL)25.4;
	param->N5  = (REAL)273.15;
}

void setUnits_SI(aga3param *param){
	param->R   = (REAL)8314.51;
	param->Mr  = (REAL)28.9625;
	param->Nc  = (REAL)1.0;
	param->NIc = (REAL)1.0;
	param->N3  = (REAL)1.0;
	param->N4  = (REAL)0.0254;
	param->N5  = (REAL)0.0;
}
