#pragma once
#include <vector>
#include <DirectXCollision.h>

#include "IDrawable.hpp"

constexpr auto QT_SIZE = 120;


class QuadTree {
	struct Instance {
		IDrawable* pdrawable;
		bool pickedForRendering;

		Instance(IDrawable* d);
	};

	struct QuadTreeNode {
		QuadTreeNode* subQuad[4];
		std::vector<Instance*> pinstance;
		QuadTreeNode();
	};

	QuadTreeNode* root;
	std::vector<Instance*> drawables;

	void QuadTreeInsert(QuadTreeNode* node, float size, DirectX::XMFLOAT3 center, Instance* i);
	void DestroyQuadTree(QuadTreeNode* root);
	void ViewFrustumCulling(QuadTreeNode* node, float size, DirectX::XMFLOAT3 center, BoundingFrustum frustum, std::vector<IDrawable*>& renderQueue);
	
public:
	QuadTree();
	~QuadTree();

	void Add(IDrawable* drawable);
	std::vector<IDrawable*> GetRenderQueue(BoundingFrustum frustum);
	std::vector<IDrawable*> GetRenderQueue();
};
