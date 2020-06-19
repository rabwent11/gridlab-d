// $Id: sync_check.h
//	Copyright (C) 2020 Battelle Memorial Institute

#ifndef GLD_POWERFLOW_SYNC_CHECK_H_
#define GLD_POWERFLOW_SYNC_CHECK_H_

#include "powerflow.h"

EXPORT SIMULATIONMODE interupdate_sync_check(OBJECT *obj, unsigned int64 delta_time, unsigned long dt, unsigned int iteration_count_val, bool interupdate_pos);

class sync_check : public powerflow_object
{
public:
	static CLASS *oclass;
	static CLASS *pclass;

public:
	sync_check(MODULE *mod);
	sync_check(CLASS *cl = oclass) : powerflow_object(cl){};
	int create(void);
	int init(OBJECT *parent = NULL);
	int isa(char *classname);

	TIMESTAMP presync(TIMESTAMP t0);
	TIMESTAMP sync(TIMESTAMP t0);
	TIMESTAMP postsync(TIMESTAMP t0);

	SIMULATIONMODE inter_deltaupdate_sync_check(unsigned int64 delta_time, unsigned long dt, unsigned int iteration_count_val, bool interupdate_pos);

private:
	/* Flags & Buffer */
	bool reg_dm_flag;		  // Flag for indicating the registration of deltamode (array & func)
	bool deltamode_inclusive; // Boolean for deltamode calls - pulled from object flags

	bool metrics_flag; // True - both metrics are satisfied, False - otherwise
	double t_sat;	   // Timer for checking the metrics

	gld_property *temp_property_pointer; // temporary pointer of property

private: /* Measurements, actuator, gld objs, & Nominal Values */
	double volt_norm;

	gld_object *swt_fm_node;
	gld_object *swt_to_node;

	double swt_fm_node_freq;
	double swt_to_node_freq;

	gld_property *prop_fm_node_freq;
	gld_property *prop_to_node_freq;

	complex swt_fm_volt_A;
	complex swt_fm_volt_B;
	complex swt_fm_volt_C;

	gld_property *prop_fm_node_volt_A;
	gld_property *prop_fm_node_volt_B;
	gld_property *prop_fm_node_volt_C;

	complex swt_to_volt_A;
	complex swt_to_volt_B;
	complex swt_to_volt_C;

	gld_property *prop_to_node_volt_A;
	gld_property *prop_to_node_volt_B;
	gld_property *prop_to_node_volt_C;

	gld_property *swt_prop_status;

	set swt_phases;
	bool swt_ph_A_flag;
	bool swt_ph_B_flag;
	bool swt_ph_C_flag;

private: /* Published Variables*/
	bool sc_enabled_flag;

	double frequency_tolerance_hz;
	double voltage_tolerance_pu;
	double voltage_tolerance;
	double metrics_period_sec;

	enum VOLT_COMP_MODE
	{
		MAG_DIFF = 0,
		SEP_DIFF = 1
	} volt_compare_mode;

	double voltage_magnitude_tolerance_pu;
	double voltage_magnitude_tolerance;
	double voltage_angle_tolerance_deg;

	//Deltamode trigger/maintainer variables
	double delta_trigger_mult;
	double frequency_tolerance_hz_deltamode_trig;
	double voltage_tolerance_pu_deltamode_trig;
	double voltage_magnitude_tolerance_pu_deltamode_trig;
	double voltage_angle_tolerance_deg_deltamode_trig;
	bool deltamode_trigger_keep_flag;
	TIMESTAMP next_trigger_update_time;
	SIMULATIONMODE deltamode_check_return_val;

private:
	/* Funcs mainly used in create() */
	void init_vars();

	/* Funcs mainly used in init() */
	void data_sanity_check(OBJECT *par = NULL);
	void reg_deltamode_check();
	void init_norm_values(OBJECT *par = NULL);
	void init_sensors(OBJECT *par = NULL);

	/* FUncs mainly used in presync() */
	void reg_deltamode();

private: /* Funcs for Deltamode */
	void update_measurements();
	void check_metrics(bool deltamode_check);
	void check_excitation(unsigned long);
	void reset_after_excitation();
};

// Utility Funcs
// TODO: make a func for the get property process

#endif // GLD_POWERFLOW_SYNC_CHECK_H_