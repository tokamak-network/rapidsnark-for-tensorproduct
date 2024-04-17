#include <gmp.h>
#include <memory>
#include <string>
#include <cstring>
#include <stdexcept>
#include <alt_bn128.hpp>
#include <nlohmann/json.hpp>
#include <time.h>
#include <iostream>

#include "fr.hpp"
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include "wtns_utils.hpp"

int tensor_product(
    const void *zkey_buffer,   
    unsigned long  zkey_size,
    const void *zkey_buffer1,
    unsigned long  zkey_size1
) {
    try {
        BinFileUtils::BinFile zkey(zkey_buffer, zkey_size, "zkey", 1);
        auto zkeyHeader = ZKeyUtils::loadHeader(&zkey);
        
        BinFileUtils::BinFile zkey1(zkey_buffer1, zkey_size1, "zkey", 2);
        auto zkey1Header = ZKeyUtils::loadHeader(&zkey1);

        AltBn128::FrElement *fYK = (AltBn128::FrElement *)zkey1.getSectionData(1);
        AltBn128::FrElement *scaled = (AltBn128::FrElement *)zkey.getSectionData(1);

        for (u_int32_t i=0; i<1024; i++) {
            for (u_int32_t j=0; j<32; j++ ){
                typename AltBn128::FrElement aux;
                E.fr.mul(
                    aux,
                    fYK[0][j], 
                    scaled[i][0]
                );
            }
        }

    } catch (std::exception& e) {

        if (error_msg) {
            strncpy(error_msg, e.what(), error_msg_maxsize);
        }
        return PPROVER_ERROR;

    } catch (std::exception *e) {

        if (error_msg) {
            strncpy(error_msg, e->what(), error_msg_maxsize);
        }
        delete e;
        return PPROVER_ERROR;

    } catch (...) {
        if (error_msg) {
            strncpy(error_msg, "unknown error", error_msg_maxsize);
        }
        return PPROVER_ERROR;
    }

    return PRPOVER_OK;
}