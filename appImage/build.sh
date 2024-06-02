#!/bin/sh

set -e
set -u

#export LD_LIBRARY_PATH=/opt/Qt5.15.2/5.15.2/gcc_64/lib:$LD_LIBRARY_PATH

QT_QMAKE_PATH=/home/ghalavand/Pkg/Qt/Qt5.15.2/5.15.2/gcc_64/bin/qmake
TARGET_LIB=AppDir/usr/lib
PROJ_BUILD_DIR=/home/ghalavand/Prj/UAVC4I/build-UAV_C4I_News-Qt5_15_2-Debug
PROJ_SRC_DIR=/home/ghalavand/Prj/UAVC4I/uav-news
LINUXDEPLOYQT_DIR=/home/ghalavand/Pkg/linuxdeployqt.AppImage

read -p "Did you build application?" yn
case $yn in
    [Nn]*) 
       exit 1;;
    [Yy]*) 
       ;;
    *)
       echo "Assuming yes!";;
esac

rm -rf *.AppImage

cp -f /lib/x86_64-linux-gnu/libkeyutils.so.1 $TARGET_LIB/
cp $PROJ_BUILD_DIR/UAV_C4I_News AppDir/usr/bin/UAV_C4I_News


ARCH=x86_64 DESTDIR=$PWD $LINUXDEPLOYQT_DIR $PROJ_BUILD_DIR/UAV_C4I_News \
	-qmake=$QT_QMAKE_PATH \
	-appimage \
	-qmldir=$PROJ_SRC_DIR \
	-always-overwrite \
	-verbose=2 \
	-no-translations \
	-bundle-non-qt-libs \
	-executable=$PROJ_BUILD_DIR/UAV_C4I_News \
	-extra-plugins=geoservices/libqtgeoservices_osm.so,geoservices/libqtgeoservices_itemsoverlay.so,imageformats/libqjpeg.so,imageformats/libqsvg.so,imageformats/libqtiff.so \
