#include "glash/glash_pch.hpp"
#include "ColorScript.hpp"

int ColorScript::counter = 0;

void ColorScript::OnUpdate(Cine::Timestep ts)
{
	m_Timer.OnUpdate(ts);

	float time = m_Timer.GetElapsed();
	m_SpriteRendererComponent->Color.r = 0.5f * sin(time) + 0.5f;
	m_SpriteRendererComponent->Color.g = 0.5f * sin(time + 2.0f) + 0.5f;
	m_SpriteRendererComponent->Color.b = 0.5f * sin(time + 4.0f) + 0.5f;
}