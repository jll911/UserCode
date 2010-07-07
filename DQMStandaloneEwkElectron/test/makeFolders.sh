#!/bin/bash

topLevelFolder=electronTopLevelPlots
folderList="${topLevelFolder} electronKinematicsPlots electronShowerShapePlots electronHoEPlots electronIsolationPlots electronTrackExtrapolationPlots electronTrackClusterMatchPlots electronVertexPlots electronChargePlots electronMiscPlots zPlots wPlots superClusterPlots metPlots metPlotsDetails jetCALOCORRPlots/ETABELOW3 jetCALOPlots/ETABELOW3 jetGENPlots/ETABELOW3 jetPFCORRPlots/ETABELOW3 jetPFPlots/ETABELOW3 jetCALOCORRPlots/ETAOVER3 jetCALOPlots/ETAOVER3 jetGENPlots/ETAOVER3 jetPFCORRPlots/ETAOVER3 jetPFPlots/ETAOVER3 triggerPlots"

for folder in $folderList
do
  mkdir -p $folder
done

for folder in ${folderList}
do
  
  cd $folder
  pwd
  cd -

done