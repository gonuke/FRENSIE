//---------------------------------------------------------------------------//
//!
//! \file   Utility_PropertyTreeCompatibleObject.hpp
//! \author Alex Robinson
//! \brief  Property tree compatible object base class declaration
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_PROPERTY_TREE_COMPATIBLE_OBJECT_HPP
#define UTILITY_PROPERTY_TREE_COMPATIBLE_OBJECT_HPP

// FRENSIE Includes
#include "Utility_PropertyTree.hpp"

namespace Utility{

/*! The base class for Utility::PropertyTree compatible objects
 * \ingroup ptree
 */
class PropertyTreeCompatibleObject 
{

public:

  //! Check if data is inlined by default when converting to a property tree
  virtual bool isDataInlinedByDefault() const = 0;

  //! Method for converting the type to a property tree
  virtual Utility::PropertyTree toPropertyTree( const bool inline_data ) const = 0;

  //! Method for converting the type to a property tree
  Utility::PropertyTree toPropertyTree() const
  { return this->toPropertyTree( this->isDataInlinedByDefault() ); }
  
  //! Method for initializing the object from a property tree
  virtual void fromPropertyTree( const Utility::PropertyTree& node,
                                 std::vector<std::string>& unused_children ) = 0;
  
  //! Method for initializing the object from a property tree
  void fromPropertyTree( const Utility::PropertyTree& node,
                         const bool log_unused_children = true );
};

/*! \brief Specialization of Utility::ToPropertyTreeTraits for 
 * Utility::PropertyTreeCompatibleObject
 * \ingroup ptree_traits
 */
template<>
struct ToPropertyTreeTraits<PropertyTreeCompatibleObject>
{
  //! Convert an object of DerivedType to a Utility::PropertyTree
  static inline PropertyTree toPropertyTree(
                                       const PropertyTreeCompatibleObject& obj,
                                       const bool inline_data )
  { return obj.toPropertyTree( inline_data ); }
};

/*! \brief Partial specialization of Utility::ToPropertyTreeTraits for 
 * types that inherit from Utility::PropertyTreeCompatibleObject
 * \ingroup ptree_traits
 */
template<typename DerivedType>
struct ToPropertyTreeTraits<DerivedType,typename std::enable_if<std::is_base_of<PropertyTreeCompatibleObject,DerivedType>::value>::type> : public ToPropertyTreeTraits<PropertyTreeCompatibleObject>
{ /* ... */ };

/*! \brief Overload of Utility::toPropertyTree for
 * Utility::PropertyTreeCompatibleObject
 *
 * The data inlining default value will be retrieved from the 
 * Utility::PropertyTreeCompatibleObject that is passed in.
 * \ingroup ptree
 */
template<typename DerivedType>
inline typename std::enable_if<std::is_base_of<PropertyTreeCompatibleObject,DerivedType>::value,Utility::PropertyTree>::type
toPropertyTree( const DerivedType& obj )
{
  return Utility::ToPropertyTreeTraits<PropertyTreeCompatibleObject>::toPropertyTree( obj, obj.isDataInlinedByDefault() );
}

/*! \brief Specialization of Utility::FromPropertyTreeTraits for types that
 * inherit from Utility::PropertyTreeCompatibleObject
 * \ingroup ptree_traits
 */
template<typename DerivedType>
struct FromPropertyTreeTraits<DerivedType,typename std::enable_if<std::is_base_of<PropertyTreeCompatibleObject,DerivedType>::value && !std::is_abstract<DerivedType>::value>::type>
{
  //! The type that a property tree will be converted to
  typedef DerivedType ReturnType;
  
  //! Convert the property tree to an object of type T
  template<template<typename,typename...> class STLCompliantSequenceContainer>
  static inline ReturnType fromPropertyTree(
                  const PropertyTree& ptree,
                  STLCompliantSequenceContainer<std::string>& unused_children )
  { 
    ReturnType obj;

    std::vector<std::string> unused_children_copy;

    obj.fromPropertyTree( ptree, unused_children );

    if( unused_children_copy.size() > 0 )
    {
      unused_children.insert( unused_children.end(),
                              unused_children_copy.begin(),
                              unused_children_copy.end() );
    }
    
    return obj;
  }

  //! Convert the property tree to an object of type T
  static inline ReturnType fromPropertyTree(
                                    const PropertyTree& ptree,
                                    std::vector<std::string>& unused_children )
  { 
    ReturnType obj;

    obj.fromPropertyTree( ptree, unused_children );

    return obj;
  }
};

// Method for initializing the object from a property tree
inline void PropertyTreeCompatibleObject::fromPropertyTree(
                                             const Utility::PropertyTree& node,
                                             const bool log_unused_children )
{
  std::vector<std::string> unused_children;

  this->fromPropertyTree( node, unused_children );

  if( log_unused_children )
    Utility::logUnusedChildrenOfPropertyTree( unused_children );
}
  
} // end Utility namespace

#endif // end UTILITY_PROPERTY_TREE_COMPATIBLE_OBJECT_HPP

//---------------------------------------------------------------------------//
// end Utility_PropertyTreeCompatibleObject.hpp
//---------------------------------------------------------------------------//
