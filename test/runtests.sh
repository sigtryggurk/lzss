#!/bin/bash 

for filename in test/testcase/*;
do
  echo "Running ${filename}"
  bin/runner ${filename} test/compfile 0
  bin/runner test/compfile test/decompfile 1
  DIFF=$(diff ${filename} test/decompfile)
  if [ -z "${DIFF}" ];
  then
    echo "${filename} passed!"
  else
    echo "${filename} failed!"
    echo ${DIFF}
  fi
done
