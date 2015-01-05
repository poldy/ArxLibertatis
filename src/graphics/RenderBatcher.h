/*
 * Copyright 2011-2012 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARX_GRAPHICS_RENDERBATCHER_H
#define ARX_GRAPHICS_RENDERBATCHER_H

#include "graphics/Renderer.h"
#include "graphics/VertexBuffer.h"
#include "graphics/data/TextureContainer.h"
#include "graphics/texture/TextureStage.h"

#include <list>
#include <map>
#include <vector>

struct TexturedQuad {
	ProjectedVertex v[4];
};

struct RenderMaterial {

	enum BlendType {
		Opaque,
		Additive,
		AlphaAdditive,
		Screen,
		Subtractive,
		Subtractive2
	};

	enum Layer {
		Decal,
		Effect,
		EffectForeground,
		FullscreenEffect,
		HUDEffect
	};

	RenderMaterial();

	bool operator<(const RenderMaterial & other) const;
	void apply() const;

	Texture * getTexture() const { return m_texture; }
	void resetTexture() { m_texture = NULL; }
	void setTexture(Texture * tex) { m_texture = tex; }
	void setTexture(TextureContainer * texContainer) {
		m_texture = texContainer ? (Texture *)texContainer->m_pTexture : NULL;
	}

	bool getDepthTest() const { return m_depthTest; }
	void setDepthTest(bool bEnable) { m_depthTest = bEnable; }

	BlendType getBlendType() const { return m_blendType; }
	void setBlendType(BlendType type) { m_blendType = type; }

	Layer getLayer() const { return m_layer; }
	void setLayer(Layer layer) { m_layer = layer; }

	TextureStage::WrapMode getWrapMode() const { return m_wrapMode; }
	void setWrapMode(TextureStage::WrapMode mode) { m_wrapMode = mode; }

	int getDepthBias() const { return m_depthBias; }
	void setDepthBias(int bias) { m_depthBias = bias; }

	Renderer::CullingMode getCulling() const { return m_cullingMode; }
	void setCulling(Renderer::CullingMode cullMode) { m_cullingMode = cullMode; }
	
	// TODO remove this - construct material instead of setting render state directly
	static RenderMaterial getCurrent();
	
private:
	Texture * m_texture;
	bool m_depthTest;
	BlendType m_blendType;
	Layer m_layer;
	TextureStage::WrapMode m_wrapMode;
	int m_depthBias;
	Renderer::CullingMode m_cullingMode;
};

class RenderBatcher {
public:
	RenderBatcher();
	~RenderBatcher();

	void add(const RenderMaterial& mat, const TexturedVertex(&vertices)[3]);
	void add(const RenderMaterial& mat, const TexturedQuad& sprite);

	//! Render all batches
	void render();

	//! Remove all batches
	void clear();

	//! Free all memory pools
	void reset();

	u32 getMemoryUsed() const;

	void initialize();
	void shutdown();

	static RenderBatcher& getInstance();
	
private:
	typedef std::vector<TexturedVertex> VertexBatch;
	typedef std::map<RenderMaterial, VertexBatch> Batches;
	
private:
	Batches m_BatchedSprites;
	CircularVertexBuffer<TexturedVertex> * m_VertexBuffer;
};

#endif // ARX_GRAPHICS_RENDERBATCHER_H
