/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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


/////////////////////////////////////////////////////////////////////////////
//
// Group manager for vj input devices and proxies
//
////////////////////////////////////////////////////////////////////////////
#ifndef VJ_INPUT_MANAGER_H
#define VJ_INPUT_MANAGER_H

#include <vjConfig.h>
#include <map>

#include <Kernel/vjDebug.h>
#include <Kernel/vjConfigChunkHandler.h>

// Proxies
#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjKeyboardProxy.h>
#include <Input/InputManager/vjGestureProxy.h>

//: The InputManager holds an manages all vj Input devices.
//
//  The InputManager, handles all the
//  details behind organizing the input devices in the library.  It
//  provides an API for adding devices by their configChunk and deleting
//  them by their chunk (or just their string name).
//
//  Proxies are used to abstract away from the devices so any type and number
//  of devices may be accessed in the same way.  These proxies are also
//  set up by configChunks and should be accessed by number, rather than name.
//  (for speed)
//-------------------------------------------------------------------------------
//!PUBLIC_API:
class vjInputManager : public vjConfigChunkHandler
{
public:
   vjInputManager();
   virtual ~vjInputManager();

   friend std::ostream& operator<<(std::ostream& out, vjInputManager& iMgr);

 //---------------------------//
 //      CONFIG               //
 //---------------------------//
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //! POST: (chunk is proxy) ==> (returns == false)
   //+       (chunk is device) ==> (devices is removed && proxies are stupified)
   //+       (chunk is proxyAlias) ==> (proxyAlias is removed && devInterfaces.refreshAll())
   //!RETURNS: success
   bool configRemove(vjConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   bool configCanHandle(vjConfigChunk* chunk);

private:
   //: Load the device for the given chunk
   //!RETURNS: true - Device was configured and added
   bool configureDevice(vjConfigChunk* chunk);

   //: Load the Proxy for the given chunk
   //!RETURNS: true - Proxy was configured and added
   bool configureProxy(vjConfigChunk* chunk);

   //: Remove the device associated with the given chunk
   //!RETURNS: true - Device was removed
   bool removeDevice(vjConfigChunk* chunk);


   // ------------------------------- //
   //        DEVICE API               //
   // ------------------------------- //
public:
   //: Update the Data in all the devices.
   //
   // Call UpdateData() on all the devices in the devArray
   // Also updates all proxies.
   //
   //! MODIFIES: instances pointed to by devArray
   void updateAllData();

private:
   //: Add a device to vjInputManager.
   //
   // Add the devPtr to the device Array, devPtr should
   // not already be in the array.  Returns -1 on failure
   //
   //! MODIFIES: self
   //! POST: m_devVector' = m_devVector \/ devPtr
   //+       return = devNum (position in the array)
   //                or -1 for fail
   int addDevice(vjInput* devPtr);

   //: Remove a device from the vjInputManager.
   // Remove the device at position devNum from the
   // device Array.  Returns true on success.
   //
   //! MODIFIES: self
   //! POST: m_devVector[devNum]' = NULL
   bool removeDevice(int devNum);
   bool removeDevice(char* instName);

   //: Get the device number from its Instance Name.
   //
   //  Find the devNum of the device whose name is instName
   //
   //! RETURNS: -1 if fails
   int findDeviceNum(const char* instName);

   //: Get a pointer to the instance at devNum.
   // return the pointer at the devNum position in the array. <br>
   //
   //!POST: return = m_devVector[devNum]  (this can be NULL)
   vjInput* getDevice(unsigned int devNum);

public:
   // Return a vjInput ptr to a deviced named
   //!RETURN: NULL - Not found
   vjInput* getDevice(std::string deviceName);


   /*********************************************************
    *          PROXIES                                      *
    *********************************************************/
public:

   //: Add a proxy to the proxy table
   //! RETURN: true - added correctly
   bool addProxy(std::string proxyName, vjProxy* proxy);

   //: Get a proxy for the given proxy name (or alias)
   //! RETURNS: NULL - Not found
   vjProxy* getProxy(std::string proxyName);

protected:
   std::vector<vjInput*>                  m_devVector;
   std::map<std::string, vjProxy*>        mProxyTable;      // list of proxies in the system
   std::map<std::string, std::string>     mProxyAliases;     // List of alias names for proxies

private:
   //: Function to configure the proxy Alias array
   bool configureProxyAlias(vjConfigChunk* chunk);

   //: Remove a proxy alias
   bool removeProxyAlias(vjConfigChunk* chunk);

   //: Add a proxy alias
   void addProxyAlias(std::string alias_name, std::string proxy_name);
};

// Write out the status of the input manager
std::ostream& operator<<(std::ostream& out, vjInputManager& iMgr);

#endif
