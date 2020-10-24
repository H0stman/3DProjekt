#include "QuadTree.hpp"

QuadTree::QuadTree() : root(nullptr)
{
	root = new QuadTreeNode();
}

QuadTree::~QuadTree()
{
	DestroyQuadTree(root);
	for(Instance* i : drawables) {
		i->pdrawable == nullptr;		// the object is safeguarded in the models vector in Engine
		delete i;
	}
}

QuadTree::Instance::Instance(IDrawable* d) {
	pdrawable = d;
	pickedForRendering = false;
}

QuadTree::QuadTreeNode::QuadTreeNode() : subQuad() { }

void QuadTree::Add(IDrawable* drawable) {
	Instance* i = new Instance(drawable);
	drawables.push_back(i);
	QuadTreeInsert(root, QT_SIZE, { 0.0, 0.0, 0.0 }, i);
}

void QuadTree::Add(std::vector<IDrawable*> drawables)
{
	for (IDrawable* d : drawables) {
		Add(d);
	}
}

void QuadTree::DestroyQuadTree(QuadTreeNode* node) {
	for (int i = 0; i < 4; ++i)
		if (node->subQuad[i] != nullptr) DestroyQuadTree(node->subQuad[i]);
	delete node;
}

void QuadTree::QuadTreeInsert(QuadTreeNode* node, float size, DirectX::XMFLOAT3 center, Instance* i) {
	if (size > 1) {
		float halfSize = size / 2;

		BoundingBox box = i->pdrawable->GetBoundingBox();
		int iTestXYplane = box.Intersects(XMVectorSet(0.0, 0.0, 1.0, -center.z)); 
		int iTestYZplane = box.Intersects(XMVectorSet(1.0, 0.0, 0.0, -center.x)); 

		if (iTestXYplane <= INTERSECTING && iTestYZplane <= INTERSECTING) {
			if (node->subQuad[0] == nullptr) node->subQuad[0] = new	QuadTreeNode();
			QuadTreeInsert(node->subQuad[0], halfSize, { center.x + halfSize, center.y, center.z + halfSize }, i);
		}

		if (iTestXYplane <= INTERSECTING && iTestYZplane >= INTERSECTING) {
			if (node->subQuad[1] == nullptr) node->subQuad[1] = new	QuadTreeNode();
			QuadTreeInsert(node->subQuad[1], halfSize, { center.x + halfSize, center.y, center.z - halfSize }, i);
		}

		if (iTestXYplane >= INTERSECTING && iTestYZplane >= INTERSECTING) {
			if (node->subQuad[2] == nullptr) node->subQuad[2] = new	QuadTreeNode();
			QuadTreeInsert(node->subQuad[2], halfSize, { center.x - halfSize, center.y, center.z - halfSize }, i);
		}

		if (iTestXYplane >= INTERSECTING && iTestYZplane <= INTERSECTING) {
			if (node->subQuad[3] == nullptr) node->subQuad[3] = new	QuadTreeNode();
			QuadTreeInsert(node->subQuad[3], halfSize, { center.x - halfSize, center.y, center.z + halfSize }, i);
		}
	}
	else {
		node->pinstance.push_back(i);
	}
}

std::vector<IDrawable*> QuadTree::GetRenderQueue(BoundingFrustum frustum) {
	std::vector<IDrawable*> renderqueue;
	ViewFrustumCulling(root, QT_SIZE, { 0.0, 0.0, 0.0 }, frustum, renderqueue);
	for(Instance* i : drawables)
		i->pickedForRendering = false;
	return renderqueue;
}

std::vector<IDrawable*> QuadTree::GetRenderQueue() {
	std::vector<IDrawable*> renderqueue;
	for (Instance* i : drawables) {
		renderqueue.push_back(i->pdrawable);
	}
	return renderqueue;
}

void QuadTree::ViewFrustumCulling(QuadTreeNode* node, float size, DirectX::XMFLOAT3 center, BoundingFrustum frustum, std::vector<IDrawable*>& renderQueue) {
	if (node == nullptr) {
		return;
	}
	if (size > 1) {
		float halfSize = size / 2;

		int iTestXYplane = frustum.Intersects(XMVectorSet(0.0, 0.0, 1.0, -center.z)); 
		int iTestYZplane = frustum.Intersects(XMVectorSet(1.0, 0.0, 0.0, -center.x)); 

		if (iTestXYplane <= INTERSECTING && iTestYZplane <= INTERSECTING) {
			if (node->subQuad[0] == nullptr) node->subQuad[0] = new	QuadTreeNode();
			ViewFrustumCulling(node->subQuad[0], halfSize, { center.x + halfSize, center.y, center.z + halfSize },frustum, renderQueue);
		}

		if (iTestXYplane <= INTERSECTING && iTestYZplane >= INTERSECTING) {
			
			if (node->subQuad[1] == nullptr) node->subQuad[1] = new	QuadTreeNode();
			ViewFrustumCulling(node->subQuad[1], halfSize, { center.x + halfSize, center.y, center.z - halfSize },frustum, renderQueue);
		}

		if (iTestXYplane >= INTERSECTING && iTestYZplane >= INTERSECTING) {
			if (node->subQuad[2] == nullptr) node->subQuad[2] = new	QuadTreeNode();
			ViewFrustumCulling(node->subQuad[2], halfSize, { center.x - halfSize, center.y, center.z - halfSize },frustum, renderQueue);
		}

		if (iTestXYplane >= INTERSECTING && iTestYZplane <= INTERSECTING) {
			if (node->subQuad[3] == nullptr) node->subQuad[3] = new	QuadTreeNode();
			ViewFrustumCulling(node->subQuad[3], halfSize, { center.x - halfSize, center.y, center.z + halfSize },frustum, renderQueue);
		}
	}
	else {
		for (Instance* i : node->pinstance) {
			if (!i->pickedForRendering) {
				i->pickedForRendering = true;
				OutputDebugString(L"Leaf\n");
				if (frustum.Contains(i->pdrawable->GetBoundingBox()) > DISJOINT) {
					renderQueue.push_back(i->pdrawable);
					OutputDebugStringA(i->pdrawable->GetName().c_str());
					OutputDebugStringA("\n");
				}
			}
		}
	}
}

