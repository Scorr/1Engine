#include "UIRenderer.h"

#include "core/math/Math.h"
#include "engine-common/dusk/Element.h"
#include "renderer/utils/rrMeshBuilder2D.h"
#include "renderer/utils/rrTextBuilder2D.h"

struct ErScissorParams
{
	ErScissorParams ( const Vector2f& scissor_position, const Vector2f& scissor_size )
		: data(scissor_position, scissor_size )
		{};

	Vector4f	data;
	operator const Vector4f&() const
	{
		return data;
	}
};

struct ErVertexEffectParams
{
	ErVertexEffectParams ( const float additive_mouse_glow, const float multiplicative_mouse_glow )
		: data(
			additive_mouse_glow > 0 ? additive_mouse_glow : -multiplicative_mouse_glow,
			0.0F,
			0.0F,
			0.0F)
		{};

	Vector4f	data;
	operator const Vector4f&() const
	{
		return data;
	}
};

void dusk::UIRendererContext::setScissor ( const Rect& scissor )
{
	m_currentScissor = scissor;

	Vector2f offset, multiplier;
	m_mb2->getScreenMapping(multiplier, offset);

	m_currentScissor.pos = m_currentScissor.pos.mulComponents(multiplier);
	m_currentScissor.pos += offset;
	m_currentScissor.size = m_currentScissor.size.mulComponents(multiplier);

	// Unflip.
	m_currentScissor.pos.y += m_currentScissor.size.y;
	m_currentScissor.size.y = -m_currentScissor.size.y;
}
void dusk::UIRendererContext::setFocus ( FocusStyle style )
{
	m_focusType = style;
}
void dusk::UIRendererContext::setColor ( ColorStyle style, size_t subelement )
{
	m_colorType = style;
	m_colorSubelement = subelement;
}
void dusk::UIRendererContext::setTextSettings ( TextStyleSettings settings )
{
	m_textType = settings;
}


void dusk::UIRendererContext::generateColor( Element* source )
{
	// Constants for the theme
	const Color kAccentColor (0.16F, 0.32F, 0.96F, 1.00F);
	const Color kBackgroundColor (0.16F, 0.16F, 0.16F, 1.00F);
	const Color kBackgroundBump (0.16F, 0.16F, 0.16F, 1.00F);
	const Color kElementBump = kBackgroundBump * 0.5F;
	const Color kAccentColorGray = kAccentColor.luminosityRGBA();

	// Set up the theme defaults
	Color l_bgDisabledColor = kBackgroundColor * 0.5F;
	Color l_bgHoveredColor = kBackgroundColor + kBackgroundBump;
	Color l_bgActiveColor = kBackgroundColor;

	Color l_focusNormalColor = kAccentColor;
	Color l_focusDisabledColor = kAccentColorGray;

	// Create specific colors for each theme bit
	if (m_colorType == kColorStyleBackground)
	{
	}
	else if (m_colorType == kColorStyleElement)
	{
		l_bgDisabledColor = kBackgroundColor - kElementBump * 1.0F;
		l_bgHoveredColor = kBackgroundColor + kBackgroundBump * 2.0F;
		l_bgActiveColor = kBackgroundColor + kElementBump * 1.0F;
	}
	else if (m_colorType == kColorStyleElementEmphasized)
	{
		l_bgDisabledColor = kBackgroundColor + kBackgroundBump * 0.5F;
		l_bgHoveredColor = kBackgroundColor + kBackgroundBump * 2.0F;
		l_bgActiveColor = kBackgroundColor + kBackgroundBump * 1.0F;
	}
	else if (m_colorType == kColorStyleElementEmphasized2)
	{
		l_bgDisabledColor = kBackgroundColor + kBackgroundBump * 0.75F;
		l_bgHoveredColor = kBackgroundColor + kBackgroundBump * 3.5F;
		l_bgActiveColor = kBackgroundColor + kBackgroundBump * 2.5F;
	}
	else if (m_colorType == kColorStyleShapeNormal)
	{
		l_bgDisabledColor = kBackgroundColor + kBackgroundBump * 0.5F;
		l_bgHoveredColor = kBackgroundColor + kBackgroundBump * 2.0F;
		l_bgActiveColor = kBackgroundColor + kBackgroundBump * 1.0F;
	}
	else if (m_colorType == kColorStyleShapeAccented)
	{
		l_bgDisabledColor = kAccentColor * 0.5F;
		l_bgHoveredColor = kAccentColor + kBackgroundBump;
		l_bgActiveColor = kAccentColor;
	}

	// focused is normal color but adds glow
	// disabled is normal color but gray
	// hovered is brighter color in general

	// Update backgrounds
	if (m_focusType == kFocusStyleDisabled 
		|| (m_focusType == kFocusStyleAutomatic && !source->m_isEnabled)
		|| (m_focusType == kFocusStyleAutomaticNoHover && !source->m_isEnabled)
		)
	{
		m_dsColorBackground = l_bgDisabledColor;
		m_dsDrawBackground = true;
	}
	else if (m_focusType == kFocusStyleHovered
		|| (m_focusType == kFocusStyleAutomatic && source->m_isMouseIn)
		)
	{
		m_dsColorBackground = l_bgHoveredColor;
		if (source->m_isActivated)
		{	// Half-bump for activation
			m_colors->setBackgroundClickPulse(source, m_colorSubelement, m_dsColorBackground + kBackgroundBump * 0.5F);
		}
		m_dsDrawBackground = true;
	}
	else if (m_focusType == kFocusStyleActive
		|| (m_focusType == kFocusStyleAutomatic && source->m_isEnabled)
		|| (m_focusType == kFocusStyleAutomaticNoHover && source->m_isEnabled)
		|| (m_focusType == kFocusStyleFocused)
		)
	{
		m_dsColorBackground = l_bgActiveColor;
		if (source->m_isActivated)
		{	// Half-bump for activation
			m_colors->setBackgroundClickPulse(source, m_colorSubelement, m_dsColorBackground + kBackgroundBump * 0.5F);
		}
		m_dsDrawBackground = true;
	}
	m_dsColorBackground.a = kBackgroundColor.a;

	// Disable animations except for some key colors
	if (m_colorType != kColorStyleShapeAccented)
	{
		m_colors->setBackgroundColor(source, m_colorSubelement, m_dsColorBackground);
		m_dsColorBackground = m_colors->getBackgroundColor(source, m_colorSubelement);
	}

	// Update outlines
	if (m_focusType == kFocusStyleFocused
		|| (m_focusType == kFocusStyleAutomatic && m_uir->m_renderMouselessFocus && source->m_isFocused)
		|| (m_focusType == kFocusStyleAutomaticNoHover && m_uir->m_renderMouselessFocus && source->m_isFocused))
	{
		m_dsColorOutline = l_focusNormalColor;
		m_dsDrawOutline = true;
	}
	else if ((m_focusType == kFocusStyleDisabled && source->m_isFocused))
	{
		m_dsColorOutline = l_focusDisabledColor;
		m_dsDrawOutline = true;
	}
	else
	{
		m_dsDrawOutline = false;
	}
}
void dusk::UIRendererContext::generateDrawState( Element* source )
{

}


float dusk::UIRendererContext::getTextHeight ( TextFontStyle font )
{
	return 16; // TODO: Use font setting given by either renderer, or grabbed from renderer
}

float dusk::UIRendererContext::getTextWidth ( TextFontStyle font, const char* str )
{
	rrTextBuilder2D* l_textBuilder = static_cast<rrTextBuilder2D*>(m_mb2);

	Vector2f l_screenMultiplier, l_screenOffset;
	l_textBuilder->getScreenMapping(l_screenMultiplier, l_screenOffset);
	Vector2f l_size = l_textBuilder->predictTextSize(str) / l_screenMultiplier.x;
	return l_size.x;
}

void dusk::UIRendererContext::drawRectangle ( Element* source, const Rect& rectangle, const DrawRectParams& params )
{
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotNormal);
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotUV1);
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotUV2);
	auto tVertexCount = m_mb2->getModelDataVertexCount();

	generateColor(source);

	if (m_dsDrawBackground)
		m_mb2->addRect(rectangle, m_dsColorBackground, false);
	if (m_dsDrawOutline)
		m_mb2->addRect(rectangle, m_dsColorOutline, true);

	for (uint16_t i = tVertexCount; i < m_mb2->getModelDataVertexCount(); ++i)
	{
		// Zero out normals to disable texture use on this shape
		m_modeldata->normal[i] = Vector3f(0.0F, 0.0F, 0.0F);
		// Disable mouse glow effects
		m_modeldata->texcoord2[i] = ErVertexEffectParams(params.interactible ? 1.0F : 0.0F, 0.0F);
	}

	// TODO: Make this less of a hack
	// Add the mouse key area hover
	if ( !params.disable_hover_outline )
	{
		tVertexCount = m_mb2->getModelDataVertexCount();
		if (params.interactible)
			m_mb2->addRect(rectangle, m_dsColorBackground, true);

		for (uint16_t i = tVertexCount; i < m_mb2->getModelDataVertexCount(); ++i)
		{
			// Zero out normals to disable texture use on this shape
			m_modeldata->normal[i] = Vector3f(0.0F, 0.0F, 0.0F);
			// Disable mouse glow effects
			m_modeldata->texcoord2[i] = ErVertexEffectParams(0.0F, 1.0F);
		}
	}
}

void dusk::UIRendererContext::drawBorder ( Element* source, const Rect& rectangle )
{
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotNormal);
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotUV1);
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotUV2);
	auto tVertexCount = m_mb2->getModelDataVertexCount();

	generateColor(source);

	if (m_dsDrawBackground)
		m_mb2->addRect(rectangle, m_dsColorBackground, true, 2.0F);
	if (m_dsDrawOutline)
		m_mb2->addRect(rectangle, m_dsColorOutline, true);

	for (uint16_t i = tVertexCount; i < m_mb2->getModelDataVertexCount(); ++i)
	{
		// Zero out normals to disable texture use on this shape
		m_modeldata->normal[i] = Vector3f(0.0F, 0.0F, 0.0F);
		// Disable mouse glow effects
		m_modeldata->texcoord2[i] = ErVertexEffectParams(1.0F, 0.0F);
	}
}

void dusk::UIRendererContext::drawText ( Element* source, const Vector2f& position, const char* str )
{
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotUV1);
	m_mb2->enableAttribute(renderer::shader::kVBufferSlotUV2);
	auto tVertexCount = m_mb2->getModelDataVertexCount();

	rrTextBuilder2D* l_textBuilder = static_cast<rrTextBuilder2D*>(m_mb2);
	Vector2f l_drawPosition (math::round(position.x), math::round(position.y + 0.9F * getTextHeight(m_textType.font)));

	Vector2f l_screenMultiplier, l_screenOffset;
	l_textBuilder->getScreenMapping(l_screenMultiplier, l_screenOffset);

	if (m_textType.align_vertical == dusk::kTextAlignBottom)
		l_drawPosition.y -= getTextHeight(kTextFontButton);
	else if (m_textType.align_vertical == dusk::kTextAlignMiddle)
		l_drawPosition.y -= getTextHeight(kTextFontButton) * 0.5F;

	Vector2f l_size = l_textBuilder->predictTextSize(str).divComponents(l_screenMultiplier);
	if (m_textType.align_horizontal == dusk::kTextAlignCenter)
		l_drawPosition.x -= l_size.x * 0.5F;
	else if (m_textType.align_horizontal == dusk::kTextAlignRight)
		l_drawPosition.x -= l_size.x;

	l_textBuilder->addText(
		l_drawPosition,
		source->m_isEnabled ? Color(1.0F, 1.0F, 1.0F, 1.0F) : Color(0.7F, 0.7F, 0.7F, 1.0F), // TODO: Make this use the style rules above
		str);

	for (uint16_t i = tVertexCount; i < m_mb2->getModelDataVertexCount(); ++i)
	{
		// Shunt current scissor params into this shape
		m_modeldata->texcoord1[i] = Vector4f(m_currentScissor.pos, m_currentScissor.size);
		// Disable mouse glow effects
		m_modeldata->texcoord2[i] = ErVertexEffectParams(0.0F, 0.0F);
	}
}