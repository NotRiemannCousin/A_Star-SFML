#include <math.h>
#include <algorithm>
#include <vector>
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
