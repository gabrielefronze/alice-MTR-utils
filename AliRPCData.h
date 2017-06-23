/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#ifndef AliRPCData_h
#define AliRPCData_h

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <limits>
#include "TObject.h"
#include "TString.h"
#include "TSortedList.h"
#include "AliRPCRunStatistics.h"
#include "AliOCDBRun.h"

using namespace std;

class AliRPCData : public TObject{
public:
	AliRPCData();
	AliRPCData(const AliRPCData &obj)  : TObject(obj){};
	~AliRPCData();
	Bool_t AddRunStatistics (Int_t plane, Int_t side, Int_t RPC, AliRPCRunStatistics *stats);
	Double_t GetMeanDarkCurrent(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetMeanTotalCurrent(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetMeanNetCurrent(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetMeanHV(UInt_t runNumber) const;
	inline Double_t GetMeanHV(UInt_t runNumber, Bool_t ) const{return GetMeanHV(runNumber);};
	Double_t GetTotalScalerCountsBending(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetTotalScalerCountsNotBending(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;

	Double_t GetMeanRateBending(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetMeanRateNotBending(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetMeanIntegratedCharge(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetMeanTimeStampStart(UInt_t runNumber, Bool_t normalizeToArea=kFALSE) const;

	inline vector<AliOCDBRun*> GetRunList(Int_t plane, Int_t side, Int_t RPC) const {return fRunNumbers[plane][side][RPC];};
    inline vector<AliRPCRunStatistics*> GetRunStatistics(Int_t plane, Int_t side, Int_t RPC){return fRunStatistics[plane][side][RPC];}
	Double_t GetAverageTotalCurrent(Int_t plane, Int_t side, Int_t RPC, Bool_t normalizeToArea=kFALSE)const;
	Double_t GetAverageNetCurrent(Int_t plane, Int_t side, Int_t RPC, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetAverageHV(Int_t plane, Int_t side, Int_t RPC) const;
	inline Double_t GetAverageHV(Int_t plane, Int_t side, Int_t RPC, Bool_t ) const{return GetAverageHV(plane,side, RPC);};
	Double_t GetAverageRateBending(Int_t plane, Int_t side, Int_t RPC, Bool_t normalizeToArea=kFALSE) const;
	Double_t GetAverageRateNotBending(Int_t plane, Int_t side, Int_t RPC, Bool_t normalizeToArea=kFALSE) const;

    UInt_t GetTotalEntries();

    Bool_t IsThereThisRun(Int_t plane, Int_t side, Int_t RPC, UInt_t runNumber, Int_t &index) const;

    inline vector<AliRPCRunStatistics*> operator()(Int_t iPlane,Int_t iSides,Int_t iRPC){return fRunStatistics[iPlane][iSides][iRPC];};

private:
	const static Int_t fNSides=2;

    const static Int_t fNPlanes=4;

    const static Int_t fNRPC=9;

    const static Int_t fNLocalBoards=234;

    const static Int_t fNCathodes=2;

    Double_t fAreas[fNRPC][fNPlanes];

    vector<AliRPCRunStatistics*> fRunStatistics[fNPlanes][fNSides][fNRPC];
    vector<AliOCDBRun*> fRunNumbers[fNPlanes][fNSides][fNRPC];

    Bool_t fIsDarkRun;

//	Bool_t IsThereThisRunStupido(Int_t plane, Int_t side, Int_t RPC, UInt_t runNumber, Int_t &index);
    Double_t GetMeanSomething(UInt_t runNumber, Bool_t normalizeToArea,Double_t (AliRPCRunStatistics::*funky)() const)const;
    Double_t GetAverageSomething(Int_t plane, Int_t side, Int_t RPC, Bool_t normalizeToArea,Double_t (AliRPCRunStatistics::*funky)() const)const;

    ClassDef(AliRPCData,3);
};

#endif
