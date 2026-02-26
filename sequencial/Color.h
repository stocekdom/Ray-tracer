//
// Created by dominik on 18.02.26.
//

#ifndef SEQUENCIAL_COLOR_H
#define SEQUENCIAL_COLOR_H
#include <cstdint>
#include <iosfwd>

/**
 * @brief Representation of a color using floats for color channels.
 *
 * Uses the RGBA format. The color channels use float, and the alpha uses 8-bit integer
 * The channel values may be any float value, and the operations on colors don't clamp them. However, for outputting the result, the values will need to be clamped or tone mapped
 * The range [0,1] of the floats maps to [0,255] of the uint8 required for classic RGB format
 */
class Color
{
   public:
      Color();

      Color( float R, float G, float B, uint8_t alpha = 255 );

      Color( const Color& c ) = default;

      Color& operator=( const Color& c ) = default;

      float R;
      float G;
      float B;
      uint8_t alpha;
};


std::ostream& operator<<( std::ostream& os, const Color& v );

/**
 * @brief Checks if the two colors are equal
 *
 * Compares all elements of the color class that also includes the alpha channel. RGBA comparison
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return True if the colors are equal and false if they are not
 */
bool operator==( const Color& c1, const Color& c2 );

/**
 * @brief Checks if the two colors are not equal
 *
 * Compares all elements of the color class that also includes the alpha channel. RGBA comparison
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return True if the colors are not equal and false if they are equal
 */
bool operator!=( const Color& c1, const Color& c2 );

/**
 * @brief Memberwise addition of the color RGB components.
 *
 * Does a memberwise addition of RGB channels. Alpha channel isn't used in this operation.
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return The resulting color
 */
Color operator+( const Color& c1, const Color& c2 );

/**
 * @brief Memberwise addition of the color RGB components.
 *
 * Does a memberwise addition of RGB channels. Alpha channel isn't used in this operation.
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return Reference to the left color
 */
Color& operator+=( Color& c1, const Color& c2 );

/**
 * @brief Memberwise subtraction of the color RGB components.
 *
 * Does a memberwise subtraction of RGB channels. Alpha channel isn't used in this operation.
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return The resulting color
 */
Color operator-( const Color& c1, const Color& c2 );

/**
 * @brief Memberwise subtraction of the color RGB components.
 *
 * Does a memberwise subtraction of RGB channels. Alpha channel isn't used in this operation.
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return Reference to the left color
 */
Color& operator-=( Color& c1, const Color& c2 );


/**
 * @brief Memberwise multiplication of the color RGB components.
 *
 * Does a memberwise multiplication of RGB channels. Alpha channel isn't used in this operation.
 *
 * @param c1 Left operand color
 * @param c2 Right operand color
 * @return Reference to the left color
 */
Color operator*( const Color& c1, const Color& c2 );

/**
 * @brief Multiplies all color components by a scalar factor.
 * 
 * Scales the R, G, B, and alpha components of the color by the given factor,
 * clamping the results to the valid uint8 range [0, 255]. Useful for adjusting
 * color intensity or brightness.
 *
 * @note Values of the factor should be near 1. Example: f=1.5 increases the color components by half. f=0.5 decreases the color components by half
 * @param c1 The color to scale.
 * @param f The scaling factor to apply.
 * @return The resulting color with scaled components.
 */
Color operator*( const Color& c1, float f );

/**
 * @brief Multiplies all color components by a scalar factor.
 *
 * Scales the R, G, B, and alpha components of the color by the given factor,
 * clamping the results to the valid uint8 range [0, 255]. Useful for adjusting
 * color intensity or brightness.
 *
 * @param c1 The color to scale.
 * @param f The scaling factor to apply.
 * @return Reference to the multiplied color
 */
Color& operator*=( Color& c1, float f );

#endif //SEQUENCIAL_COLOR_H
