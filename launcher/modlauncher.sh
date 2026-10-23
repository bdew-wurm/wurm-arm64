#!/usr/bin/env bash
if test -e "logging.properties"; then
	LOGGING=-Djava.util.logging.config.file=logging.properties
fi

export LD_LIBRARY_PATH="$PWD/nativelibs"

java "-Dworkdir=$PWD" "-Djava.library.path=$PWD/nativelibs" "-Djava.ext.dirs=$PWD/ext" $LOGGING -Xmn256M -Xms512m -Xmx2048m -XX:+OptimizeStringConcat -XX:+AggressiveOpts -jar ./modlauncher.jar "$@"
