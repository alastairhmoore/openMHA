// This file is part of the HörTech Open Master Hearing Aid (openMHA)
// Copyright © 2015 2018 2019 2021 HörTech gGmbH
// Copyright © 2022 Hörzentrum Oldenburg gGmbH
//
// openMHA is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// openMHA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License, version 3 for more details.
//
// You should have received a copy of the GNU Affero General Public License, 
// version 3 along with openMHA.  If not, see <http://www.gnu.org/licenses/>.

#include "pos2doa.h"

// #define PATCH_VAR(var) patchbay.connect(&var.valuechanged, this, &acSteer::update_cfg)
// #define INSERT_PATCH(var) insert_member(var); PATCH_VAR(var)

/** Construct the plugin
 * 
 */
pos2doa_t::pos2doa_t(MHA_AC::algo_comm_t & iac, const std::string & configured_name)
    : MHAPlugin::plugin_t<int>("pos2doa manipulates ac variables",iac),
    steer_index(ac, "acSteerIndex", 36, true)
{
    /* iac is passed to the base class where it is assiged to the member variable ac */
    (void)configured_name;/* ignore 2nd parameter */

    ac_name_pos_in = "acSteerPos";
    ac_name_rot_in = "acHeadRot";
    ac_name_az_deg_out = "acSteerAzDeg";
    // ac_name_steerbf_index_out = "acSteerIndex";

    // chaging these requires a new configuration
    // INSERT_PATCH(variable_name);

    //changing these does not require a configuration change
    // insert_member(angle_ind);
    // insert_member(angle_src);
    
    ac.insert_var_float(ac_name_az_deg_out, &az_deg);
    // ac.insert_var_int(ac_name_steerbf_index_out, &steer_index);

}

/** Deconstructor */
pos2doa_t::~pos2doa_t(){}

/** Plugin preparation.
 * 
 * @param signal_info 
 *   Structure containing a description of the form of the signal (domain,
 *   number of channels, frames per block, sampling rate.
 */
void pos2doa_t::prepare(mhaconfig_t & signal_info)
{
    /* may need to do stuff here in the future */
    (void)signal_info;/* ignore 2nd parameter */  
}


/** Does the actual processing of ac variables
 * 
 */
void pos2doa_t::process()
{
    /* Do stuff here */
    // assuming receiver is at origin, get the doa as a cartesian vector
    const std::vector<float> pos = MHA_AC::get_var_vfloat(ac, ac_name_pos_in);
    double x = pos[0];
    double y = pos[1];
    double z = pos[2];


    // get rotation of head in TASCAR Euler coordinates (applied in sequence)
    // rotZ, rotY, rotX
    const std::vector<float> rot = MHA_AC::get_var_vfloat(ac, ac_name_rot_in);

    // apply inverse rotation to target vector (code from TASCAR coordinates.h)
    // -rotX, -rotY, -rotZ
    // rotX
    double a = DEG2RAD * -rot[2];
    if(a != 0) {
        // 1   0    0
        // 0  cos -sin
        // 0  sin  cos
        double zn = cos(a) * z + sin(a) * y;
        double yn = cos(a) * y - sin(a) * z;
        z = zn;
        y = yn;
    }
    // rotY
    a = DEG2RAD * -rot[1];
    if(a != 0) {
        //  cos 0  sin
        //   0  1   0
        // -sin 0  cos
        double xn = cos(a) * x + sin(a) * z;
        double zn = cos(a) * z - sin(a) * x;
        z = zn;
        x = xn;
    }
    // rotZ
    a = DEG2RAD * -rot[0];
    if(a != 0) {
        // cos -sin  0
        // sin  cos  0
        //  0    0   1
        double xn = cos(a) * x - sin(a) * y;
        double yn = cos(a) * y + sin(a) * x;
        x = xn;
        y = yn;
    }

    // convert doa to spherical coordinates
    az_deg = RAD2DEG * atan2f(y, x);


    /* directly calculate mapping
     * TODO: use lookup table
     */
    int i_of_0 = 36;
    int nangles = 72; // our actual steerbf weights have 73 entries becasue it includes ±180 degrees
    float az_deg_ = fmod(az_deg, 360);
    if (az_deg_ < 0)
        az_deg_ += 360;
    // az_deg_ = round(az_deg_); // this gives integer version which could be used for LUT
    int steer_index_ = i_of_0 + round(az_deg_ / 5);
    if (steer_index_ >= nangles)
        steer_index_ -= nangles;

    /* write the value to the required ac variable */
    ac.insert_var_float(ac_name_az_deg_out, &az_deg);
    //ac.insert_var_int(ac_name_steerbf_index_out, &steer_index);
    steer_index.data = (float) steer_index_;
    steer_index.insert();
}



/*
 * This macro connects the pos2doa_t class with the \mha plugin C interface
 * The first argument is the class name, the other arguments define the 
 * input and output domain of the algorithm.
 * 
 * Since we accept both wave in/out and spec in/out two callbacks are registered
 */
MHAPLUGIN_CALLBACKS(pos2doa,pos2doa_t,wave,wave)
MHAPLUGIN_PROC_CALLBACK(pos2doa,pos2doa_t,spec,spec)


/*
 * This macro creates code classification of the plugin and for
 * automatic documentation.
 *
 * The first argument to the macro is a space separated list of
 * categories, starting with the most relevant category. The second
 * argument is a LaTeX-compatible character array with some detailed
 * documentation of the plugin.
 */
MHAPLUGIN_DOCUMENTATION\
(pos2doa,
 "algorithm-communication data-flow",
 "An empty plugin ready for doing ac variable conversion\n\n"
 "This plugin converts ac variables."
 )



// Local Variables:
// compile-command: "make"
// c-basic-offset: 4
// indent-tabs-mode: nil
// coding: utf-8-unix
// End:
