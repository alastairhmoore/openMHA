// This file is a third party plugin for use with with the HörTech Open 
// Master Hearing Aid (openMHA)
// Copyright © 2023 Alastair H. Moore
// 
// The structure and style is derived from openMHA 
//
// pos2doa is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// pos2doa is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License, version 3 for more details.
//
// You should have received a copy of the GNU Affero General Public License, 
// version 3 along with openMHA.  If not, see <http://www.gnu.org/licenses/>.

/*
 * An \mha plugin which leaves the audio untouced.
 *
 * This plugin converts ac parameters providing an interface between real-world
 * parameters and parameters which can be used by other plugins.
 * 
 * v0: Empty structure - No conversion takes place
 */

#include "mha_plugin.hh"
#include <math.h>


#ifndef POS2DOA_H
#define POS2DOA_H

#define DEG2RAD M_PI/180.0
#define RAD2DEG 180.0/M_PI


// class pos2doa_config {

// public:
//     pos2doa_config(MHA_AC::algo_comm_t & ac,
//                    const mhaconfig_t in_cfg,
//                    pos2doa_t *pos2doa_t);
//     ~pos2doa_config();
//     mha_spec_t* process(mha_spec_t*);

// private:
//     unsigned int nchan;
//     unsigned int nfreq;
//     MHASignal::spectrum_t outSpec;
//     mha_spec_t bf_vec;
//     unsigned int nangle;
//     steerbf *_steerbf;
//     MHA_AC::algo_comm_t & ac;
//     std::string bf_src_copy;
// };



class pos2doa_t : public MHAPlugin::plugin_t<int> {

public:
    pos2doa_t(MHA_AC::algo_comm_t & iac, const std::string & configured_name);
    ~pos2doa_t();
    void prepare(mhaconfig_t&);
    void release(void) {/* Do nothing in release */}

    // audio processing functions just call the ac processing and return unchangd audio
    mha_wave_t* process(mha_wave_t* s){process(); return s;}; 
    mha_spec_t* process(mha_spec_t* s){process(); return s;}; 

    //declare MHAParser variables here
    std::string ac_name_pos_in;
    std::string ac_name_az_deg_out;
    std::string ac_name_steerbf_index_out;

    // MHAParser::string_t ac_name_pos_in;   // the string itself is obtained from ac_name_pos_in.data
    // MHAParser::string_t az_name_az_deg_out;
    
private:
    void process(); // implementation of ac processing will go in here
    void update_cfg();

    /* patch bay for connecting configuration parser
       events with local member functions: */
    // MHAEvents::patchbay_t<pos2doa_t> patchbay;

    /* Azimuth of object (propagated to AC handle): */
    float az_deg;
    // int steer_index;
    MHA_AC::float_t steer_index;
};

#endif // POS2DOA_H

// Local Variables:
// compile-command: "make"
// c-basic-offset: 4
// indent-tabs-mode: nil
// coding: utf-8-unix
// End:
