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

/** Do-nothing constructor.  The constructor has to take these two
 * arguments, but it does not have to use them.  The base
 * class has to be initialized.
 * More advanced plugins use a custom configuration class for the template MHAPlugin::plugin_t
 */
pos2doa_t::pos2doa_t(MHA_AC::algo_comm_t & iac, const std::string & configured_name)
    : MHAPlugin::plugin_t<int>("pos2doa manipulates ac variables",iac)
{
    /* iac is passed to the base class where it is assiged to the member variable ac */
    (void)configured_name;/* ignore 2nd parameter */
}

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
