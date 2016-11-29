///////////////////////////////////////////////////////////////
//
// mdlQueue: model loading queue
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   12.07.2016 18:46 - created (moved from parkSys)
//   24.11.2016 12:12 - increased max queue size to conform increased parking lane count
//


enum MDLQUEUEDEFS {
    MDLQUEUE_SIZE = 128, //64,
};
enum MODELSTATE {
    MDS_NONE,
    MDS_REQUESTED,
    MDS_LOADED,
    MDS_RELEASED,
};
struct MODELQUEUEENTRY {
    DWORD state;
    DWORD model;
};
struct MODELQUEUE {
    DWORD mqLength;
    MODELQUEUEENTRY mqEntry[MDLQUEUE_SIZE];
};


void mqInit(MODELQUEUE* pMQ);
DWORD mqEnqueueModel(MODELQUEUE* pMQ, DWORD model);
BOOL mqAreModelsStillLoading(MODELQUEUE* pMQ);
void mqReleaseLoadedModels(MODELQUEUE* pMQ);


