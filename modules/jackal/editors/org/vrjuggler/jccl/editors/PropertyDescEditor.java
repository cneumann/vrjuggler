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
package org.vrjuggler.jccl.editors;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.event.*;
import java.beans.*;
import java.util.List;
import javax.swing.*;
import javax.swing.table.*;

import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.DescEnum;
import org.vrjuggler.jccl.config.PropertyDesc;
import org.vrjuggler.jccl.config.ValType;
import org.vrjuggler.jccl.config.VarValue;

/**
 * Describes an editor component bean useful for editing PropertyDescs.
 */
public class PropertyDescEditor
   extends JTabbedPane
   implements Scrollable
{
   /**
    * Create a new editor for PropertyDescs. This editor has no PropertyDesc to
    * edit by default.
    */
   public PropertyDescEditor()
   {
      // JBuilder can't handle BoxLayout ...
      setupGeneralTab();
      setupEnumTab();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      PropertyEditorManager.registerEditor(ValType.class, ValTypeEditor.class);

      // Setup the basic table
      basicTable.setModel(basicTableModel);
      basicTable.setBackground(UIManager.getColor("Menu"));
      basicTable.getColumnModel().getColumn(0).
            setCellRenderer(new BoldCellRenderer());
      basicTable.getColumnModel().getColumn(1).
            setCellEditor(new BasicCellEditor());

      // Setup the items table
      itemsTable.setModel(itemsTableModel);
      itemsTable.setBackground(UIManager.getColor("Menu"));
      itemsTable.getColumnModel().getColumn(0).
            setCellRenderer(new BoldCellRenderer());
      itemsTable.getColumnModel().getColumn(1).
            setCellRenderer(new ItemsDefaultValueCellRenderer());

      // Setup the enums table
      enumTable.setModel(enumsTableModel);
      enumTable.setBackground(UIManager.getColor("Menu"));

      // Setup the inital property desc to empty
      setPropertyDesc(null);

      // Listener support on the text area
      helpTextArea.addFocusListener(new FocusListener()
      {
         public void focusGained(FocusEvent evt)
         {
         }

         public void focusLost(FocusEvent evt)
         {
            propertyDesc.setHelp(helpTextArea.getText());
            System.out.println("Updating property desc help");
         }
      });
   }

   public void setPropertyDesc(PropertyDesc desc)
   {
      propertyDesc = desc;
      basicTableModel.setPropertyDesc(desc);
      itemsTableModel.setPropertyDesc(desc);
      enumsTableModel.setPropertyDesc(desc);
      if (desc != null)
      {
         helpTextArea.setText(propertyDesc.getHelp());
      }
      else
      {
         helpTextArea.setText("");
      }

      // Tweak the max sizes of the components
      Dimension maxSize;

      maxSize = basicTable.getPreferredSize();
      maxSize.width = Integer.MAX_VALUE;
      basicTable.setPreferredScrollableViewportSize(maxSize);
      basicScrollPane.setMaximumSize(maxSize);

      maxSize = itemsTable.getPreferredSize();
      maxSize.width = Integer.MAX_VALUE;
      itemsTable.setPreferredScrollableViewportSize(maxSize);
      itemsScrollPane.setMaximumSize(maxSize);

      maxSize = enumTable.getPreferredSize();
      maxSize.width = Integer.MAX_VALUE;
      enumTable.setPreferredScrollableViewportSize(maxSize);
      enumScrollPane.setMaximumSize(maxSize);

      updateUI();
   }

   public PropertyDesc getPropertyDesc()
   {
      return propertyDesc;
   }

   /**
    * JBuilder can't handle BoxLayout which the General tab requires, so we set
    * it up here for JBuilder.
    */
   private void setupGeneralTab()
   {
      basicScrollPane.setViewportView(basicTable);
      basicScrollPane.setAlignmentX(LEFT_ALIGNMENT);
      itemsScrollPane.setViewportView(itemsTable);
      itemsScrollPane.setAlignmentX(LEFT_ALIGNMENT);

      JLabel basic_lbl = new JLabel("Basic Properties:");
      basic_lbl.setAlignmentX(LEFT_ALIGNMENT);
      JLabel items_lbl = new JLabel("Items:");
      items_lbl.setAlignmentX(LEFT_ALIGNMENT);

      // Setup the items button panel
      addItemBtn.setText("Add");
      removeItemBtn.setText("Remove");
      addItemBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addItem();
         }
      });
      removeItemBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeItem();
         }
      });
      JPanel itemsButtonPane = new JPanel();
      itemsButtonPane.setAlignmentX(LEFT_ALIGNMENT);
      itemsButtonPane.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5));
      itemsButtonPane.setLayout(new BoxLayout(itemsButtonPane, BoxLayout.X_AXIS));
      itemsButtonPane.add(Box.createHorizontalGlue());
      itemsButtonPane.add(addItemBtn);
      itemsButtonPane.add(Box.createHorizontalStrut(10));
      itemsButtonPane.add(removeItemBtn);

      Box box = new Box(BoxLayout.Y_AXIS);
      generalTab.setViewportView(box);
      box.add(basic_lbl);
      box.add(basicScrollPane);
      box.add(Box.createVerticalStrut(12));
      box.add(items_lbl);
      box.add(itemsScrollPane);
      box.add(itemsButtonPane);
      box.add(Box.createVerticalGlue());
   }

   /**
    * JBuilder can't handle BoxLayout which the Enums tab requires, so we set it
    * up here for JBuilder.
    */
   private void setupEnumTab()
   {
      // Setup the enum table
      enumScrollPane.setViewportView(enumTable);
      enumScrollPane.setAlignmentX(LEFT_ALIGNMENT);
      
      // Setup the enum button panel
      addEnumBtn.setText("Add");
      removeEnumBtn.setText("Remove");
      addEnumBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addEnum();
         }
      });
      removeEnumBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeEnum();
         }
      });
      JPanel enumButtonPane = new JPanel();
      enumButtonPane.setAlignmentX(LEFT_ALIGNMENT);
      enumButtonPane.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5));
      enumButtonPane.setLayout(new BoxLayout(enumButtonPane, BoxLayout.X_AXIS));
      enumButtonPane.add(Box.createHorizontalGlue());
      enumButtonPane.add(addEnumBtn);
      enumButtonPane.add(Box.createHorizontalStrut(10));
      enumButtonPane.add(removeEnumBtn);

      // Setup the base part of this panel
      Box box = new Box(BoxLayout.Y_AXIS);
      enumTab.setViewportView(box);
      box.add(enumScrollPane);
      box.add(enumButtonPane);
      box.add(Box.createVerticalGlue());
   }

   /**
    * Adds a new item to the current property desc.
    */
   protected void addItem()
   {
      PropertyDesc.Item item = new PropertyDesc.Item(propertyDesc.getValType());
      propertyDesc.addItem(item);
   }

   /**
    * Removes the currently selected item from the current property desc.
    */
   protected void removeItem()
   {
      // Get the currently selected item
      int row = itemsTable.getSelectedRow();
      if (row != -1)
      {
         PropertyDesc.Item item = itemsTableModel.getItemAt(row);
         propertyDesc.removeItem(item);
      }
   }

   /**
    * Adds a new enumeration to the current property desc.
    */
   protected void addEnum()
   {
      propertyDesc.addEnum(new DescEnum("New Enumeration",
                                        propertyDesc.getValType()));
   }

   /**
    * Removes the currently selected enumeration from the current property desc.
    */
   protected void removeEnum()
   {
      int row = enumTable.getSelectedRow();
      if (row != -1)
      {
         DescEnum the_enum = enumsTableModel.getEnumAt(row);
         propertyDesc.removeEnum(the_enum);
      }
   }

   /**
    * Gets the preferred size of the viewport on this component.
    */
   public Dimension getPreferredScrollableViewportSize()
   {
      return basicScrollPane.getPreferredSize();
   }

   public int getScrollableBlockIncrement(Rectangle visRect, int orientation,
                                          int direction)
   {
      return basicTable.getScrollableBlockIncrement(visRect, orientation,
                                                    direction);
   }

   public boolean getScrollableTracksViewportHeight()
   {
      return false;
   }

   public boolean getScrollableTracksViewportWidth()
   {
      return true;
   }

   public int getScrollableUnitIncrement(Rectangle visRect, int orientation,
                                         int direction)
   {
      return basicTable.getScrollableBlockIncrement(visRect, orientation,
                                                    direction);
   }

   /**
    * Let Jbuilder init the UI.
    */
   private void jbInit()
      throws Exception
   {
      this.setEnabled(true);
      this.add(generalTab, "General");
      this.add(enumTab, "Enumerations");
      this.add(helpTab,  "Help");
      helpTab.setViewportView(helpTextArea);
   }

   /**
    * The table model for the basic table.
    */
   BasicTableModel basicTableModel = new BasicTableModel();

   /**
    * The table model for the items table.
    */
   ItemsTableModel itemsTableModel = new ItemsTableModel();

   /**
    * The table model for the enumerations table.
    */
   EnumTableModel enumsTableModel = new EnumTableModel();

   /**
    * The property desc for this editor.
    */
   PropertyDesc propertyDesc = null;

   //--- JBuilder generated GUI variables ---//
   private JScrollPane enumTab = new JScrollPane();
   private JScrollPane enumScrollPane = new JScrollPane();
   private JTable enumTable = new JTable();
   private JScrollPane helpTab = new JScrollPane();
   private JTextArea helpTextArea = new JTextArea();
   private JScrollPane generalTab = new JScrollPane();
   private JScrollPane basicScrollPane = new JScrollPane();
   private JTable basicTable = new JTable();
   private JScrollPane itemsScrollPane = new JScrollPane();
   private JTable itemsTable = new JTable();
   private JButton addItemBtn = new JButton();
   private JButton removeItemBtn = new JButton();
   private JButton addEnumBtn = new JButton();
   private JButton removeEnumBtn = new JButton();

   /**
    * A table model for the Basic table.
    */
   class BasicTableModel
      extends AbstractTableModel
   {
      public void setPropertyDesc(PropertyDesc desc)
      {
         propertyDesc = desc;
         fireTableDataChanged();
      }

      public PropertyDesc getPropertyDesc()
      {
         return propertyDesc;
      }

      public String getColumnName(int col)
      {
         return columnNames[col].toString();
      }

      public int getRowCount()
      {
         // We only have rows if we have a property desc to model
         if (propertyDesc != null)
         {
            return 4;
         }
         else
         {
            return 0;
         }
      }

      public int getColumnCount() { return columnNames.length; }

      public Object getValueAt(int row, int col)
      {
         // Asking for the row name
         if (col == 0)
         {
            return rowNames[row];
         }
         else
         {
            switch (row)
            {
            case 0: // name
               return propertyDesc.getName();
            case 1: // token
               return propertyDesc.getToken();
            case 2: // type
               return propertyDesc.getValType();
            case 3: // variable
               return new Boolean(propertyDesc.hasVariableNumberOfValues());
            }
         }
         return null;
      }

      public boolean isCellEditable(int row, int col)
      {
         // Only the value column is editable
         return (col == 1);
      }

      public void setValueAt(Object value, int row, int col)
      {
         // Assuming they mean column 1 ...
         switch (row)
         {
         case 0: // name
            propertyDesc.setName((String)value);
            break;
         case 1: // token
            propertyDesc.setToken((String)value);
            break;
         case 2: // type
            propertyDesc.setValType((ValType)value);
            break;
         case 3: // variable
            propertyDesc.setHasVariableNumberOfValues(((Boolean)value).booleanValue());
            break;
         default:
            System.err.println("Bad row: "+row);
         }

         fireTableCellUpdated(row, col);
      }

      /**
       * The names of the columns.
       */
      private String[] columnNames = { "Property", "Value" };

      /**
       * The names of the rows.
       */
      private String[] rowNames = { "Name"
                                  , "Token"
                                  , "Type"
                                  , "Variable"
                                  };

      /**
       * The PropertyDesc this table model represents.
       */
      private PropertyDesc propertyDesc = null;
   }

   /**
    * A table model for the items table.
    */
   class ItemsTableModel
      extends AbstractTableModel
      implements PropertyChangeListener
   {
      public void setPropertyDesc(PropertyDesc desc)
      {
         // Update our listener status
         if (propertyDesc != null)
         {
            propertyDesc.removePropertyChangeListener(this);
         }
         propertyDesc = desc;
         if (propertyDesc != null)
         {
            propertyDesc.addPropertyChangeListener(this);
         }
         fireTableDataChanged();
      }

      public void propertyChange(PropertyChangeEvent evt)
      {
         // We only care if the items have changed
         if (evt.getPropertyName().equals("item"))
         {
            // TODO: Someday, fire off an event saying that only the items that
            // actually changed have really changed
            fireTableDataChanged();
         }
      }

      public PropertyDesc getPropertyDesc()
      {
         return propertyDesc;
      }

      public String getColumnName(int col)
      {
         return columnNames[col].toString();
      }

      public int getRowCount()
      {
         // We only have rows if we have a property desc to model
         if (propertyDesc != null)
         {
            return propertyDesc.getItems().size();
         }
         else
         {
            return 0;
         }
      }

      public int getColumnCount() { return columnNames.length; }

      public Object getValueAt(int row, int col)
      {
         List items = propertyDesc.getItems();
         PropertyDesc.Item item = (PropertyDesc.Item)items.get(row);
         if (col == 0)
         {
            return item.getLabel();
         }
         else
         {
            return item.getDefaultValue().get();
         }
      }

      public PropertyDesc.Item getItemAt(int row)
      {
         List items = propertyDesc.getItems();
         return (PropertyDesc.Item)items.get(row);
      }

      public boolean isCellEditable(int row, int col)
      {
         // Embedded chunks can't have direct default values
         if (propertyDesc.getValType() == ValType.EMBEDDEDCHUNK)
         {
            return false;
         }
         return true;
      }

      public void setValueAt(Object value, int row, int col)
      {
         List items = propertyDesc.getItems();
         PropertyDesc.Item item = (PropertyDesc.Item)items.get(row);
         if (col == 0)
         {
            item.setLabel((String)value);
         }
         else
         {
            VarValue varval = null;
            if (value instanceof Boolean)
            {
               varval = new VarValue(((Boolean)value).booleanValue());
            }
            else if (value instanceof Integer)
            {
               varval = new VarValue(((Integer)value).intValue());
            }
            else if (value instanceof Float)
            {
               varval = new VarValue(((Float)value).floatValue());
            }
            else if (value instanceof String)
            {
               varval = new VarValue((String)value);
            }
            item.setDefaultValue(varval);
         }

         fireTableCellUpdated(row, col);
      }

      /**
       * The names of the columns.
       */
      private String[] columnNames = { "Label", "Default Value" };

      /**
       * The PropertyDesc this table model represents.
       */
      private PropertyDesc propertyDesc = null;
   }

   /**
    * A table model for the enumerations table.
    */
   class EnumTableModel
      extends AbstractTableModel
      implements PropertyChangeListener
   {
      public void setPropertyDesc(PropertyDesc desc)
      {
         // Update our listener status
         if (propertyDesc != null)
         {
            propertyDesc.removePropertyChangeListener(this);
         }
         propertyDesc = desc;
         if (propertyDesc != null)
         {
            propertyDesc.addPropertyChangeListener(this);
         }
         fireTableDataChanged();
      }

      public void propertyChange(PropertyChangeEvent evt)
      {
         // We only care if the enumerations have changed
         if (evt.getPropertyName().equals("enumeration"))
         {
            // TODO: Someday, fire off a more specific event with information
            // about only the enumerations that have actually changed
            fireTableDataChanged();
         }
      }

      public PropertyDesc getPropertyDesc()
      {
         return propertyDesc;
      }

      public String getColumnName(int col)
      {
         return columnNames[col].toString();
      }

      public int getRowCount()
      {
         // We only have rows if we have a property desc to model
         if (propertyDesc != null)
         {
            return propertyDesc.getNumEnums();
         }
         else
         {
            return 0;
         }
      }

      public int getColumnCount() { return columnNames.length; }

      public Object getValueAt(int row, int col)
      {
         DescEnum desc_enum = propertyDesc.getEnumAt(row);
         if (col == 0)
         {
            return desc_enum.getName();
         }
         else
         {
            VarValue value = desc_enum.getValue();
            if (value != null)
            {
               return value.get();
            }
            else
            {
               return null;
            }
         }
      }

      public DescEnum getEnumAt(int row)
      {
         return propertyDesc.getEnumAt(row);
      }

      public boolean isCellEditable(int row, int col)
      {
         return true;
      }

      public void setValueAt(Object value, int row, int col)
      {
         DescEnum desc_enum = propertyDesc.getEnumAt(row);
         if (col == 0)
         {
            desc_enum.setName((String)value);
         }
         else
         {
            VarValue new_val = null;
            if (value instanceof Boolean)
            {
               new_val = new VarValue(((Boolean)value).booleanValue());
            }
            else if (value instanceof Integer)
            {
               new_val = new VarValue(((Integer)value).intValue());
            }
            else if (value instanceof Float)
            {
               new_val = new VarValue(((Float)value).floatValue());
            }
            else if (value instanceof String)
            {
               new_val = new VarValue((String)value);
            }

            desc_enum.setValue(new_val);
         }
         fireTableCellUpdated(row, col);
      }

      /**
       * The names of the columns.
       */
      private String[] columnNames = { "Name", "Value" };

      /**
       * The PropertyDesc this table model represents.
       */
      private PropertyDesc propertyDesc = null;
   }

   /**
    * Cell renderer that makes the cell's font bold.
    */
   class BoldCellRenderer
      extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(JTable table,
                                                     Object value,
                                                     boolean selected,
                                                     boolean focus,
                                                     int row,
                                                     int col)
      {
         Component comp = super.getTableCellRendererComponent(table, value,
                                                              selected, focus,
                                                              row, col);
         comp.setFont(comp.getFont().deriveFont(Font.BOLD));
         return comp;
      }
   }

   /**
    * Cell renderer for the items table default value column.
    */
   class ItemsDefaultValueCellRenderer
      extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(JTable table,
                                                     Object value,
                                                     boolean selected,
                                                     boolean focus,
                                                     int row,
                                                     int col)
      {
         Component comp = super.getTableCellRendererComponent(table, value,
                                                              selected, focus,
                                                              row, col);
         setFont(getFont().deriveFont(Font.PLAIN));
         if (value instanceof ConfigChunk)
         {
            setFont(getFont().deriveFont(Font.ITALIC));
            setText("Embedded Config Chunk");
         }

         return comp;
      }
   }

   /**
    * Cell editor for the basic table.
    */
   class BasicCellEditor
      extends AbstractCellEditor
      implements TableCellEditor
   {
      public Object getCellEditorValue()
      {
         return editor.getValue();
      }

      public boolean stopCellEditing()
      {
         try
         {
            if (editorComponent instanceof JComboBox)
            {
               JComboBox combo = (JComboBox)editorComponent;
               editor.setAsText((String)combo.getSelectedItem());
            }
            else if (editorComponent instanceof JTextField)
            {
               JTextField txt_field = (JTextField)editorComponent;
               editor.setAsText(txt_field.getText());
            }
            return super.stopCellEditing();
         }
         catch (IllegalArgumentException iae)
         {
            // TODO: Print this out to the Tweek console
            iae.printStackTrace();
//            System.out.println(iae.getMessage());
         }
         return false;
      }

      public Component getTableCellEditorComponent(JTable table, Object value,
                                                   boolean selected,
                                                   int row, int col)
      {
         editor = PropertyEditorManager.findEditor(value.getClass());
         editor.setValue(value);
         if (editor.getTags() != null)
         {
            JComboBox box = new JComboBox(editor.getTags());
            box.setSelectedItem(editor.getAsText());
            box.setFont(box.getFont().deriveFont(Font.PLAIN));
            box.setBackground(table.getBackground());
            box.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  stopCellEditing();
               }
            });
            editorComponent = box;
         }
         else
         {
            JTextField txt = new JTextField(editor.getAsText());
            txt.setBorder(BorderFactory.createEmptyBorder(0,0,0,0));
            txt.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  stopCellEditing();
               }
            });
            editorComponent = txt;
         }

         editorComponent.setForeground(table.getForeground());

         return editorComponent;
      }

      private PropertyEditor editor = null;
      private Component editorComponent = null;
   }
}