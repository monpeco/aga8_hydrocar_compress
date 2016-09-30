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



void testUnits(){
    
   UNITS a,b,c,d,e,f;
   SPFLOAT value;

   a = 0x0310; /* Miles de Metros Cubicos */
   b = 0x0010; /* Metros Cubicos */
   c = 0x0011;  /* Pies Cubicos */
   d = 0x0016;  /* Gallons U */
   e = 0x0012;  /* Pulgadas Cubicas */


   value = 35;
   printf("%f Miles de Metros Cubicos son: %f Metros Cubicos\n",value,convertUnits(value,a,b));
   printf("%f Metros Cubicos son: %f Miles de Metros Cubicos\n",value,convertUnits(value,b,a));
   printf("%f US GALLONS son: %f Pulgadas Cubicas\n",value,convertUnits(value,d,e));


   printf("%f KPascal son: %f pascal\n",value,convertUnits(value,PUNITS_KPAS,PUNITS_PAS));
   printf("%f KPascal son: %f Mpascal\n",value,convertUnits(value,PUNITS_KPAS,PUNITS_MPAS));

}

void initAga8Param (void) {
    
	//Parameters for Gulf Coast
	aga8parameters.MFC[CID_METHANE] = 96.5222;
	aga8parameters.MFC[CID_NITROGEN] = 0.2595;
	aga8parameters.MFC[CID_CARBON_DIOXIDE] = 0.5956;
	aga8parameters.MFC[CID_ETHANE] = 1.8186;
	aga8parameters.MFC[CID_PROPANE] = 0.4596;
	aga8parameters.MFC[CID_I_BUTANE] = 0.0977;
	aga8parameters.MFC[CID_N_BUTANE] = 0.1007;
	aga8parameters.MFC[CID_I_PENTANE] = 0.0473;
	aga8parameters.MFC[CID_N_PENTANE] = 0.0324;
	aga8parameters.MFC[CID_N_HEXANE] = 0.0664;
	aga8parameters.MFC[CID_N_HEPTANE] = 0.0000;
	aga8parameters.MFC[CID_N_OCTANE] = 0.0000;

	aga8parameters.ncc = 12;

	aga8parameters.ci[1] = CID_METHANE;
	aga8parameters.ci[2] = CID_NITROGEN;
	aga8parameters.ci[3] = CID_CARBON_DIOXIDE;
	aga8parameters.ci[4] = CID_ETHANE;
	aga8parameters.ci[5] = CID_PROPANE;
	aga8parameters.ci[6] = CID_I_BUTANE;
	aga8parameters.ci[7] = CID_N_BUTANE;
	aga8parameters.ci[8] = CID_I_PENTANE;
	aga8parameters.ci[9] = CID_N_PENTANE;
	aga8parameters.ci[10] = CID_N_HEXANE;
	aga8parameters.ci[11] = CID_N_HEPTANE;
	aga8parameters.ci[12] = CID_N_OCTANE;


	aga8parameters.Tb = 273.15;
   	aga8parameters.Pb = 0.10155977;

}



void initAga8ParamFlowCheck (void) {
	//Parameters for Gulf Coast
	aga8parameters.MFC[CID_METHANE] = 90.6724;
	aga8parameters.MFC[CID_NITROGEN] = 3.1284;
	aga8parameters.MFC[CID_CARBON_DIOXIDE] = 0.4676;
	aga8parameters.MFC[CID_ETHANE] = 4.5279;
	aga8parameters.MFC[CID_PROPANE] = 0.828;
	aga8parameters.MFC[CID_I_BUTANE] = 0.1037;
	aga8parameters.MFC[CID_N_BUTANE] = 0.1563;
	aga8parameters.MFC[CID_I_PENTANE] = 0.0321;
	aga8parameters.MFC[CID_N_PENTANE] = 0.0443;
	aga8parameters.MFC[CID_N_HEXANE] = 0.0393;
	aga8parameters.MFC[CID_N_HEPTANE] = 0.0000;
	aga8parameters.MFC[CID_N_OCTANE] = 0.0000;

	aga8parameters.ncc = 12;

	aga8parameters.ci[1] = CID_METHANE;
	aga8parameters.ci[2] = CID_NITROGEN;
	aga8parameters.ci[3] = CID_CARBON_DIOXIDE;
	aga8parameters.ci[4] = CID_ETHANE;
	aga8parameters.ci[5] = CID_PROPANE;
	aga8parameters.ci[6] = CID_I_BUTANE;
	aga8parameters.ci[7] = CID_N_BUTANE;
	aga8parameters.ci[8] = CID_I_PENTANE;
	aga8parameters.ci[9] = CID_N_PENTANE;
	aga8parameters.ci[10] = CID_N_HEXANE;
	aga8parameters.ci[11] = CID_N_HEPTANE;
	aga8parameters.ci[12] = CID_N_OCTANE;


	aga8parameters.Tb = 288.15;
    aga8parameters.Pb = 0.101325;

}

void testAGA7(double s) {

   double vb,vf,Fpm,Fpv,Ftm,Ftb,Fpb,tb,pb,pf,pa,tf;

   pb = 101.325; /* Kilo Psacal */
   tb = 15; /* celsius */
   vf = 40;
   tf = 10;
   pa = 101.325; /* Kilo Pascal */
   pf = 4000;  /* Kilo Pascal */
   Fpm = (pf + pa) / pb;
   Fpb = 1;
   Ftm = (tb + 273.15) /(tf + 273.15);
   Ftb = 1;
   //s = 0.997761/0.906663;

   vb = vf*Fpm*Fpb*Ftm*Ftb*s;

   printf("Vlume at base conditions is: %f\n",vb);

}

void sample01(void) {
	parameters.a1 = 0.00000925; /*OP*/
	parameters.a2 = 0.00000620; /*pipe*/
	parameters.Tr = 68;
	parameters.dr = 2.000;
	parameters.Dr = 4.025;
	parameters.u  = 0.010268; /* cp for natural gas */
	parameters.COpressureTaps = CO_PRESSURE_TAPS_FLANGE;


	setUnits_US(&parameters);

	measuredvalues.Tf = 86;
	measuredvalues.Pf2 = 199.3;
	measuredvalues.DP = 20;

	/* This value needs to be calculated using AGA 8 */
	measuredvalues.pf = 0.660758;

}


void main(void) {

    DPFLOAT tb,db,zb,pb;
    DPFLOAT tf,df,zf,pf;
    
    testUnits();
    
    init_bip_tables();
    
    //   initAga8ParamFlowCheck();
    initAga8Param();
    
    calc_CHARDL(&aga8parameters,&aga8calculated);
    
    printf("Molar Mass: %f\n",aga8calculated.MWX);
    
    tf = 273.15;
    tf = 283.15;
    tb = 310.92777;
    pb = 2.757902;
    pb = 2.0684;
    
    calc_TEMP(&aga8parameters,&aga8calculated,tb);
    
    db = calc_DDETAIL(&aga8parameters,&aga8calculated,pb,tb);
    zb = calc_ZDETAIL(&aga8parameters,&aga8calculated,db,tb);
    
    printf("For BAse T:%f\tP:%f\tZ:%f\tD:%f\n",tb,pb,zb,db);
    
    calc_TEMP(&aga8parameters,&aga8calculated,tf);
    
    //	df = calc_DDETAIL(&aga8parameters,&aga8calculated,4.00,tf);
    //	zf = calc_ZDETAIL(&aga8parameters,&aga8calculated,df,tf);
    
    df = calc_DDETAIL(&aga8parameters,&aga8calculated,4.101325,tf);
    zf = calc_ZDETAIL(&aga8parameters,&aga8calculated,df,tf);
    
    printf("For Flow T:%f\tP:%f\tZ:%f\tD:%f\n",tf,pf,zf,df);
    
    testAGA7(zb/zf);
    /*   testUnits();*/
    /*	calcAga8();*/
    
    getch();
}