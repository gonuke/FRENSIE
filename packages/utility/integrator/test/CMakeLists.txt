FRENSIE_INITIALIZE_PACKAGE_TESTS(utility_integrator)

FRENSIE_ADD_TEST_EXECUTABLE(GaussKronrodIntegrator DEPENDS tstGaussKronrodIntegrator.cpp)
FRENSIE_ADD_TEST(GaussKronrodIntegrator)

FRENSIE_ADD_TEST_EXECUTABLE(BoostIntegrator DEPENDS tstBoostIntegrator.cpp)
FRENSIE_ADD_TEST(BoostIntegrator)

FRENSIE_ADD_TEST_EXECUTABLE(GaussRadauQuadratureSet DEPENDS tstGaussRadauQuadratureSet.cpp)
FRENSIE_ADD_TEST(GaussRadauQuadratureSet)

FRENSIE_ADD_TEST_EXECUTABLE(LegendrePolynomial DEPENDS tstLegendrePolynomial.cpp)
FRENSIE_ADD_TEST(LegendrePolynomial)

FRENSIE_ADD_TEST_EXECUTABLE(GaussLegendreQuadratureSet DEPENDS tstGaussLegendreQuadratureSet.cpp)
FRENSIE_ADD_TEST(GaussLegendreQuadratureSet)

FRENSIE_ADD_TEST_EXECUTABLE(SloanRadauQuadrature DEPENDS tstSloanRadauQuadrature.cpp)
FRENSIE_ADD_TEST(SloanRadauQuadrature)

FRENSIE_FINALIZE_PACKAGE_TESTS(utility_integrator)
