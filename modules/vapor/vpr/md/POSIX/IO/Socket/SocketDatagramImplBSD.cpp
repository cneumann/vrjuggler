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

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <Utils/Assert.h>
#include <md/POSIX/SocketDatagramImpBSD.h>


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpBSD::recvfrom (void* msg, const size_t length,
                                const int flags, InetAddr& from,
                                ssize_t& bytes_read)
{
    socklen_t fromlen;
    Status retval;

    fromlen    = from.size();
    bytes_read = ::recvfrom(m_handle->m_fdesc, msg, length, flags,
                            (struct sockaddr*) &from.m_addr, &fromlen);

    if ( bytes_read == -1 ) {
        fprintf(stderr,
                "[vpr::SocketDatagramImpBSD] ERROR: Could not read from socket (%s:%hu): %s\n",
                m_remote_addr.getAddressString().c_str(),
                m_remote_addr.getPort(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpBSD::recvfrom (std::string& msg, const size_t length,
                                const int flags, InetAddr& from,
                                ssize_t& bytes_read)
{
    msg.resize(length);
    memset(&msg[0], '\0', msg.size());

    return recvfrom((void*) &msg[0], msg.size(), flags, from, bytes_read);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpBSD::recvfrom (std::vector<vpr::Uint8>& msg,
                                const size_t length, const int flags,
                                InetAddr& from, ssize_t& bytes_read)
{
    Status retval;

    msg.resize(length);

    memset(&msg[0], '\0', msg.size());
    retval = recvfrom((void*) &msg[0], msg.size(), flags, from, bytes_read);

    // Size it down if needed, if (bytes_read==length), then resize does
    // nothing.
    if ( bytes_read >= 0 ) {
        msg.resize(bytes_read);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpBSD::sendto (const void* msg, const size_t length,
                              const int flags, const InetAddr& to,
                              ssize_t& bytes_sent)
{
    Status retval;

    bytes_sent = ::sendto(m_handle->m_fdesc, msg, length, flags,
                          (struct sockaddr*) &to.m_addr, to.size());

    if ( bytes_sent == -1 ) {
        fprintf(stderr,
                "[vpr::SocketDatagramImpBSD] ERROR: Could not send to %s:%hu on socket (%s:%hu): %s\n",
                to.getAddressString().c_str(), to.getPort(),
                m_remote_addr.getAddressString().c_str(),
                m_remote_addr.getPort(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpBSD::sendto (const std::string& msg, const size_t length,
                              const int flags, const InetAddr& to,
                              ssize_t& bytes_sent)
{
    vprASSERT(length <= msg.size() && "Length is bigger than data given");
    return sendto(msg.c_str(), length, flags, to, bytes_sent);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpBSD::sendto (const std::vector<vpr::Uint8>& msg,
                              const size_t length, const int flags,
                              const InetAddr& to, ssize_t& bytes_sent)
{
    vprASSERT(length <= msg.size() && "Length is bigger than data given");
    return sendto((const void*) &msg[0], length, flags, to, bytes_sent);
}

}; // End of vpr namespace
