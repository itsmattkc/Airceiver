find_path(QTZEROCONF_INCLUDE_DIRS
    include/QtZeroConf/qzeroconf.h
  HINTS
    "${QTZEROCONF_LOCATION}"
    "$ENV{QTZEROCONF_LOCATION}"
  PATH_SUFFIXES
    include/
)

find_library(QTZEROCONF_LIBRARIES
    QtZeroConf
  HINTS
    "${QTZEROCONF_LOCATION}"
    "$ENV{QTZEROCONF_LOCATION}"
  PATH_SUFFIXES
    lib/
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(QtZeroConf
    REQUIRED_VARS
        QTZEROCONF_LIBRARIES
        QTZEROCONF_INCLUDE_DIRS
)
