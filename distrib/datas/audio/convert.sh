#!/bin/bash
for f in *.wav; do
  echo "Processing $f file..."
  afconvert -f caff -d LEI16@44100 -c 1 "$f" "$f.caf"
done