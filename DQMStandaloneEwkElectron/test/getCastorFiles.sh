#!/bin/bash

# usage . getCastorFiles.sh castorDirectory

if [[ $# != 1 ]]
    then
    echo "Usage: . getCastorFiles.sh castorDirectory"
    exit
fi

castorDir=$1

if [[ ! -e ${castorDir} ]]
    then
    mkdir ${castorDir}
fi

for file in `nsls /castor/cern.ch/user/j/jleonard/${castorDir}/`
  do
  if [[ -e ${castorDir}/${file} ]]
      then
      echo "File ./${castorDir}/${file} already exists, skipping"
  else
      echo "Copying /castor/cern.ch/user/j/jleonard/${castorDir}/${file} to ./${castorDir}/${file}"
      rfcp /castor/cern.ch/user/j/jleonard/${castorDir}/${file} ./${castorDir}/${file}
  fi
  done

