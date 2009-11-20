import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("RecoMET/METProducers/BeamHaloSummary_cfi")
process.load("RecoMET/Configuration/RecoMET_BeamHaloId_cff")
process.GlobalTag.globaltag ='STARTUP31X_V4::All'

process.load("Configuration/StandardSequences/ReconstructionCosmics_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            debugFlag = cms.untracked.bool(True),
                            debugVebosity = cms.untracked.uint32(10),
                            fileNames = cms.untracked.vstring(


    # Skim from ReReco'd Beam08 data 
#   'rfio:/castor/cern.ch/user/r/rcr/BeamHalo/Run62232/SingleBeam08_MET20_Skim_1.root',
#   'rfio:/castor/cern.ch/user/r/rcr/BeamHalo/Run62232/SingleBeam08_MET20_Skim_2.root',
#   'rfio:/castor/cern.ch/user/r/rcr/BeamHalo/Run62232/SingleBeam08_MET20_Skim_3.root',
#   'rfio:/castor/cern.ch/user/r/rcr/BeamHalo/Run62232/SingleBeam08_MET20_Skim_4.root'
   

    # 326 Reco BeamHalo Relval
    
    '/store/relval/CMSSW_3_2_6/RelValBeamHalo/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/964133E3-FA9A-DE11-A228-0030487A18A4.root',
    '/store/relval/CMSSW_3_2_6/RelValBeamHalo/GEN-SIM-RECO/STARTUP31X_V7-v1/0012/C2BD8B15-239A-DE11-98B6-001D09F29597.root',
    '/store/relval/CMSSW_3_2_6/RelValBeamHalo/GEN-SIM-RECO/STARTUP31X_V7-v1/0012/B4D814F9-219A-DE11-BF6D-001D09F25109.root',
    '/store/relval/CMSSW_3_2_6/RelValBeamHalo/GEN-SIM-RECO/STARTUP31X_V7-v1/0012/A23604E8-249A-DE11-8CD7-001617E30CD4.root',
    )
    )

process.p = cms.Path(process.BeamHaloId*process.BeamHaloSummary)
#process.p = cms.Path(process.CSCHaloData*process.EcalHaloData*process.HcalHaloData*process.GlobalHaloData)


                                         
                                     
