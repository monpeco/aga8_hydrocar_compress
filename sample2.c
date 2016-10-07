void sample02(void) {
	parameters.a1 = 0.00000925; /*OP*/
	parameters.a2 = 0.00000620; /*pipe*/
	parameters.Tr = 68;
	parameters.dr = 2.000;
	parameters.Dr = 4.025;
	parameters.u  = 0.010268; /* cp for natural gas */
	parameters.COpressureTaps = CO_PRESSURE_TAPS_FLANGE;


	setUnits_US(&parameters);

	measuredvalues.Tf = 86;
	measuredvalues.Pf1 = 200.0;
	measuredvalues.DP = 20.0;

	/* This value needs to be calculated using AGA 8 */
	measuredvalues.pf = 0.659307;

}
