cmd_Release/obj.target/tensor/tp.o := c++ -o Release/obj.target/tensor/tp.o ../tp.cpp '-DNODE_GYP_MODULE_NAME=tensor' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-DV8_DEPRECATION_WARNINGS' '-DV8_IMMINENT_DEPRECATION_WARNINGS' '-D_GLIBCXX_USE_CXX11_ABI=1' '-D_DARWIN_USE_64_BIT_INODE=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DOPENSSL_NO_PINSHARED' '-DOPENSSL_THREADS' '-DBUILDING_NODE_EXTENSION' -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/include/node -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/src -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/deps/openssl/config -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/deps/openssl/openssl/include -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/deps/uv/include -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/deps/zlib -I/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/deps/v8/include  -O3 -gdwarf-2 -mmacosx-version-min=10.15 -arch arm64 -Wall -Wendif-labels -W -Wno-unused-parameter -std=gnu++17 -stdlib=libc++ -fno-rtti -fno-exceptions -fno-strict-aliasing -MMD -MF ./Release/.deps/Release/obj.target/tensor/tp.o.d.raw   -c
Release/obj.target/tensor/tp.o: ../tp.cpp /usr/local/include/node/node.h \
  /usr/local/include/node/v8.h /usr/local/include/node/cppgc/common.h \
  /Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/include/node/v8config.h \
  /usr/local/include/node/v8-array-buffer.h \
  /usr/local/include/node/v8-local-handle.h \
  /usr/local/include/node/v8-internal.h \
  /usr/local/include/node/v8-version.h \
  /usr/local/include/node/v8config.h /usr/local/include/node/v8-object.h \
  /usr/local/include/node/v8-maybe.h \
  /usr/local/include/node/v8-persistent-handle.h \
  /usr/local/include/node/v8-weak-callback-info.h \
  /usr/local/include/node/v8-primitive.h \
  /usr/local/include/node/v8-data.h /usr/local/include/node/v8-value.h \
  /usr/local/include/node/v8-traced-handle.h \
  /usr/local/include/node/v8-container.h \
  /usr/local/include/node/v8-context.h \
  /usr/local/include/node/v8-snapshot.h \
  /usr/local/include/node/v8-date.h /usr/local/include/node/v8-debug.h \
  /usr/local/include/node/v8-script.h \
  /usr/local/include/node/v8-callbacks.h \
  /usr/local/include/node/v8-promise.h \
  /usr/local/include/node/v8-message.h \
  /usr/local/include/node/v8-exception.h \
  /usr/local/include/node/v8-extension.h \
  /usr/local/include/node/v8-external.h \
  /usr/local/include/node/v8-function.h \
  /usr/local/include/node/v8-function-callback.h \
  /usr/local/include/node/v8-template.h \
  /usr/local/include/node/v8-memory-span.h \
  /usr/local/include/node/v8-initialization.h \
  /usr/local/include/node/v8-isolate.h \
  /usr/local/include/node/v8-embedder-heap.h \
  /usr/local/include/node/v8-microtask.h \
  /usr/local/include/node/v8-statistics.h \
  /usr/local/include/node/v8-unwinder.h \
  /usr/local/include/node/v8-embedder-state-scope.h \
  /usr/local/include/node/v8-platform.h \
  /usr/local/include/node/v8-json.h /usr/local/include/node/v8-locker.h \
  /usr/local/include/node/v8-microtask-queue.h \
  /usr/local/include/node/v8-primitive-object.h \
  /usr/local/include/node/v8-proxy.h /usr/local/include/node/v8-regexp.h \
  /usr/local/include/node/v8-typed-array.h \
  /usr/local/include/node/v8-value-serializer.h \
  /usr/local/include/node/v8-wasm.h \
  /usr/local/include/node/node_version.h \
  /usr/local/include/node/node_api.h \
  /usr/local/include/node/js_native_api.h \
  /usr/local/include/node/js_native_api_types.h \
  /usr/local/include/node/node_api_types.h ../fr.hpp \
  /usr/local/include/gmp.h
../tp.cpp:
/usr/local/include/node/node.h:
/usr/local/include/node/v8.h:
/usr/local/include/node/cppgc/common.h:
/Users/hwangjaeseung/Library/Caches/node-gyp/20.10.0/include/node/v8config.h:
/usr/local/include/node/v8-array-buffer.h:
/usr/local/include/node/v8-local-handle.h:
/usr/local/include/node/v8-internal.h:
/usr/local/include/node/v8-version.h:
/usr/local/include/node/v8config.h:
/usr/local/include/node/v8-object.h:
/usr/local/include/node/v8-maybe.h:
/usr/local/include/node/v8-persistent-handle.h:
/usr/local/include/node/v8-weak-callback-info.h:
/usr/local/include/node/v8-primitive.h:
/usr/local/include/node/v8-data.h:
/usr/local/include/node/v8-value.h:
/usr/local/include/node/v8-traced-handle.h:
/usr/local/include/node/v8-container.h:
/usr/local/include/node/v8-context.h:
/usr/local/include/node/v8-snapshot.h:
/usr/local/include/node/v8-date.h:
/usr/local/include/node/v8-debug.h:
/usr/local/include/node/v8-script.h:
/usr/local/include/node/v8-callbacks.h:
/usr/local/include/node/v8-promise.h:
/usr/local/include/node/v8-message.h:
/usr/local/include/node/v8-exception.h:
/usr/local/include/node/v8-extension.h:
/usr/local/include/node/v8-external.h:
/usr/local/include/node/v8-function.h:
/usr/local/include/node/v8-function-callback.h:
/usr/local/include/node/v8-template.h:
/usr/local/include/node/v8-memory-span.h:
/usr/local/include/node/v8-initialization.h:
/usr/local/include/node/v8-isolate.h:
/usr/local/include/node/v8-embedder-heap.h:
/usr/local/include/node/v8-microtask.h:
/usr/local/include/node/v8-statistics.h:
/usr/local/include/node/v8-unwinder.h:
/usr/local/include/node/v8-embedder-state-scope.h:
/usr/local/include/node/v8-platform.h:
/usr/local/include/node/v8-json.h:
/usr/local/include/node/v8-locker.h:
/usr/local/include/node/v8-microtask-queue.h:
/usr/local/include/node/v8-primitive-object.h:
/usr/local/include/node/v8-proxy.h:
/usr/local/include/node/v8-regexp.h:
/usr/local/include/node/v8-typed-array.h:
/usr/local/include/node/v8-value-serializer.h:
/usr/local/include/node/v8-wasm.h:
/usr/local/include/node/node_version.h:
/usr/local/include/node/node_api.h:
/usr/local/include/node/js_native_api.h:
/usr/local/include/node/js_native_api_types.h:
/usr/local/include/node/node_api_types.h:
../fr.hpp:
/usr/local/include/gmp.h:
