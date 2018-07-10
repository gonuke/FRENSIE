//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ParticleCrossingSurfaceEventObserver.hpp
//! \author Alex Robinson
//! \brief  Particle crossing surface event observer base class declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_PARTICLE_CROSSING_SURFACE_EVENT_OBSERVER_HPP
#define MONTE_CARLO_PARTICLE_CROSSING_SURFACE_EVENT_OBSERVER_HPP

// Boost Includes
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

// FRENSIE Includes
#include "MonteCarlo_ParticleEventTags.hpp"
#include "MonteCarlo_ParticleState.hpp"
#include "Geometry_Model.hpp"
#include "Utility_Vector.hpp"
#include "Utility_ExplicitSerializationTemplateInstantiationMacros.hpp"
#include "Utility_SerializationHelpers.hpp"

namespace MonteCarlo{

/*! The particle crossing surface event observer base class
 * \ingroup particle_crossing_surface_event
 */
class ParticleCrossingSurfaceEventObserver
{

public:

  //! Typedef for the observer event tag
  typedef ParticleCrossingSurfaceEvent EventTag;

  //! Constructor
  ParticleCrossingSurfaceEventObserver()
  { /* ... */ }

  //! Destructor
  virtual ~ParticleCrossingSurfaceEventObserver()
  { /* ... */ }

  //! Update the observer
  virtual void updateFromParticleCrossingSurfaceEvent(
	  const ParticleState& particle,
	  const Geometry::Model::InternalSurfaceHandle surface_crossing,
	  const double angle_cosine ) = 0;

private:

  // Serialize the observer
  template<typename Archive>
  void serialize( Archive& ar, const unsigned version )
  { /* ... */ }

  // Declare the boost serialization access object as a friend
  friend class boost::serialization::access;
};

} // end MonteCarlo namespace

BOOST_CLASS_VERSION( MonteCarlo::ParticleCrossingSurfaceEventObserver, 0 );
BOOST_SERIALIZATION_ASSUME_ABSTRACT( MonteCarlo::ParticleCrossingSurfaceEventObserver );
EXTERN_EXPLICIT_CLASS_SERIALIZE_INST( MonteCarlo, ParticleCrossingSurfaceEventObserver );

#endif // end MONTE_CARLO_PARTICLE_CROSSING_SURFACE_EVENT_OBSERVER_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ParticleCrossingSurfaceEventObserver.hpp
//---------------------------------------------------------------------------//
