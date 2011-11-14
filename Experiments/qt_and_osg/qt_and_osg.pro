HEADERS += \
    MainWindow.h \
    OsgViewerWidget.h

SOURCES += \
    MainWindow.cpp \
    main.cpp \
    OsgViewerWidget.cpp

FORMS += \
    MainWindow.ui

LIBS += \
    -losg \
    -losgQt \
    -losgDB \
    -losgViewer
