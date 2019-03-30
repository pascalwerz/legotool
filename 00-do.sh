#!/bin/zsh

MYDIR=$(dirname "$0")

OPTIONS=""

while [ "$1" = "-c" \
     -o "$1" = "-d" \
     -o "$1" = "-D" \
     -o "$1" = "-E" \
     -o "$1" = "-g" \
     -o "$1" = "-i" \
     -o "$1" = "-I" \
     -o "$1" = "-k" \
     -o "$1" = "-l" \
     -o "$1" = "-n" \
     -o "$1" = "-v" \
     -o "$1" = "-x" \
     -o "$1" = "-z" \
     -o "$1" = "-Z" \
     -o "$1" = "-%" ]
do
  OPTIONS=($OPTIONS $1)
  if [  "$1" = "-c" \
     -o "$1" = "-D" \
     -o "$1" = "-E" \
     -o "$1" = "-g" \
     -o "$1" = "-i" \
     -o "$1" = "-I" \
     -o "$1" = "-x" \
     -o "$1" = "-Z" \
     -o "$1" = "-%" ]
  then
    OPTIONS=($OPTIONS $2)
    shift
  fi
  shift
done

if [ "$1" = "" ]
then
  echo "usage: $0 [options] directory" >&2
  echo "       where directory contains all savegamedata files" >&2
  echo "       for options, see legotool" >&2
  exit 1
fi

while [ "$1" != "" ]
do
  DIR="$1"
  shift

  if [ "$DIR" = "" ]
  then
    echo "usage: $0 directory" >&2
    echo "       where directory contains all savegamedata files" >&2
    exit 1
  fi

  (find "$DIR" -iname 'game*.savegamedata' | sort; find "$DIR" -iname 'dlc*.savegamedata' | sort) | while read FILE
  do
    "$MYDIR/legotool" $OPTIONS "$FILE"
  done
done
