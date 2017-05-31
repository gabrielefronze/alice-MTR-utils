//
// Created by Gabriele Gaetano Fronzé on 30/05/2017.
//

#ifndef TBUFFEREDVECTOR_H
#define TBUFFEREDVECTOR_H

#include "TSmartTree.h"
#include "TBranch.h"

template <class T> class TBufferedVector : public TObject{
private:
    TSmartTree fSmartTree;
    TBranch *fBranch;
    T* fContentPointer;
    Long64_t fBegin;
    Long64_t fEnd;
    Long64_t fPosition;

public:

    TBufferedVector<T>(const char* treeName = "tree", const char* branchName = "branch") : TObject(),
                                                                                           fSmartTree(treeName,treeName){
        fBegin = 0;
        fEnd = 0;
        fPosition = 0;
        fContentPointer = new T();
        fBranch = fSmartTree.Branch(branchName,branchName,fContentPointer);
    }

    T* operator[](Long64_t i){
        fSmartTree.GetSortedEntry(i);
        fPosition = i;
        return fContentPointer;
    }

    T* At(Long64_t i){
        fSmartTree.GetSortedEntry(i);
        fPosition = i;
        return fContentPointer;
    }

    void Add(T element){
        fContentPointer = &element;
        fEnd++;
        fSmartTree.Fill();
    }

    void Sort(const char* majorname, const char* minorname = "0"){
        fSmartTree.Sort(majorname,minorname);
    }

    Int_t GetEntries(){
        return fSmartTree.GetEntries();
    }
};

#ifdef ROOTCLING
#pragma link C++ class TBufferedVector<TObject>+;
#endif

#endif //TBUFFEREDVECTOR_H
