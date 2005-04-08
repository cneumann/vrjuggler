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


#ifndef _vjNullMutex_h_
#define _vjNullMutex_h_
//----------------------------------------------
// vjNullMutex
//
// Purpose:
//    Null mutex wrapper.  Used to pass a do nothing
//	mutex as a template type.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-21-97
//-----------------------------------------------

#include <vjConfig.h>

//: Null mutex wrapper.  Used to pass a "do nothing" mutex as a template type.
//!PUBLIC_API:
class vjNullMutex
{
public:
    vjNullMutex () {}
    ~vjNullMutex() {}


    //---------------------------------------------------------
    //: Lock the mutex.
    //
    //! RETURNS:  1 - Acquired
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int acquire() const
    {
        return 1;
    }

    //----------------------------------------------------------
    //: Acquire a read mutex.
    //----------------------------------------------------------
    int acquireRead() const
    {
        return this->acquire();	    // No special "read" semaphore -- For now
    }

    //----------------------------------------------------------
    //: Acquire a write mutex.
    //----------------------------------------------------------
    int acquireWrite() const
    {
        return this->acquire();	    // No special "write" semaphore -- For now
    }

    //---------------------------------------------------------
    //: Try to acquire the lock.  Returns immediately even if
    //+ we don't acquire the lock.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    //---------------------------------------------------------
    int tryAcquire () const
    {
        return 1;	    // Try 100 spins.
    }

    //----------------------------------------------------------
    //: Try to acquire a read mutex.
    //----------------------------------------------------------
    int tryacquire_read () const
    {
        return this->tryAcquire();
    }

    //----------------------------------------------------------
    //: Try to acquire a write mutex.
    //----------------------------------------------------------
    int tryacquire_write () const
    {
        return this->tryAcquire();
    }

    //---------------------------------------------------------
    //: Release the mutex.
    //
    //! RETURNS:  0 - Success
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int release() const
    {
        return 0;
    }

    //------------------------------------------------------
    //: Test the current lock status.
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    //------------------------------------------------------
    int test()
    {
        return 0;     // Just return 0 since it is a null lock
    }

    //---------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
#ifndef VJ_OS_Win32
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
    {
       std::cerr << message << "NULL Mutex\n";
    }
#endif

protected:
    // = Prevent assignment and initialization.
    void operator= (const vjNullMutex &) {}
    vjNullMutex (const vjNullMutex &) {}
};

#endif