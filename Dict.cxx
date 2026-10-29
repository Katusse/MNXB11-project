// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "include/load_data.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_Measurement(void *p = nullptr);
   static void *newArray_Measurement(Long_t size, void *p);
   static void delete_Measurement(void *p);
   static void deleteArray_Measurement(void *p);
   static void destruct_Measurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Measurement*)
   {
      ::Measurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Measurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Measurement", ::Measurement::Class_Version(), "load_data.h", 9,
                  typeid(::Measurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Measurement::Dictionary, isa_proxy, 4,
                  sizeof(::Measurement) );
      instance.SetNew(&new_Measurement);
      instance.SetNewArray(&newArray_Measurement);
      instance.SetDelete(&delete_Measurement);
      instance.SetDeleteArray(&deleteArray_Measurement);
      instance.SetDestructor(&destruct_Measurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Measurement*)
   {
      return GenerateInitInstanceLocal(static_cast<::Measurement*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Measurement*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Measurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Measurement::Class_Name()
{
   return "Measurement";
}

//______________________________________________________________________________
const char *Measurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Measurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Measurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Measurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Measurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Measurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Measurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Measurement*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Measurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class Measurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Measurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(Measurement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Measurement(void *p) {
      return  p ? new(p) ::Measurement : new ::Measurement;
   }
   static void *newArray_Measurement(Long_t nElements, void *p) {
      return p ? new(p) ::Measurement[nElements] : new ::Measurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_Measurement(void *p) {
      delete (static_cast<::Measurement*>(p));
   }
   static void deleteArray_Measurement(void *p) {
      delete [] (static_cast<::Measurement*>(p));
   }
   static void destruct_Measurement(void *p) {
      typedef ::Measurement current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Measurement

namespace {
  void TriggerDictionaryInitialization_Dict_Impl() {
    static const char* headers[] = {
"include/load_data.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/apps/root/include/",
"/home/crosler/git/MNXB11-project/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$include/load_data.h")))  Measurement;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "include/load_data.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Measurement", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Dict() {
  TriggerDictionaryInitialization_Dict_Impl();
}
