/**************************************************************************
 * Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#include "AliRPCRunStatistics.h"
#include "TClass.h"

//Default constructor
AliRPCRunStatistics::AliRPCRunStatistics() : TObject(){
    fRunNumber=0;
    fTimeStampStart=0;
    fTimeStampStop=0;
    fYear=0;
    fIsCalib=kFALSE;
    fLHCStatus=kNONE;
    fBeamType=knone;
    fMeanDarkCurrent=0.;
    fMeanTotalCurrent=0.;
    fMeanHV=0.;
    fTotalScalerCountsBending=0.;
    fTotalScalerCountsNotBending=0.;
};

//Class constructor
AliRPCRunStatistics::AliRPCRunStatistics(UInt_t runNumber, ULong64_t timeStampStart, ULong64_t timeStampStop, UInt_t year, Bool_t isCalib, TLHCStatus LHCStatus, TBeamType beamType, Double_t meanDarkCurrent, Double_t meanTotalCurrent, Double_t meanHV, ULong64_t totalScalerCountsBending, ULong64_t totalScalerCountsNotBending): TObject(){
    fRunNumber=runNumber;
    fTimeStampStart=timeStampStart;
    fTimeStampStop=timeStampStop;
    fYear=year;
    fIsCalib=isCalib;
    fLHCStatus=LHCStatus;
    fBeamType=beamType;
    fMeanDarkCurrent=(meanDarkCurrent>0.?meanDarkCurrent:0.);
    fMeanTotalCurrent=(meanTotalCurrent>0.?meanTotalCurrent:0.);
    if(fMeanTotalCurrent<fMeanDarkCurrent) fMeanTotalCurrent=fMeanDarkCurrent;
    fMeanHV=meanHV;
    fTotalScalerCountsBending=totalScalerCountsBending;
    fTotalScalerCountsNotBending=totalScalerCountsNotBending;
};

//IsEqual and Compare are useful to sort TLists
Bool_t AliRPCRunStatistics::IsEqual (const TObject *obj) const {
    return fTimeStampStart == ((AliRPCRunStatistics*)obj)->GetTimeStampStart();
};

Int_t AliRPCRunStatistics::Compare(const TObject *obj) const {
    if ( fTimeStampStart <  ((AliRPCRunStatistics*)obj)->GetTimeStampStart() ) return -1;
    if ( fTimeStampStart == ((AliRPCRunStatistics*)obj)->GetTimeStampStart() ) return 0;
    else return 1;
};


void AliRPCRunStatistics::PrintHumanReadable(){
    printf("#############\nRun: %u\n start: %llu\n stop: %llu\n year: %u\n calib: %u\n lhcstatus: %u\n beamtype: %u\n darkcurrent: %f\n totalcurrent:%f\n HV: %f\n scalerB: %llu\n scalerNB: %llu\n rate:%f\n#############",fRunNumber,fTimeStampStart,fTimeStampStop,fYear,fIsCalib,fLHCStatus,fBeamType,fMeanDarkCurrent,fMeanTotalCurrent,fMeanHV,fTotalScalerCountsBending,fTotalScalerCountsNotBending,GetMeanRateBending());
    return;
};
