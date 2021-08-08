#!/bin/bash
if [ -z "$1" ]
then
  echo "Usage: $0 <directory>"
fi

for FILE in $(find $1 -type f)
do
  if file "$FILE" | grep -qE 'image|bitmap'; then
    if [ "${FILE: -4}" != ".png" ]
    then
      echo "Converting $FILE"
      mogrify -format png -quality 100 "$FILE"
      rm "$FILE"
    fi
  elif [ "${FILE: -4}" == ".mtl" ]
  then
    if $(grep "map_Kd" "$FILE" | grep -q -v ".png")
    then
        echo "Changing image references in $FILE"
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
          sed -i -e "s/jpg/png/g" -e "s/ppm/png/g" "$FILE"
        elif [[ "$OSTYPE" == "darwin"* ]]; then
          # MacOS / BSD sed needs some special syntax for in place editing
          sed -i '' -e "s/jpg/png/g" -e "s/ppm/png/g" "$FILE"
        fi
    fi
  fi
done

echo "Done."