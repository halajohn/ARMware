// ARMware - an ARM emulator
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#if CONFIG_DATABASE_XML

#include <cassert>

#include <sstream>
#include <libxml++/libxml++.h>

#include "DatabaseDep.hpp"
#include "../MachineImpl.hpp"

namespace ARMware
{
  //////////////////////////////// Private ////////////////////////////////////
  //============================== Attribute ==================================
  
  std::string const DatabaseDep::KEY_DATABASE_ROOT_NAME = "ARMware";
  std::string const DatabaseDep::KEY_MACHINE_NAME = "name";
  std::string const DatabaseDep::KEY_ROM_FILE = "rom_file";
  std::string const DatabaseDep::KEY_SCREEN_WIDTH = "screen_width";
  std::string const DatabaseDep::KEY_SCREEN_HEIGHT = "screen_height";
  
  //============================ Operation ====================================
  
  std::string
  DatabaseDep::get_field_content(xmlpp::Element * const element)
  {
    xmlpp::Node::NodeList nodelist1 = element->get_children();
    
    for (std::list<xmlpp::Node *>::iterator iter1 = nodelist1.begin();
         iter1 != nodelist1.end();
         ++iter1)
    {
      xmlpp::Node * node1 = *iter1;
      
      if (typeid(*node1) == typeid(xmlpp::TextNode))
      {
        xmlpp::TextNode * textnode1 = static_cast<xmlpp::TextNode *>(node1);
        
        if (false == textnode1->is_white_space())
        {
          return textnode1->get_content();
        }
      }
    }
    
    assert(!"Should not reach here.");
    return "error";
  }
  
  void
  DatabaseDep::parse_machine_record(xmlpp::Node * const xmlnode, MachineImpl * const machine_impl)
  {
    xmlpp::Node::NodeList nodelist = xmlnode->get_children();
    std::stringstream s;
    
    for (std::list<xmlpp::Node *>::iterator iter = nodelist.begin();
         iter != nodelist.end();
         ++iter)
    {
      xmlpp::Node * node = *iter;
      
      if (typeid(*node) == typeid(xmlpp::Element))
      {
        if (KEY_MACHINE_NAME == node->get_name())
        {
          machine_impl->set_machine_name(get_field_content(static_cast<xmlpp::Element *>(node)));
        }
        else if (KEY_ROM_FILE == node->get_name())
        {
          machine_impl->set_rom_file(get_field_content(static_cast<xmlpp::Element *>(node)));
        }
        else if (KEY_SCREEN_WIDTH == node->get_name())
        {
          uint32_t width;
          
          s << get_field_content(static_cast<xmlpp::Element *>(node));
          s >> width;
          
          machine_impl->set_screen_width(width);
          
          s.clear();
          s.str("");
        }
        else if (KEY_SCREEN_HEIGHT == node->get_name())
        {
          uint32_t height;
          
          s << get_field_content(static_cast<xmlpp::Element *>(node));
          s >> height;
          
          machine_impl->set_screen_height(height);
          
          s.clear();
          s.str("");
        }
      }
    }
  }
  
  void
  DatabaseDep::store_machine_record(xmlpp::Element * const root, MachineImpl const * const machine)
  {
    std::stringstream s;
    std::string str;
    xmlpp::Element * machine_node;
    xmlpp::Element * element;
    
    machine_node = root->add_child("machine");
    
    element = machine_node->add_child("name");
    element->add_child_text(machine->get_machine_name());
    
    element = machine_node->add_child("rom_file");
    element->add_child_text(machine->get_rom_file());
    
    element = machine_node->add_child("screen_width");
    s << machine->get_screen_width();
    s >> str;
    element->add_child_text(str);
    
    s.clear();
    s.str("");
    str.clear();
    
    element = machine_node->add_child("screen_height");
    s << machine->get_screen_height();
    s >> str;
    element->add_child_text(str);
  }
  
  //////////////////////////////// Public /////////////////////////////////////
  //============================ Life cycle ===================================
  
  DatabaseDep::DatabaseDep()
  {
    try
    {
      xmlpp::DomParser parser(m_db_filename.c_str());
      xmlpp::Document * document = parser.get_document();
      xmlpp::Element * root = document->get_root_node();
      
      assert(KEY_DATABASE_ROOT_NAME == root->get_name());
      
      xmlpp::Node::NodeList nodelist = root->get_children();
      
      for (std::list<xmlpp::Node *>::iterator iter = nodelist.begin();
           iter != nodelist.end();
           ++iter)
      {
        xmlpp::Node * node = *iter;
        
        if (typeid(*node) == typeid(xmlpp::Element))
        {
          assert("machine" == node->get_name());
          
          MachineImpl new_machine_impl;
          
          parse_machine_record(node, &new_machine_impl);
          
          m_list.push_back(new_machine_impl);
        }
      }
    }
    catch (xmlpp::parse_error &/* e */)
    {
      // :NOTE: Wei 2004-Jun-03:
      //
      // Create an initial xml file.
      xmlpp::Document document;
      
      document.create_root_node(KEY_DATABASE_ROOT_NAME);
      document.write_to_file_formatted(m_db_filename.c_str());
    }
    catch (...)
    {
      // :NOTE: Wei 2004-Jun-03:
      //
      // unknown error, need trace.
      assert(!"Should not reach here.");
    }
  }
  
  DatabaseDep::~DatabaseDep()
  {
    xmlpp::Document document;
    
    document.create_root_node(KEY_DATABASE_ROOT_NAME);
    xmlpp::Element * root = document.get_root_node();
    
    for (std::deque<MachineImpl>::const_iterator iter = m_list.begin();
         iter != m_list.end();
         ++iter)
    {
      store_machine_record(root, &(*iter));
    }
    
    document.write_to_file_formatted(m_db_filename.c_str());
  }
}

#endif
