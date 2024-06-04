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
    FrElementMatrix fftMulPoly(FrElementMatrix& array1, FrElementMatrix& array2, FFT<typename Engine::FrElement>& fft);
    FrElementMatrix QapDiv(FrElementMatrix& array1, size_t n, size_t sMax, FFT<typename Engine::FrElement>& fft);
  
  private:
    FrElementMatrix createTX(size_t n, size_t padX, FFT<typename Engine::FrElement>& fft);
    FrElementMatrix createTY(size_t sMax, size_t padY, FFT<typename Engine::FrElement>& fft);
    FrElementMatrix divideByTXandTY(FrElementMatrix& pXY, FrElementMatrix& tXTY, FFT<typename Engine::FrElement>& fft);
    FrElementMatrix _fft2dMulPoly(FrElementMatrix& poly1, FrElementMatrix& poly2, FFT<typename Engine::FrElement>& fft);
    FrElementVector _fft1dMulPoly(FrElementVector& poly1, FrElementVector& poly2, FFT<typename Engine::FrElement>& fft);
    FrElementMatrix _autoTransFromObject(StringMatrix& input);
    StringMatrix _transToObject(FrElementMatrix& input);
    FrElementMatrix reduceDimPoly(FrElementMatrix& coefs);
    void paddingMatrix(FrElementMatrix& matrix, size_t targetRowLength, size_t targetColLength);
    size_t minPowerOfTwo(size_t x);
    FrElementMatrix _transpose(const FrElementMatrix& matrix);

    struct OrderPoly {
        int xOrder;
        int yOrder;
    };

    struct PolyOrder {
        int xOrder;
        int yOrder;
        typename Engine::FrElement coefficient;
    };

    OrderPoly _orderPoly(FrElementMatrix& coefs);
    PolyOrder _findOrder(FrElementMatrix& coefs, int dir);
  };
}

#endif // POLY_UTILS_H
