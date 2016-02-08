#pragma once

#include <glm/glm.hpp>
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>
#include "Vertex.h"
#include <opencv2/opencv.hpp>

class RenderManager;
class Camera;

namespace pmtree {
	class TreeNode {
	public:
		std::vector<float> curvesV;

		int index;
		boost::shared_ptr<TreeNode> parent;

	public:
		TreeNode(boost::shared_ptr<TreeNode> parent, int level, int index);
		void generateRandom();
		std::string to_string();
	};

	class PMTree2D {
	public:
		boost::shared_ptr<TreeNode> root;

	public:
		PMTree2D();

		void generateRandom();
		bool generateGeometry(RenderManager* renderManager);
		std::string to_string();

	private:
		bool generateGeometry(RenderManager* renderManager, const glm::mat4& modelMat, float segment_length, float segment_width, boost::shared_ptr<TreeNode>& node, std::vector<Vertex>& vertices);
	};

}