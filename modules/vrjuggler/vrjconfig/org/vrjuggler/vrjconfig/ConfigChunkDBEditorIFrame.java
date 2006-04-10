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

package org.vrjuggler.vrjconfig;

import javax.swing.*;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.editors.*;
import java.awt.*;

/**
 * A specialization of JInternalFrame that contains a ConfigChunkDB editor.
 */
public class ConfigChunkDBEditorIFrame
   extends JInternalFrame
{
   public ConfigChunkDBEditorIFrame()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   /**
    * GUI init code automagically generated by JBuilder.
    */
   private void jbInit()
      throws Exception
   {
      this.setClosable(true);
      this.setIconifiable(true);
      this.setMaximizable(true);
      this.setResizable(true);
      this.setTitle("Configuration Collection: ");
      this.getContentPane().add(editor,  BorderLayout.CENTER);
   }

   public String getFilename()
   {
      return filename;
   }

   public void setFilename(String filename)
   {
      this.filename = filename;
      setTitle("Configuration Collection: "+filename);
   }

   public ConfigContext getConfigContext()
   {
      return context;
   }

   public void setConfigContext(ConfigContext context)
   {
      this.context = context;
   }

   public ConfigChunkDBEditor getEditor()
   {
      return editor;
   }

   private String filename = "";
   private ConfigContext context = new ConfigContext();

   // JBuilder GUI variables.
   private ConfigChunkDBEditor editor = new ConfigChunkDBEditor();
}