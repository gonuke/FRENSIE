//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_SourceModuleInterface_Native.hpp
//! \author Alex Robinson
//! \brief  Native specialization decl. of the source ModuleInterface class
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_SOURCE_MODULE_INTERFACE_NATIVE_HPP
#define MONTE_CARLO_SOURCE_MODULE_INTERFACE_NATIVE_HPP

// Std Lib Includes
#include <memory>

// FRENSIE Includes
#include "MonteCarlo_SourceModuleInterfaceDecl.hpp"
#include "MonteCarlo_ParticleSource.hpp"
#include "MonteCarlo_ParticleBank.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

/*! The specialization of the source module inteface class for the native
 * source handler.
 * \ingroup source_module
 */
template<>
class SourceModuleInterface<ParticleSource>
{

public:

  //! The external source handle class (used within the source handler)
  typedef int ExternalSourceHandle;
  
  //! The internal source handle class (used within FRENSIE)
  typedef ModuleTraits::InternalSourceHandle InternalSourceHandle;

  //! The value of an invalid external source handle
  static const ExternalSourceHandle invalid_external_source_handle;
  
  //! Initialize the source
  static void setHandlerInstance( const std::shared_ptr<ParticleSource>& source );
  
  //! Sample a particle state (or possibly states)
  static void sampleParticleState( ParticleBank& bank,
				   const unsigned long long history );

  //! Return the sampling efficiency
  static double getSamplingEfficiency();
  
private:

  //! Constructor
  SourceModuleInterface();

  // Pointer to source
  static std::shared_ptr<ParticleSource> source;
};

// Sample the starting particle state
inline void SourceModuleInterface<ParticleSource>::sampleParticleState( 
					     ParticleBank& bank,
					     const unsigned long long history )
{
  testPrecondition( SourceModuleInterface::source.get() );
  
  SourceModuleInterface::source->sampleParticleState( bank, history );
}

// Get the sampling efficiency
inline double SourceModuleInterface<ParticleSource>::getSamplingEfficiency()
{
  testPrecondition( SourceModuleInterface::source.get() );
  
  return SourceModuleInterface::source->getSamplingEfficiency();
}

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_SOURCE_MODULE_INTERFACE_NATIVE_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_SourceModuleInterface_Native.hpp
//---------------------------------------------------------------------------//
