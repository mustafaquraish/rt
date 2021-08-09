#!/bin/bash
if [ -z "$1" ]
then
  echo "Usage: $0 <directory>"
  exit 1
fi

FORMAT=png
if [ $# -gt 1 ]
then
  FORMAT=$2
fi

function convert_image() {
  local file=$1
  echo "Converting $FILE"
  mogrify -format $FORMAT -quality 100 $file 2>&1 | grep -v "warning"
  rm $file
}

for FILE in $(find $1 -type f)
do
  if file "$FILE" | grep -qE 'image|bitmap'; then
    if [[ "$FILE" != *"$FORMAT" ]]
    then
      convert_image $FILE &
    fi
  elif [ "${FILE: -4}" == ".mtl" ]
  then
    if $(grep "map_Kd" "$FILE" | grep -q -v ".$FORMAT")
    then
        echo "Changing image references in $FILE"
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
          sed -i -e "s/\(map_Kd.*\)\.[^.]*$/\1\.$FORMAT/g" "$FILE"
        elif [[ "$OSTYPE" == "darwin"* ]]; then
          # MacOS / BSD sed needs some special syntax for in place editing
          sed -i '' -e "s/\(map_Kd.*\)\.[^.]*$/\1\.$FORMAT/g" "$FILE"
        fi
    fi
  fi
done

wait

echo "Done."