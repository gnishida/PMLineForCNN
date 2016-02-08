#include "PMTree2D.h"
#include "Utils.h"
#include <sstream>
#include <iostream>
#include "RenderManager.h"
#include "Camera.h"

namespace pmtree {

	const float M_PI = 3.1415926535f;
	const int NUM_SEGMENTS = 30;

	TreeNode::TreeNode(boost::shared_ptr<TreeNode> parent, int level, int index) {
		this->parent = parent;
		this->index = index;

		for (int k = 0; k < NUM_SEGMENTS - 1; ++k) {
			curvesV.push_back(0);
		}
	}

	void TreeNode::generateRandom() {
		for (int k = 0; k < NUM_SEGMENTS - 1; ++k) {
			curvesV[k] = utils::uniform(-20, 20);
		}
	}

	std::string TreeNode::to_string() {
		std::stringstream ss;
		ss << "Level: ";
		return ss.str();
	}

	PMTree2D::PMTree2D() {
		root = boost::shared_ptr<TreeNode>(new TreeNode(NULL, 0, 0));
	}

	void PMTree2D::generateRandom() {
		root = boost::shared_ptr<TreeNode>(new TreeNode(NULL, 0, 0));
		root->generateRandom();
	}

	bool PMTree2D::generateGeometry(RenderManager* renderManager) {
		bool underground = false;

		glm::mat4 modelMat;
		float length = 0.3f;
		float width = 0.3f;

		std::vector<Vertex> vertices;
		if (generateGeometry(renderManager, modelMat, length, width, root, vertices)) underground = true;
		renderManager->addObject("tree", "", vertices, true);

		return underground;
	}

	bool PMTree2D::generateGeometry(RenderManager* renderManager, const glm::mat4& modelMat, float segment_length, float segment_width, boost::shared_ptr<TreeNode>& node, std::vector<Vertex>& vertices) {
		bool underground = false;

		glm::mat4 mat = modelMat;

		float w1 = segment_width;
		glm::vec4 p0(-w1 * 0.5, 0, 0, 1);
		glm::vec4 p1(w1 * 0.5, 0, 0, 1);
		p0 = mat * p0;
		p1 = mat * p1;

		for (int k = 0; k < NUM_SEGMENTS; ++k) {
			w1 = segment_width * (NUM_SEGMENTS - k) / NUM_SEGMENTS;
			float w2 = segment_width * (NUM_SEGMENTS - k - 1) / NUM_SEGMENTS;
			glm::vec4 p2(w2 * 0.5, segment_length, 0, 1);
			glm::vec4 p3(-w2 * 0.5, segment_length, 0, 1);
			p2 = mat * p2;
			p3 = mat * p3;

			if (p2.y < 0 || p3.y < 0) underground = true;

			std::vector<glm::vec3> pts(4);
			pts[0] = glm::vec3(p0);
			pts[1] = glm::vec3(p1);
			pts[2] = glm::vec3(p2);
			pts[3] = glm::vec3(p3);
			glutils::drawPolygon(pts, glm::vec4(0, 0, 0, 1), vertices);

			p0 = p3;
			p1 = p2;

			// 最後のsegmentなら、終了
			if (k >= NUM_SEGMENTS - 1) break;

			mat = glm::translate(mat, glm::vec3(0, segment_length, 0));

			mat = glm::rotate(mat, node->curvesV[k] / 180.0f * M_PI, glm::vec3(0, 0, 1));
		}

		return underground;
	}

	std::string PMTree2D::to_string() {
		std::stringstream ss;

		for (int i = 0; i < root->curvesV.size(); ++i) {
			if (i > 0) {
				ss << ",";
			}
			ss << (root->curvesV[i] + 20) / 40.0f;
		}

		return ss.str();
	}

}