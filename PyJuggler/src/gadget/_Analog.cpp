/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/Analog.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_Analog_Wrapper: gadget::Analog
{
    gadget_Analog_Wrapper(PyObject* self_):
        gadget::Analog(), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::Analog::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::Analog::readObject(p0);
    }

    bool config(jccl::ConfigElementPtr p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::Analog::config(p0);
    }

    std::string getBaseType() {
        try
        {
            return call_method< std::string >(self, "getBaseType");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKONWN");
    }

    std::string default_getBaseType() {
        return gadget::Analog::getBaseType();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Analog_getAnalogData_overloads_0_1, getAnalogData, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_Analog()
{
    class_< gadget::Analog, boost::noncopyable, pyj::gadget_Analog_Wrapper >("Analog", init<  >())
        .def("writeObject", (vpr::ReturnStatus (gadget::Analog::*)(vpr::ObjectWriter*) )&gadget::Analog::writeObject, (vpr::ReturnStatus (pyj::gadget_Analog_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Analog_Wrapper::default_writeObject)
        .def("readObject", (vpr::ReturnStatus (gadget::Analog::*)(vpr::ObjectReader*) )&gadget::Analog::readObject, (vpr::ReturnStatus (pyj::gadget_Analog_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Analog_Wrapper::default_readObject)
        .def("config", &gadget::Analog::config, &pyj::gadget_Analog_Wrapper::default_config)
        .def("getBaseType", &gadget::Analog::getBaseType, &pyj::gadget_Analog_Wrapper::default_getBaseType)
        .def("getAnalogData", &gadget::Analog::getAnalogData, pyj::gadget_Analog_getAnalogData_overloads_0_1())
        .def("addAnalogSample", &gadget::Analog::addAnalogSample)
        .def("swapAnalogBuffers", &gadget::Analog::swapAnalogBuffers)
        .def("getAnalogDataBuffer", &gadget::Analog::getAnalogDataBuffer, return_internal_reference< 1 >())
    ;

}
