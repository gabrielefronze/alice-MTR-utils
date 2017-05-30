//
// Created by Gabriele Gaetano Fronzé on 30/05/2017.
//

#ifndef ALICE_MTR_UTILS_TSMARTTREE_H
#define ALICE_MTR_UTILS_TSMARTTREE_H

#include "TTree.h"
#include "TTreeIndex.h"

class TSmartTree : public TTree{
private:
    Bool_t fIsSorted;
    Long64_t *fTreeIndex;
public:

    TSmartTree() : TTree(){
        fIsSorted = kFALSE;
    }

    TSmartTree(const char* name, const char* title, Int_t splitlevel = 99) : TTree(name,title,splitlevel){
        fIsSorted = kFALSE;
    }

    void Sort(const char* majorname, const char* minorname = "0"){

        if (fIsSorted) return;

        this->BuildIndex(majorname,minorname);
        fTreeIndex = ((TTreeIndex*)this->GetTreeIndex())->GetIndex();
        fIsSorted = kTRUE;
    };

    Int_t Fill(){
        fIsSorted = kFALSE;
        return TTree::Fill();
    }

    void GetSortedEntry(Long64_t i){
        if(fIsSorted) this->GetEntry(fTreeIndex[i]);
        else this->GetEntry(i);
    }

    void operator[](Long64_t i){
        this->GetSortedEntry(i);
    }

    ClassDef(TSmartTree,1);
};


#endif //ALICE_MTR_UTILS_TSMARTTREE_H
