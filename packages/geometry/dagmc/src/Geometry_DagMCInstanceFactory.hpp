//---------------------------------------------------------------------------//
//!
//! \file   Geometry_DagMCInstanceFactory.hpp
//! \author Alex Robinson
//! \brief  DagMC instance factory class declaration.
//!
//---------------------------------------------------------------------------//

#ifndef GEOMETRY_DAGMC_INSTANCE_FACTORY_HPP
#define GEOMETRY_DAGMC_INSTANCE_FACTORY_HPP

// Std Lib Includes
#include <stdexcept>

// Trilinos Includes
#include <Teuchos_ParameterList.hpp>

namespace Geometry{

//! The DagMC instance factory
class DagMCInstanceFactory
{
  
public:
  
  //! Initialize DagMC (singleton class)
  static void initializeDagMC( const Teuchos::ParameterList& geom_rep );

private:

  // Validate a geometry representation
  static void validateGeometryRep( const Teuchos::ParameterList& geom_rep );
};

//! The invalid geometry representation error
class InvalidGeometryRepresentation : public std::logic_error
{

public:

  InvalidGeometryRepresentation( const std::string& what_arg )
    : std::logic_error( what_arg )
  { /* ... */ }
};

} // end Geometry namespace

#endif // end GEOMETRY_DAGMC_INSTANCE_FACTORY_HPP

//---------------------------------------------------------------------------//
// end Geometry_DagMCInstanceFactory.hpp
//---------------------------------------------------------------------------//