/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_RIM_START_BLOCK_H
#define _GADGET_RIM_START_BLOCK_H

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/Util/Debug.h>
#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>

namespace cluster
{

class GADGET_CLASS_API StartBlock : public Packet
{
public:
   StartBlock()
   {;}

   /**
    * Create a StartBlock packet to signal that the local node is ready to start.
    *
    * @param plugin_guid -GUID of the ClusterPlugin that should handle this packet.
    * @param frame_number -The current number of frames that have been drawn.
    */
   StartBlock(const vpr::GUID& plugin_id, const vpr::Uint32& frame_number)
   {
      // Set the local member variables using the given values.
      mPluginId = plugin_id;
   
      // Create a Header for this packet with the correect type and size.
      mHeader = new Header(Header::RIM_PACKET,
                                       Header::RIM_START_BLOCK,
                                       Header::RIM_PACKET_HEAD_SIZE
                                       +16/*Plugin GUID*/
                                       ,frame_number);
      // Serialize the given data.
      serialize();
   }

   
   /**
    * Serializes member variables into a data stream.
    */
   void serialize()
   {
      // Clear the data stream.
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Serialize the header.
      mHeader->serializeHeader();
      
      // Serialize the Plugin ID
      mPluginId.writeObject(mPacketWriter);
   }

   /**
    * Parses the data stream into the local member variables.
    */
   virtual void parse(vpr::BufferObjectReader* reader)
   {
      // De-Serialize the Plugin ID
      mPluginId.readObject(reader);
   }
   
   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         <<  clrOutBOLD(clrYELLOW,"====== Start BLOCK ======\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);
      
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Plugin ID:    ") << mPluginId.toString()
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         <<  clrOutBOLD(clrYELLOW,"=======================\n") << vprDEBUG_FLUSH;
         
   }
   
   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getPacketFactoryType()
   {
       return(Header::RIM_START_BLOCK);
   }
};
}

#endif




