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

#ifndef _GADGET_GLOVE_H_
#define _GADGET_GLOVE_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/GloveData.h>
#include <gadget/Type/DigitalData.h> /* For getGloveDataFromDigitalData */
#include <gadget/Type/SampleBuffer.h>
#include <gadget/Type/PositionInterface.h>
#include <boost/concept_check.hpp>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <vpr/IO/SerializableObject.h>

namespace gadget
{

const unsigned short MSG_DATA_GLOVE = 520;

/**
 * This is the abstract base glove class.
 * It specifies the interface to all glove objects in the system.
 * Gadgeteer will deal only with gloves using this interface.
 */
class GADGET_CLASS_API Glove : public vpr::SerializableObject
{
public:
   typedef gadget::SampleBuffer<GloveData> SampleBuffer_t;

public:
   Glove();

   virtual ~Glove() {}

   virtual bool config(jccl::ConfigElementPtr element);

public:
   /* Data access functions */

   /**
    * Returns a vector ponting "out" of the component.
    * Can be used for selection, etc.
    */
   gmtl::Vec3f getTipVector(GloveData::GloveComponent component, int devNum);

   /**
    * Return the transform matrix of the component tip of the specified component
    */
   gmtl::Matrix44f getTipTransform(GloveData::GloveComponent component, int devNum);
   
   /**
    * Returns the transform matrix of the specified joint in world space.
    */
   gmtl::Matrix44f getJointTransform(GloveData::GloveComponent component, GloveData::GloveJoint joint, int devNum);

   /** Returns a copy of the glove data struct. */
   GloveData getGloveData(int devNum);
   
   /* Buffer functions */

   /** Helper method to add a sample to the sample buffers.
    * This MUST be called by all glove devices to add a new sample.
    * The data samples passed in will then be modified by any local filters.
    * @post Sample is added to the buffers and the local filters are run on that sample.
    */
   void addGloveSample(const std::vector< GloveData >& gloveSample)
   {
      // Locks and then swaps the indices.
      mGloveSamples.lock();
      mGloveSamples.addSample(gloveSample);
      mGloveSamples.unlock();
   }

   /** Swap the glove data buffers.
    * @post If ready has values, then copy values from ready to stable
    *        if not, then stable keeps its old values
    */
   void swapGloveBuffers()
   {
      mGloveSamples.swapBuffers();
   }

   const SampleBuffer_t::buffer_t& getGloveDataBuffer()
   {
      return mGloveSamples.stableBuffer();
   }

   virtual std::string getBaseType()
   {
      return std::string("Glove");
   }

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);
 
   /** Utility function to generate GloveData from DigitalData */
   std::vector<GloveData> getGloveDataFromDigitalData(const std::vector<DigitalData> &digitalData);

protected:
   Glove(const gadget::Glove& g):vpr::SerializableObject(){;}
   void operator=(const gadget::Glove& g){;}

   SampleBuffer_t  mGloveSamples;   /**< Glove samples */
   GloveData       mDefaultValue;   /**< Default glove data to return */

   std::vector<PositionInterface> mGlovePositions;
};

} // End of gadget namespace

#endif
