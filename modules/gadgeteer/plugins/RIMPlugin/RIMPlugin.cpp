/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <cluster/PluginConfig.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <plugins/RIMPlugin/RIMPlugin.h>

// Sharing Devices
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/InputManager.h>
#include <plugins/RIMPlugin/DeviceServer.h>
#include <plugins/RIMPlugin/VirtualDevice.h>

// ClusterNetwork
#include <cluster/ClusterNetwork.h>
#include <gadget/Node.h>
#include <cluster/ClusterManager.h>

// IO Packets
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/DeviceAck.h>
#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/DataPacket.h>

// Configuration
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/ConfigElement.h>

#include <boost/lexical_cast.hpp>
#include <map>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT vpr::Uint32 getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT void initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(cluster::RIMPlugin::create());
   }
}

namespace cluster
{

struct DeviceNamePred
{
   DeviceNamePred(const std::string& deviceName)
      : mDeviceName(deviceName)
   {
      /* Do nothing. */ ;
   }

   bool operator()(DeviceServerPtr ds)
   {
      return ds->getName() == mDeviceName;
   }

   const std::string& mDeviceName;
};

RIMPlugin::RIMPlugin()
   : mHandlerGUID("9c3fb301-b142-4c6f-8ca3-1570898974d0")
{;}

RIMPlugin::~RIMPlugin()
{
   mVirtualDevices.clear();
   mDeviceServers.clear();

   // TODO: Make ConfigManager use shared_ptrs.
   jccl::ConfigManager::instance()->removeConfigElementHandler(this);
}

ClusterPluginPtr RIMPlugin::create()
{
   RIMPlugin* rim_plugin = new RIMPlugin();

   // TODO: Make ConfigManager use shared_ptrs.
   jccl::ConfigManager::instance()->addConfigElementHandler(rim_plugin);
   return ClusterPluginPtr(rim_plugin);
}

void RIMPlugin::preDraw()
{
   // Do nothing we are only here to sync.
}

void RIMPlugin::postPostFrame()
{
   // Update all local device servers and send their data.
   for ( device_server_list_t::iterator itr = mDeviceServers.begin(); itr != mDeviceServers.end(); itr++ )
   {
      (*itr)->updateLocalData();
      (*itr)->send();
   }

   //      second.setNow();
   //      vpr::Interval diff_time4(second-first);
   //      std::cout << "Recv DeviceData Time: " << diff_time4.getBaseVal() << std::endl;
}

/** Add the pending elm to the configuration.
 *  @pre configCanHandle (elm) == true.
 *  @return true iff elm was successfully added to configuration.
 */
bool RIMPlugin::configAdd(jccl::ConfigElementPtr elm)
{
   vprASSERT(ClusterManager::instance()->recognizeRemoteDeviceConfig(elm));
   vprASSERT(ClusterManager::instance()->isClusterActive());
   std::string device_name = elm->getName();

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
      << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
      << "Adding device: " << device_name
      << std::endl << vprDEBUG_FLUSH;

   vprASSERT(cluster::ClusterManager::instance()->isClusterActive() && "RIM called in non-cluster mode.");
   bool master = cluster::ClusterManager::instance()->isMaster();
   bool result(false);

   // If we are the master, configure the device and tell all slaves to prepare
   // virtual devices.
   if (master)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
         << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
         << "Configuring device on master node: " << device_name
         << std::endl << vprDEBUG_FLUSH;

      gadget::InputManager::instance()->configureDevice(elm);
      gadget::InputPtr input_device = gadget::InputManager::instance()->getDevice(device_name);
      if ( input_device != NULL )
      {
         result = addDeviceServer(device_name, input_device);
         DeviceServerPtr device_server = getDeviceServer(device_name);
         vprASSERT(NULL != device_server.get() && "Must have device server.");

         const vpr::Uint16 dev_type_id(input_device->getTypeId());
         const vpr::GUID& temp_guid(device_server->getId());
         DeviceAckPtr device_ack = DeviceAck::create(mHandlerGUID, temp_guid,
                                                     device_name, dev_type_id,
                                                     true);

         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
            << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
            << "Sending device ack [" << device_name << "] to all cluster nodes."
            << std::endl << vprDEBUG_FLUSH;
         cluster::ClusterManager::instance()->getNetwork()->sendToAll(device_ack);
      }
   }
   else
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
         << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
         << "Configuring device on slave node: " << device_name
         << std::endl << vprDEBUG_FLUSH;

      result = true;
   }

   return result;
}

/** Remove the pending element from the current configuration.
 *  @pre configCanHandle (element) == true.
 *  @return true iff the element (and any objects it represented)
 *          were successfully removed.
 */
bool RIMPlugin::configRemove(jccl::ConfigElementPtr elm)
{
   boost::ignore_unused_variable_warning(elm);
   return false;
}

/** Checks if this handler can process element.
 *  Typically, an implementation of handler will check the element's
 *  description name/token to decide if it knows how to deal with
 *  it.
 *  @return true iff this handler can process element.
 */
bool RIMPlugin::configCanHandle(jccl::ConfigElementPtr elm)
{
   return ClusterManager::instance()->recognizeRemoteDeviceConfig(elm);
}

bool RIMPlugin::isPluginReady()
{
   return true;
}

void RIMPlugin::recoverFromLostNode(gadget::NodePtr lostNode)
{
   removeVirtualDevicesOnHost(lostNode->getHostname());
}

/**
 * Handle a incoming packet.
 */
void RIMPlugin::handlePacket(cluster::PacketPtr packet, gadget::NodePtr node)
{
   //We are only handling data packets right now.
   if ( NULL != packet.get() && NULL != node.get() )
   {
      switch ( packet->getPacketType() )
      {
      case cluster::Header::RIM_DEVICE_ACK:
         {
            cluster::DeviceAckPtr device_ack = boost::dynamic_pointer_cast<cluster::DeviceAck>(packet);
            vprASSERT(NULL != device_ack.get() && "Dynamic cast failed!");
            std::string device_name = device_ack->getDeviceName();
            vprASSERT(device_ack->getAck() && "We only have device ACKs now.");

            gadget::InputPtr input_dev = getVirtualDevice(device_name);
            if ( NULL != input_dev.get() )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED, "ERROR:")
               << "Somehow we already have a virtual device named: " << device_name << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               addVirtualDevice(device_ack->getId(), device_name,
                                device_ack->getDeviceBaseType(),
                                device_ack->getHostname());

               // Add this virtual device to the InputManager's list of devices.
               input_dev = getVirtualDevice(device_name);
               vprASSERT(NULL != input_dev.get() && "Can't have a NULL device.");
               gadget::InputManager::instance()->addRemoteDevice(input_dev, device_name);

               // obtain config element for device
               jccl::ConfigElementPtr dev_cfg =
                  jccl::ConfigManager::instance()->getElementNamed(device_name);

               if ( NULL != dev_cfg.get() )
               {
                  input_dev->config(dev_cfg);
               }
               else
               {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                     << clrOutBOLD(clrRED, "ERROR:")
                     << "[RIMPlugin::handlePacket] No config element found for device '"
                     << device_name << "'!\n"
                     << vprDEBUG_FLUSH;
               }
            }
            break;
         }
      case cluster::Header::RIM_DATA_PACKET:
         {
            cluster::DataPacketPtr data_packet = boost::dynamic_pointer_cast<cluster::DataPacket>(packet);
            vprASSERT(NULL != data_packet.get() && "Dynamic cast failed!");

            //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "RIM::handlePacket()..." << std::endl <<  vprDEBUG_FLUSH;
            //data_packet->printData(1);

            gadget::InputPtr virtual_device = getVirtualDevice(data_packet->getObjectId());
            if ( NULL != virtual_device.get() )
            {
               vpr::BufferObjectReader* reader = data_packet->getPacketReader();
               reader->setAttrib("rim.timestamp.delta", node->getDelta());
               virtual_device->readObject(reader);
            }
            break;
         }
      default:
         {
            throw ClusterException("RIMPlugin does not handle packet type: "
               + boost::lexical_cast<std::string>(packet->getPacketType()), VPR_LOCATION);
            break;
         }
      }
   }
}

bool RIMPlugin::addVirtualDevice(const vpr::GUID& deviceId,
                                 const std::string& name,
                                 const vpr::Uint16 deviceBaseType,
                                 const std::string& hostname)
{
   using namespace gadget;

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
      << "Creating Virtual Device: " << name << std::endl << vprDEBUG_FLUSH;

   InputPtr input_device(       
      BaseTypeFactory::instance()->createObject(deviceBaseType)
   );
   vprASSERT(NULL != input_device.get() &&
             "BaseTypeFactory returned a NULL input device.");

   // Create a new VirtualDevice.
   VirtualDevicePtr virtual_device(
      VirtualDevice::create(name, deviceId, deviceBaseType, hostname,
                            input_device)
   );

   mVirtualDevices[deviceId] = virtual_device;

   return true;
}

gadget::InputPtr RIMPlugin::getVirtualDevice(const vpr::GUID& deviceId)
{
   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).first == deviceId )
      {
         return((*i).second->getDevice());
      }
   }
   return gadget::InputPtr();
}

gadget::InputPtr RIMPlugin::getVirtualDevice(const std::string& deviceName)
{
   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).second->getName() == deviceName )
      {
         return((*i).second->getDevice());
      }
   }
   return gadget::InputPtr();
}

bool RIMPlugin::removeVirtualDevicesOnHost(const std::string& hostName)
{
   // - Get a list of all remote devices on the given host
   // - Remove them from the current configuration
   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end(); i++ )
   {
      if ( (*i).second->getRemoteHostname() == hostName )
      {
         removeVirtualDevice((*i).second->getName());
      }
   }

   return true;
}

void RIMPlugin::removeVirtualDevice(const vpr::GUID& device_id)
{
   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).first == device_id )
      {
         // Remove remote device from the InputManager
         gadget::InputManager::instance()->removeDevice((*i).second->getName());
         mVirtualDevices.erase(i);
         return;
      }
   }
}

void RIMPlugin::removeVirtualDevice(const std::string& device_name)
{
   // Remove remote device from the InputManager
   gadget::InputManager::instance()->removeDevice(device_name);

   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).second->getName() == device_name )
      {
         (*i).second->debugDump(vprDBG_CONFIG_LVL);
         mVirtualDevices.erase(i);
         return;
      }
   }
}

void RIMPlugin::debugDumpVirtualDevices(int debug_level)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                    std::string("-------------- Virtual Devices --------------\n"),
                                    std::string("---------------------------------------------\n"));
   for ( virtual_device_map_t::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++ )
   {
      (*j).second->debugDump(debug_level);
   }
}

bool RIMPlugin::addDeviceServer(const std::string& name,
                                gadget::InputPtr device)
{
   DeviceServerPtr temp_device_server =
      DeviceServer::create(name, device, mHandlerGUID);
   mDeviceServers.push_back(temp_device_server);

   return true;
}

DeviceServerPtr RIMPlugin::getDeviceServer(const std::string& deviceName)
{
   DeviceNamePred pred(deviceName);
   device_server_list_t::iterator found = std::find_if(mDeviceServers.begin(), mDeviceServers.end(), pred);

   if (found != mDeviceServers.end())
   {
      return *found;
   }
   return DeviceServerPtr();
}

void RIMPlugin::removeDeviceServer(const std::string& deviceName)
{
   DeviceNamePred pred(deviceName);
   std::remove_if(mDeviceServers.begin(), mDeviceServers.end(), pred);
}

void RIMPlugin::debugDumpDeviceServers(int debug_level)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                    std::string("-------------- Device Servers --------------\n"),
                                    std::string("---------------------------------------------\n"));
   for ( device_server_list_t::iterator itr = mDeviceServers.begin(); itr != mDeviceServers.end(); itr++ )
   {
      (*itr)->debugDump(debug_level);
   }
}

}  // end namespace cluster
