//
//  TestTSmartTree.cpp
//  alice_MTR_utils
//
//  Created by Gabriele Gaetano Fronzé on 07/06/2017.
//
//

#include <iostream>
#include <TFile.h>
#include <TRandom.h>
#include "TSmartTree.h"
//#include "TBufferedVector.h"
#include "AliRPCValueDCS.h"
#include "AliRPCValueScaler.h"
#include "AliRPCValueCurrent.h"
#include "AliRPCValueVoltage.h"

int TestSmartTree(){
    
    TFile *file = new TFile("file","RECREATE");
    file->cd();
    
    TSmartTree *st = new TSmartTree("st","st",0);
    st->Write();
    
    auto ptr = new AliRPCValueDCS();
    
    auto branch = st->Branch("branch", ptr, 0);
    
    for (int i = 0; i < 10; i++) {
        auto type = (int)(gRandom->Uniform(0.,3.));
        
        switch (type) {
            case 0:
                ptr = new AliRPCValueScaler();
                branch->SetAddress(&ptr);
                st->Fill();
                std::cout << "Scaler" << std::endl;
                continue;
                
            case 1:
                ptr = new AliRPCValueCurrent();
                branch->SetAddress(&ptr);
                st->Fill();
                std::cout << "Current" << std::endl;
                continue;
                
            case 2:
                ptr = new AliRPCValueVoltage();
                branch->SetAddress(&ptr);
                st->Fill();
                std::cout << "Voltage" << std::endl;
                continue;
                
            default:
                break;
        }
    }
    
    st->Write("",TObject::kOverwrite);
    
    auto ptrR = new AliRPCValueDCS();
    
    for (int j = 0; j < st->GetEntries(); j++) {
        branch->SetAddress(&ptrR);
        auto check = st->GetEntry(j);
        std::cout << check <<std::endl;
        if (check == 0) continue;
        std::cout << "Entry is a ";
        if (ptrR->IsVoltage()) {
            std::cout << "Voltage ";
            auto ptrVolt = reinterpret_cast<AliRPCValueVoltage*>(ptrR);
            ptrVolt->SetVSupp(777);
        }
        else if (ptrR->IsCurrent()) {
            std::cout << "Current ";
            auto ptrCurr = reinterpret_cast<AliRPCValueCurrent*>(ptrR);
            ptrCurr->SetITot(1.7);
        }
        else if (ptrR->IsScaler()) {
            std::cout << "Scaler ";
            auto ptrScal = reinterpret_cast<AliRPCValueScaler*>(ptrR);
            ptrScal->SetScalerCounts(77);
        }
        else std::cout << "Unknown";
        std::cout << std::endl;
    }
    
    st->Write("",TObject::kOverwrite);
    
    for (int j = 0; j < st->GetEntries(); j++) {
        branch->SetAddress(&ptrR);
        auto check = st->GetEntry(j);
        std::cout << check <<std::endl;
        if (check == 0) continue;
        std::cout << "Entry is a ";
        if (ptrR->IsVoltage()) {
            std::cout << "Voltage ";
            auto ptrVolt = reinterpret_cast<AliRPCValueVoltage*>(ptrR);
            std::cout << ptrVolt->GetVSupp();
        }
        else if (ptrR->IsCurrent()) {
            std::cout << "Current ";
            auto ptrCurr = reinterpret_cast<AliRPCValueCurrent*>(ptrR);
            std::cout << ptrCurr->GetINet();
        }
        else if (ptrR->IsScaler()) {
            std::cout << "Scaler ";
            auto ptrScal = reinterpret_cast<AliRPCValueScaler*>(ptrR);
            std::cout << ptrScal->GetScalerCounts();
        }
        else std::cout << "Unknown";
        std::cout << std::endl;
    }
    
    
    return 0;
}
