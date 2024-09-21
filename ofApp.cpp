#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofSetLineWidth(2);

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
	int count = 120;
	for (int k = 0; k < count; k++) {

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;

		glm::vec3 last_location;
		float last_theta;

		float random = ofRandom(1000);
		float deg = ofRandom(360);
		float span = ofRandom(0.01, 0.08);
		float head_size = 2;
		int len = 30;
		for (int i = 0; i < len; i++) {

			auto radius = ofMap(sin((random + ofGetFrameNum() + i) * span), 0, 1, 280, 320);
			auto next_radius = ofMap(sin((random + ofGetFrameNum() + i + 1) * span), 0, 1, 280, 320);

			auto location = glm::vec3(radius * cos((ofGetFrameNum() + i + deg) * DEG_TO_RAD), radius * sin((ofGetFrameNum() + i + deg) * DEG_TO_RAD), 0);
			auto next = glm::vec3(next_radius * cos((ofGetFrameNum() + i + 1 + deg) * DEG_TO_RAD), next_radius * sin((ofGetFrameNum() + i + 1 + deg) * DEG_TO_RAD), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}


		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		mesh.addVertex(last_location);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(head_size * cos(theta), head_size * sin(theta), 0));
			frame.push_back(last_location + glm::vec3(head_size * cos(theta), head_size * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		ofColor color;
		color.setHsb(ofMap(k, 0, count, 0, 255), 180, 255);

		ofSetColor(color);
		ofNoFill();
		ofBeginShape();
		ofVertices(frame);
		std::reverse(right.begin(), right.end());
		ofVertices(right);
		ofVertices(left);
		ofEndShape(true);

		ofSetColor(ofColor(color, 168));
		mesh.draw();
	}

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}