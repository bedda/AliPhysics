/*
 * AliFemtoDreamTrackHist.cxx
 *
 *  Created on: Nov 14, 2017
 *      Author: gu74req
 */

#include "AliFemtoDreamTrackHist.h"
#include "TMath.h"
ClassImp(AliFemtoDreamTrackHist)
AliFemtoDreamTrackHist::AliFemtoDreamTrackHist()
:fMinimalBooking(false)
,fHistList(0)
,fConfig(0)
,fCutCounter(0)
,fDCAXYPtBins(0)
,fNSigCom(0)
{
  for (int i=0;i<2;++i) {
    fTrackCutQA[i] = nullptr;
    fpTDist[i] = nullptr;
    fpTPCDist[i] = nullptr;
    fetaDist[i] = nullptr;
    fphiDist[i] = nullptr;
    fTPCCls[i] = nullptr;
    fTPCClsS[i] = nullptr;
    fTrackChi2[i] = nullptr;
    fShrdClsITS[i] = nullptr;
    fDCAxy[i] = nullptr;
    fDCAz[i] = nullptr;
    fTPCCrossedRows[i] = nullptr;
    fTPCRatio[i] = nullptr;
    fTPCdedx[i] = nullptr;
    fTOFbeta[i] = nullptr;
    fNSigTPC[i] = nullptr;
    fNSigTOF[i] = nullptr;
    fTPCStatus[i] = nullptr;
    fTOFStatus[i] = nullptr;
    fTPCClsCPiluUp[i] = nullptr;
    fITShrdClsPileUp[i] = nullptr;
  }
}
AliFemtoDreamTrackHist::AliFemtoDreamTrackHist(bool DCADist,bool CombSig)
:fMinimalBooking(false)
{
  TString sName[2] = {"before", "after"};
  float ptmin = 0;
  float ptmax = 5;
  int ptBins = 100;
  int twoDBins = 400;
  fHistList = new TList();
  fHistList->SetName("TrackCuts");
  fHistList->SetOwner();

  fConfig = new TProfile("TrackCutConfig", "Track Cut Config", 25, 0, 25);
  fConfig->SetStats(0);
  fConfig->GetXaxis()->SetBinLabel(1, "pT_{min}");
  fConfig->GetXaxis()->SetBinLabel(2, "pT_{max}");
  fConfig->GetXaxis()->SetBinLabel(3, "#eta_{min}");
  fConfig->GetXaxis()->SetBinLabel(4, "#eta_{max}");
  fConfig->GetXaxis()->SetBinLabel(5, "Charge");
  fConfig->GetXaxis()->SetBinLabel(6, "ClsTPC_{min}");
  fConfig->GetXaxis()->SetBinLabel(7, "Filter Bit");
  fConfig->GetXaxis()->SetBinLabel(8, "Is MC");
  fConfig->GetXaxis()->SetBinLabel(9, "DCA_{xy}");
  fConfig->GetXaxis()->SetBinLabel(10, "DCA_{z}");
  fConfig->GetXaxis()->SetBinLabel(11, "Max Shared Cls");
  fConfig->GetXaxis()->SetBinLabel(12, "Shared Cls");
  fConfig->GetXaxis()->SetBinLabel(13, "TPC Crossed Rows");
  fConfig->GetXaxis()->SetBinLabel(14, "Ratio TPC Crossed Rows");
  fConfig->GetXaxis()->SetBinLabel(15, "PID Mom Thresh");
  fConfig->GetXaxis()->SetBinLabel(16, "PID nsig");
  fConfig->GetXaxis()->SetBinLabel(17, "Reject Pions");
  fConfig->GetXaxis()->SetBinLabel(18, "Smallest Sig");
  fConfig->GetXaxis()->SetBinLabel(19, "ITS Hit");
  fConfig->GetXaxis()->SetBinLabel(20, "TOF Timing");
  fConfig->GetXaxis()->SetBinLabel(21, "Pile Up Rej");
  fConfig->GetXaxis()->SetBinLabel(22, "TPC Refit");
  fConfig->GetXaxis()->SetBinLabel(23, "#chi2 min");
  fConfig->GetXaxis()->SetBinLabel(24, "#chi2 max");
  fConfig->GetXaxis()->SetBinLabel(25, "ESDFiltering");

  fHistList->Add(fConfig);

  fCutCounter = new TH1F("CutCounter", "Cut Counter", 24, 0, 24);
  fCutCounter->GetXaxis()->SetBinLabel(1, "Input");
  fCutCounter->GetXaxis()->SetBinLabel(2, "Filter Bit");
  fCutCounter->GetXaxis()->SetBinLabel(3, "p_{T} Cut");
  fCutCounter->GetXaxis()->SetBinLabel(4, "#eta Cut");
  fCutCounter->GetXaxis()->SetBinLabel(5, "Charge");
  fCutCounter->GetXaxis()->SetBinLabel(6, "PileUpITS");
  fCutCounter->GetXaxis()->SetBinLabel(7, "PileUpTOF");
  fCutCounter->GetXaxis()->SetBinLabel(8, "PileUp");
  fCutCounter->GetXaxis()->SetBinLabel(9, "nClsTPC");
  fCutCounter->GetXaxis()->SetBinLabel(10, "Max Shared Cls TPC");
  fCutCounter->GetXaxis()->SetBinLabel(11, "Shared Cls");
  fCutCounter->GetXaxis()->SetBinLabel(12, "TPC Refit");
  fCutCounter->GetXaxis()->SetBinLabel(13, "TPC Crossed Rows");
  fCutCounter->GetXaxis()->SetBinLabel(14, "TPC Row Ratio");
  fCutCounter->GetXaxis()->SetBinLabel(15, "#chi2 OK");
  fCutCounter->GetXaxis()->SetBinLabel(16, "TPC OK");
  fCutCounter->GetXaxis()->SetBinLabel(17, "Reject Pions");
  fCutCounter->GetXaxis()->SetBinLabel(18, "TPC PID");
  fCutCounter->GetXaxis()->SetBinLabel(19, "TPC TOF OK");
  fCutCounter->GetXaxis()->SetBinLabel(20, "TPC TOF PID");
  fCutCounter->GetXaxis()->SetBinLabel(21, "Smallest Sig");
  fCutCounter->GetXaxis()->SetBinLabel(22, "Passes PID");
  fCutCounter->GetXaxis()->SetBinLabel(23, "DCA_{Z}");
  fCutCounter->GetXaxis()->SetBinLabel(24, "DCA_{XY}");

  fHistList->Add(fCutCounter);

  for (int i = 0;i<2;++i) {
    fTrackCutQA[i] = new TList();
    fTrackCutQA[i]->SetName(sName[i].Data());
    fTrackCutQA[i]->SetOwner();
    fHistList->Add(fTrackCutQA[i]);

    TString ptName=Form("pTDist_%s", sName[i].Data());
    fpTDist[i] = new TH1F(ptName.Data(), ptName.Data(), ptBins, ptmin, ptmax);
    fpTDist[i]->Sumw2();
    fpTDist[i]->GetXaxis()->SetTitle("p_{T}");
    fTrackCutQA[i]->Add(fpTDist[i]);

    TString pTPCName=Form("pTPCDist_%s", sName[i].Data());
    fpTPCDist[i] = new TH1F(pTPCName.Data(), pTPCName.Data(), ptBins, ptmin, ptmax);
    fpTPCDist[i]->Sumw2();
    fpTPCDist[i]->GetXaxis()->SetTitle("p_{TPC}");
    fTrackCutQA[i]->Add(fpTPCDist[i]);

    TString etaName = Form("EtaDist_%s", sName[i].Data());
    fetaDist[i] = new TH1F(etaName.Data(), etaName.Data(), 200, -10., 10.);
    fetaDist[i]->Sumw2();
    fetaDist[i]->GetXaxis()->SetTitle("#eta");
    fTrackCutQA[i]->Add(fetaDist[i]);

    TString phiName = Form("phiDist_%s", sName[i].Data());
    fphiDist[i] = new TH1F(phiName.Data(), phiName.Data(), 200, 0., 2*TMath::Pi());
    fphiDist[i]->Sumw2();
    fphiDist[i]->GetXaxis()->SetTitle("#phi");
    fTrackCutQA[i]->Add(fphiDist[i]);

    TString TPCName = Form("TPCCls_%s", sName[i].Data());
    fTPCCls[i] = new TH1F(TPCName.Data(), TPCName.Data(), 100, 0, 200.);
    fTPCCls[i]->Sumw2();
    fTPCCls[i]->GetXaxis()->SetTitle("# cls TPC");
    fTrackCutQA[i]->Add(fTPCCls[i]);

    TString DCAXYName = Form("DCAXY_%s", sName[i].Data());
    fDCAxy[i] = new TH2F(DCAXYName.Data(), DCAXYName.Data(), ptBins, ptmin, ptmax, 2.5*twoDBins, -5., 5.);
    fDCAxy[i]->Sumw2();
    fDCAxy[i]->GetXaxis()->SetTitle("p_{T}");
    fDCAxy[i]->GetYaxis()->SetTitle("DCA_{xy}");

    fTrackCutQA[i]->Add(fDCAxy[i]);

    TString DCAZName = Form("DCAZ_%s", sName[i].Data());
    fDCAz[i] = new TH2F(DCAZName.Data(), DCAZName.Data(), ptBins, ptmin, ptmax, 2.5*twoDBins, -5., 5.);
    fDCAz[i]->Sumw2();
    fDCAz[i]->GetXaxis()->SetTitle("p_{T}");
    fDCAz[i]->GetYaxis()->SetTitle("DCA_{z}");
    fTrackCutQA[i]->Add(fDCAz[i]);

    TString TPCCRName = Form("CrossedRows_%s", sName[i].Data());
    fTPCCrossedRows[i] = new TH1F(TPCCRName.Data(), TPCCRName.Data(), 100, 0, 200.);
    fTPCCrossedRows[i]->Sumw2();
    fTPCCrossedRows[i]->GetXaxis()->SetTitle("# of crossed Rows");
    fTrackCutQA[i]->Add(fTPCCrossedRows[i]);

    TString TPCratioName = Form("TPCRatio_%s", sName[i].Data());
    fTPCRatio[i] = new TH1F(TPCratioName.Data(), TPCratioName.Data(), 100, 0., 2.);
    fTPCRatio[i]->Sumw2();
    fTPCRatio[i]->GetXaxis()->SetTitle("Ratio");
    fTrackCutQA[i]->Add(fTPCRatio[i]);

    TString TPCClsSName=Form("TPCClsS_%s",sName[i].Data());
    fTPCClsS[i]=new TH1F(TPCClsSName.Data(),TPCClsSName.Data(),200,0,200);
    fTPCClsS[i]->Sumw2();
    fTPCClsS[i]->GetXaxis()->SetTitle("TPC Cls S");
    fTrackCutQA[i]->Add(fTPCClsS[i]);

    TString ChiSquareName=Form("TrackChi2_%s",sName[i].Data());
    fTrackChi2[i]=new TH2F(ChiSquareName.Data(),ChiSquareName.Data(),ptBins, ptmin, ptmax, 100, 0, 20);
    fTrackChi2[i]->Sumw2();
    fTrackChi2[i]->GetXaxis()->SetTitle("#it{p}_T");
    fTrackChi2[i]->GetYaxis()->SetTitle("#chi^{2}/NDF");
    fTrackCutQA[i]->Add(fTrackChi2[i]);

    TString ShrdClsITSName=Form("SharedClsITS_%s",sName[i].Data());
    fShrdClsITS[i]=new TH2F(ShrdClsITSName.Data(),ShrdClsITSName.Data(),
                            6,1,7,2,0,2);
    fShrdClsITS[i]->Sumw2();
    fShrdClsITS[i]->GetXaxis()->SetTitle("ITS Layer");
    fShrdClsITS[i]->GetYaxis()->SetBinLabel(1,"Has shared Cls");
    fShrdClsITS[i]->GetYaxis()->SetBinLabel(2,"Has no Shard Cls");
    fTrackCutQA[i]->Add(fShrdClsITS[i]);

    TString TPCdedxName = Form("TPCdedx_%s", sName[i].Data());
    fTPCdedx[i] = new TH2F(TPCdedxName.Data(), TPCdedxName.Data(), ptBins, ptmin, ptmax, 2*twoDBins, 0., 400);
    fTPCdedx[i]->Sumw2();
    fTPCdedx[i]->GetXaxis()->SetTitle("p_{TPC}");
    fTPCdedx[i]->GetYaxis()->SetTitle("dEdx (arb. Units)");

    fTrackCutQA[i]->Add(fTPCdedx[i]);

    TString TOFbetaName = Form("TOFbeta_%s", sName[i].Data());
    fTOFbeta[i] = new TH2F(TOFbetaName.Data(), TOFbetaName.Data(), ptBins, ptmin, ptmax, 3.5*twoDBins, 0.4, 1.1);
    fTOFbeta[i]->Sumw2();
    fTOFbeta[i]->GetXaxis()->SetTitle("p_{TPC}");
    fTOFbeta[i]->GetYaxis()->SetTitle("#beta_{TOF}");

    fTrackCutQA[i]->Add(fTOFbeta[i]);

    TString NSigTPCName = Form("NSigTPC_%s", sName[i].Data());
    fNSigTPC[i] = new TH2F(NSigTPCName.Data(), NSigTPCName.Data(), ptBins, ptmin, ptmax, 3.*twoDBins, -60., 60.);
    fNSigTPC[i]->Sumw2();
    fNSigTPC[i]->GetXaxis()->SetTitle("p_{TPC}");
    fNSigTPC[i]->GetYaxis()->SetTitle("n#sigma_{TPC}");

    fTrackCutQA[i]->Add(fNSigTPC[i]);

    TString NSigTOFName = Form("NSigTOF_%s", sName[i].Data());
    fNSigTOF[i] = new TH2F(NSigTOFName.Data(), NSigTOFName.Data(), ptBins, ptmin, ptmax, 3.*twoDBins, -60., 60.);
    fNSigTOF[i]->Sumw2();
    fNSigTOF[i]->GetXaxis()->SetTitle("p_{TPC}");
    fNSigTOF[i]->GetYaxis()->SetTitle("n#sigma_{TOF}");

    fTrackCutQA[i]->Add(fNSigTOF[i]);

    TString TPCstatusname = Form("TPCStatus_%s",sName[i].Data());
    fTPCStatus[i] = new TH1F(TPCstatusname.Data(),TPCstatusname.Data(),4,0,4);
    fTPCStatus[i]->GetXaxis()->SetBinLabel(1, "kDetNoSignal");
    fTPCStatus[i]->GetXaxis()->SetBinLabel(2, "kDetPidOk");
    fTPCStatus[i]->GetXaxis()->SetBinLabel(3, "kDetMismatch");
    fTPCStatus[i]->GetXaxis()->SetBinLabel(4, "kDetNoParams");

    fTrackCutQA[i]->Add(fTPCStatus[i]);

    TString TOFstatusname = Form("TOFStatus_%s",sName[i].Data());
    fTOFStatus[i] = new TH1F(TOFstatusname.Data(),TOFstatusname.Data(),4,0,4);
    fTOFStatus[i]->GetXaxis()->SetBinLabel(1, "kDetNoSignal");
    fTOFStatus[i]->GetXaxis()->SetBinLabel(2, "kDetPidOk");
    fTOFStatus[i]->GetXaxis()->SetBinLabel(3, "kDetMismatch");
    fTOFStatus[i]->GetXaxis()->SetBinLabel(4, "kDetNoParams");

    fTrackCutQA[i]->Add(fTOFStatus[i]);

    TString TPCClsCPileUpName=Form("TPCClsCPileUp_%s",sName[i].Data());
    fTPCClsCPiluUp[i]=new TH2F(TPCClsCPileUpName.Data(),
                               TPCClsCPileUpName.Data(),
                               15,0,15,200,0,200);
    fTPCClsCPiluUp[i]->Sumw2();
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(1,"Hit ITS Layer 1");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(2,"Hit ITS Layer 2");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(3,"Hit ITS Layer 3");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(4,"Hit ITS Layer 4");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(5,"Hit ITS Layer 5");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(6,"Hit ITS Layer 6");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(7,"TOF Bunch.Xing");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(8,"no Hit ITS Layer 1");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(9,"no Hit ITS Layer 2");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(10,"no Hit ITS Layer 3");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(11,"no Hit ITS Layer 4");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(12,"no Hit ITS Layer 5");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(13,"no Hit ITS Layer 6");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(14,"no TOF Bunch.Xing");
    fTPCClsCPiluUp[i]->GetXaxis()->SetBinLabel(15,"all ITS-TOF Req false");
    fTrackCutQA[i]->Add(fTPCClsCPiluUp[i]);

    TString ITSShrdClsPileUpName=Form("ITSShrdClsPileUp_%s",sName[i].Data());
    fITShrdClsPileUp[i]=new TH2F(ITSShrdClsPileUpName.Data(),
                                 ITSShrdClsPileUpName.Data(),
                                 15,0,15,12,0,12);
    fITShrdClsPileUp[i]->Sumw2();
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(1,"Hit ITS Layer 1");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(2,"Hit ITS Layer 2");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(3,"Hit ITS Layer 3");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(4,"Hit ITS Layer 4");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(5,"Hit ITS Layer 5");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(6,"Hit ITS Layer 6");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(7,"TOF Bunch.Xing");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(8,"no Hit ITS Layer 1");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(9,"no Hit ITS Layer 2");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(10,"no Hit ITS Layer 3");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(11,"no Hit ITS Layer 4");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(12,"no Hit ITS Layer 5");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(13,"no Hit ITS Layer 6");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(14,"no Hit TOF Bunch.Xing");
    fITShrdClsPileUp[i]->GetXaxis()->SetBinLabel(15,"all Hit ITS-TOF Req false");

    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(1,"Has shared Cls Layer 1");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(2,"Has shared Cls Layer 2");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(3,"Has shared Cls Layer 3");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(4,"Has shared Cls Layer 4");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(5,"Has shared Cls Layer 5");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(6,"Has shared Cls Layer 6");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(7,"Has no Shard Cls 1");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(8,"Has no Shard Cls 2");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(9,"Has no Shard Cls 3");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(10,"Has no Shard Cls 4");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(11,"Has no Shard Cls 5");
    fITShrdClsPileUp[i]->GetYaxis()->SetBinLabel(12,"Has no Shard Cls 6");
    fTrackCutQA[i]->Add(fITShrdClsPileUp[i]);
  }
  if (CombSig) {
    fNSigCom = new TH3F("NSigComb","NSigComb",14,0.5,4.0,300,-30,30,300,-30,30);
    fHistList->Add(fNSigCom);
    fNSigCom->Sumw2();
  } else {
    fNSigCom=0;
  }
  if (DCADist) {
    TString dcaPtBinName = Form("DCAXYPtBinningTot");
    fDCAXYPtBins = new TH2F(dcaPtBinName.Data(), dcaPtBinName.Data(),20,0.5,4.05,500,-5,5);
    fDCAXYPtBins->Sumw2();
    fDCAXYPtBins->GetXaxis()->SetTitle("P#_{T}");
    fDCAXYPtBins->GetYaxis()->SetTitle("dca_{XY}");
    fHistList->Add(fDCAXYPtBins);
  } else {
    fDCAXYPtBins=0;
  }
}

AliFemtoDreamTrackHist::AliFemtoDreamTrackHist(TString MinimalBooking)
:fMinimalBooking(true)
,fConfig(0)
,fCutCounter(0)
,fDCAXYPtBins(0)
,fNSigCom(0)
{
  for (int i=0;i<2;++i) {
    fTrackCutQA[i] = nullptr;
    fpTPCDist[i] = nullptr;
    fetaDist[i] = nullptr;
    fphiDist[i] = nullptr;
    fTPCCls[i] = nullptr;
    fTPCClsS[i] = nullptr;
    fShrdClsITS[i] = nullptr;
    fDCAxy[i] = nullptr;
    fDCAz[i] = nullptr;
    fTPCCrossedRows[i] = nullptr;
    fTPCRatio[i] = nullptr;
    fTPCdedx[i] = nullptr;
    fTOFbeta[i] = nullptr;
    fNSigTPC[i] = nullptr;
    fNSigTOF[i] = nullptr;
    fTPCStatus[i] = nullptr;
    fTOFStatus[i] = nullptr;
    fTPCClsCPiluUp[i] = nullptr;
    fITShrdClsPileUp[i] = nullptr;
  }
  fHistList=new TList();
  fHistList->SetOwner();
  fHistList->SetName(MinimalBooking.Data());

  fpTDist[0]=0;
  TString ptName=Form("pTDist_%s", "after");
  fpTDist[1]=new TH1F(ptName.Data(), ptName.Data(), 100,0,5);
  fpTDist[1]->Sumw2();
  fHistList->Add(fpTDist[1]);
}

AliFemtoDreamTrackHist::~AliFemtoDreamTrackHist() {
  delete fHistList;
}

void AliFemtoDreamTrackHist::FillNSigComb(
    float pT,float nSigTPC,float nSigTOF)
{
  if(!fMinimalBooking)fNSigCom->Fill(pT,nSigTPC,nSigTOF);
}

void AliFemtoDreamTrackHist::FillDCAXYPtBins(float pT,float dcaxy) {
  if(!fMinimalBooking)fDCAXYPtBins->Fill(pT,dcaxy);
}
