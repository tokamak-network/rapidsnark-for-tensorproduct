#ifndef POLY_UTILS_H
#define POLY_UTILS_H
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "fft.hpp"

typedef std::vector<std::vector<AltBn128::FrElement>> FrElementMatrix;
typedef std::vector<AltBn128::FrElement> FrElementVector;
typedef std::vector<std::vector<std::string>> StringMatrix;
typedef std::vector<std::string> StringVector;
namespace PolyUtils {
  // template <typename Engine>
  class Poly {
    // FFT<typename Engine::Fr> *fft;
  public:
    FrElementMatrix tensorProduct(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix addPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix mulPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix subPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    // FrElementMatrix fftMulPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix scalePoly(FrElementMatrix& array1, AltBn128::FrElement& array2);
    

  private:
    
    // FrElementMatrix _autoTransFromObject(const std::vector<std::vector<std::string>>& input);
    // StringMatrix _transToObject(FrElementMatrix& input);
    // size_t minPowerOfTwo(size_t x);
    // void paddingMatrix(FrElementMatrix& matrix, size_t targetRowLength, size_t targetColLength);
  };

}

#endif // POLY_UTILS_H
