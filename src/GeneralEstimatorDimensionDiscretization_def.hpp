//---------------------------------------------------------------------------//
//!
//! \file   GeneralEstimatorDimensionDiscretization.hpp
//! \author Alex Robinson
//! \brief  General estimator dimension discretization class declaration
//!
//---------------------------------------------------------------------------//

#ifndef GENERAL_ESTIMATOR_DIMENSION_DISCRETIZATION_DEF_HPP
#define GENERAL_ESTIMATOR_DIMENSION_DISCRETIZATION_DEF_HPP

// FACEMC Includes
#include "ContractException.hpp"
#include "SearchAlgorithms.hpp"
#include "SortAlgorithms.hpp"

namespace FACEMC{

// Constructor
template<PhaseSpaceDimension dimension>
GeneralEstimatorDimensionDiscretization<dimension>::GeneralEstimatorDimensionDiscretization( const Teuchos::Array<typename DT::dimensionType>& dimension_bin_boundaries )
  : EstimatorDimensionDiscretization( dimension ),
    d_dimension_bin_boundaries( dimension_bin_boundaries )
{
  // Make sure there is at least one bin
  testPrecondition( dimension_bin_boundaries.size() >= 2 );
  // Make sure the bins are valid
  testPrecondition( dimension_bin_boundaries.front() >= DT::lowerBound() );
  testPrecondition( dimension_bin_boundaries.back() <= DT::upperBound() );
  // Make sure the bins are sorted
  testPrecondition( Sort::isSortedAscending( dimension_bin_boundaries.begin(),
					     dimension_bin_boundaries.end() ));
}

// Return the dimension name that has been discretized
template<PhaseSpaceDimension dimension>
inline std::string GeneralEstimatorDimensionDiscretization<dimension>::getDimensionName() const
{
  return DT::name();
}

// Return the number of bins in the discretization
template<PhaseSpaceDimension dimension>
inline unsigned GeneralEstimatorDimensionDiscretization<dimension>::getNumberOfBins() const
{
  return d_dimension_bin_boundaries.size() - 1u;
}

// Check if the value is contained in the dimension discretization
template<PhaseSpaceDimension dimension>
inline bool GeneralEstimatorDimensionDiscretization<dimension>::isValueInDiscretization( const Teuchos::any& any_container ) const
{
  typename DT::dimensionType value = DT::clarifyValue( any_container );

  return value >= d_dimension_bin_boundaries.front() &&
    value <= d_dimension_bin_boundaries.back();
}

// Calculate the index of the bin that the value falls in
template<PhaseSpaceDimension dimension>
unsigned GeneralEstimatorDimensionDiscretization<dimension>::calculateBinIndex( const Teuchos::any& any_container ) const
{
  // Make sure the value is in the dimension discretization
  testPrecondition( isValueInDiscretization( any_container ) );
  
  typename DT::dimensionType value = DT::clarifyValue( any_container );
  
  typename Teuchos::Array<typename DT::dimensionType>::const_iterator 
    start, end, upper_bin_boundary;
  
  start = d_dimension_bin_boundaries.begin();
  end = d_dimension_bin_boundaries.end();
  upper_bin_boundary = Search::binarySearchDiscreteData( start, end, value );
  
  unsigned distance = 
    std::distance( d_dimension_bin_boundaries.begin(), upper_bin_boundary );
  
  if( distance != 0u )
    return distance - 1u;
  else
    return distance;
}

// Print the boundaries of a bin
/*! \details Note: A new line character is not added after print the bin
 * boundaries.
 */
template<PhaseSpaceDimension dimension>
void GeneralEstimatorDimensionDiscretization<dimension>::printBoundariesOfBin(
					       std::ostream& os,
					       const unsigned bin_index ) const
{
  // Make sure the bin requested is valid
  testPrecondition( bin_index < getNumberOfBins() );
  
  os << DT::name();
  
  if( bin_index == 0u )
    os << " Bin: [";
  else
    os << " Bin: (";

  os << d_dimension_bin_boundaries[bin_index] << "," 
     << d_dimension_bin_boundaries[bin_index+1u] << "]";
}

// Print the dimension discretization
template<PhaseSpaceDimension dimension>
void GeneralEstimatorDimensionDiscretization<dimension>::print(
						       std::ostream& os ) const
{
  os << DT::name() << " Bin Boundaries: ";
    
  for( unsigned i = 0; i < d_dimension_bin_boundaries.size(); ++i )
    os << d_dimension_bin_boundaries[i] << " ";
}

} // end FACEMC namespace						     

#endif // end GENERAL_ESTIMATOR_DIMENSION_DISCRETIZATION_DEF_HPP

//---------------------------------------------------------------------------//
// end GeneralEstimatorDimensionDiscretization_def.hpp
//---------------------------------------------------------------------------//