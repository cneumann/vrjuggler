/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _JCCL_XMLCONFIGCOMMUNICATOR_H_
#define _JCCL_XMLCONFIGCOMMUNICATOR_H_

#include <jccl/jcclConfig.h>
#include <jccl/Net/NetCommunicator.h>
#include <cppdom/cppdom.h>

namespace jccl
{

class Connect;
class ConfigManager;


/** Communications protocol handler for the Environment Manager.
 *  This design is based off of Control's
 *  Components/Network/NetCommunicator pluggable communications
 *  interface.
 */
class XMLConfigCommunicator: public NetCommunicator
{
private:

   ConfigManager* mConfigManager;

public:


   /** Constructor */
   XMLConfigCommunicator( ConfigManager* _config_manager );


   /** Destructor */
   virtual ~XMLConfigCommunicator();


   //: Called when a new connection is established.
   //  Used to send any initial messages on a new connection.
   //  Useful for any initial data queries that need to be sent
   //  when a new connection is opened.  Frex, requesting the
   //  current configuration of whatever's on the other side of
   //  the connection.
   //! PRE: _connection is open & valid for writing to; connection
   //+      is NULL.
   //! POST: true.
   virtual void initConnection( Connect* _connection );


   //: Called when the connection is shut down.
   //  Useful for doing things like removing the Active chunkdb/descdb
   //  (at least on control's side).
   //! PRE: connection != NULL.
   //! POST: connection = NULL.
   virtual void shutdownConnection();


   //: True if the identifier represents a stream we can read.
   //  The id is the token in the handler attribute of a
   //  protocol tag in the EM/C command stream.  A given
   //  NetCommunicator can theoretically serve several handlers;
   //  this is useful for backwards compatibility.
   //! RETURNS: True - if self knows how to parse this stream.
   //! RETURNS: False - otherwise.
   virtual bool acceptsStreamIdentifier( const std::string& id );


   //: Reads data from a communications stream.
   //  This should only be called by the Connect object self is
   //  owned by.
   //  The Communicator should read data until it reaches the end of
   //  the protocol stream (signified by the character string
   //  "</protocol>".  readStream should read that string and
   //  absolutely no further (as this can confuse parsing of later
   //  protocol streams on the same connection).
   //! PRE: connection != NULL;
   //! RETURNS: true - if reading the protocol stream was succesful.
   //! RETURNS: false - if EOF or a fatal error occurs.  This will
   //+                  kill the Connect.
   virtual bool readStream( Connect* con, std::istream& instream );

   bool interpretNode( Connect* con, cppdom::XMLNodePtr doc );


   //----------------------- ConfigStatus Stuff ------------------------

   void configChanged();


}; // class XMLConfigCommunicator


}; // namespace jccl

#endif