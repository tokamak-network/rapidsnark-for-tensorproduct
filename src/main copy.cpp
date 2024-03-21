#include <iostream>
#include <fstream>
#include <vector>
#include <alt_bn128.hpp>
#include <nlohmann/json.hpp> 
#include "binfile_utils.hpp"

const size_t BufferSize = 16384;
const size_t NUM_ROWS = 1024;
const size_t NUM_COLS = 32;


int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <zkeyFilename> <zkey1Filename>\n";
        return EXIT_FAILURE;

        RawFr& fr = RawFr::field;

    }
    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);

    try {
        std::string zkeyFilename = argv[1];
        std::string zkey1Filename = argv[2];

        auto zkeyData = BinFileUtils::openExisting(zkeyFilename, "zkey", 1);
        auto zkey1Data = BinFileUtils::openExisting(zkey1Filename, "zkey", 1);

        auto fyK = reinterpret_cast<FrElement**>(zkeyData->getSectionData(2)); 
        auto scaled = reinterpret_cast<FrElement**>(zkey1Data->getSectionData(2)); 

        std::vector<std::vector<FrElement>> products(1024, std::vector<FrElement>(32)); 

        vector<vector<RawFr::Element>> products(NUM_ROWS, vector<RawFr::Element>(NUM_COLS));

        for (size_t i = 0; i < NUM_ROWS; ++i) {
            for (size_t j = 0; j < NUM_COLS; ++j) {
                fr.mul(products[i][j], fyK[i][j], scaled[i][j]);
            }
        }

        json jsonResult;
        for (size_t i = 0; i < NUM_ROWS; ++i) {
            for (size_t j = 0; j < NUM_COLS; ++j) {
                jsonResult[std::to_string(i)][std::to_string(j)] = fr.toString(products[i][j]);
            }
        }

        ofstream outputFile("products.json");
        outputFile << jsonResult.dump(4); 
        outputFile.close();

    } catch (const exception& e) {
        cerr << "Exception caught: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
