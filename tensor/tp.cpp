#include <node/node.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "fr.hpp"
#include <iostream>

namespace demo {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Number;
    using v8::Value;

    void tensorProduct(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        Fr_init();

        // FrElement a;
        // FrElement b;
        FrRawElement c;

        clock_t start1 = clock();

        #pragma omp parallel for
        for (u_int64_t i=0; i<1024; i++) {
            for (u_int64_t j=0; j<32; j++ ){
                Fr_rawMMul(
                    c,
                    &i,
                    &j
                );
            }
        }
        clock_t end1 = clock();
        std::cout <<"time1 duration: "<< double(end1 - start1) / CLOCKS_PER_SEC <<"\n"<<std::endl;
        // args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    }
    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "tensor", tensorProduct);
    }
    NODE_MODULE(addon, init)


}