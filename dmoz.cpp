/**
 * Copyright (c) 2015, Jozef Stefan Institute, Quintelligence d.o.o. and contributors
 * All rights reserved.
 *
 * This source code is licensed under the FreeBSD license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "dmoz.h"

using namespace v8;

void TJsDmoz::Init(v8::Handle<v8::Object> Exports) {
    v8::Isolate* Isolate = v8::Isolate::GetCurrent();
    v8::HandleScope HandleScope(Isolate);

    // Add all methods, getters and setters here.
    NODE_SET_METHOD(Exports, "config", _config);
}

void TJsDmoz::config(const v8::FunctionCallbackInfo<v8::Value>& Args) {
    v8::Isolate* Isolate = v8::Isolate::GetCurrent();
    v8::HandleScope HandleScope(Isolate);

    Args.GetReturnValue().Set(123);
}

void Init(Handle<Object> Exports) {
    TJsDmoz::Init(Exports);
}

NODE_MODULE(dmoz, Init);
