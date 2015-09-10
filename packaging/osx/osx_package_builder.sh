#!/bin/bash
#
# scirpt to package the 2dx installer with package maker 
#
# Marcel Arheit  21.01.2011

if [ $# -lt 3 ]
then
	echo "Not enough arguments specified"
	echo "Usage: `basename $0:` <root_dir> <package_dir> <version>" >&2
	echo 'Aborting!'
	exit 1
fi
PARENT_DIR=`dirname $0`
echo "MAC OSX INSALLATION FILES DIR=$PARENT_DIR"
ROOT=$1
PACKAGE_DIR=$2
VERSION=$3

PRODUCT_FOLDER=`echo "${PACKAGE_DIR}/2dx-${VERSION}"`
PRODUCT_DMG=`echo "2dx-${VERSION}.dmg"`
PRODUCT_VOLNAME=`echo "2dx-${VERSION}"`

echo '*############################################################################*'
echo '| Preparing                                                                  |'
echo '*============================================================================*'
echo '|                                                                            |'
echo "SUPPLIED VERSION: ${VERSION}"
echo "SUPPLIED PACKAGE DIRECTORY: ${PACKAGE_DIR}"
echo ''
echo "WILL CREATE PRODUCT IN: $PRODUCT_FOLDER"
echo "PRODUCT DMG FILE WILL BE: $PRODUCT_DMG"
echo "PRODUCT VOLNAME FILE WILL BE: $PRODUCT_VOLNAME"

if [ -d $PACKAGE_DIR ]
then
	echo "Removing previous version in $PACKAGE_DIR" 
	rm -r $PACKAGE_DIR
        mkdir $PACKAGE_DIR
else
        mkdir -p $PACKAGE_DIR
fi
echo '|                                                                            |'
echo '*============================================================================*'
echo ''
echo ''
echo ''
echo ''

echo '*############################################################################*'
echo '| The contents of package will be:                                           |'
echo "| ($ROOT})"
echo '*============================================================================*'
echo '|                                                                            |'           
ls $ROOT
echo '|                                                                            |'
echo '*============================================================================*'
echo ''
echo ''
echo ''
echo ''


echo '*############################################################################*'
echo '| Compressing to disk Image                                                  |'
echo '*============================================================================*'
echo '|                                                                            |'
mkdir -p "${PRODUCT_FOLDER}/2dx/${VERSION}"
cp -r "${ROOT}" "${PRODUCT_FOLDER}/2dx/${VERSION}"
ln -s /Applications "${PRODUCT_FOLDER}/"
hdiutil create \
  -volname ${PRODUCT_VOLNAME} \
  -srcfolder ${PRODUCT_FOLDER} \
  -ov \
  ${PACKAGE_DIR}/${PRODUCT_DMG}
    
echo '|                                                                            |'
echo '*============================================================================*'
echo ''
echo ''
echo ''
echo ''
