/**
 * Copyright (c) 2015, Jozef Stefan Institute, Quintelligence d.o.o. and contributors
 * All rights reserved.
 *
 * This source code is licensed under the FreeBSD license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "dmoz.h"

using namespace v8;

void TJsDmoz::Init(v8::Local<v8::Object> Exports) {
    v8::Isolate* Isolate = v8::Isolate::GetCurrent();
    v8::HandleScope HandleScope(Isolate);

    // Add all methods, getters and setters here.
    NODE_SET_METHOD(Exports, "init", _initDmoz);
    NODE_SET_METHOD(Exports, "load", _loadDmoz);
    NODE_SET_METHOD(Exports, "classify", _classify);
}

PDMozCfy TJsDmoz::DMozCfy = NULL;

void TJsDmoz::initDmoz(const v8::FunctionCallbackInfo<v8::Value>& Args) {
    v8::Isolate* Isolate = v8::Isolate::GetCurrent();
    v8::HandleScope HandleScope(Isolate);

    // parse out parameters
    TStr BowFNm = TNodeJsUtil::GetArgStr(Args, 0, "bow", "");
    TStr BowPartFNm = TNodeJsUtil::GetArgStr(Args, 0, "bowPart", "");
    TStr FilterFNm = TNodeJsUtil::GetArgStr(Args, 0, "filter", "");
    TStr OutFNm = TNodeJsUtil::GetArgStr(Args, 0, "classifier", "");
    // check we got all the necessary values
    EAssert(!BowFNm.Empty()); EAssert(!BowPartFNm.Empty());
    EAssert(!FilterFNm.Empty()); EAssert(!OutFNm.Empty());
    // create classifier
    printf("Loading BowDocBs ...\n");
    PBowDocBs BowDocBs = TBowDocBs::LoadBin(BowFNm);
    printf("Loading BowDocPart ...\n");
    PBowDocPart BowDocPart = TBowDocPart::LoadBin(BowPartFNm);
    printf("Creating classifier ...\n");
    DMozCfy = TDMozCfy::New(BowDocBs, BowDocPart, FilterFNm);
    printf("Saving classifier ...\n");
    TFOut FOut(OutFNm); DMozCfy->Save(FOut);
    printf("Done!\n");
}

void TJsDmoz::loadDmoz(const v8::FunctionCallbackInfo<v8::Value>& Args) {
    v8::Isolate* Isolate = v8::Isolate::GetCurrent();
    v8::HandleScope HandleScope(Isolate);

    // parse out parameters
    TStr CfyFNm = TNodeJsUtil::GetArgStr(Args, 0, "classifier", "");
    // check we got all the necessary values
    EAssert(!CfyFNm.Empty());
    // create classifier
    printf("Loading classifier ...\n");
    TFIn FIn(CfyFNm); DMozCfy = TDMozCfy::Load(FIn);
    printf("Done!\n");
}

void TJsDmoz::classify(const v8::FunctionCallbackInfo<v8::Value>& Args) {
    v8::Isolate* Isolate = v8::Isolate::GetCurrent();
    v8::HandleScope HandleScope(Isolate);

    // parse out parameters
    TStr Str = TNodeJsUtil::GetArgStr(Args, 0);
    const int MxCats = TNodeJsUtil::GetArgInt32(Args, 1, 3);
    // make sure we have classifier loaded
    EAssert(!DMozCfy.Empty());
    // classify
    TStrFltKdV CatNmWgtV, KeyWdWgtV;
    DMozCfy->Classify(Str, CatNmWgtV, KeyWdWgtV, MxCats);
    // translate to json
    v8::Local<v8::Object> ResObj = v8::Object::New(Isolate);
    v8::Local<v8::Object> CatNmArr = v8::Array::New(Isolate, CatNmWgtV.Len());
    for (int CatNmN = 0; CatNmN < CatNmWgtV.Len(); CatNmN++) {
        Nan::Set(CatNmArr, CatNmN, v8::String::NewFromUtf8(Isolate, CatNmWgtV[CatNmN].Key.CStr()).ToLocalChecked());
    }
    Nan::Set(ResObj, v8::String::NewFromUtf8(Isolate, "categories").ToLocalChecked(), CatNmArr);
    v8::Local<v8::Object> KeyWdArr = v8::Array::New(Isolate, KeyWdWgtV.Len());
    for (int KeyWdN = 0; KeyWdN < KeyWdWgtV.Len(); KeyWdN++) {
        Nan::Set(KeyWdArr, KeyWdN, v8::String::NewFromUtf8(Isolate, KeyWdWgtV[KeyWdN].Key.CStr()).ToLocalChecked());
    }
    Nan::Set(ResObj, v8::String::NewFromUtf8(Isolate, "keywords").ToLocalChecked(), KeyWdArr);
    Args.GetReturnValue().Set(ResObj);
}

void Init(Local<Object> Exports) {
    TJsDmoz::Init(Exports);
}

NODE_MODULE(dmoz, Init);
