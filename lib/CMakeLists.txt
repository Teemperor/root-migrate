include_directories(../include)

set(SOURCES
  Upgrade.cpp
  UpgradeFactory.cpp
  Variable.cpp
  Versions.cpp
)

add_library(migrate ${SOURCES})
