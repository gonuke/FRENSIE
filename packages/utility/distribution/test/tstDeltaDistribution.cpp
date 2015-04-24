//---------------------------------------------------------------------------//
//!
//! \file   tstDeltaDistribution.cpp
//! \author Alex Robinson
//! \brief  Delta distribution unit tests.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <limits>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_XMLParameterListCoreHelpers.hpp>
#include <Teuchos_VerboseObject.hpp>

// FRENSIE Includes
#include "Utility_TabularOneDDistribution.hpp"
#include "Utility_DeltaDistribution.hpp"
#include "Utility_PhysicalConstants.hpp"

//---------------------------------------------------------------------------//
// Testing Variables
//---------------------------------------------------------------------------//

Teuchos::RCP<Teuchos::ParameterList> test_dists_list;

Teuchos::RCP<Utility::TabularOneDDistribution>
  tab_distribution( new Utility::DeltaDistribution( 0.0 ) );

Teuchos::RCP<Utility::OneDDistribution>
  distribution( tab_distribution );

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that the distribution can be evaluated
TEUCHOS_UNIT_TEST( DeltaDistribution, evaluate )
{
  TEST_EQUALITY_CONST( distribution->evaluate( 1.0 ), 0.0 );
  TEST_EQUALITY_CONST( distribution->evaluate( -1.0 ), 0.0 );
  TEST_EQUALITY_CONST( distribution->evaluate( 0.0 ), 
		       std::numeric_limits<double>::infinity() );
}

//---------------------------------------------------------------------------//
// Check that the PDF can be evaluated
TEUCHOS_UNIT_TEST( DeltaDistribution, evaluatePDF )
{
  TEST_EQUALITY_CONST( distribution->evaluatePDF( 1.0 ), 0.0 );
  TEST_EQUALITY_CONST( distribution->evaluatePDF( -1.0 ), 0.0 );
  TEST_EQUALITY_CONST( distribution->evaluatePDF( 0.0 ), 1.0 );
}

//---------------------------------------------------------------------------//
// Check that the CDF can be evaluated
TEUCHOS_UNIT_TEST( DeltaDistribution, evaluateCDF )
{
  TEST_EQUALITY_CONST( tab_distribution->evaluateCDF( -1.0 ), 0.0 );
  TEST_EQUALITY_CONST( tab_distribution->evaluateCDF( 0.0 ), 1.0 );
  TEST_EQUALITY_CONST( tab_distribution->evaluateCDF( 1.0 ), 1.0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled
TEUCHOS_UNIT_TEST( DeltaDistribution, sample )
{
  TEST_EQUALITY_CONST( distribution->sample(), 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled
TEUCHOS_UNIT_TEST( DeltaDistribution, sampleAndRecordTrials )
{
  unsigned trials = 0;
  double sample = distribution->sampleAndRecordTrials( trials );
  
  TEST_EQUALITY_CONST( sample, 0.0 );
  TEST_EQUALITY_CONST( trials, 1 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled
TEUCHOS_UNIT_TEST( DeltaDistribution, sampleAndRecordBinIndex )
{
  unsigned bin_index = 0;
  double sample = tab_distribution->sampleAndRecordBinIndex( bin_index );
  
  TEST_EQUALITY_CONST( sample, 0.0 );
  TEST_EQUALITY_CONST( bin_index, 0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled
TEUCHOS_UNIT_TEST( DeltaDistribution, sampleWithRandomNumber )
{
  double sample = tab_distribution->sampleWithRandomNumber( 0.0 );
  
  TEST_EQUALITY_CONST( sample, 0.0 );

  sample = tab_distribution->sampleWithRandomNumber( 0.5 );

  TEST_EQUALITY_CONST( sample, 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled
TEUCHOS_UNIT_TEST( DeltaDistribution, sampleInSubrange )
{
  double sample = tab_distribution->sampleInSubrange( 1.0 );
  
  TEST_EQUALITY_CONST( sample, 0.0 );

  sample = tab_distribution->sampleInSubrange( 1.0 );

  TEST_EQUALITY_CONST( sample, 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled
TEUCHOS_UNIT_TEST( DeltaDistribution, sampleWithRandomNumberInSubrange )
{
  double sample = 
    tab_distribution->sampleWithRandomNumberInSubrange( 0.0, 1.0 );
  
  TEST_EQUALITY_CONST( sample, 0.0 );

  sample = tab_distribution->sampleWithRandomNumberInSubrange( 0.5, 2.0 );

  TEST_EQUALITY_CONST( sample, 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the upper bound of the distribution independent variable can
// be returned
TEUCHOS_UNIT_TEST( DeltaDistribution, getUpperBoundOfIndepVar )
{
  TEST_EQUALITY_CONST( distribution->getUpperBoundOfIndepVar(), 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the lower bound of the distribution independent variable can 
// be returned
TEUCHOS_UNIT_TEST( DeltaDistribution, getLowerBoundOfIndepVar )
{
  TEST_EQUALITY_CONST( distribution->getLowerBoundOfIndepVar(), 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution type can be returned
TEUCHOS_UNIT_TEST( DeltaDistribution, getDistributionType )
{
  TEST_EQUALITY_CONST( distribution->getDistributionType(), 
		       Utility::DELTA_DISTRIBUTION );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be written to an xml file
TEUCHOS_UNIT_TEST( DeltaDistribution, toParameterList )
{
  Teuchos::RCP<Utility::DeltaDistribution> true_distribution =
    Teuchos::rcp_dynamic_cast<Utility::DeltaDistribution>( distribution );
  
  Teuchos::ParameterList parameter_list;
  
  parameter_list.set<Utility::DeltaDistribution>( "test distribution", 
						  *true_distribution );

  Teuchos::writeParameterListToXmlFile( parameter_list,
					"delta_dist_test_list.xml" );
  
  Teuchos::RCP<Teuchos::ParameterList> read_parameter_list = 
    Teuchos::getParametersFromXmlFile( "delta_dist_test_list.xml" );
  
  TEST_EQUALITY( parameter_list, *read_parameter_list );
  
  Teuchos::RCP<Utility::DeltaDistribution> 
    copy_distribution( new Utility::DeltaDistribution );
  
  *copy_distribution = read_parameter_list->get<Utility::DeltaDistribution>(
							  "test distribution");

  TEST_EQUALITY( *copy_distribution, *true_distribution );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be read from an xml file
TEUCHOS_UNIT_TEST( DeltaDistribution, fromParameterList )
{
  Utility::DeltaDistribution distribution = 
    test_dists_list->get<Utility::DeltaDistribution>( "Delta Distribution A" );

  TEST_EQUALITY_CONST( distribution.getLowerBoundOfIndepVar(), 0 );

  distribution = 
    test_dists_list->get<Utility::DeltaDistribution>( "Delta Distribution B" );

  TEST_EQUALITY_CONST( distribution.getLowerBoundOfIndepVar(), 
		       Utility::PhysicalConstants::pi );

  distribution = 
    test_dists_list->get<Utility::DeltaDistribution>( "Delta Distribution C" );
  
  TEST_EQUALITY_CONST( distribution.getLowerBoundOfIndepVar(), 
		       -Utility::PhysicalConstants::pi/2 );
}

//---------------------------------------------------------------------------//
// Custom main function
//---------------------------------------------------------------------------//
int main( int argc, char** argv )
{
  std::string test_dists_xml_file;

  Teuchos::CommandLineProcessor& clp = Teuchos::UnitTestRepository::getCLP();

  clp.setOption( "test_dists_xml_file",
		 &test_dists_xml_file,
		 "Test distributions xml file name" );

  const Teuchos::RCP<Teuchos::FancyOStream> out = 
    Teuchos::VerboseObjectBase::getDefaultOStream();
  
  Teuchos::CommandLineProcessor::EParseCommandLineReturn parse_return = 
    clp.parse(argc,argv);

  if ( parse_return != Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL ) 
  {
    *out << "\nEnd Result: TEST FAILED" << std::endl;
    return parse_return;
  }

  TEUCHOS_ADD_TYPE_CONVERTER( Utility::DeltaDistribution );

  test_dists_list = Teuchos::getParametersFromXmlFile( test_dists_xml_file );

  Teuchos::GlobalMPISession mpiSession( &argc, &argv );
  
  const bool success = Teuchos::UnitTestRepository::runUnitTests(*out);

  if (success)
    *out << "\nEnd Result: TEST PASSED" << std::endl;
  else
    *out << "\nEnd Result: TEST FAILED" << std::endl;

  clp.printFinalTimerSummary(out.ptr());

  return (success ? 0 : 1);
}

//---------------------------------------------------------------------------//
// end tstDeltaDistribution.cpp
//---------------------------------------------------------------------------//
