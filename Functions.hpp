#include <math.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include "Node.hpp"

int dist(Vector2i v1, Vector2i v2)
{
	return abs(v1.x - v2.x) + abs(v1.y - v2.y);
}

bool compareColor(Color c1, Color c2)
{
	return (c1.r + c1.g + c1.b) > (c2.r + c2.g + c2.b);
}

Node *search(std::vector<Node> &N, Vector2i s)
{
	for (int k = 0; k < N.size(); k++)
	{
		if (N[k].getNode() == s)
		{
			return &N[k];
		}
	}
	return NULL;
}

bool map(std::vector<Node> N, Vector2i &pos)
{
	for (auto k : N)
	{
		if (k.getNode() == pos)
		{
			return 1;
		}
	}
	return 0;
}

Node min(std::vector<Node> N)
{
	int m = INT_MAX;
	Node n;
	for (auto k : N)
	{
		if (k.getF() < m)
		{
			m = k.getF();
			n = k;
			//*c = Color::White;
		}
	}
	//N->erase(std::vector<Node>::allocator_type(*n));
	return n;
}

namespace button
{
void *reset(Image *img, Texture *tex, std::string path, bool *pf, int *index, std::vector<Node> *openList, std::vector<Node> *closedList)
{
	img->loadFromFile(path);
	tex->update(*img);
	*pf = false;
	*index = 0;
	openList->clear();
	closedList->clear();
}
}