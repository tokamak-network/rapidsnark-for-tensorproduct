const { sh, cli } = require("tasksfile");

function cleanAll() {
    sh("rm -rf build");
}

function createFieldSources() {
    sh("mkdir -p build");
    sh("npm install", {cwd: "depends/ffiasm"});
    sh("node ../depends/ffiasm/src/buildzqfield.js -q 21888242871839275222246405745257275088696311157297823662689037894645226208583 -n Fq", {cwd: "build"});
    sh("node ../depends/ffiasm/src/buildzqfield.js -q 21888242871839275222246405745257275088548364400416034343698204186575808495617 -n Fr", {cwd: "build"});
    
    if (process.platform === "darwin") {
        sh("nasm -fmacho64 --prefix _ fq.asm", {cwd: "build"});
    }  else if (process.platform === "linux") {
        sh("nasm -felf64 fq.asm", {cwd: "build"});
    } else throw("Unsupported platform");

    if (process.platform === "darwin") {
        sh("nasm -fmacho64 --prefix _ fr.asm", {cwd: "build"});
    }  else if (process.platform === "linux") {
        sh("nasm -felf64 fr.asm", {cwd: "build"});
    } else throw("Unsupported platform");
}

function buildPistache() {
    sh("git submodule init && git submodule update");
    sh("mkdir -p build", {cwd: "depends/pistache"});
    sh("cmake -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=Release ..", {cwd: "depends/pistache/build"});
    sh("make", {cwd: "depends/pistache/build"});
}


function buildProverServer() {
    sh("g++" +
        " -I."+
        " -I../src"+
        " -I../depends/pistache/include"+
        " -I../depends/json/single_include"+
        " -I../depends/ffiasm/c"+
        " ../src/main_proofserver.cpp"+
        " ../src/proverapi.cpp"+
        " ../src/fullprover.cpp"+
        " ../src/binfile_utils.cpp"+
        " ../src/wtns_utils.cpp"+
        " ../src/zkey_utils.cpp"+
        " ../src/logger.cpp"+
        " ../depends/ffiasm/c/misc.cpp"+
        " ../depends/ffiasm/c/naf.cpp"+
        " ../depends/ffiasm/c/splitparstr.cpp"+
        " ../depends/ffiasm/c/alt_bn128.cpp"+
        " fq.cpp"+
        " fq.o"+
        " fr.cpp"+
        " fr.o"+
        " -L../depends/pistache/build/src -lpistache"+
        " -o proverServer"+
        " -pthread -std=c++17  -lgmp -lsodium -g -DSANITY_CHECK", {cwd: "build", nopipe: true}
    );
}


function buildProver() {
    sh("g++" +
        " -I."+
        " -I../src"+
        " -I../depends/ffiasm/c"+
        // " -I /usr/local/Cellar/gmp/6.2.1_1/include"+
        " -I../depends/json/single_include"+
        // " -I /usr/local/Cellar/libsodium/1.0.18_1/include" +
        " ../src/main_prover.cpp"+
        " ../src/binfile_utils.cpp"+
        " ../src/zkey_utils.cpp"+
        " ../src/wtns_utils.cpp"+
        " ../src/logger.cpp"+
        " ../depends/ffiasm/c/misc.cpp"+
        " ../depends/ffiasm/c/naf.cpp"+
        " ../depends/ffiasm/c/splitparstr.cpp"+
        " ../depends/ffiasm/c/alt_bn128.cpp"+
        " fq.cpp"+
        " fq.o"+
        " fr.cpp"+
        " fr.o"+
        " -o prover" +
        " -fmax-errors=5 -std=c++17 -pthread -lgmp -lsodium -O3 ", {cwd: "build", nopipe: true}
    );
}

function buildTensorProduct() {
    sh("g++" +
        " -I."+
        " -I../src"+
        " -I../depends/ffiasm/c"+
        // " -I../depends/json/single_include"+
        " ../src/tensor_product.cpp"+
        // " ../src/binfile_utils.cpp"+
        // " ../src/zkey_utils.cpp"+
        // " ../src/wtns_utils.cpp"+
        // " ../src/logger.cpp"+
        " ../depends/ffiasm/c/misc.cpp"+
        " ../depends/ffiasm/c/naf.cpp"+
        " ../depends/ffiasm/c/splitparstr.cpp"+
        " ../depends/ffiasm/c/alt_bn128.cpp"+
        " fq.cpp"+
        " fq.o"+
        " fr.cpp"+
        " fr.o"+
        " -o tensorProduct" +
        " -fmax-errors=5 -std=c++17 -pthread -lgmp -lsodium -O3 ", {cwd: "build", nopipe: true}
    );
}

// function buildTensorProduct() {
    // sh("g++-13" +
    //    " -fopenmp" +
    //    " ../src/tensor_product.cpp" +
    //    " -I /usr/local/Cellar/nlohmann-json/3.11.2/include" +
    //    " -I../depends/ffiasm/c" +
    //    " -I /usr/local/Cellar/libsodium/1.0.18_1/include" +
    //    " -I."+
    //    " -I../src"
    // )
    // sh("g++-13 -fopenmp src/tensor_product.cpp -I../depends/json/single_include -I /usr/local/Cellar/nlohmann-json/3.11.2/include -I ./depends/ffiasm/c -I /usr/local/Cellar/libsodium/1.0.18_1/include")
// }


cli({
    cleanAll,
    createFieldSources,
    buildPistache,
    buildProverServer,
    buildProver,
    buildTensorProduct
});
