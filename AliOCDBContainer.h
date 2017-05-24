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
    std::vector<AliRPCValueScaler> fContainerScaler[4][2][9];

    void SortSpecificContainer(UInt_t MT, UInt_t side, UInt_t RPC){

        auto lambdaSort = []( const AliRPCValueDCS *a, const AliRPCValueDCS *b){ return a->GetTimeStamp() < b->GetTimeStamp(); };

        std::sort(fContainerCurrentHV[MT][side][RPC].begin(),fContainerCurrentHV[MT][side][RPC].end(),lambdaSort);
        std::sort(fContainerScaler[MT][side][RPC].begin(),fContainerScaler[MT][side][RPC].end(),lambdaSort);
    }

    void SortContainer(){
        for (int iMT = 0; iMT < 4; ++iMT) {
            for (int iSide = 0; iSide < 2; ++iSide) {
                for (int iRPC = 0; iRPC < 9; ++iRPC) {
                    SortSpecificContainer(iMT, iSide, iRPC);
                }
            }
        }
    }
    
};


#endif //ALIOCDBCONTAINER_H
