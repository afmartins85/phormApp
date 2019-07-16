QT += qml quick sql svg printsupport widgets
TARGET = phormapp
!android: !ios: qtHaveModule(widgets): QT += widgets

include(src/src.pri)
#include(../shared/shared.pri)

OTHER_FILES += \
    main.qml \
    content/AndroidDelegate.qml

RESOURCES += \
    resources.qrc
DISTFILES += \
    content/EnrollPage.qml \
    content/ClientsPage.qml \
    content/SuppliersPage.qml \
    content/DoctorsPage.qml \
    content/MaintenancePage.qml \
    content/PhormPharmaData.qml \
    content/UnidadesData.qml \
    content/NetConfigPage.qml \
    content/OrderPage.qml \
    content/Box.qml \
    content/ElementsPage.qml \
    content/GroupsData.qml \
    SubGroupData.qml \
    content/SubGroupData.qml \
    content/NewElementDialog.qml \
    content/ValidationData.qml \
    content/ExcipientData.qml \
    content/CapSpecData.qml \
    content/PacksData.qml \
    content/CapsForPacksData.qml \
    content/PriceUpData.qml \
    content/PriceUpDialog.qml \
    content/NewClientsDialog.qml \
    content/NewDoctorsDialog.qml \
    content/NewSuppliersDialog.qml \
    content/NewOrderDialog.qml \
    content/EndOrderDialog.qml \
    content/OrderPrintDialog.qml \
    content/FindOrderDialog.qml \
    content/SelectCostumerDialog.qml \
    content/PacksPriceUpDialog.qml \
    content/RePrintLabels.qml \
    content/PrinterSelectDialog.qml \

#INCLUDEPATH += \
#            ../qpdflib
#CONFIG += static

#target.path = /usr/
#INSTALLS += target

#unix:LIBS += -L$$OUT_PWD/../qpdflib
#unix:LIBS += -lqpdf
