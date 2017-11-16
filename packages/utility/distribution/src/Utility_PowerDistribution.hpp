//---------------------------------------------------------------------------//
//!
//! \file   Utility_PowerDistribution.hpp
//! \author Alex Robinson
//! \brief  Power distribution class declaration
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_POWER_DISTRIBUTION_HPP
#define UTILITY_POWER_DISTRIBUTION_HPP

// FRENSIE Includes
#include "Utility_OneDDistribution.hpp"
#include "Utility_TypeNameTraits.hpp"

namespace Utility{

/*! Power distribution class
 * \ingroup one_d_distributions
 */
template<size_t N, typename IndependentUnit, typename DependentUnit = void>
class UnitAwarePowerDistribution : public UnitAwareOneDDistribution<IndependentUnit,DependentUnit>
{
  // Typedef for base type
  typedef UnitAwareOneDDistribution<IndependentUnit,DependentUnit> BaseType;
  
  // The independent unit to power N+1
  typedef UnitTraits<typename UnitTraits<IndependentUnit>::template GetUnitToPowerType<N+1>::type> IndepUnitTraitsNp1;

  // The independent unit to power N+1 quantity type
  typedef typename IndepUnitTraitsNp1::template GetQuantityType<double>::type IndepQuantityNp1;

  // The distribution multiplier unit traits
  typedef UnitTraits<typename UnitTraits<DependentUnit>::template GetMultipliedUnitType<typename UnitTraits<IndependentUnit>::template GetUnitToPowerType<N,-1>::type>::type> DistMultiplierUnitTraits;

  // The distribution multiplier quantity type
  typedef typename DistMultiplierUnitTraits::template GetQuantityType<double>::type DistMultiplierQuantity;

  // The distribution normalization quantity type
  typedef typename BaseType::DistNormQuantity DistNormQuantity;

  // Typedef for QuantityTraits<double>
  typedef QuantityTraits<double> QT;

  // Typedef for QuantityTraits<IndepQuantity>
  typedef QuantityTraits<typename BaseType::IndepQuantity> IQT;

  // Typedef for QuantityTraits<InverseIndepQuantity>
  typedef QuantityTraits<typename BaseType::InverseIndepQuantity> IIQT;

  // Typedef for QuantityTraits<DepQuantity>
  typedef QuantityTraits<typename BaseType::DepQuantity> DQT;

  // Typedef for QuantityTraits<DistMultiplierQuantity>
  typedef QuantityTraits<DistMultiplierQuantity> DMQT;

public:

  //! This distribution type
  typedef UnitAwarePowerDistribution<N,IndependentUnit,DependentUnit> ThisType;

  //! The independent quantity type
  typedef typename BaseType::IndepQuantity IndepQuantity;

  //! The inverse independent quantity type
  typedef typename BaseType::InverseIndepQuantity InverseIndepQuantity;

  //! The dependent quantity type
  typedef typename BaseType::DepQuantity DepQuantity;

  //! Constructor ( A*x^N : x in (a,b) )
  template<typename InputIndepQuantity = IndepQuantity>
  UnitAwarePowerDistribution(
                        const double constant_multiplier =
                        ThisType::getDefaultConstMultiplier<double>(),
                        const InputIndepQuantity min_indep_limit =
                        ThisType::getDefaultLowerLimit<InputIndepQuantity>(),
                        const InputIndepQuantity max_indep_limit =
                        ThisType::getDefaultUpperLimit<InputIndepQuantity>() );

  //! Copy constructor
  template<typename InputIndepUnit, typename InputDepUnit>
  UnitAwarePowerDistribution( const UnitAwarePowerDistribution<N,InputIndepUnit,InputDepUnit>& dist_instance );

  //! Construct distribution from a unitless dist. (potentially dangerous)
  static UnitAwarePowerDistribution fromUnitlessDistribution( const UnitAwarePowerDistribution<N,void,void>& unitless_distribution );

  //! Assignment operator
  UnitAwarePowerDistribution& operator=( const UnitAwarePowerDistribution& dist_instance );

  //! Destructor (virtual for python interface extenstions)
  virtual ~UnitAwarePowerDistribution()
  { /* ... */ }

  //! Evaluate the distribution
  DepQuantity evaluate( const IndepQuantity indep_var_value ) const override;

  //! Evaluate the PDF
  InverseIndepQuantity evaluatePDF( const IndepQuantity indep_var_value) const override;

  //! Return a random sample from the distribution
  IndepQuantity sample() const override;

  //! Return a random sample from the distribution
  static IndepQuantity sample( const IndepQuantity min_independent_value,
			       const IndepQuantity max_independent_value );

  //! Return a random sample and record the number of trials
  IndepQuantity sampleAndRecordTrials( DistributionTraits::Counter& trials ) const override;

  //! Return the upper bound of the distribution independent variable
  IndepQuantity getUpperBoundOfIndepVar() const override;

  //! Return the lower bound of the distribution independent variable
  IndepQuantity getLowerBoundOfIndepVar() const override;

  //! Return the distribution type
  OneDDistributionType getDistributionType() const override;

  //! Return the distribution type name
  static std::string typeName( const bool verbose_name,
                               const bool use_template_params = false,
                               const std::string& delim = std::string() );

  //! Test if the distribution is continuous
  bool isContinuous() const override;

  //! Method for placing the object in an output stream
  void toStream( std::ostream& os ) const override;

  //! Method for initializing the object from an input stream
  void fromStream( std::istream& is, const std::string& delims ) override;

  //! Method for initializing the object from an input stream
  using IStreamableObject::fromStream;

  //! Method for converting the type to a property tree
  Utility::PropertyTree toPropertyTree( const bool inline_data ) const override;

  //! Method for converting the type to a property tree
  using PropertyTreeCompatibleObject::toPropertyTree;

  //! Method for initializing the object from a property tree
  void fromPropertyTree( const Utility::PropertyTree& node,
                         std::vector<std::string>& unused_children ) override;

  //! Method for converting to a property tree
  using PropertyTreeCompatibleObject::fromPropertyTree;

  //! Equality comparison operator
  bool operator==( const UnitAwarePowerDistribution& other ) const;

  //! Inequality comparison operator
  bool operator!=( const UnitAwarePowerDistribution& other ) const;

protected:

  //! Copy constructor (copying from unitless distribution only)
  UnitAwarePowerDistribution( const UnitAwarePowerDistribution<N,void,void>& unitless_dist_instance, int );

  //! Return the distribution type name
  std::string getDistributionTypeName( const bool verbose_name,
                                       const bool lowercase ) const override;

  //! Test if the dependent variable can be zero within the indep bounds
  bool canDepVarBeZeroInIndepBounds() const;

  //! Get the default constant multiplier
  template<typename InputDepQuantity>
  static InputDepQuantity getDefaultConstMultiplier()
  { return QuantityTraits<InputDepQuantity>::one(); }

  //! Get the default lower limit
  template<typename InputIndepQuantity>
  static InputIndepQuantity getDefaultLowerLimit()
  { return QuantityTraits<InputIndepQuantity>::zero(); }

  //! Get the default upper limit
  template<typename InputIndepQuantity>
  static InputIndepQuantity getDefaultUpperLimit()
  { return QuantityTraits<InputIndepQuantity>::inf(); }

private:

  // Initialize the distribution
  void initializeDistribution();

  // Extract a shape parameter from a node
  template<typename QuantityType>
  static void extractShapeParameterFromNode(
                             const Utility::PropertyTree& shape_parameter_data,
                             QuantityType& shape_parameter );

  // Extract a shape parameter
  template<typename QuantityType>
  static void extractShapeParameter(
                                  const Utility::Variant& shape_parameter_data,
                                  QuantityType& shape_parameter );

  // Verify that the shape parameters are valid
  static void verifyValidShapeParameters( const DepQuantity& const_multiplier,
                                          const IndepQuantity& lower_limit,
                                          const IndepQuantity& upper_limit );

  // Save the distribution to an archive
  template<typename Archive>
  void save( Archive& ar, const unsigned version ) const;

  // Load the distribution from an archive
  template<typename Archive>
  void load( Archive& ar, const unsigned version );

  BOOST_SERIALIZATION_SPLIT_MEMBER();

  // Declare the boost serialization access object as a friend
  friend class boost::serialization::access;

  // All possible instantiations are friends
  template<size_t M, typename FriendIndepUnit, typename FriendDepUnit>
  friend class UnitAwarePowerDistribution;

  // The constant multiplier value key (used in property trees)
  static const std::string s_const_multiplier_value_key;

  // The constant multiplier min match string (used in property trees)
  static const std::string s_const_multiplier_value_min_match_string;

  // The lower limit value key (used in property trees)
  static const std::string s_lower_limit_value_key;

  // The lower limit min match string (used in property trees)
  static const std::string s_lower_limit_value_min_match_string;

  // The upper limit value key (used in property trees)
  static const std::string s_upper_limit_value_key;

  // The upper limit min match string (used in property trees)
  static const std::string s_upper_limit_value_min_match_string;

  // The multiplier
  DistMultiplierQuantity d_multiplier;

  // The min independent variable limit
  IndepQuantity d_min_indep_limit;

  // The min independent variable limit to the power N+1
  IndepQuantityNp1 d_min_indep_limit_to_power_Np1;

  // The max independent variable limit
  IndepQuantity d_max_indep_limit;

  // The max independent variable limit to the power N+1
  IndepQuantityNp1 d_max_indep_limit_to_power_Np1;

  // The normalization constant
  DistNormQuantity d_norm_constant;
};

/*! The power distribution (unit-agnostic)
 * \ingroup one_d_distributions
 */
template<unsigned N> using PowerDistribution =
  UnitAwarePowerDistribution<N,void,void>;

/*! Partial specialization of Utility::TypeNameTraits for unit aware
 * evaporation distribution
 * \ingroup one_d_distributions
 * \ingroup type_name_traits
 */
template<size_t N, typename IndependentUnit,typename DependentUnit>
struct TypeNameTraits<UnitAwarePowerDistribution<N,IndependentUnit,DependentUnit> >
{
  //! Check if the type has a specialization
  typedef std::true_type IsSpecialized;

  //! Get the type name
  static inline std::string name()
  {
    return UnitAwarePowerDistribution<N,IndependentUnit,DependentUnit>::typeName( true, true  );
  }
};

/*! Specialization of Utility::TypeNameTraits for evaporation distribution
 * \ingroup one_d_distributions
 * \ingroup type_name_traits
 */
template<size_t N>
struct TypeNameTraits<PowerDistribution<N> >
{
  //! Check if the type has a specialization
  typedef std::true_type IsSpecialized;

  //! Get the type name
  static inline std::string name()
  { return PowerDistribution<N>::typeName( true, false ); }
};

} // end Utility namespace

BOOST_DISTRIBUTION_CLASS_VERSION_EXTRA( UnitAwarePowerDistribution, size_t, N, 0 );
BOOST_DISTRIBUTION_CLASS_EXPORT_KEY2_EXTRA_INT( PowerDistribution, size_t, N );

//---------------------------------------------------------------------------//
// Template includes
//---------------------------------------------------------------------------//

#include "Utility_PowerDistribution_def.hpp"

//---------------------------------------------------------------------------//

#endif // end UTILITY_POWER_DISTRIBUTION_HPP

//---------------------------------------------------------------------------//
// end Utility_PowerDistribution.hpp
//---------------------------------------------------------------------------//
