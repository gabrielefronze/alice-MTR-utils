//
// AliRPCRunStatistics.cpp
// 
//   Created by Gabriele Gaetano Fronzé
//   Copyright © 2016 Gabriele Gaetano Fronzé. All rights reserved.
//

#include "AliRPCRunStatistics.h"
#include "TClass.h"

AliRPCRunStatistics::AliRPCRunStatistics() : TObject(){
    fRunNumber=0;
    fTimeStampStart=0;
    fTimeStampStop=0;
    fYear=0;
    fIsCalib=kFALSE;
    fIsDark=kFALSE;
    fMeanDarkCurrent=0.;
    fMeanTotalCurrent=0.;
    fMeanHV=0.;
    fTotalScalerCountsBending=0.;
    fTotalScalerCountsNotBending=0.;
};

AliRPCRunStatistics::AliRPCRunStatistics(UInt_t runNumber, ULong64_t timeStampStart, ULong64_t timeStampStop, UInt_t year, Bool_t isCalib, Bool_t isDark, Double_t meanDarkCurrent, Double_t meanTotalCurrent,Double_t meanHV, ULong64_t totalScalerCountsBending, ULong64_t totalScalerCountsNotBending): TObject(){
    fRunNumber=runNumber;
    fTimeStampStart=timeStampStart;
    fTimeStampStop=timeStampStop;
    fYear=year;
    fIsCalib=isCalib;
    fIsDark=isDark;
    fMeanDarkCurrent=(meanDarkCurrent>0?meanDarkCurrent:0);
    fMeanTotalCurrent=(meanTotalCurrent>0?meanTotalCurrent:0);
    if(fMeanTotalCurrent<fMeanDarkCurrent) fMeanTotalCurrent=fMeanDarkCurrent;
    fMeanHV=meanHV;
    fTotalScalerCountsBending=totalScalerCountsBending;
    fTotalScalerCountsNotBending=totalScalerCountsNotBending;
};

Bool_t AliRPCRunStatistics::IsEqual (const TObject *obj) const {
    return fTimeStampStart == ((AliRPCRunStatistics*)obj)->GetTimeStampStart();
};

Int_t AliRPCRunStatistics::Compare(const TObject *obj) const {
    if ( fTimeStampStart <  ((AliRPCRunStatistics*)obj)->GetTimeStampStart() ) return -1;
    if ( fTimeStampStart == ((AliRPCRunStatistics*)obj)->GetTimeStampStart() ) return 0;
    else return 1;
};


void AliRPCRunStatistics::PrintHumanReadable(){
    printf("#############Run: %u\n start: %llu\n stop %llu\n year%u\n calib:%u\n dark:%u\n darkcurrent %f\n totalcurrent:%f\n HV:%f\n scalerB:%llu\n scalerNB:%llu\n rate:%f\n#############",fRunNumber,fTimeStampStart,fTimeStampStop,fYear,(fIsCalib?1:0),(fIsDark?1:0),fMeanDarkCurrent,fMeanTotalCurrent,fMeanHV,fTotalScalerCountsBending,fTotalScalerCountsNotBending,GetMeanRateBending());
    return;
};

