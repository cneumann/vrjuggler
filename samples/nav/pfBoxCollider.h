/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#ifndef JUGGLER_PFBOX_COLLIDER
#define JUGGLER_PFBOX_COLLIDER

#include <collider.h>
#include <pfTerryBoxCollide.h>

//: Collider class for testing collisions in Performer
//
// This class will test collisions against a given world model
//
// NOTE: Remember that the navigation system is dealing
//     in Juggler (OpenGL) coordinates, not in Performer coords.
//     So there is some coord system conversion that will be
//     going on here.
class pfBoxCollider : public collider
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfBoxCollider(pfNode* world) : terryCollide( 0x1 )
   {
      mWorldNode = world;
   }

   //: test a movement
   //!ARGS: whereYouAre - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 whereYouAre, vjVec3 delta, vjVec3& correction, bool whereYouAreWithDelta = false);

public:
   pfNode* mWorldNode;        // The world to collide with
   pfTerryBoxCollide terryCollide;
};


bool pfBoxCollider::testMove( vjVec3 feetPosition, vjVec3 feetDelta, vjVec3& correction, bool whereYouAreWithDelta)
{
   pfVec3 pf_feet_position = vjGetPfVec(feetPosition);
   pfVec3 pf_feet_delta = vjGetPfVec(feetDelta);
   pfVec3 pf_feet_destination = (pf_feet_position + pf_feet_delta);
   pfVec3 pf_correction;         // Needs to be set
   
   // whats the maximum step height?
   float step_height( 5.0f ); // value picked out of my ass.
   pfVec3 up( 0.0f, 0.0f, 1.0f );
   // whats the ray start position that would test for that step height?
   pfVec3 pf_step_destination = pf_feet_destination + up * step_height;
   
         
   // get the speed, can only be positive bye definition of .length()
   
   float speed = pf_feet_delta.length();
   
   // get the radius..       // Terry hack
   float vol_radius = speed * 2.0f;
  
   // constrain value to always have unit length or greater
   if(vol_radius < 1.0f)
      vol_radius = 1.0f;
   
   terryCollide.setRadius( vol_radius );    // Setup collision volume
   
   
   //terryCollide.setVelocity( pf_feet_delta );
   
   if (terryCollide.collide( pf_correction, mWorldNode, pf_step_destination) )
   {
      correction = vjGetVjVec(pf_correction);

      setDidCollide(true);
      return true;
   }
   else
   {
      setDidCollide(false);
      return false;
   }
}

#endif
