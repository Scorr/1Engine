#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "core/math/random/Random.h"
#include "core/math/Math.h"

#include "Color.h"

// Implicit cast to Vector4.
Color::operator Vector4f() const
{
	return Vector4f(x, y, z, w);
}

//Randomize the color
void Color::randomize (void)
{
	red = (Real)(Random.Next() % 256) / 255.0f;
	green = (Real)(Random.Next() % 256) / 255.0f;
	blue = (Real)(Random.Next() % 256) / 255.0f;
	alpha = (Real)(Random.Next() % 256) / 255.0f;
}

//===============================================================================================//
// HSL + INT Conversion
//===============================================================================================//
void Color::GetHSLC ( Color& outHSL ) const
{
	Real hue, saturation, lightness;
	Real chroma;

	// compute lightness
	Real fmax, fmin;
	fmax = std::max<Real>( std::max<Real>( red, green ), blue );
	fmin = std::min<Real>( std::min<Real>( red, green ), blue );
	lightness	= (fmax + fmin) / 2.0F;
	chroma		= fmax - fmin;

	if ( fmax == fmin )
	{
		hue = 0.0F;
		saturation = 0.0F;
	}
	else
	{
		// compute hue
			 if ( fmax == red )
			hue = (green - blue) / chroma + (green < blue ? 6.0F : 0.0F);
		else if ( fmax == green )
			hue = (blue - red)   / chroma + 2.0f;
		else if ( fmax == blue )
			hue = (red - green)  / chroma + 4.0f;
		else
			hue = 0;
		hue /= 6.0F;

		// compute saturation
		if ( chroma > 0.0F && lightness < 1.0F )
			saturation = chroma / (1.0F - fabs(2.0F * lightness - 1.0F));
		else
			saturation = 0.0F;
	}

	outHSL.red	 = hue;
	outHSL.green = saturation;
	outHSL.blue	 = lightness;
	outHSL.alpha = chroma;
}

FORCE_INLINE Real HueToRgb ( Real p, Real q, Real t )
{
	if (t < 0.0F)
		t += 1.0F;
	if (t > 1.0F)
		t -= 1.0F;
	if (t < 1.0F / 6.0F)
		return p + (q - p) * 6.0F * t;
	if (t < 1.0F / 2.0F)
		return q;
	if (t < 2.0F / 3.0F)
		return p + (q - p) * (2.0F / 3.0F - t) * 6.0F;
	return p;
}
void Color::SetHSL ( const Color& inHSL )
{
	Real hue = inHSL.red;
	Real saturation = inHSL.green;
	Real lightness = inHSL.blue;

	if ( saturation > 0.0F )
	{
		Real chroma = lightness < 0.5f ? (lightness * (1 + saturation)) : lightness + saturation - lightness * saturation;
		//Real chroma = ( 1.0f - fabs( 2*lightness - 1.0f )) * saturation;

		float p = 2 * lightness - chroma;
		red		= HueToRgb(p, chroma, hue + 1.0F / 3.0F);
		green	= HueToRgb(p, chroma, hue);
		blue	= HueToRgb(p, chroma, hue - 1.0F / 3.0F);
	}
	else
	{
		red		= lightness;
		green	= lightness;
		blue	= lightness;
	}
}

ColorRGBA8 Color::ToRGBA8 ( const ColorRangeMapping& mapping ) const
{
	Vector4f temp = *this;
	temp -= mapping.min_float;
	temp *= (Real)((mapping.max_int + 1) - mapping.min_int) / (mapping.max_float - mapping.min_float);
	temp += (Real)mapping.min_int;

	return ColorRGBA8(
		(uint8_t)math::clamp<int32_t>((int32_t)temp.x, 0, UINT8_MAX),
		(uint8_t)math::clamp<int32_t>((int32_t)temp.y, 0, UINT8_MAX),
		(uint8_t)math::clamp<int32_t>((int32_t)temp.z, 0, UINT8_MAX),
		(uint8_t)math::clamp<int32_t>((int32_t)temp.w, 0, UINT8_MAX));
}

ColorRGBA16	Color::ToRGBA16 ( const ColorRangeMapping& mapping ) const
{
	Vector4f temp = *this;
	temp -= mapping.min_float;
	temp *= (Real)((mapping.max_int + 1) - mapping.min_int) / (mapping.max_float - mapping.min_float);
	temp += (Real)mapping.min_int;

	return ColorRGBA16(
		(int16_t)math::clamp<int32_t>((int32_t)temp.x, INT16_MIN, INT16_MAX),
		(int16_t)math::clamp<int32_t>((int32_t)temp.y, INT16_MIN, INT16_MAX),
		(int16_t)math::clamp<int32_t>((int32_t)temp.z, INT16_MIN, INT16_MAX),
		(int16_t)math::clamp<int32_t>((int32_t)temp.w, INT16_MIN, INT16_MAX));
}

Color ColorRGBA8::ToRGBAFloat ( const ColorRangeMapping& mapping ) const
{
	Vector4f temp ((Real)x, (Real)y, (Real)z, (Real)w);
	temp -= (Real)mapping.min_int;
	temp *= 1.0F / (Real)(mapping.max_int - mapping.min_int);
	temp += mapping.min_float;

	return Color(temp.x, temp.y, temp.z, temp.w);
}

Color ColorRGBA16::ToRGBAFloat ( const ColorRangeMapping& mapping ) const
{
	Vector4_d temp ((Real64)x, (Real64)y, (Real64)z, (Real64)w);
	temp -= (Real64)mapping.min_int;
	temp *= 1.0 / (Real64)(mapping.max_int - mapping.min_int);
	temp += mapping.min_float;

	return Color(temp.x, temp.y, temp.z, temp.w);
}
