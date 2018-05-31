//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ParticleResponse.cpp
//! \author Alex Robinson
//! \brief  Particle response base class definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <limits>
#include <memory>

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
#include "MonteCarlo_ParticleResponse.hpp"
#include "Utility_HDF5IArchive.hpp"
#include "Utility_HDF5OArchive.hpp"
#include "Utility_ToStringTraits.hpp"
#include "Utility_ExceptionTestMacros.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

//! The default particle response
class DefaultParticleResponse : public ParticleResponse
{
  
public:

  //! Constructor
  DefaultParticleResponse()
    : ParticleResponse( "f(particle) = 1" )
  { /* ... */ }

  //! Destructor
  ~DefaultParticleResponse()
  { /* ... */ }

  //! Evaluate the response at the desired phase space point
  double evaluate( const ParticleState& ) const final override
  { return 1.0; }

  //! Check if the response is spatially uniform
  bool isSpatiallyUniform() const final override
  { return true; }

private:

  // Serialize the particle response
  template<typename Archive>
  void serialize( Archive& ar, const unsigned version )
  { ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( ParticleResponse ); }

  // Declare the boost serialization access object as a friend
  friend class boost::serialization::access;
};

// Constructor
ParticleResponse::ParticleResponse( const std::string& name )
  : d_name( name )
{ 
  // Make sure that the name is valid
  testPrecondition( name.size() > 0 );
}

// Destructor
ParticleResponse::~ParticleResponse()
{ /* ... */ }

// Default response function (always evaluates to 1.0)
std::shared_ptr<const ParticleResponse>
ParticleResponse::getDefault()
{
  return std::make_shared<DefaultParticleResponse>();
}

// Return the name of the response function
const std::string& ParticleResponse::getName() const
{
  return d_name;
}

// Evaluate the response function at the desired phase space point
double ParticleResponse::evaluate( const ParticleState& ) const
{
  return 1.0;
}

// Check if the response function is spatially uniform
bool ParticleResponse::isSpatiallyUniform() const
{
  return true;
}
  
} // end MonteCarlo namespace

BOOST_SERIALIZATION_CLASS_VERSION( DefaultParticleResponse, MonteCarlo, 0 );
BOOST_SERIALIZATION_CLASS_EXPORT_STANDARD_KEY( DefaultParticleResponse, MonteCarlo );
BOOST_SERIALIZATION_CLASS_EXPORT_IMPLEMENT( DefaultParticleResponse, MonteCarlo );

EXPLICIT_MONTE_CARLO_CLASS_SERIALIZE_INST( MonteCarlo::ParticleResponse );
EXPLICIT_MONTE_CARLO_CLASS_SERIALIZE_INST( MonteCarlo::DefaultParticleResponse );

//---------------------------------------------------------------------------//
// end MonteCarlo_ParticleResponse.cpp
//---------------------------------------------------------------------------//
