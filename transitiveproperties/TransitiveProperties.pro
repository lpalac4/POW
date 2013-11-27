# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE7CCAC97

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp \
    territory.cpp \
    water.cpp \
    player.cpp \
    color.cpp \
    resources.cpp \
    mapeditor.cpp \
    mapdata.cpp \
    graphicstool.cpp \
    edge.cpp \
    maptest.cpp \
    combatprocessor.cpp \
    mapcreator.cpp \  
    testdatagenerator.cpp \
    nullplayer.cpp \
    mapcreatortest.cpp \
    cell.cpp\
    utility.cpp \
    mapimage.cpp \
    playerturnstatus.cpp \
    playerindicator.cpp \
    playerresourceswidget.cpp \
    squarelabel.cpp \
    foreignaffairsmenu.cpp \
    transportmenu.cpp \
    popup.cpp \
    mainmenu.cpp \
    gamesetup.cpp \
    gameoptions.cpp \
    territoryselection.cpp \
    combatmodescreen.cpp \
    playerindicatorview.cpp \
    loadprogressscreen.cpp \
    productionscreen.cpp \
    winscreen.cpp \
    mybutton.cpp \
    combatcomparator.cpp \
    claimcomparator.cpp \
    transportmenuoptions.cpp

HEADERS += mainwindow.h \
    territory.h \
    water.h \
    player.h \
    color.h \
    resources.h \
    strengths.h \
    mapeditor.h \
    mapdata.h \
    graphicstool.h \
    edge.h \
    combatprocessor.h \
    mapcreator.h \
    maptest.h \
    nullplayer.h \
    mapcreatortest.h \
    testingflags.h \
    cell.h\
    utility.h \
    imagepathnames.h \
    playerturnstatus.h \
    playerindicator.h \
    mapimage.h \
    production.h \
    productioncost.h \
    playerresourceswidget.h \
    squarelabel.h \
    foreignaffairsmenu.h \
    transportmenu.h \
    popup.h \
    mainmenu.h \
    gamesetup.h \
    gameoptions.h \
    territoryselection.h \
    combatmodescreen.h \
    playerindicatorview.h \
    productionscreen.h \
    combatResultStruct.h \
    loadprogressscreen.h \
    mybutton.h \
    winscreen.h \
    combatcomparator.h \
    claimcomparator.h \
    transportmenuoptions.h

FORMS += mainwindow.ui \
    gamesetup.ui \
    gameoptions.ui \
    gamescreen.ui \
    mainmenu.ui \
    territoryselection.ui \
    production.ui \
    playerresourceswidget.ui \
    foreignaffairsmenu.ui \
    transportmenu.ui \
    loadprogressscreen.ui \
    winscreen.ui \
    transportmenu2.ui \
    transportmenuoptions.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-nb/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-es/strings.xml \
    android/version.xml \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    android/res/values-id/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-de/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/values-id/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/layout/splash.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-nl/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-ru/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-es/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-de/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/res/values-el/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-ru/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/values-de/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-es/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ja/strings.xml

CONFIG += uitools

RESOURCES += \
    Resources.qrc

