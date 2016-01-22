/* Gadgeteer Driver for 'A.R.T. DTrack' Tracker
 * Copyright (C) 2005-2014, Advanced Realtime Tracking GmbH
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Purpose: Gadgeteer driver class
 *
 * Authors: Sylvain Brandel, LSIIT-IGG (http://igg.u-strasbg.fr)
 *          Kurt Achatz, Advanced Realtime Tracking GmbH (http://www.ar-tracking.de)
 *          Viktor Mukha, Advanced Realtime Tracking GmbH (http://www.ar-tracking.de)
 *
 */

#ifndef _GADGET_DTRACKWRAPPER_H_
#define _GADGET_DTRACKWRAPPER_H_

#include <gadget/Devices/DriverConfig.h>

#include <boost/mpl/inherit.hpp>

#include <gadget/Type/InputDevice.h>

#include "DTrackStandalone.h"


namespace gadget
{

class DTrackWrapper
        : public InputDevice< boost::mpl::inherit< Digital, Analog, Position >::type >
{
public:
	DTrackWrapper();
	~DTrackWrapper();
	
	/** Returns a string that matches this device's configuration element type. */
	static std::string getElementType();
	
	/** Passes configuration data to the driver. */
	virtual bool config( jccl::ConfigElementPtr e );
	
	/** Begins sampling. */
	virtual bool startSampling();
	
	/** Samples a value. */
	virtual bool sample();
	
	/** Stops sampling. */
	virtual bool stopSampling();
	
	/** Swap the data indices. */
	virtual void updateData();
	
private:
	vpr::Thread* thrThread;
	void thrFunction();
	bool thrRunning;
	
	DTrackStandalone* standalone;
	int receive_port;
	bool use_commands;
	std::string server_name;
	int command_port;
	
	std::vector< PositionData > curPosition;
	std::vector< DigitalData > curDigital;
	std::vector< AnalogData > curAnalog;
	
	void resize_curPosition( int n );
	void resize_curDigital( int n );
	void resize_curAnalog( int n );
	
	gmtl::Matrix44f getpos( dtrack_body_type& bod );
	gmtl::Matrix44f getpos( dtrack_flystick_type& bod );
	gmtl::Matrix44f getpos( dtrack_meatool_type& bod );
	gmtl::Matrix44f getpos_default();
};

} // End of gadget namespace


#endif  // _GADGET_DTRACKWRAPPER_H_


