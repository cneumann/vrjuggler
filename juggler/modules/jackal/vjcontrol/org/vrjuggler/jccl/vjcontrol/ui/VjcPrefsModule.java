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


package VjComponents.UI;

import java.io.*;
import java.awt.event.*;
import javax.swing.JMenuItem;
import java.util.Vector;

import VjComponents.UI.*;
import VjComponents.UI.Widgets.EasyMenuBar;
import VjControl.Core;
import VjControl.VjComponent;
import VjControl.DefaultCoreModule;
import VjComponents.UI.ControlUIModule;
import VjComponents.ConfigEditor.ConfigUIHelper;
import VjConfig.*;



/** Provides facilities for editing VjControl's preferences.
 *  This module requires a ControlUI module and a ConfigUIHelper module, 
 *  and is used to edit/save/etc. VjControl.Core's gui_chunkdb.
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class VjcPrefsModule
    extends DefaultCoreModule
    implements ActionListener { 


    protected ControlUIModule ui_module;
    protected ConfigUIHelper confighelper_module;

    protected JMenuItem editgprefs_mi;
    protected JMenuItem saveprefs_mi;



    public VjcPrefsModule () {
	super ();

        ui_module = null;
        confighelper_module = null;
        component_name = "Unconfigured VjcPrefsModule";
    }


//      public vjComponent[] getVjComponents (ConfigChunk ch, Class[] interfaces) {
//          // get pointers to the modules we need.
//          int length = interfaces.length;
//          int i, j;
//          vjComponent[] comps = new vjComponent[length];
//          for (i = 0; i < length; i++)
//              comps[i] = null;
//          Property p = ch.getPropertyFromToken ("Dependencies");
//          if (p != null) {
//              int n = p.getNum();
//              String s;
//              VjComponent c;
//              for (i = 0; i < n; i++) {
//                  s = p.getValue(i).toString();
//                  c = Core.getComponentFromRegistry(s);
//                  if (c != null) {
//                      for (j = 0; j < comps.length; j++)
//                          if (c instanceof (interfaces[j]))
//                              comps[j] = c;
//                  }
//              }
//          }
//      }

    public boolean configure (ConfigChunk ch) {
        component_chunk = ch;
        component_name = ch.getName();

//          Class[] interfaces = new Class[3];
//          interfaces[0] = ControlUIModule;
//          interfaces[1] = ConfigModule;
//          interfaces[2] = ConfigUIHelper;
//          vjComponent[] comps = gtVjComponents (ch, interfaces);
//          ui_module = (ControlUIModule)comps[0];
//          config_module = (ConfigModule)comps[1];
//          confighelper_module = (ConfigUIHelper)comps[2];


        // get pointers to the modules we need.
        Property p = ch.getPropertyFromToken ("Dependencies");
        if (p != null) {
            int i;
            int n = p.getNum();
            String s;
            VjComponent c;
            for (i = 0; i < n; i++) {
                s = p.getValue(i).toString();
                c = Core.getComponentFromRegistry(s);
                if (c != null) {
                    if (c instanceof ControlUIModule)
                        ui_module = (ControlUIModule)c;
                    if (c instanceof ConfigUIHelper)
                        confighelper_module = (ConfigUIHelper)c;
                }
            }
        }
        if (ui_module == null)
            ui_module = (ControlUIModule)Core.getComponentFromRegistry ("ControlUI Module");
        if (confighelper_module == null)
            confighelper_module = (ConfigUIHelper)Core.getComponentFromRegistry ("ConfigUIHelper Module");
        if ((ui_module == null) || (confighelper_module == null)) {
            Core.consoleErrorMessage (component_name, "Instantiated with unmet VjComponent Dependencies. Fatal Configuration Error!");
            return false;
        }

        EasyMenuBar mb = ui_module.getEasyMenuBar();
        editgprefs_mi = mb.addMenuItem ("Options/Edit Global Preferences...");
        saveprefs_mi = mb.addMenuItem ("Options/Save Preferences");

        editgprefs_mi.addActionListener (this);
        saveprefs_mi.addActionListener (this);

        return true;
    }



    public ConfigChunk getConfiguration () {
        return component_chunk;
    }



    public boolean addConfig (ConfigChunk ch) {
        return false;
    }



    public boolean removeConfig (String name) {
        return false;
    }



    public void destroy () {
//         EasyMenuBar mb = ui_module.getEasyMenuBar();
//         mb.removeMenuItem ("");
//         mb.removeMenuItem ("");
    }



    public void actionPerformed (ActionEvent e) {
        Object o = e.getSource();
        if (o == editgprefs_mi) {
 	    Vector v = Core.gui_chunkdb.getOfDescToken ("vjcontrol");
 	    if (v.size() >= 1) {
                ConfigChunk chunk = (ConfigChunk)v.elementAt(0);
                confighelper_module.openChunkFrame (Core.gui_chunkdb, chunk);
            }
        }
        else if (o == saveprefs_mi) {
            try {
                DataOutputStream out = new DataOutputStream(new FileOutputStream(Core.gui_chunkdb.file));
                out.writeBytes(Core.gui_chunkdb.fileRep());
                Core.consoleInfoMessage (component_name, "Saved VjControl preferences: " + Core.gui_chunkdb.file);
                Core.gui_chunkdb.need_to_save = false;
                /* do some fixing up if the name changed */
            }
            catch (IOException x) {
                Core.consoleErrorMessage (component_name, "IOerror saving VjControl preferences");
            }
        }
    }



}



