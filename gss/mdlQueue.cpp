///////////////////////////////////////////////////////////////
//
// mdlQueue: model loading queue
//
// Copyright © 2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   12.07.2016 18:46 - created (moved from parkSys)
//


#include "uDef.h"
#include "tunic.h"
#include "tstr.h"
#include "uLog.h"

#include "gssDef.h"
#include "gssScriptCall.h"

#include "parkDef.h"

#include "mdlQueue.h"


void mqInit(MODELQUEUE* pMQ)
{
    DWORD iMdl;
    pMQ->mqLength = 0;
    for(iMdl = 0; iMdl < MDLQUEUE_SIZE; iMdl++)
    {
        pMQ->mqEntry[iMdl].state = MDS_NONE;
        pMQ->mqEntry[iMdl].model = 0x12121212;
    }
}

DWORD mqEnqueueModel(MODELQUEUE* pMQ, DWORD model)
{
    MODELQUEUEENTRY* pEntry;
    BOOL bMdlDup;
    DWORD iMQSlot;
    DWORD mdlState;
    DWORD queueLength;
    // search if that model is already listed
    queueLength = pMQ->mqLength;
    bMdlDup = FALSE;
    iMQSlot = queueLength;
    if(queueLength > 0)
    {
        for(iMQSlot = 0; iMQSlot < queueLength; iMQSlot++)
        {
            if(pMQ->mqEntry[iMQSlot].model == model)
            {
                bMdlDup = TRUE;
                break;
            }
        }
    }
    // if not, add it to list
    if(bMdlDup == FALSE)
    {
        pEntry = &pMQ->mqEntry[iMQSlot];
        pEntry->model = model;
        // ...and request
        mdlState = MDS_LOADED;
        ///ulogf(UL_INFO, "checking model %08X", dwModel);
        if(SN::ASSET::HAS_MODEL_LOADED(model) == FALSE)
        {
            lss << UL::INFO << L("requesting model ") << model << UL::ENDL;
            mdlState = MDS_REQUESTED;
            SN::ASSET::REQUEST_MODEL(model);
            lss << UL::INFO << L("model has been requested") << UL::ENDL;
        }
        pEntry->state = mdlState;
        // increase model queue length
        pMQ->mqLength++;
    }
    return iMQSlot;
}

BOOL mqAreModelsStillLoading(MODELQUEUE* pMQ)
{
    MODELQUEUEENTRY* pEntry;
    BOOL bLoadingModel;
    BOOL bStillLoading;
    DWORD iMQSlot;
    DWORD queueLength;
    bLoadingModel = FALSE;
    bStillLoading = FALSE;
    queueLength = pMQ->mqLength;
    // mark all loaded models as spawnable
    for(iMQSlot = 0; iMQSlot < queueLength; iMQSlot++)
    {
        pEntry = &pMQ->mqEntry[iMQSlot];
        bLoadingModel = FALSE;
        lss << UL::INFO << L("checking model [") << iMQSlot << L("] ") << ulhex(pEntry->model) << L(" : ") << pEntry->state << L(" : ");
        if(pEntry->state == MDS_REQUESTED)
        {
            bLoadingModel = (SN::ASSET::HAS_MODEL_LOADED(pEntry->model) == FALSE);
            if(bLoadingModel == FALSE)
            {
                pEntry->state = MDS_LOADED;
            }
        }
        // is there any model that is not loaded yet?
        bStillLoading |= bLoadingModel;
        lss << UL::INFO << (bLoadingModel == FALSE) << UL::ENDL;
    }
    return bStillLoading;
}

void mqReleaseLoadedModels(MODELQUEUE* pMQ)
{
    MODELQUEUEENTRY* pEntry;
    DWORD iMQSlot;
    DWORD queueLength;
    queueLength = pMQ->mqLength;
    // release all loaded models
    for(iMQSlot = 0; iMQSlot < queueLength; iMQSlot++)
    {
        pEntry = &pMQ->mqEntry[iMQSlot];
        if(pEntry->state == MDS_LOADED)
        {
            SN::ASSET::MARK_MODEL_AS_NO_LONGER_NEEDED(pEntry->model);
            pEntry->state = MDS_RELEASED;
        }
    }
}


