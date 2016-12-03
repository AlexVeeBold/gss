///////////////////////////////////////////////////////////////
//
// idef: object communication protocol (definitions)
//
// Copyright © 2009-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-clauses BSD License.
//
//
//   21.07.2009 01:51 - created
//   14.01.2011 01:55 - added small usage help (comments)
//   23.11.2016 08:24 - simplified usage
//

#undef ICALL
#undef ICALLVA
#undef IENTRY
#undef IPURE

// Declaring interface:
/*
(iinterface.h)

    // using virtual interface definitions
    #include "idecl.h"

    class IInterface {
    public:
        IENTRY void ICALL interfaceMethod(...) IPURE;
    };
*/
// Declaring implementation class:
/*
(foobar.cpp)

    #include "iinterface.h"

    // using interface implementation definitions
    #include "iimpl.h"

    class FooBarClass : public IInterface {
    private:
        SOME data;
    public:
        IENTRY void ICALL interfaceMethod(...) IPURE;
    };
*/

/*  stdcall:
    (this) is placed on stack
    arguments are passed from right to left
    stack is cleared by callee
*/
#define ICALL       __stdcall           // x86 only, will be compiled as __fastcall on x86-64
#define ICALLVA     __cdecl             // x86 only

#ifdef IDEF
#define IENTRY      virtual
#define IPURE       = 0
#else //IDEF
#define IENTRY      //nothing
#define IPURE       //nothing
#endif //IDEF

