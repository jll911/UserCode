#!/bin/bash
source /afs/cern.ch/cms/sw/cmsset_default.sh
cd /afs/cern.ch/user/j/jleonard/scratch0/CMSSW_3_3_6_patch5/src/UserCode/DQMStandaloneEwkElectron/test
cmsenv
cmsRun ewkElecStandaloneDQM_cfg.py

