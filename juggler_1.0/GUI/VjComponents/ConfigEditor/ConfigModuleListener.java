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

package VjComponents.ConfigEditor;

import VjComponents.ConfigEditor.ConfigModuleEvent;

/** Listener interface for ConfigModule events.
 *  By becoming a ConfigModuleListener, an object can receive 
 *  events about addition or removal of ConfigChunkDBs and 
 *  ChunkDescDBs.
 *  <p>
 *  ConfigModuleEvents may be sent by any thread; a listener intending
 *  to update GUI components based on the event must use 
 *  SwingUtilities.invokeLater or something similar to ensure correct
 *  synchronization.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public interface ConfigModuleListener extends java.util.EventListener {

    public abstract void addChunkDB (ConfigModuleEvent e);
    public abstract void removeChunkDB (ConfigModuleEvent e);
    public abstract void addDescDB (ConfigModuleEvent e);
    public abstract void removeDescDB (ConfigModuleEvent e);

}

