//
// Created by Filippo Valle on 24/05/2017.
//

#ifndef ALIOCDBCONTAINER_H
#define ALIOCDBCONTAINER_H

#include "TROOT.h"
#include "TObject.h"
#include <vector>
#include "AliRPCValueDCS.h"
#include "AliRPCValueScaler.h"

class AliOCDBContainer : TObject {

public:
    std::vector<AliRPCValueDCS> fContainerCurrentHV[4][2][9];
    std::vector<AliRPCValueDCS> fContainerScaler[4][2][9][2];
    std::vector<AliRPCValueDCS> fContainerScalerLB[4][2][234];

    auto lambdaSort = []( const AliRPCValueDCS *a, const AliRPCValueDCS *b){ return a->GetTimeStamp() < b->GetTimeStamp(); };

    void SortSpecificContainer(UInt_t MT, UInt_t side, UInt_t RPC){

        std::sort(fContainerCurrentHV[MT][side][RPC].begin(),fContainerCurrentHV[MT][side][RPC].end(),lambdaSort);
        std::sort(fContainerScaler[MT][side][RPC][0].begin(),fContainerScaler[MT][side][RPC][0].end(),lambdaSort);
        std::sort(fContainerScaler[MT][side][RPC][1].begin(),fContainerScaler[MT][side][RPC][1].end(),lambdaSort);

    }

    void SortContainer(){
        for (int iMT = 0; iMT < 4; ++iMT) {
            for (int iSide = 0; iSide < 2; ++iSide) {
                for (int iRPC = 0; iRPC < 9; ++iRPC) {
                    SortSpecificContainer(iMT, iSide, iRPC);
                }

                for (int iLB = 0; iLB < 234; ++iLB) {
                    std::sort(fContainerScalerLB[iMT][iSide][iLB].begin(),fContainerScalerLB[iMT][iSide][iLB].end(),lambdaSort);
                }
            }
        }
    }
    
};


#endif //ALIOCDBCONTAINER_H
