#include "BookEnginePreCompile.h"
#include "FontRender.h"

namespace nsBookEngine {

	void FontRender::Draw(RenderContext& rc)
	{
		if (m_text == nullptr) {
			return;
		}

		RenderingEngine::GetInstance()->AddRenderObject(this);
	}
}
