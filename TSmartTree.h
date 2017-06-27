/* Copyright(c) 1998-2017, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Offline MTR/MID detector performance analysis class
//     Authors: Gabriele Gaetano Fronze' (INFN),
//              Filippo Valle (UniTo)
//-------------------------------------------------------------------------

#ifndef ALICE_MTR_UTILS_TSMARTTREE_H
#define ALICE_MTR_UTILS_TSMARTTREE_H

#include "TTree.h"
#include "TBranch.h"
#include "TTreeIndex.h"
#include <iostream>

class TSmartTree : public TTree{

private:
    Bool_t fIsSorted;
    Long64_t *fTreeIndex;
    Long64_t fLastCall;
public:

    TSmartTree() : TTree(){
        fIsSorted = kFALSE;
        fLastCall = 0;
        fTreeIndex = nullptr;
    }

    TSmartTree(const char* name, const char* title, Int_t splitlevel = 99) : TTree(name,title,splitlevel){
        fIsSorted = kFALSE;
        fLastCall = 0;
        fTreeIndex = nullptr;
    }

    Bool_t Sort(const char* majorname, const char* minorname = "0"){

        if (this->GetEntries() == 0) return kFALSE;

//        if (fIsSorted) return kTRUE;

        this->BuildIndex(majorname,minorname);
        fTreeIndex = ((TTreeIndex*)this->GetTreeIndex())->GetIndex();
        fIsSorted = kTRUE;

        return ( fTreeIndex && fIsSorted );
    };

    Int_t Fill(){
        fIsSorted = kFALSE;
        return TTree::Fill();
    }

    int GetSortedEntry(Long64_t i){
        fLastCall = i;
        if(fIsSorted && fTreeIndex) return this->GetEntry(fTreeIndex[i]);
        else return this->GetEntry(i);
    }

    void operator[](Long64_t i){
        this->GetSortedEntry(i);
    }

    Bool_t Next(){
        this->GetSortedEntry(fLastCall++);
        return fLastCall < this->GetEntries();
    }

    inline Bool_t GetIsSorted(){ return fIsSorted; };

    Long64_t GetLastCall(){
        return fLastCall+1;
    }
    
    inline void PrintStatusBar(){
        for(Int_t i=0; i<20;i++)
        std::cout<<(i/20.<(Double_t)fLastCall/this->GetEntries()?"*":" ");
        printf("\t %f.2 %s",(Double_t)fLastCall/this->GetEntries()*100,"%");
        std::cout<<"\r";
    }

    ClassDef(TSmartTree,2);
};


#endif //ALICE_MTR_UTILS_TSMARTTREE_H
