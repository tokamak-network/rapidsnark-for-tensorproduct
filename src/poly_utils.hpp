#ifndef POLY_UTILS_H
#define POLY_UTILS_H

#include <string>
#include <vector>
#include "fft.hpp"

namespace PolyUtils {
  template <typename Engine>
  class Poly {
    FFT<typename Engine::Fr> *fft;
  public:
    typedef std::vector<std::vector<typename Engine::FrElement>> FrElementMatrix;
    typedef std::vector<typename Engine::FrElement> FrElementVector;
    typedef std::vector<std::vector<std::string>> StringMatrix;
    typedef std::vector<std::string> StringVector;

    FrElementMatrix tensorProduct(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix addPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix mulPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix subPoly(FrElementMatrix& array1, FrElementMatrix& array2);
    FrElementMatrix scalePoly(FrElementMatrix& poly, typename Engine::FrElement& scaler);
  };
}

#endif // POLY_UTILS_H
