
void sample03(void) {
	parameters.a1 = 0.00001665; /*OP*/
	parameters.a2 = 0.00001116; /*pipe*/
	parameters.Tr = 15;
	parameters.dr = 50.8;
	parameters.Dr = 102.24;
	parameters.u  = 0.010268; /* cp for natural gas */
	parameters.COpressureTaps = CO_PRESSURE_TAPS_FLANGE;


	setUnits_METRIC(&parameters);

	measuredvalues.Tf = 30.0;
	measuredvalues.Pf2 = 13.74;
	measuredvalues.DP = 49.8;

	/* This value needs to be calculated using AGA 8 */
	measuredvalues.pf = 10.5834;

}

