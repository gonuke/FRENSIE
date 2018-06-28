//---------------------------------------------------------------------------//
//!
//! \file   Utility_Mesh.cpp
//! \author Alex Robinson
//! \brief  The mesh base class definition
//!
//---------------------------------------------------------------------------//

// Boost Includes
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>

// FRENSIE Includes
#include "Utility_HDF5OArchive.hpp" // This must be included first
#include "Utility_HDF5IArchive.hpp" // This must be included second
#include "Utility_Mesh.hpp"
#include "Utility_MOABException.hpp"
#include "Utility_ExceptionTestMacros.hpp"
#include "Utility_ContractException.hpp"
#include "FRENSIE_config.hpp"

// Moab Includes
#ifdef HAVE_FRENSIE_MOAB
#include <moab/Interface.hpp>
#endif // end HAVE_FRENSIE_MOAB

namespace Utility{

//! Default implementation of export method
void Mesh::exportImpl( const std::string& output_file_name,
                       const TagNameSet& tag_root_names,
                       const MeshElementHandleDataMap& mesh_tag_data,
                       void* obfuscated_moab_interface,
                       const ElementHandle mesh_handle,
                       const std::function<ElementHandle(const ElementHandle)>&
                       convert_external_element_handle_to_internal_handle ) const
{
#ifdef HAVE_FRENSIE_MOAB
  // Extract the moab interface pointer from the void point
  moab::Interface* moab_interface =
    (moab::Interface*)obfuscated_moab_interface;

  moab::ErrorCode return_value;
  
  std::map<std::string,std::vector<moab::Tag> > tags;

  if( !tag_root_names.empty() )
  {
    // Initialize the tags
    size_t tag_size = 0;

    if( !mesh_tag_data.empty() )
    {
      auto mesh_element_tag_data_it = mesh_tag_data.begin()->second.begin();
      auto mesh_element_tag_data_end = mesh_tag_data.begin()->second.end();

      if( mesh_element_tag_data_it != mesh_element_tag_data_end )
        tag_size = mesh_element_tag_data_it->second.size();
    }
    
    for( auto&& tag_root_name : tag_root_names )
      tags[tag_root_name].resize( tag_size );

    for( auto&& mesh_element_tag_data : mesh_tag_data )
    {
      moab::EntityHandle mesh_element_handle =
        convert_external_element_handle_to_internal_handle( mesh_element_tag_data.first );

      for( auto&& tag_data : mesh_element_tag_data.second )
      {
        const std::string& tag_name_prefix = tag_data.first;

        TEST_FOR_EXCEPTION( tags.find( tag_name_prefix ) == tags.end(),
                            "Tag name prefix " << tag_name_prefix <<
                            "for mesh element "
                            << mesh_element_tag_data.first << " does not "
                            "correspond to any of the specified "
                            "tag names (" << tag_root_names << ")!" );

        std::vector<moab::Tag>& tag = tags[tag_name_prefix];

        TEST_FOR_EXCEPTION( tag.size() != tag_data.second.size(),
                            "Tag " << tag_name_prefix << " for mesh element "
                            << mesh_element_tag_data.first << " does not "
                            "have the correct size (" << tag.size() " != "
                            << tag_data.second.size() << ")!" );

        for( size_t i = 0; i < tag_data.second.size(); ++i )
        {
          const std::string tag_name =
            tag_name_prefix + "_" + tag_data[i].first;

          return_value = moab_interface->tag_get_handle(
                                       tag_name.c_str(),
                                       1,
                                       moab::MB_TYPE_DOUBLE,
                                       tag[i],
                                       moab::MB_TAG_DENSE|moab::MB_TAG_CREAT );

          TEST_FOR_EXCEPTION( return_value != moab::MB_SUCCESS,
                              Utility::MOABException,
                              moab::ErrorCodeStr[return_value] );

          return_value = moab_interface->tag_set_data(
                                               tag[i],
                                               &mesh_element_handle,
                                               1,
                                               &tag_data.second[i] );

          TEST_FOR_EXCEPTION( return_value != moab::MB_SUCCESS,
                              Utility::MOABException,
                              moab::ErrorCodeStr[return_value] );
        }
      }
    }
  }

  // Consolidate the created tags
  std::vector<moab::Tag> output_tags;

  for( auto&& tag_data : tags )
  {
    output_tags.insert( output_tags.end(),
                        tag_data.second.begin(),
                        tag_data.second.end() );
  }

  // Export the mesh
  moab::EntityHandle internal_mesh_handle = mesh_handle;
  
  return_value = moab_interface->write_file( output_file_name.c_str(),
                                             NULL,
                                             NULL,
                                             &internal_mesh_handle,
                                             1,
                                             output_tags.data(),
                                             output_tags.size() );

  TEST_FOR_EXCEPTION( return_value != moab::MB_SUCCESS,
                      Utility::MOABException,
                      moab::ErrorCodeStr[return_value] );
#endif // end HAVE_FRENSIE_MOAB
}

} // end Utility namespace

EXPLICIT_CLASS_SERIALIZE_INST( Utility, Mesh );

//---------------------------------------------------------------------------//
// end Utility_Mesh.cpp
//---------------------------------------------------------------------------//
