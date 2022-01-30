#pragma once

#include <Core/EngineTypes.h>
#include <Memory/Array.h>
#include <random>
#include <chrono>

/*!
 * A utility class allowing to generate random numbers.
 */
class Random {

public:

    /*!
     *  Generates a random bool.
     *
     *  \return A random bool.
     */
    static bool randomBool();

    /*!
     *  Generates a random integer.
     *
     *  \tparam NumberType The type of integer to generate.
     *  \param[in] min The minimal value to generate.
     *  \param[in] max The maximal value to generate.
     *  \return A random integer.
     */
    template<typename NumberType>
    static NumberType randomInteger(NumberType min, NumberType max);

    /*!
     *  Generates a random floating point number.
     *
     *  \tparam NumberType The type of float to generate.
     *  \param[in] min The minimal value to generate.
     *  \param[in] max The maximal value to generate.
     *  \return A random floating point number.
     */
    template<typename NumberType>
    static NumberType randomFloat(NumberType min, NumberType max);

    /*!
     *  Generates an array containing random floating point numbers.
     *
     *  \tparam NumberType The type of float to generate.
     *  \param[in] size The amount of numbers to generate.
     *  \param[in] min The minimal value to generate.
     *  \param[in] max The maximal value to generate.
     *  \return An array containing random floating point numbers.
     */
    template<typename NumberType>
    static Array<NumberType> randomFloatArray(size_t size, NumberType min, NumberType max);

    /*!
     *  Generates an array containing random integers.
     *
     *  \tparam NumberType The type of integer to generate.
     *  \param[in] size The amount of numbers to generate.
     *  \param[in] min The minimal value to generate.
     *  \param[in] max The maximal value to generate.
     *  \return An array containing random integers.
     */
    template<typename NumberType>
    static Array<NumberType> randomIntegerArray(size_t size, NumberType min, NumberType max);

private:

    /*!
     *  Instantiates and seeds a new random number generator.
     *
     *  \return The random number generator.
     */
    static std::minstd_rand createGenerator();

    /*!
     * The generator used to generate random numbers.
     */
    inline static std::minstd_rand generator = createGenerator();

};

#define __RAND_INCL

#include <Utilities/Random.inl>

#undef __RAND_INCL
