#include <system_error>
#include <string>
#include <memory.h>
#include <stdexcept>
#include <alt_bn128.hpp>

#include "poly_utils.hpp"

namespace PolyUtils {

// static std::vector<AltBn128::FrElement> Poly::tensorProduct(AltBn128::FrElement *array1, AltBn128::FrElement *array2) {
//   size_t numRows = array1.size();
//   size_t numCols = array2.size();
//   if (numRows == 1) {
//     return std::vector<AltBn128::Fr.zero()>
//   }

//   if (numCols == 1) {
//     return std::vector<AltBn128::Fr.zero()>
//   }

//   std::vector<AltBn128::FrElement> products;
//   for (u_int64_t i=0; i < NUM_ROWS; i++) {
//     for (u_int64_t j=0; j < NUM_COLS; j++ ) {
//       AltBn128::Fr.mul(
//         products[i*j],
//         array1[i],
//         array2[j]
//       );
//     }
//   }
//   return products
// }
typedef std::vector<AltBn128::FrElement> FrElementMatrix;

// 두 유한 필드 배열의 텐서 곱을 계산하는 함수
std::vector<std::vector<AltBn128::FrElement>> Poly::tensorProduct(
  std::vector<std::vector<AltBn128::FrElement>>& array1, 
  std::vector<std::vector<AltBn128::FrElement>>& array2
) {
  size_t numRows = array1.size();
  size_t numCols = array2[0].size();
  // std::vector<std::vector<AltBn128::FrElement>> result(numRows, std::vector<AltBn128::FrElement>(numCols));
  std::vector<std::vector<AltBn128::FrElement>> result(numRows, std::vector<AltBn128::FrElement>(numCols));

  for (size_t i = 0; i < numRows; ++i) {
    for (size_t j = 0; j < numCols; ++j) {
      AltBn128::Fr.mul(result[i][j], array1[i][0], array2[0][j]); // 두 원소의 곱셈
    }
  }

  return result;
}

// std::vector<AltBn128::FrElement> mulPoly(const std::vector<AltBn128::FrElement>& poly1, const std::vector<AltBn128::FrElement>& poly2) {
//   size_t maxDegree = poly1.size() + poly2.size() - 1;
//   std::vector<AltBn128::FrElement> result(maxDegree, AltBn128_FrElement()); // 결과 다항식의 계수 초기화

//   // 각 다항식의 계수를 곱하여 결과 배열에 저장
//   for (size_t i = 0; i < poly1.size(); ++i) {
//     for (size_t j = 0; j < poly2.size(); ++j) {
//       AltBn128::FrElement temp;
//       Fr_mul(&temp, &poly1[i], &poly2[j]);  // 두 계수를 곱함
//       Fr_add(&result[i + j], &result[i + j], &temp);  // 결과에 추가
//     }
//   }

//   return result;
// }

// std::vector<AltBn128_FrElement> subPoly(const std::vector<AltBn128_FrElement>& poly1, const std::vector<AltBn128_FrElement>& poly2) {
//     size_t maxDegree = std::max(poly1.size(), poly2.size());
//     std::vector<AltBn128_FrElement> result(maxDegree, AltBn128_FrElement()); // 결과 다항식의 계수 초기화

//     // 더 긴 다항식의 길이에 맞춰 계산
//     for (size_t i = 0; i < maxDegree; ++i) {
//         AltBn128_FrElement temp1 = (i < poly1.size()) ? poly1[i] : AltBn128_FrElement(); // poly1의 i번째 계수, 없으면 0
//         AltBn128_FrElement temp2 = (i < poly2.size()) ? poly2[i] : AltBn128_FrElement(); // poly2의 i번째 계수, 없으면 0

//         Fr_sub(&result[i], &temp1, &temp2); // 두 계수를 뺌
//     }

//     return result;
// }

// typedef std::vector<AltBn128_FrElement> Poly;

// struct PolyOrder {
//     int degree;
//     AltBn128_FrElement coefficient;
// };

// // 다항식의 최고 차수와 해당 계수를 찾는 함수
// PolyOrder findOrder(const Poly& poly) {
//     PolyOrder order;
//     order.degree = -1;  // 초기 차수는 -1로 설정 (모든 계수가 0인 경우)

//     // 다항식을 역순으로 탐색하여 첫 번째 비-0 계수를 찾습니다
//     for (int i = poly.size() - 1; i >= 0; --i) {
//         if (!Fr_rawIsZero(poly[i].v)) {
//             order.degree = i;
//             Fr_copy(&order.coefficient, &poly[i]);  // 계수 복사
//             break;
//         }
//     }

//     return order;
// }

// typedef std::vector<std::vector<AltBn128_FrElement>> PolyMatrix;

// // 객체 형태의 유한 필드 요소를 FrElement 타입으로 변환하는 함수
// PolyMatrix autoTransFromObject(const PolyMatrix& poly) {
//     PolyMatrix transformed(poly.size(), std::vector<AltBn128_FrElement>(poly[0].size()));

//     for (size_t i = 0; i < poly.size(); ++i) {
//         for (size_t j = 0; j < poly[i].size(); ++j) {
//             Fr_fromObject(&transformed[i][j], &poly[i][j]); // 객체에서 FrElement로 변환
//         }
//     }

//     return transformed;
// }

// // 다항식의 차수를 감소시키는 함수
// Poly reduceDimPoly(const Poly& poly) {
//     // 최고 차수의 비-0 계수를 찾습니다.
//     int maxNonZeroIndex = -1;
//     for (int i = poly.size() - 1; i >= 0; --i) {
//         if (!Fr_rawIsZero(poly[i].v)) {
//             maxNonZeroIndex = i;
//             break;
//         }
//     }

//     // 모든 계수가 0이면, 최소 차수의 0 다항식을 반환합니다.
//     if (maxNonZeroIndex == -1) {
//         return Poly(1, AltBn128_FrElement());  // 차수 0의 0 다항식
//     }

//     // 필요한 계수만을 포함하는 새로운 다항식을 생성합니다.
//     Poly reducedPoly(maxNonZeroIndex + 1);
//     for (int i = 0; i <= maxNonZeroIndex; ++i) {
//         Fr_copy(&reducedPoly[i], &poly[i]);  // 계수를 새로운 다항식으로 복사
//     }

//     return reducedPoly;
// }

}