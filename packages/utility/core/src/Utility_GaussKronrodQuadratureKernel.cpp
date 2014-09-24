//---------------------------------------------------------------------------//
//!
//! \file   Utility_GaussKronrodQuadratureKernel.cpp
//! \author Alex Robinson
//! \brief  Gauss-Kronrod quadrature kernel
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "Utility_GaussKronrodQuadratureKernel.hpp"
#include "Utility_ContractException.hpp"

namespace Utility{

// Initialize static member data
gsl_error_handler_t* GaussKronrodQuadratureKernel::default_gsl_error_handler =
  NULL;

// Allow the kernel to throw exceptions
void GaussKronrodQuadratureKernel::throwExceptions( const bool allow_throw )
{
  if( allow_throw )
  {
    GaussKronrodQuadratureKernel::default_gsl_error_handler = 
      gsl_set_error_handler_off();
  }
  else if( default_gsl_error_handler )
    gsl_set_error_handler( default_gsl_error_handler );
  // else - default is already being used
}

// Constructor
GaussKronrodQuadratureKernel::GaussKronrodQuadratureKernel( 
				              const double relative_error_tol,
					      const double absolute_error_tol,
				              const size_t subinterval_limit,
					      const size_t workspace_size )
  : d_relative_error_tol( relative_error_tol ),
    d_absolute_error_tol( absolute_error_tol ),
    d_subinterval_limit( subinterval_limit ),
    d_workspace_size( workspace_size )
{
  // Make sure the error tolerances are valid
  testPrecondition( relative_error_tol >= 0.0 );
  testPrecondition( absolute_error_tol >= 0.0 );
  // Make sure the work space size is valid
  testPrecondition( workspace_size > 0 );
  // Make sure the subinterval limit is valid
  testPrecondition( subinterval_limit > 0 );
  testPrecondition( subinterval_limit <= workspace_size );
}

// Destructor
GaussKronrodQuadratureKernel::~GaussKronrodQuadratureKernel()
{ /* ... */ }

} // end Utility namespace

//---------------------------------------------------------------------------//
// end Utility_GaussKronrodQuadratureKernel.cpp
//---------------------------------------------------------------------------//
