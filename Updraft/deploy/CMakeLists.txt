# SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open source application for glider flight planning and visualization")
# SET(CPACK_PACKAGE_VENDOR "team...")
# SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")

SET(CPACK_PACKAGE_NAME "Updraft")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/licence.txt")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Updraft")
SET(CPACK_PACKAGE_EXECUTABLES "updraft" "Updraft")
SET(CPACK_INSTALLED_DIRECTORIES "${PROJECT_BINARY_DIR}/bin;bin;${DEPENDENCIES_DIR};bin")

IF(WIN32)

# File types association: 
SET(CPACK_NSIS_DEFINES "!include ${CMAKE_CURRENT_SOURCE_DIR}\\\\FileAssociation.nsh") 
        
SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "\\\${registerExtension} \\\"$INSTDIR\\\\bin\\\\Updraft.exe\\\" \\\".igc\\\" \\\"Updraft flight data\\\" 
\\\${registerExtension} \\\"$INSTDIR\\\\bin\\\\Updraft.exe\\\" \\\".cup\\\" \\\"Updraft turpoints\\\"
\\\${registerExtension} \\\"$INSTDIR\\\\bin\\\\Updraft.exe\\\" \\\".tsk\\\" \\\"Updraft flight task\\\"") 

SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "\\\${unregisterExtension} \\\".igc\\\" \\\"Updraft flight data\\\"
\\\${unregisterExtension} \\\".cup\\\" \\\"Updraft turpoints\\\"
\\\${unregisterExtension} \\\".tsk\\\" \\\"Updraft flight task\\\"") 


# There is a bug in NSI that does not handle full unix paths properly. Make
# sure there is at least one set of four (4) backlasshes.
# SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
# SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
# SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\updraft.github.com")
# SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")  
SET(CPACK_NSIS_DISPLAY_NAME "Updraft ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "updraft.exe")
INCLUDE(CPack)

ENDIF(WIN32)

IF(APPLE)
  ADD_CUSTOM_TARGET(
    package
    "${CMAKE_CURRENT_SOURCE_DIR}/mac/deploy-mac.sh" "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" "${OSG_DIR}" "${OSGEARTH_DIR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/mac")
ENDIF(APPLE)