//---------------------------------------------------------------------------//
//!
//! \file   Utility_TetMesh.hpp
//! \author Alex Robinson, Eli Moll
//! \brief  Tetrahedral mesh class declaration
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_TET_MESH_HPP
#define UTILITY_TET_MESH_HPP

// Std Lib Includes
#include <memory>

// FRENSIE Includes
#include "Utility_Mesh.hpp"
#include "Utility_Tuple.hpp"
#include "Utility_Map.hpp"

namespace Utility{

class TetMeshImpl;

/*! The tetrahedral mesh class
 * \details This class stores the mesh itself and can be used to acquire
 * important information from the mesh (e.g. intersections of a line segment
 * with mesh elements).
 */
class TetMesh : public Mesh
{

public:

  //! The mesh element handle
  typedef Mesh::ElementHandle ElementHandle;

  //! The plane index
  typedef size_t PlaneIndex;

  //! Iterator over all mesh elements
  typedef Mesh::ElementHandleIterator ElementHandleIterator;

  //! The mesh element handle, primary intersection point, track length tuple array
  typedef Mesh::ElementHandleTrackLengthArray ElementHandleTrackLengthArray;

  //! The mesh element handle, volume map
  typedef Mesh::ElementHandleVolumeMap ElementHandleVolumeMap;

  //! The mesh data tag name set
  typedef Mesh::TagNameSet TagNameSet;

  //! The mesh element handle, data map
  typedef Mesh::MeshElementHandleDataMap MeshElementHandleDataMap;

  //! Constructor
  TetMesh( const std::string& input_mesh_file_name,
           const bool verbose_construction = true,
           const bool display_warnings = true );

  //! Destructor
  ~TetMesh()
  { /* ... */ }

  //! Get the start iterator of the tet handle list
  ElementHandleIterator getStartElementHandleIterator() const final override;

  //! Get the end iterator of the tet handle list
  ElementHandleIterator getEndElementHandleIterator() const final override;

  //! Get the number of tet elements
  size_t getNumberOfElements() const final override;

  //! Returns the volumes of each mesh element
  void getElementVolumes( ElementHandleVolumeMap& tet_volumes ) const final override;

  //! Check if a point is inside of the mesh
  bool isPointInMesh( const double point[3] ) const final override;

  //! Returns the tet that contains a given point
  ElementHandle whichElementIsPointIn( const double point[3] ) const final override;

  //! Determine the mesh elements that a line segment intersects
  void computeTrackLengths( const double start_point[3],
                            const double end_point[3],
                            ElementHandleTrackLengthArray&
                            tet_element_track_lengths ) const final override;

  //! Export the mesh to a vtk file (type determined by suffix - e.g. mesh.vtk)
  virtual void export( const std::string& output_file_name,
                       TagNameSet& tag_root_names,
                       MeshElementHandleDataMap& mesh_tag_data ) const final override;

private:

  // Save the data to an archive
  template<typename Archive>
  void save( Archive& ar, const unsigned version ) const;

  // Load the data from an archive
  template<typename Archive>
  void load( Archive& ar, const unsigned version );

  BOOST_SERIALIZATION_SPLIT_MEMBER();

  // The tet mesh implementation
  std::unique_ptr<const TetMeshImpl> d_impl;
};
  
} // end Utility namespace

BOOST_CLASS_VERSION( Utility::TetMesh, 0 );
BOOST_SERIALIZATION_CLASS_EXPORT_STANDARD_KEY( TetMesh, Utility );
EXTERN_EXPLICIT_CLASS_SAVE_LOAD_INST_ALWAYS( Utility, TetMesh );

#endif // end UTILITY_TET_MESH_HPP

//---------------------------------------------------------------------------//
// end Utility_TetMesh.hpp
//---------------------------------------------------------------------------//
