/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_BUFFER_OBJECT_READER_H
#define _VPR_BUFFER_OBJECT_READER_H

#include <vpr/vprConfig.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/ObjectReader.h>


namespace vpr
{

/** Object reader that reads out of a data buffer
*
* @todo: Add smart buffering for type sizes
*/
class BufferObjectReader : public ObjectReader
{
public:
   BufferObjectReader(std::vector<vpr::Uint8>* data, unsigned curPos=0)
   {
      mData = data;
      mCurHeadPos = curPos;
   }

   void setCurPos(unsigned val)
   { mCurHeadPos = val; }
   unsigned getCurPos()
   { return mCurHeadPos; }

   virtual vpr::Uint8 readUint8();
   virtual vpr::Uint16 readUint16();
   virtual vpr::Uint32 readUint32();
   virtual vpr::Uint64 readUint64();
   virtual float readFloat();
   virtual double readDouble();
   virtual std::string readString(unsigned len);
   virtual bool readBool();

   /* Read raw data of length len
   * POST: Pointer to data returned
   * NOTE: data points to data owned elsewhere.
   * DO NOT MODIFY THE DATA and DO NOT RELY ON THE DATA STAYING THERE LONG.
   */
   inline vpr::Uint8* readRaw(unsigned len=1);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned                   mCurHeadPos;
};

/* Read out the single byte.
* @post: data = old(data)+val, mCurHeadPos advaced 1
*/
inline vpr::Uint8 BufferObjectReader::readUint8()
{
   vpr::Uint8 temp_data;
   memcpy(&temp_data, readRaw(1), 1);
   return temp_data;
}

inline vpr::Uint16 BufferObjectReader::readUint16()
{
   vpr::Uint16 nw_val;
   memcpy(&nw_val, readRaw(2), 2);

   return vpr::System::Ntohs(nw_val);
}

inline vpr::Uint32 BufferObjectReader::readUint32()
{
   vpr::Uint32 nw_val;
   memcpy(&nw_val, readRaw(4), 4);

   return vpr::System::Ntohl(nw_val);
}

inline vpr::Uint64 BufferObjectReader::readUint64()
{
   vpr::Uint64 nw_val;
   memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);

   return h_val;
}

inline float BufferObjectReader::readFloat()
{
   // We are reading the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);

   vpr::Uint32 nw_val;
   memcpy(&nw_val, readRaw(4), 4);
   vpr::Uint32 h_val = vpr::System::Ntohl(nw_val);

   return *((float*)&h_val);
}

inline double BufferObjectReader::readDouble()
{
   // We are reading the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);

   vpr::Uint64 nw_val;
   memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);
   double d_val = *((double*)&h_val);

   return d_val;
}


inline std::string BufferObjectReader::readString(unsigned len)
{
   std::string ret_val;
   char tempChar;
   for(unsigned i=0; i<len;++i)
   {
      tempChar = (char)(*readRaw(1));
      ret_val += tempChar;
   }
   return ret_val;
}

inline bool BufferObjectReader::readBool()
{
   return (bool)*(readRaw(1));
}


inline vpr::Uint8* BufferObjectReader::readRaw(unsigned len)
{
   mCurHeadPos += len;
   vprASSERT((mCurHeadPos-len) < mData->size());

   return &((*mData)[mCurHeadPos-len]);
}

} // namespace vpr

#endif
