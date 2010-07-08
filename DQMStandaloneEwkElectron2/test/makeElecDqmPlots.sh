#!/bin/bash

# usage: 
# ./makeElecDqmPlots.sh histosFile.root




if [[ $# != 2 ]]
    then
    echo "Usage: ./makeElecDqmPlots.sh histosFile.root referenceFile.root"
    exit
fi

filename=$1
refFilename=$2

# REFERENCE FILE TO USE!
#refFilename="DQMHistos-29mar10-test.root"

dateFolder=$(date +%d%b%y)

mkdir $dateFolder

#runFolder=`sed s/\.root//g`
dateFolder=`pwd`/$dateFolder
cd $dateFolder

topLevelFolder=electronTopLevelPlots
triggerFolder=triggerPlots

folderList="${topLevelFolder} electronKinematicsPlots electronShowerShapePlots electronHoEPlots electronIsolationPlots electronTrackExtrapolationPlots electronTrackClusterMatchPlots electronVertexPlots electronChargePlots electronMiscPlots zPlots wPlots superClusterPlots metPlots metPlotsDetails jetCALOCORRPlots/ETABELOW3 jetCALOPlots/ETABELOW3 jetGENPlots/ETABELOW3 jetPFCORRPlots/ETABELOW3 jetPFPlots/ETABELOW3 jetCALOCORRPlots/ETAOVER3 jetCALOPlots/ETAOVER3 jetGENPlots/ETAOVER3 jetPFCORRPlots/ETAOVER3 jetPFPlots/ETAOVER3 ${triggerFolder}"

for folder in $folderList
do
  mkdir -p $folder
done

# load batch-mode root macro with proper filename of histos file

#root -b -q "../dqmDisplayFull.C(\"../${filename}\",\"../${refFilename}\")" # will this work?? Yes!
root -b -q "${CMSSW_BASE}/src/UserCode/DQMStandaloneEwkElectron/test/dqmDisplayFull.C(\"../${filename}\",\"../${refFilename}\")" # will this work?? Yes!

# generate webpages in each dir
for folder in ${folderList}
do
  
  cd $folder

#  echo "Generating index.html for ${folder}"

  echo "<html><head><title>" >> index.html
  echo "EWK DQM Display: ${folder}" >> index.html
  echo "</title></head>" >> index.html
  echo "<body>" >> index.html
  echo "<h1>EWK DQM Display: ${folder}</h1>" >> index.html
#  echo "... header done"

  # if top level, put links to other pages
#  if [[ "${folder}" -eq "${topLevelFolder}" ]] # -eq doesn't work for strings!
  if [[ ${folder} == ${topLevelFolder} ]]
      then
      for otherFolder in ${folderList}
	do
	if [[ ${otherFolder} != ${topLevelFolder} ]]
	    then
	    echo "Go to <a href=\"../${otherFolder}/index.html\">${otherFolder}</a><br>" >> index.html
	fi
      done
  # if other levels, put links back to top level
  else
      echo "Go back to <a href=\"../${topLevelFolder}/index.html\">${topLevelFolder}</a><br>" >> index.html
  fi

  # for all .png file in directory 
  echo "Linking pictures for "
  pwd
  if [[ ${folder} == ${triggerFolder} ]]
      then
      echo "<br><br><br>" >> index.html
      echo "<h2>Absolute efficiencies</h2>" >> index.html
      for picture in `ls --ignore=*REL* --ignore=*.C`
      do
	echo "<img src=\"${picture}\">" >> index.html
      done
      echo "<br><br><br>" >> index.html
      echo "<h2>Relative efficiencies</h2>" >> index.html
      for picture in `ls *REL*.png`
      do
	echo "<img src=\"${picture}\">" >> index.html
      done
  else
      echo "<br><br><br>" >> index.html
      for picture in `ls *.png`
	do
  # add stuff to filename and stick into index.html
	echo "<img src=\"${picture}\">" >> index.html
      done
  fi
  

  echo "<br><br>" >> index.html
  # if top level, put links to other pages
  if [[ ${folder} == ${topLevelFolder} ]]
      then
      for otherFolder in ${folderList}
	do
	if [[ ${otherFolder} != ${topLevelFolder} ]]
	    then
	    echo "Go to <a href=\"../${otherFolder}/index.html\">${otherFolder}</a><br>" >> index.html
	fi
      done
  # if other levels, put links back to top level
  else
      echo "Go back to <a href=\"../${topLevelFolder}/index.html\">${topLevelFolder}</a>" >> index.html
  fi
  
  echo "</body></html>" >> index.html
  
#  cd .. # doesn't deal with folder1/folder2 structure
#  cd  $dateFolder # could also use
  cd -
   
done

cd ..

exit
