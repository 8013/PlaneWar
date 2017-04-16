#pragma once
#include <vector>
using namespace std;

class GameObject{
protected:
	int speed;
	int window_width = 800;
	int window_height = 950;
	int image_width, image_height;
	CPoint position;
	CImage image;
public:
	GameObject() { position.x = 0; position.y = 0; }
	~GameObject() { ; }

	CPoint getPosition() { return position; }
	int getImageWidth() { return image_width; }
	int getImageHeight() { return image_height; }
	int getSpeed() { return speed; }
	virtual void Draw(CDC *pDC) { ; }
	virtual void Move() { ; }

	bool Contains(int x, int y) {
		if (position.x < x && x < position.x + image_width) {
			if (position.y < y && y < position.y + image_height) {
				return true;
			}
		}
		return false;
	}

};

