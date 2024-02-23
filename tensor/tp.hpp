#ifndef TENSOR_HPP
#define TENSOR_HPP

#ifdef __cplusplus
extern "C" {
#endif

//Error codes returned by the functions.
#define PRPOVER_OK                  0x0
#define PPROVER_ERROR               0x1
#define PPROVER_ERROR_SHORT_BUFFER  0x2


/**
 * @return error code:
 *         PRPOVER_OK - in case of success.
 *         PPROVER_ERROR - in case of an error.
 */

int tensor_product(
  const void *zkey_buffer,   
  unsigned long  zkey_size,
  const void *zkey_buffer1,
  unsigned long  zkey_size1
);

#ifdef __cplusplus
}
#endif


#endif // TENSOR_HPP
