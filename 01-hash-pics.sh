#!/bin/zsh

MYDIR=$(dirname "$0")
MYDIR=$(cd "$MYDIR"; pwd -P)

if [ "$1" = "-?" ]
then
  echo "usage: $0 [/path/to/some/SAVEDGAMES/CONTENT]" >&2
  echo "       if no path is given, it defaults to current user's Marvel's Avenger SAVEDGAMES/CONTENT directory." >&2
  exit 1
fi

if [ "x$1" != "x" ]
then
  cd "$1"
else
  cd ~/Library/Application\ Support/Feral\ Interactive/LEGO\ Marvel\ Super\ Heroes\ 2/VFS/User/AppData/Roaming/Warner\ Bros.\ Interactive\ Entertainment/LEGO\ Atlas/SAVEDGAMES/CONTENT
fi

for SLOT in 1 2 3 4
do
  GLOBAL=S${SLOT}-SYSGLOBAL
  (
  if [ -d $GLOBAL/PICS ]
  then
    cd $GLOBAL/PICS
    find . -iname '*.JPEG' ! -iname 'THUMBNAIL*' | while read FILE
    do
      LABEL=${GLOBAL}:PICS/$(basename $FILE)
      $MYDIR/hash $LABEL
    done
  fi
  )
done
