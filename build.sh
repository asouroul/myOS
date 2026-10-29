#!/bin/sh
set -e
. ./config.sh

# Create the sysroot headers folder early to avoid errors
mkdir -p "$SYSROOT$INCLUDEDIR"

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
