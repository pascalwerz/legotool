#!/bin/zsh


# known locations:
#	LEGO Marvel Super Heroes 2:
# ~/Library/Application\ Support/Feral\ Interactive/LEGO\ Marvel\ Super\ Heroes\ 2/VFS/User/AppData/Roaming/Warner\ Bros.\ Interactive\ Entertainment/LEGO\ Atlas/SAVEDGAMES/CONTENT
#	The LEGO Movie 2:
# ~/Library/Application\ Support/Feral\ Interactive/The\ LEGO\ Movie\ 2/SaveData/SAVEDGAMES/CONTENT
#	Lego Worlds, where livesaves/worlds is a link to Windows's "%USERPROFILE%/AppData/Roaming/Warner Bros. Interactive Entertainment/LEGOWorlds/SavedGames":
# livesaves/worlds/CONTENT
#  	Lego Worlds, on Windows:
# "%USERPROFILE%/AppData/Roaming/Warner Bros. Interactive Entertainment/LEGOWorlds/SavedGames/Content"

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
  echo Searching pics in $PWD/$GLOBAL/PICS
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
