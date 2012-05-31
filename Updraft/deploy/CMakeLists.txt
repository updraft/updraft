SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open source application for glider flight planning and visualization")
# SET(CPACK_PACKAGE_VENDOR "team...")
# SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/licence.txt")

SET(CPACK_PACKAGE_NAME "Updraft")

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Updraft")
SET(CPACK_PACKAGE_EXECUTABLES "updraft" "Updraft")

SET(CPACK_INSTALLED_DIRECTORIES "${PROJECT_BINARY_DIR}/bin;bin;c:/swproj/UpdraftDependencies/merged;bin")

IF(WIN32)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  # SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  # SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "Updraft ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\updraft.github.com")
  # SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
  INCLUDE(CPack)
ENDIF(WIN32)

IF(APPLE)
  ADD_CUSTOM_TARGET(
    package
    "${CMAKE_CURRENT_SOURCE_DIR}/mac/deploy-mac.sh" "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/mac")
ENDIF(APPLE)



