#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/tolu/Downloads/Computer-Graphics-Coursework3 2/build"
  make -f /Users/tolu/Downloads/Computer-Graphics-Coursework3\ 2/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/tolu/Downloads/Computer-Graphics-Coursework3 2/build"
  make -f /Users/tolu/Downloads/Computer-Graphics-Coursework3\ 2/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/tolu/Downloads/Computer-Graphics-Coursework3 2/build"
  make -f /Users/tolu/Downloads/Computer-Graphics-Coursework3\ 2/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/tolu/Downloads/Computer-Graphics-Coursework3 2/build"
  make -f /Users/tolu/Downloads/Computer-Graphics-Coursework3\ 2/build/CMakeScripts/ReRunCMake.make
fi

