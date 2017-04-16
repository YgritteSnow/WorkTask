#include "RenderManager.h"
#include "BitMap.h"
#include <random>
/* *********************************************
* 构造/析构
* *********************************************/
bool RenderManager::Init(ScreenCoord width, ScreenCoord height){
	if (RenderManager::m_instance == nullptr){
		RenderManager::m_instance = new RenderManager();
		if (RenderManager::m_instance == nullptr) {
			return false; 
		}

		if (!RenderManager::m_instance->SetupBuffer(width, height)) {
			delete RenderManager::m_instance;
			RenderManager::m_instance = nullptr;
			return false;
		}
	}
	return (RenderManager::m_instance != nullptr);
}

void RenderManager::UnInit(){
	if (RenderManager::m_instance != nullptr){
		RenderManager::m_instance->ReleaseBuffer();
		delete RenderManager::m_instance;
	}
	return;
}

bool RenderManager::IsInited(){
	return RenderManager::m_instance != nullptr;
}

RenderManager* RenderManager::GetInstance(){
	return RenderManager::m_instance;
}

/* *********************************************
* 建立和释放缓存
* *********************************************/
bool RenderManager::SetupBuffer(ScreenCoord width, ScreenCoord height) {
	m_imgBuffer_back = new TexBuffer<NormColor4>(width, height);
	if (!m_imgBuffer_back) { return false; }
	m_imgBuffer_front = new TexBuffer<NormColor4>(width, height);
	if (!m_imgBuffer_front) { return false; }
	m_imgBuffer_depth = new TexBuffer<DepthBufferPixel>(width, height);
	if (!m_imgBuffer_depth) { return false; }
	m_imgBuffer_alpha = new AlphaBuffer(width, height);
	if (!m_imgBuffer_alpha) { return false; }
	return true;
}

void RenderManager::ReleaseBuffer() {
	if (m_imgBuffer_back) {
		delete m_imgBuffer_back;
		m_imgBuffer_back = nullptr;
	}
	if (m_imgBuffer_front) {
		delete m_imgBuffer_front;
		m_imgBuffer_front = nullptr;
	}
	if (m_imgBuffer_depth) {
		delete m_imgBuffer_depth;
		m_imgBuffer_depth = nullptr;
	}
	if (m_imgBuffer_alpha) {
		delete m_imgBuffer_alpha;
		m_imgBuffer_alpha = nullptr;
	}
}

/* *********************************************
 * 渲染相关
 * *********************************************/
void RenderManager::RenderDummy(){}

void RenderManager::Clear() {
	m_imgBuffer_back->clear();
	m_imgBuffer_depth->clear(DepthBufferPixel(1000.f));
	m_imgBuffer_alpha->clear();
}

void RenderManager::Present() {
	OnBeforePresent();
	auto tmp = m_imgBuffer_front;
	m_imgBuffer_front = m_imgBuffer_back;
	m_imgBuffer_back = tmp;
	BitMap::Display(m_imgBuffer_front);
}

void RenderManager::OnBeforePresent() {
	if (CheckCurState(StateMask_Alpha, StateMaskValue_UseAlpha)){
		m_imgBuffer_alpha->Blend();
		for (int x = 0; x < m_imgBuffer_back->width; ++x) {
			for (int y = 0; y < m_imgBuffer_back->height; ++y) {
				auto pBackPixel = m_imgBuffer_back->pPixelAt(x, y);
				auto pAlphaPixel = m_imgBuffer_alpha->pPixelAt(x, y);
				*pBackPixel = BlendColor(*pBackPixel, pAlphaPixel->m_blended_color);
			}
		}
	}
}

// 设置渲染状态
void RenderManager::SetRenderState(StateMaskType state, StateMaskType value) {
	m_renderState = m_renderState & (StateMaskAll ^ state);
	m_renderState = m_renderState | value;
}
bool RenderManager::CheckCurState(StateMaskType state, StateMaskType value) {
	return CheckState(m_renderState, state, value);
}

// 渲染缓存
void RenderManager::RenderVertexIndice(VertexBuffer* vb, IndexBuffer* ib, Matrix44 modelMat) {
	// 顶点操作
	// 这一步要将顶点变换到屏幕空间
	auto vsShader = ShaderManager::GetInstance()->GetVertexShader();
	auto vb_temp = new VertexBuffer(vsShader->GetOutId(), vb->m_length);
	ShaderManager::ProcessVertex(vb, vb_temp);
	DebugManager::GetInstance()->AddVertexCount(vb->m_length);
	// 三角形操作
	// 这一步根据顶点位置和绕序，进行剔除
	MaskType* cull_mask = new MaskType[static_cast<int>(ib->size() / TRIANGLE_COUNT)];
	ProcessTriangle(vb_temp, ib, cull_mask);

	const auto& indice_count = ib->size();
	const auto& vb_vid = vb_temp->GetId();

	unsigned int tmp_triangle_count = 0;
	for (size_t idx = 0; idx < indice_count - 2; idx += 3) {
		if (IS_CULLED(cull_mask[idx / 3])) {
			continue;
		}
		tmp_triangle_count += 1;

		auto indice_idx_0 = ib->m_vec_indice[idx];
		auto indice_idx_1 = ib->m_vec_indice[idx + 1];
		auto indice_idx_2 = ib->m_vec_indice[idx + 2];

		const auto& v0 = vb_temp->GetVertex(indice_idx_0);
		const auto& v1 = vb_temp->GetVertex(indice_idx_1);
		const auto& v2 = vb_temp->GetVertex(indice_idx_2);

		if (CheckCurState(StateMask_DrawMode, StateMaskValue_Wareframe)) {
			RenderTriangle_wireframe(vb_vid, v0, v1, v2);
		}
		else if (CheckCurState(StateMask_DrawMode, StateMaskValue_Fill)) {
			RenderTriangle_fill(vb_vid, v0, v1, v2);
		}
	}
	DebugManager::GetInstance()->AddValidTriangleCount(tmp_triangle_count);
	DebugManager::GetInstance()->AddTriangleCount(indice_count / 3);

	delete[] cull_mask;
}

/* *********************************************
* 对三角形进行处理
* *********************************************/
void RenderManager::ProcessTriangle(const VertexBuffer* pVbuffer, const IndexBuffer* ib, MaskType* cullMask) {
	for (int i = 0; i <= ib->size() - TRIANGLE_COUNT; i += TRIANGLE_COUNT) {
		auto pIndex = &((*ib)[i]);
		auto pMask = cullMask + i / TRIANGLE_COUNT;
		memset(pMask, NO_CULLED, sizeof(MaskType));

		DWORD indice_idx_0 = pIndex[0];
		DWORD indice_idx_1 = pIndex[1];
		DWORD indice_idx_2 = pIndex[2];

		const auto& v0 = pVbuffer->GetVertex(indice_idx_0);
		const auto& v1 = pVbuffer->GetVertex(indice_idx_1);
		const auto& v2 = pVbuffer->GetVertex(indice_idx_2);

		STRIDE_TYPE pos_off = StructReflectManager::GetOffset<POSITION>(pVbuffer->GetId(), 0);
		const auto& v0pos = *static_cast<WorldPos*>(static_cast<void*>((v0 + pos_off)));
		const auto& v1pos = *static_cast<WorldPos*>(static_cast<void*>((v1 + pos_off)));
		const auto& v2pos = *static_cast<WorldPos*>(static_cast<void*>((v2 + pos_off)));

		(*pMask) &= ProcessTriangle_backCull(v0pos, v1pos, v2pos, this->m_renderState);
		(*pMask) &= ProcessTriangle_screenCull(v0pos, v1pos, v2pos);
	}
}
/* *********************************************
* 光栅化（顶点坐标已经转换到屏幕坐标）
* *********************************************/
void RenderManager::RenderLine(STRUCT_ID vid, byte* v1, byte* v2) {
	if (CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)) {
		VertexBrush::DrawLine(vid, v1, v2, m_imgBuffer_back);
	}
	else if (CheckCurState(StateMask_Alpha, StateMaskValue_UseAlpha)) {
		VertexBrush::DrawLine(vid, v1, v2, m_imgBuffer_alpha);
	}
}

void RenderManager::RenderTriangle_wireframe(STRUCT_ID vid, byte* v1, byte* v2, byte* v3) {
	RenderLine(vid, v1, v2);
	RenderLine(vid, v2, v3);
	RenderLine(vid, v3, v1);
}

void RenderManager::RenderTriangle_fill(STRUCT_ID vid, byte* v1, byte* v2, byte* v3) {
	if (CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)) {
		VertexBrush::DrawTriangle(vid, v1, v2, v3, m_imgBuffer_back);
	}
	else if (CheckCurState(StateMask_Alpha, StateMaskValue_UseAlpha)) {
		VertexBrush::DrawTriangle(vid, v1, v2, v3, m_imgBuffer_alpha);
	}
}
/* *********************************************
* 
* *********************************************/
RenderManager* RenderManager::m_instance = nullptr;