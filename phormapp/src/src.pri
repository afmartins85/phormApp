SOURCES += \
    $$PWD/main.cpp \
    $$PWD/clienthandler.cpp \
    $$PWD/connectiondatabase.cpp \
    $$PWD/maintenancehandler.cpp \
    $$PWD/elementhandler.cpp \
    $$PWD/doctorhandler.cpp \
    $$PWD/suppliershandler.cpp \
    $$PWD/localhandler.cpp \
    $$PWD/sortfilterproxymodel.cpp \
    $$PWD/orderhandler.cpp \
    $$PWD/printersetup.cpp \
#    $$PWD/dialogpdf.cpp

#unix:LIBS += -L/home/anderson/Workspace/la-cuisini/lib/unix -lclisitef

HEADERS += \
    $$PWD/clienthandler.h \
    $$PWD/connectiondatabase.h \
    $$PWD/maintenancehandler.h \
    $$PWD/elementhandler.h \
    $$PWD/doctorhandler.h \
    $$PWD/suppliershandler.h \
    $$PWD/localhandler.h \
    $$PWD/sortfilterproxymodel.h \
    $$PWD/orderhandler.h \
    $$PWD/printersetup.h \
#    $$PWD/dialogpdf.h

#CONFIG += static
