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

#include <vpr/IO/XMLObjectReader.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/ObjectReader.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <cppdom/cppdom.h>
#include <string>
#include <sstream>
#include <iterator>

#define XML_OR_LEVEL vprDBG_HEX_LVL


namespace vpr
{


XMLObjectReader::XMLObjectReader(std::vector<vpr::Uint8> data)
   : mCurSource(CdataSource)
{
   initCppDomTree(data);
   //mAttribSource.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit );
}

XMLObjectReader::XMLObjectReader(cppdom::NodePtr rootNode)
   : mCurSource(CdataSource)
{
   mRootNode = rootNode;
   //mAttribSource.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit );
}



XMLObjectReader::NodeState::NodeState(cppdom::Node* cur_node)
   : node(cur_node)
{
   nextChild_i = node->getChildren().begin();     // Initialize to the first child
   endChild_i = node->getChildren().end();
   //cdataSource.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit );
   cdataSource.str(node->getFullCdata());
}

XMLObjectReader::NodeState::NodeState(const NodeState& rhs)
{
   node = rhs.node;
   nextChild_i = rhs.nextChild_i;
   endChild_i = rhs.endChild_i;
   //cdataSource.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit );
   cdataSource.str(rhs.cdataSource.str());
}

XMLObjectReader::NodeState& XMLObjectReader::NodeState::operator=(const NodeState& rhs)
{
   node = rhs.node;
   nextChild_i = rhs.nextChild_i;
   endChild_i = rhs.endChild_i;
   cdataSource.str(rhs.cdataSource.str());
   return *this;
}

void XMLObjectReader::NodeState::debugDump(int debug_level)
{
   vprDEBUG(vprDBG_ALL, debug_level) << "Node:" << node->getName() << "  num children:" << node->getChildren().size() 
                                     << "  distance: nextChild-->endChild:" << std::distance(nextChild_i,endChild_i) << std::endl << vprDEBUG_FLUSH;
}


/** Initialize the members based on a serialized version of something in the data buffer */
void XMLObjectReader::initCppDomTree(std::vector<vpr::Uint8> data)
{
   std::string xml_data(data.begin(), data.end());
   std::stringstream xml_stream(xml_data);
   cppdom::ContextPtr cxt(new cppdom::Context);
   cppdom::NodePtr new_node = cppdom::NodePtr(new cppdom::Node(cxt));
   new_node->load(xml_stream, cxt);

   mRootNode = new_node;

   std::string root_name = mRootNode->getName();   // Debugging

   mCurNodeStack.clear();
}

void XMLObjectReader::debugDumpStack(int debug_level)
{
   vprDEBUG_OutputGuard(vprDBG_ALL, debug_level, "mCurNodeStack:\n", "end node stack\n");

   for(unsigned i=0; i<mCurNodeStack.size(); ++i)
   {
      mCurNodeStack[i].debugDump(debug_level);
   }
}

/** Starting a new tag
* Get the local cdata into the current data source
*/
vpr::ReturnStatus XMLObjectReader::beginTag(std::string tagName)
{
   vprDEBUG_OutputGuard(vprDBG_ALL, XML_OR_LEVEL, std::string("beginTag:[") + tagName + std::string("]\n"), "endTag");

   vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "mCurNodeStack: size: " << mCurNodeStack.size() << std::endl << vprDEBUG_FLUSH;
   debugDumpStack(XML_OR_LEVEL);

   // Find the correct child
   if(mCurNodeStack.empty())  // We should enter the root
   {
      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "mCurNodeStack is empty. Push on root.\n" << vprDEBUG_FLUSH;
      mCurNodeStack.push_back(NodeState(mRootNode.get()));
   }
   else                      // Find the next child and push it on
   {
      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "Attempting to find next child: " 
                             << " cur node (back): " << mCurNodeStack.back().node->getName()
                             << "  num children:" << mCurNodeStack.back().node->getChildren().size() << std::endl << vprDEBUG_FLUSH;

      vprASSERT(mCurNodeStack.back().nextChild_i != mCurNodeStack.back().endChild_i && "Past last child. No remaining child tags.");

      // Make sure that we get to child of type node.
      // While next child is not of type node skip over cdata...
      while( (*(mCurNodeStack.back().nextChild_i))->getType() != cppdom::xml_nt_node)
      {
         vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "Skip node: " << (*(mCurNodeStack.back().nextChild_i))->getName() << " -- non nt_node type.\n" << vprDEBUG_FLUSH;
         mCurNodeStack.back().nextChild_i++;
         vprASSERT(mCurNodeStack.back().nextChild_i != mCurNodeStack.back().endChild_i && "Skipped past last child. No remaining child tags.");
      }

      cppdom::NodePtr tag_node = *(mCurNodeStack.back().nextChild_i);   // Get the node with the given tag
      mCurNodeStack.back().nextChild_i++;                               // Step to the next child for next time
      
      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "Incrementing next child:  at end:" << (mCurNodeStack.back().nextChild_i == mCurNodeStack.back().endChild_i) << std::endl << vprDEBUG_FLUSH;

      mCurNodeStack.push_back( NodeState(tag_node.get()));              // Add the child to the stack
      vprASSERT(tag_node.get() == mCurNodeStack.back().node);

      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "Found node:" << tag_node->getName() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "  Node stack size:" << mCurNodeStack.size() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "  Num children:" << tag_node->getChildren().size() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,XML_OR_LEVEL) << "  New node child at end:" << (mCurNodeStack.back().nextChild_i == mCurNodeStack.back().endChild_i) 
                             << std::endl << vprDEBUG_FLUSH;
   }

   std::string cur_node_name = mCurNodeStack.back().node->getName();
   vprASSERT(tagName == cur_node_name && "Reading back in incorrect order");
   return vpr::ReturnStatus::Succeed;
}

/** Ends the most recently named tag.
* Just pop of the cur node state information
*/
vpr::ReturnStatus XMLObjectReader::endTag()
{
   vprASSERT(!mCurNodeStack.empty());
   mCurNodeStack.pop_back();
   return vpr::ReturnStatus::Succeed;
}

/** Starts an attribute of the name attributeName
* Get the attribute content and set the attribute source with that content
*/
vpr::ReturnStatus XMLObjectReader::beginAttribute(std::string attributeName)
{
   //std::cout << "beginAttribute: " << attributeName << std::endl;
   std::string attrib_content = mCurNodeStack.back().node->getAttribute(attributeName).getString();
   //std::cout << "  attrib content: [" << attrib_content << "]\n";
   mAttribSource.clear();                       // Clear error states
   mAttribSource.str(attrib_content);
   mAttribSource.seekg(0,std::ios::beg);        // Seek to beginning
   //std::cout << "  mAttribSource.str():[" << mAttribSource.str() << "]" << std::endl;
   mCurSource = AttribSource;             // Get content from attribute now
   return vpr::ReturnStatus::Succeed;
}

/** Ends the most recently named attribute */
vpr::ReturnStatus XMLObjectReader::endAttribute()
{
   mAttribSource.clear();
   mAttribSource.seekg(0,std::ios::beg);
   mAttribSource.str("");     // Clear the attrib content (not required, but helps point out bugs)
   mCurSource = CdataSource;     // Set back to getting data from cdata
   return vpr::ReturnStatus::Succeed;
}
//@}


vpr::Uint8 XMLObjectReader::readUint8()
{
   // Read a uint16 so that it does not treat it as a single char
   vpr::Uint16 temp_data;
   readValueStringRep(temp_data);
   return vpr::Uint8(temp_data);
}

vpr::Uint16 XMLObjectReader::readUint16()
{
   vpr::Uint16 val;
   readValueStringRep(val);
   return val;
}

vpr::Uint32 XMLObjectReader::readUint32()
{
   vpr::Uint32 val;
   readValueStringRep(val);
   return val;
}

vpr::Uint64 XMLObjectReader::readUint64()
{
   vpr::Uint64 val;
   readValueStringRep(val);
   return val;
}

float XMLObjectReader::readFloat()
{
   float val;
   readValueStringRep(val);
   return val;
}

double XMLObjectReader::readDouble()
{
   double val;
   readValueStringRep(val);
   return val;
}


std::string XMLObjectReader::readString()
{
   std::string ret_val("");

   if(AttribSource == mCurSource)   // Just return a copy of the attribute source
   {
      ret_val = mAttribSource.str();
   }
   else  // Cdata, extract the "'s around the string
   {
      char char_buffer[1024];    // Buffer for writing the final value
      char temp_char('0');

      // Get the first "
      while('"' != temp_char)
      {
         mCurNodeStack.back().cdataSource.get(temp_char);
         vprASSERT(('>' != temp_char) && ('<' != temp_char) && "String delimiter not found");
      }

      // Get the contents of the string
      bool done_reading(false);
      while(!done_reading)
      {
         mCurNodeStack.back().cdataSource.get(char_buffer,1024, '"');      // Copy over to target until we have second "
         ret_val += char_buffer;                                           // append onto end
         done_reading = (mCurNodeStack.back().cdataSource.peek() == '"');     // Done reading if got to a "
      }
      mCurNodeStack.back().cdataSource.ignore(1);                    // Ignore that " as well
   }

   return ret_val;
}

bool XMLObjectReader::readBool()
{
   bool val;
   readValueStringRep(val);
   return val;
}

} // namespace vpr

