//---------------------------------------------------------------------------//
// \file   FACEMC_UnitTestHarnessExtensions.hpp
// \author Alex Robinson
// \brief  Extentions to the Teuchos_UnitTestHarness for FACEMC
//---------------------------------------------------------------------------//

#ifndef FACEMC_UNIT_TEST_HARNESS_EXTENSIONS_HPP
#define FACEMC_UNIT_TEST_HARNESS_EXTENSIONS_HPP

// Std Lib Includes
#include <string>

// Trilinos Includes
#include <Teuchos_FancyOStream.hpp>

// FACEMC Includes
#include "TestingHelperFunctions.hpp"
#include "TupleTestingTypedefs.hpp"

// Define a new Macro for the Teuchos Unit Test Harness for creating a
// templated unit test on one type and one template template parameter
#define FACEMC_UNIT_TEST_TYPE_1_ARRAY_TEMPLATE_DECL(TEST_GROUP, TEST_NAME, TYPE, ARRAY) \
  template<typename TYPE, template<typename> class ARRAY> \
  class TEST_GROUP##_##TEST_NAME##_UnitTest : public Teuchos::UnitTestBase \
  {									\
  public:								\
    TEST_GROUP##_##TEST_NAME##_UnitTest(				\
      const std::string& typeName,					\
      const std::string& arrayName					\
    )                                                                   \
    : Teuchos::UnitTestBase(						\
	#TEST_GROUP, std::string(#TEST_NAME)+"_Teuchos::"+arrayName+"<"+typeName+">" ) \
    {}									\
    void runUnitTestImpl( Teuchos::FancyOStream &out, bool &success ) const; \
    virtual std::string unitTestFile() const { return __FILE__; }	\
    virtual long int unitTestFileLineNumber() const { return __LINE__; } \
  };									\
									\
  template<typename TYPE, template<typename> class ARRAY>		\
  void TEST_GROUP##_##TEST_NAME##_UnitTest<TYPE,ARRAY>::runUnitTestImpl( \
                      Teuchos::FancyOStream &out, bool &success ) const \
    
// Define a new Macro for the Teuchos Unit Test Harness for instantiating a
// templated unit test on one type and one template template parameter
#define FACEMC_UNIT_TEST_TYPE_1_ARRAY_TEMPLATE_INSTANT(TEST_GROUP, TEST_NAME, TYPE, ARRAY) \
  template class TEST_GROUP##_##TEST_NAME##_UnitTest<TYPE, Teuchos::ARRAY >; \
  TEST_GROUP##_##TEST_NAME##_UnitTest<TYPE, Teuchos::ARRAY >		\
  instance_##TEST_GROUP##_##TYPE##_##ARRAY##_##TEST_NAME##_UnitTest(#TYPE,#ARRAY);

// Define a new Macro for the Teuchos Unit Test Harness for creating a
// templated unit test on a Tuple Member and a type
#define FACEMC_UNIT_TEST_MEMBER_1_TUPLE_1_TEMPLATE_DECL(TEST_GROUP, TEST_NAME, MEMBER, TUPLE) \
  template<FACEMC::TupleMember MEMBER, typename TUPLE>			\
  class TEST_GROUP##_##TEST_NAME##_UnitTest : public Teuchos::UnitTestBase \
  {									\
  public:								\
    TEST_GROUP##_##TEST_NAME##_UnitTest(				\
      const std::string& memberName,					\
      const std::string& tupleName					\
    )                                                                   \
    : Teuchos::UnitTestBase(						\
	#TEST_GROUP, std::string(#TEST_NAME)+"_"+memberName+"_"+tupleName ) \
    {}									\
    void runUnitTestImpl( Teuchos::FancyOStream &out, bool &success ) const; \
    virtual std::string unitTestFile() const { return __FILE__; }	\
    virtual long int unitTestFileLineNumber() const { return __LINE__; } \
  };									\
									\
  template<FACEMC::TupleMember MEMBER, typename TUPLE>			\
  void TEST_GROUP##_##TEST_NAME##_UnitTest<MEMBER,TUPLE>::runUnitTestImpl( \
         Teuchos::FancyOStream &out, bool &success ) const \

// Define a new Macro for the Teuchos Unit Test Harness for instantiating a
// templated unit test on a Tuple Member and a type
#define FACEMC_UNIT_TEST_MEMBER_1_TUPLE_1_TEMPLATE_INSTANT(TEST_GROUP, TEST_NAME, MEMBER, TUPLE) \
  template class TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER, TUPLE>; \
  TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER, TUPLE>		\
  instance_##TEST_GROUP##_##MEMBER##_##TUPLE##_##TEST_NAME##_UnitTest(#MEMBER,#TUPLE);

// Define a new Macro for the Teuchos Unit Test Harness for create a 
// templated unit test on two Tuple Member, one Tuple and a Teuchos Array
#define FACEMC_UNIT_TEST_MEMBER_2_TUPLE_1_ARRAY_TEMPLATE_DECL(TEST_GROUP, TEST_NAME, MEMBER1, MEMBER2, TUPLE, ARRAY) \
  template<FACEMC::TupleMember MEMBER1,					\
	   FACEMC::TupleMember MEMBER2,					\
	   typename TUPLE,						\
	   template<typename> class ARRAY>				\
  class TEST_GROUP##_##TEST_NAME##_UnitTest : public Teuchos::UnitTestBase \
  {									\
  public:								\
    TEST_GROUP##_##TEST_NAME##_UnitTest(					\
      const std::string& member1Name,					\
      const std::string& member2Name,					\
      const std::string& tupleName,					\
      const std::string& arrayName					\
    )                                                                   \
    : Teuchos::UnitTestBase(						\
	#TEST_GROUP, std::string(#TEST_NAME)+"_Teuchos::"+arrayName+"<"+tupleName+">_"+member1Name+"_"+member2Name ) \
    {}									\
    void runUnitTestImpl( Teuchos::FancyOStream &out, bool &success ) const; \
    virtual std::string unitTestFile() const { return __FILE__; }	\
    virtual long int unitTestFileLineNumber() const { return __LINE__; } \
  };									\
									\
  template<FACEMC::TupleMember MEMBER1,					\
	   FACEMC::TupleMember MEMBER2,					\
	   typename TUPLE,						\
	   template<typename> class ARRAY>				\
  void TEST_GROUP##_##TEST_NAME##_UnitTest<MEMBER1,MEMBER2,TUPLE,ARRAY>::runUnitTestImpl( \
	Teuchos::FancyOStream &out, bool &success ) const \

// Define a new Macro for the Teuchos Unit Test Harness for instantiating a
// templated unit test on two Tuple Members, one Tuple and a Teuchos Array
#define FACEMC_UNIT_TEST_MEMBER_2_TUPLE_1_ARRAY_TEMPLATE_INSTANT(TEST_GROUP, TEST_NAME, MEMBER1, MEMBER2, TUPLE, ARRAY) \
  template class TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER1, FACEMC::MEMBER2, TUPLE, Teuchos::ARRAY>; \
  TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER1, FACEMC::MEMBER2, TUPLE, Teuchos::Array> \
  instance_##TEST_GROUP##_##MEMBER##_##MEMBER1##_##MEMBER2##_##TUPLE##_##ARRAY##_##TEST_NAME##_UnitTest(#MEMBER1,#MEMBER2,#TUPLE,#ARRAY);

// Define a new Macro for the Teuchos Unit Test Harness for creating a 
// templated unit test on two Tuple Members, two Tuples and a Teuchos Array
#define FACEMC_UNIT_TEST_MEMBER_2_TUPLE_2_ARRAY_TEMPLATE_DECL(TEST_GROUP, TEST_NAME, MEMBER1, MEMBER2, TUPLE1, TUPLE2, ARRAY) \
  template<FACEMC::TupleMember MEMBER1,					\
	   FACEMC::TupleMember MEMBER2,					\
	   typename TUPLE1,						\
	   typename TUPLE2,						\
	   template<typename> class ARRAY>				\
  class TEST_GROUP##_##TEST_NAME##_UnitTest : public Teuchos::UnitTestBase \
  {									\
  public:								\
    TEST_GROUP##_##TEST_NAME##_UnitTest(				\
      const std::string& member1Name,					\
      const std::string& member2Name,					\
      const std::string& tuple1Name,					\
      const std::string& tuple2Name,					\
      const std::string& arrayName					\
    )                                                                   \
    : Teuchos::UnitTestBase(						\
        #TEST_GROUP, std::string(#TEST_NAME)+"_Teuchos::"+arrayName+"<"+tuple1Name+">_"+member1Name+"_Teuchos::"+arrayName+"<"+tuple2Name+">_"+member2Name )\
    {}									\
    void runUnitTestImpl( Teuchos::FancyOStream &out, bool &success ) const; \
    virtual std::string unitTestFile() const { return __FILE__; }	\
    virtual long int unitTestFileLineNumber() const { return __LINE__; } \
  };						                        \
									\
    template<FACEMC::TupleMember MEMBER1,				\
	     FACEMC::TupleMember MEMBER2,				\
	     typename TUPLE1,						\
	     typename TUPLE2,						\
	     template<typename> class ARRAY>				\
    void TEST_GROUP##_##TEST_NAME##_UnitTest<MEMBER1,MEMBER2,TUPLE1,TUPLE2,ARRAY>::runUnitTestImpl( \
	Teuchos::FancyOStream &out, bool &success ) const \
  
// Define a new Macro for the Teuchos Unit Test Harness for instantiating a 
// templated unit test on two Tuple Members, two Tuples and a Teuchos Array
#define FACEMC_UNIT_TEST_MEMBER_2_TUPLE_2_ARRAY_TEMPLATE_INSTANT(TEST_GROUP, TEST_NAME, MEMBER1, MEMBER2, TUPLE1, TUPLE2, ARRAY) \
  template class TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER1, FACEMC::MEMBER2, TUPLE1, TUPLE2, Teuchos::ARRAY>; \
  TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER1, FACEMC::MEMBER2, TUPLE1, TUPLE2, Teuchos::Array> \
  instance_##TEST_GROUP##_##MEMBER##_##MEMBER1##_##MEMBER2##_##TUPLE1##_##TUPLE2##_##ARRAY##_##TEST_NAME##_UnitTest(#MEMBER1,#MEMBER2,#TUPLE1,#TUPLE2,#ARRAY);

// Define a new Macro for the Teuchos Unit Test Harness for creating a
// templated unit test on two types and one template template parameter
#define FACEMC_UNIT_TEST_MEMBER_1_TUPLE_1_ARRAY_TEMPLATE_DECL(TEST_GROUP, TEST_NAME, MEMBER, TYPE1, ARRAY) \
  template<FACEMC::TupleMember MEMBER, typename TYPE1, template<typename> class ARRAY> \
  class TEST_GROUP##_##TEST_NAME##_UnitTest : public Teuchos::UnitTestBase \
  {									\
  public:								\
    TEST_GROUP##_##TEST_NAME##_UnitTest(				\
      const std::string& memberName,	                                \
      const std::string& type1Name,                                     \
      const std::string& arrayName					\
    )                                                                   \
    : Teuchos::UnitTestBase(						\
	#TEST_GROUP, std::string(#TEST_NAME)+"_Teuchos::"+arrayName+"<"+type1Name+">_"+memberName )	\
    {}									\
    void runUnitTestImpl( Teuchos::FancyOStream &out, bool &success ) const; \
    virtual std::string unitTestFile() const { return __FILE__; }	\
    virtual long int unitTestFileLineNumber() const { return __LINE__; } \
  };									\
									\
  template<FACEMC::TupleMember MEMBER, typename TYPE1, template<typename> class ARRAY> \
  void TEST_GROUP##_##TEST_NAME##_UnitTest<MEMBER,TYPE1,ARRAY>::runUnitTestImpl(\
                      Teuchos::FancyOStream &out, bool &success ) const \

// Define a new Macro for the Teuchos Unit Test Harness for instantiating a
// templated unit test on two type and one template template parameter
#define FACEMC_UNIT_TEST_MEMBER_1_TUPLE_1_ARRAY_TEMPLATE_INSTANT(TEST_GROUP, TEST_NAME, MEMBER, TYPE1, ARRAY) \
  template class TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER, TYPE1, Teuchos::ARRAY >; \
  TEST_GROUP##_##TEST_NAME##_UnitTest<FACEMC::MEMBER, TYPE1, Teuchos::ARRAY > \
  instance_##TEST_GROUP##_##MEMBER##_##TYPE1##_##ARRAY##_##TEST_NAME##_UnitTest(#MEMBER,#TYPE1,#ARRAY);

// Define a new Macro for the Teuchos Unit Test Harness for comparing
// Tuples of ordinal data types or ordinal data types
#define FACEMC_TEST_EQUALITY( t1, t2 )					\
  {									\
    const bool result = FACEMC::compare(t1,#t1,t2,#t2,out,-1);		\
    (out) << TEUCHOS_PASS_FAIL(result) << "\n";				\
    if( !result ) success = false;					\
  }

// Define a new Macro for the Teuchos Unit Test Harness for comparing
// Tuples of floating point data types
#define FACEMC_TEST_FLOATING_EQUALITY( t1, t2, tol )			\
  {									\
    const bool result = FACEMC::compare(t1,#t1,t2,#t2,out,-1,tol);	\
    (out) << TEUCHOS_PASS_FAIL(result) << "\n";				\
    if( !result ) success = false;					\
  }

// Define a new Macro for the Teuchos Unit Test Harness for comparing arrays
// of Tuples of ordinal data types
#define FACEMC_TEST_COMPARE_ARRAYS( a1, a2 )				\
  {									\
    const bool result = FACEMC::compareArrays(a1,#a1,a2,#a2,out);	\
    if( !result ) success = false;					\
  }

// Define a new Macro for the Teuchos Unit Test Harness for comparing arrays
// of Tuples of floating point data types
#define FACEMC_TEST_COMPARE_FLOATING_ARRAYS( a1, a2, tol )		\
  {									\
    const bool result =	FACEMC::compareArrays(a1,#a1,a2,#a2,out,tol);	\
    if( !result ) success = false;					\
  }

#endif // end FACEMC_UNIT_TEST_HARNESS_EXTENSIONS_HPP

//---------------------------------------------------------------------------//
// end FACEMC_UnitTestHarnessExtensions.hpp 
//---------------------------------------------------------------------------//
