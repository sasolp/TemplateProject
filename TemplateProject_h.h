

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Apr 22 02:14:54 2015
 */
/* Compiler settings for TemplateProject.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __TemplateProject_h_h__
#define __TemplateProject_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITemplateProject_FWD_DEFINED__
#define __ITemplateProject_FWD_DEFINED__
typedef interface ITemplateProject ITemplateProject;
#endif 	/* __ITemplateProject_FWD_DEFINED__ */


#ifndef __TemplateProject_FWD_DEFINED__
#define __TemplateProject_FWD_DEFINED__

#ifdef __cplusplus
typedef class TemplateProject TemplateProject;
#else
typedef struct TemplateProject TemplateProject;
#endif /* __cplusplus */

#endif 	/* __TemplateProject_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __TemplateProject_LIBRARY_DEFINED__
#define __TemplateProject_LIBRARY_DEFINED__

/* library TemplateProject */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TemplateProject;

#ifndef __ITemplateProject_DISPINTERFACE_DEFINED__
#define __ITemplateProject_DISPINTERFACE_DEFINED__

/* dispinterface ITemplateProject */
/* [uuid] */ 


EXTERN_C const IID DIID_ITemplateProject;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("70FEA115-0C3A-483A-91F4-6B0BFC71CEE8")
    ITemplateProject : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ITemplateProjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITemplateProject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITemplateProject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITemplateProject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITemplateProject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITemplateProject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITemplateProject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITemplateProject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITemplateProjectVtbl;

    interface ITemplateProject
    {
        CONST_VTBL struct ITemplateProjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITemplateProject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITemplateProject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITemplateProject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITemplateProject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITemplateProject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITemplateProject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITemplateProject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ITemplateProject_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TemplateProject;

#ifdef __cplusplus

class DECLSPEC_UUID("655B0A83-AED4-4ECA-AEED-9E9027D5C1EF")
TemplateProject;
#endif
#endif /* __TemplateProject_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


