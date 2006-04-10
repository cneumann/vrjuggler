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

#include <gadget/gadgetConfig.h>

#include <string>
#include <stdio.h>                      /* need stdio for sprintf */
#include <vpr/System.h>

#include <gadget/Devices/Fakespace/PinchGloveStandalone.h> /* Fakespace pinch driver */
#include <gadget/Devices/Fakespace/PinchGlove.h> /* Gadgeteer pinch driver */
#include <gadget/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>


namespace gadget
{

bool PinchGlove::config(jccl::ConfigChunkPtr c)
{
   if ( ! (Input::config(c) /*&& Glove::config(c)*/ ) )
   {
      return false;
   }

   vprASSERT(mThread == NULL);      // This should have been set by Input(c)

   mGlove = new PinchGloveStandalone();

   return true;
}

PinchGlove::~PinchGlove ()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

int PinchGlove::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[Pinch] Begin sampling\n"
                                    << vprDEBUG_FLUSH;

   if ( mThread == NULL )
   {
      int maxAttempts=0;
      bool result = false;
      while ( result == false && maxAttempts < 5 )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[Pinch] Connecting to "
                                          << mPort << " at "
                                          << mBaudRate << "...\n"
                                          << vprDEBUG_FLUSH;
         result = mGlove->connectToHardware( mPort , mBaudRate);
         if ( result == false )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR,0)
               << "[Pinch] ERROR: Can't open or it is already opened."
               << vprDEBUG_FLUSH;
            vpr::System::usleep(14500);
            maxAttempts++;
         }
      }

      vprDEBUG(gadgetDBG_INPUT_MGR,0)
         << "[Pinch] Successfully connected, Now sampling pinch data."
         << vprDEBUG_FLUSH;

/* Don't need anymore because SampleBuffers are smarter

      DigitalData temp;
      temp=0;
      for (int i=0;i<10;i++)
      {
          mDigitalData.push_back(temp);
      }
      mDigitalSamples.lock();
      mDigitalSamples.addSample(mDigitalData);
      mDigitalSamples.unlock();
      */

      // Create a new thread to handle the control
      vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[Pinch] Spawning control thread\n"
                                       << vprDEBUG_FLUSH;
      vpr::ThreadMemberFunctor<PinchGlove>* memberFunctor =
         new vpr::ThreadMemberFunctor<PinchGlove>(this, &PinchGlove::controlLoop, NULL);

      mThread = new vpr::Thread(memberFunctor);

      if ( !mThread->valid() )
      {
         return 0;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,1) << "[Pinch] PinchGlove is active "
                                         << std::endl << vprDEBUG_FLUSH;
         mActive = true;
         return 1;
      }
   }
   else
      return 0; // already sampling
}

void PinchGlove::controlLoop(void* nullParam)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[Pinch] Entered control thread\n"
                                    << vprDEBUG_FLUSH;

   while ( 1 )
   {
      sample();
   }
}

int PinchGlove::sample()
{
   // Tell the glove to resample

   ///Get data from hardware
   std::string gesture;
   mGlove->updateStringFromHardware();
   mGlove->getSampledString( gesture );

   unsigned char character[2];
   int number;
   int i;
   for ( i=0;i<11;i++ )
   {

      if ( i<5 )
      {
         character[0]=gesture[i];
         //character[1]='\0';
         //number = atoi(character);
         number = character[0] - '0';
         mDigitalData[i]=number;
      }
      else if ( i>5 )
      {
         character[0]=gesture[i];
         //character[1]='\0';
         //number = atoi(character);
         number = character[0] - '0';
         mDigitalData[i-1]=number;
      }
   }

   mDigitalSamples.lock();
   mDigitalSamples.addSample(mDigitalData);
   mDigitalSamples.unlock();

   /////////////////////////////
   // Add Finger Angles LATER //
   // updateFingerAngles();   //
   /////////////////////////////

   /////////////////////////////////////////
   // Update the xform data               //
   // mTheData[0][progress].calcXforms(); //
   // mTheData[1][progress].calcXforms(); //
   /////////////////////////////////////////

   return 1;

}

void PinchGlove::updateData()
{
   mDigitalSamples.swapBuffers();
   return;
}

int PinchGlove::stopSampling()
{
   if ( mThread != NULL )
   {
      mThread->kill();
      delete mThread;
      mThread = NULL;
      vpr::System::usleep(100);

      // XXX: there is no "close"
      //mGlove->Close();
      vprDEBUG(gadgetDBG_INPUT_MGR,1) << "[Pinch] stopping PinchGlove.."
                                      << std::endl << vprDEBUG_FLUSH;
   }
   return 1;
}


//TODO: move this function up the hierarchy, since PinchGlove also has this one.
void PinchGlove::updateFingerAngles()
{/*

    std::string gesture;
    // the the fakespace "gesture", it's a string like this "00000.00000"
    mGlove->getSampledString( gesture );

    const int RIGHT_HAND = 0;
    const int LEFT_HAND = 1;

    assert( progress < 3 && progress >= 0 );
    assert( LEFT_HAND < GADGET_MAX_GLOVE_DEVS );
    assert( RIGHT_HAND < GADGET_MAX_GLOVE_DEVS );

    // use the digital data set the angles for each joint.
    right.setFingers( gesture[PinchGloveStandalone::RPINKY] == '1',
                     gesture[PinchGloveStandalone::RRING] == '1',
                     gesture[PinchGloveStandalone::RMIDDLE] == '1',
                     gesture[PinchGloveStandalone::RINDEX] == '1',
                     gesture[PinchGloveStandalone::RTHUMB] == '1' );
    left.setFingers( gesture[PinchGloveStandalone::LPINKY] == '1',
                     gesture[PinchGloveStandalone::LRING] == '1',
                     gesture[PinchGloveStandalone::LMIDDLE] == '1',
                     gesture[PinchGloveStandalone::LINDEX] == '1',
                     gesture[PinchGloveStandalone::LTHUMB] == '1' );

    //Now, set the ugly ambiguously named array, mTheData:

    // if that assert failed, then at least the code will still run...
    if ( RIGHT_HAND >= GADGET_MAX_GLOVE_DEVS )
    {
       // Right Pinky
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::MPJ] = right.pinky().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::PIJ] = right.pinky().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::DIJ] = right.pinky().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::ABDUCT] = right.pinky().abduct();

       // Right Ring
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::MPJ] = right.ring().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::PIJ] = right.ring().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::DIJ] = right.ring().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::ABDUCT] = right.ring().abduct();

       // Right Middle
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::MPJ] = right.middle().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::PIJ] = right.middle().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::DIJ] = right.middle().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::ABDUCT] = right.middle().abduct();

       // Right Index
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::MPJ] = right.index().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::PIJ] = right.index().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::DIJ] = right.index().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::ABDUCT] = right.index().abduct();

       // Right Thumb
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::MPJ] = right.thumb().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::PIJ] = right.thumb().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::DIJ] = right.thumb().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::ABDUCT] = right.thumb().abduct();

       // Right Wrist
       mTheData[RIGHT_HAND][progress].angles[GloveData::WRIST][GloveData::YAW] = right.yaw();
       mTheData[RIGHT_HAND][progress].angles[GloveData::WRIST][GloveData::PITCH] = right.pitch();
    }

    // if that assert failed, then at least the code will still run...
    if ( LEFT_HAND >= GADGET_MAX_GLOVE_DEVS )
    {
       // Left Pinky
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::MPJ] = left.pinky().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::PIJ] = left.pinky().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::DIJ] = left.pinky().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::ABDUCT] = left.pinky().abduct();

       // Left Ring
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::MPJ] = left.ring().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::PIJ] = left.ring().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::DIJ] = left.ring().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::ABDUCT] = left.ring().abduct();

       // Left Middle
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::MPJ] = left.middle().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::PIJ] = left.middle().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::DIJ] = left.middle().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::ABDUCT] = left.middle().abduct();

       // Left Index
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::MPJ] = left.index().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::PIJ] = left.index().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::DIJ] = left.index().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::ABDUCT] = left.index().abduct();

       // Left Thumb
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::MPJ] = left.thumb().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::PIJ] = left.thumb().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::DIJ] = left.thumb().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::ABDUCT] = left.thumb().abduct();

       // Left Wrist
       mTheData[LEFT_HAND][progress].angles[GloveData::WRIST][GloveData::YAW] = left.yaw();
       mTheData[LEFT_HAND][progress].angles[GloveData::WRIST][GloveData::PITCH] = left.pitch();
    }*/
}

} // namespace gadget