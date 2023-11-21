#ifndef GROTH16_HPP
#define GROTH16_HPP

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "binfile_utils.hpp"
#include "fft.hpp"

namespace Groth16 {

    template <typename Engine>
    class Proof {
        Engine &E;
    public:
        typename Engine::G1PointAffine A;
        typename Engine::G2PointAffine B;
        typename Engine::G1PointAffine C;

        Proof(Engine &_E) : E(_E) { };
        std::string toJsonStr();
        json toJson();

        // tensorProduct(
        //     typename Engine::FrElement *_product,
        //     typename Engine::FrElement *_array1, 
        //     typename Engine::FrElement *_array2,
        //     int array1_size,
        //     int array2_size
        // );
    };


 #pragma pack(push, 1)
    template <typename Engine>
    struct Coef {
        u_int32_t m;
        u_int32_t c;
        u_int32_t s;
        typename Engine::FrElement coef;
    };
#pragma pack(pop)

    template <typename Engine>
    class Prover {

        Engine &E;
        u_int32_t nVars;
        u_int32_t nPublic;
        u_int32_t domainSize;
        u_int64_t nCoefs;
        typename Engine::G1PointAffine &vk_alpha1;
        typename Engine::G1PointAffine &vk_beta1;
        typename Engine::G2PointAffine &vk_beta2;
        typename Engine::G1PointAffine &vk_delta1;
        typename Engine::G2PointAffine &vk_delta2;
        Coef<Engine> *coefs;
        typename Engine::G1PointAffine *pointsA;
        typename Engine::G1PointAffine *pointsB1;
        typename Engine::G2PointAffine *pointsB2;
        typename Engine::G1PointAffine *pointsC;
        typename Engine::G1PointAffine *pointsH;

        FFT<typename Engine::Fr> *fft;
    public:
        Prover(
            Engine &_E, 
            u_int32_t _nVars, 
            u_int32_t _nPublic, 
            u_int32_t _domainSize, 
            u_int64_t _nCoefs, 
            typename Engine::G1PointAffine &_vk_alpha1,
            typename Engine::G1PointAffine &_vk_beta1,
            typename Engine::G2PointAffine &_vk_beta2,
            typename Engine::G1PointAffine &_vk_delta1,
            typename Engine::G2PointAffine &_vk_delta2,
            Coef<Engine> *_coefs, 
            typename Engine::G1PointAffine *_pointsA,
            typename Engine::G1PointAffine *_pointsB1,
            typename Engine::G2PointAffine *_pointsB2,
            typename Engine::G1PointAffine *_pointsC,
            typename Engine::G1PointAffine *_pointsH,
        ) : 
            E(_E), 
            nVars(_nVars),
            nPublic(_nPublic),
            domainSize(_domainSize),
            nCoefs(_nCoefs),
            vk_alpha1(_vk_alpha1),
            vk_beta1(_vk_beta1),
            vk_beta2(_vk_beta2),
            vk_delta1(_vk_delta1),
            vk_delta2(_vk_delta2),
            coefs(_coefs),
            pointsA(_pointsA),
            pointsB1(_pointsB1),
            pointsB2(_pointsB2),
            pointsC(_pointsC),
            pointsH(_pointsH)
        { 
            fft = new FFT<typename Engine::Fr>(domainSize*2);
        };

        ~Prover() {
            delete fft;
        }

        std::unique_ptr<Proof<Engine>> prove(typename Engine::FrElement *wtns);
        std::unique_ptr<Proof<Engine>> tensorProduct(
            typename Engine::FrElement *_array1, 
            typename Engine::FrElement *_array2,
            int array1_size,
            int array2_size
        );
    };

    template <typename Engine>
    std::unique_ptr<Prover<Engine>> makeProver(
        u_int32_t nVars, 
        u_int32_t nPublic, 
        u_int32_t domainSize, 
        u_int64_t nCoefs, 
        void *vk_alpha1,
        void *vk_beta1,
        void *vk_beta2,
        void *vk_delta1,
        void *vk_delta2,
        void *coefs,
        void *pointsA,
        void *pointsB1,
        void *pointsB2,
        void *pointsC,
        void *pointsH
    );

    // template <typename Engine>
    // std::unique_ptr<Prover<Engine>> tensorProduct(void *_array1, void *_arry2);

    // template <typename Engine>
    // class TensorProduct {
    //     Engine &E;
    //     // typename Engine::G1PointAffine &_array1;
    //     // typename Engine::G1PointAffine &_array2;
    // public:
    //     Engine &E;
    //     typename Engine::FG1PointAffiner &_array1;
    //     typename Engine::G1PointAffine &_array2;
    //     typename Engine::FrElement fr;

    //     TensorProduct(
    //         typename Engine::Fr &_array1,
    //         typename Engine::Fr &_array2
    //     ) : fr {};
        
    // };
    // std::unique_ptr<Prover<Engine>> tensorProduct(void *_array1, void *_arry2);
};


#include "groth16.cpp"

#endif