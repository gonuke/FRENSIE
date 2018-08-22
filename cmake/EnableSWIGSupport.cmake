# Configure the SWIG options that will be required in this project. The
# following variables will be set
# 1.)
MACRO(ENABLE_SWIG_SUPPORT)

  # Add the user supplied SWIG prefix to help find SWIG
  IF(DEFINED SWIG_PREFIX)
    SET(CMAKE_PREFIX_PATH ${SWIG_PREFIX} ${CMAKE_PREFIX_PATH})
  ENDIF()

  # Find the SWIG package available on this system
  FIND_PACKAGE(SWIG 3.0.12 REQUIRED)
  
  # Find the python package available on this system
  FIND_PACKAGE(PythonInterp 2.7 REQUIRED)
  FIND_PACKAGE(PythonLibs 2.7 REQUIRED)

  # Find the numpy package available on this system
  FIND_PACKAGE(NumPy 1.8 REQUIRED)

  IF(CMAKE_VERBOSE_CONFIGURE)
    MESSAGE("Found SWIG! Here are the details: ")
    MESSAGE("  SWIG_DIR = ${SWIG_DIR}")
    MESSAGE("  SWIG_EXECUTABLE = ${SWIG_EXECUTABLE}")
    MESSAGE("  SWIG_VERSION = ${SWIG_VERSION}")
    MESSAGE("End of SWIG details\n")
  ENDIF()

ENDMACRO()
