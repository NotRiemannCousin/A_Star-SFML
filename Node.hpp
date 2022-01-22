#include <SFML/Graphics.hpp>

using namespace sf;

struct Node
{
  private:
	Vector2i node, pointer;
	int h = 0, g = 0, f = 0;

  public:
	Node(Vector2i n, Vector2i p, int _h, int _g)
	{
		node = n;
		pointer = p;
		h = _h;
		g = _g;
		f = g + h;
	};
	Node(){};
	~Node()
	{
		node = Vector2i(0, 0);
		pointer = Vector2i(0, 0);
		h = 0;
		f = 0;
		g = 0;
	};

	Vector2i getNode() { return node; };
	Vector2i getPointer() { return pointer; };
	int getF() { return f; };
	int getG() { return g; };
	int getH() { return h; };

	void update(Vector2i p,int _g)
	{
		pointer = p;
		g = _g;
		f = g + h;
	}

	void restart(Node N)
	{
		f = N.getF();
	};
	
	bool operator>(Node _node)
	{
		return f > _node.f;
	}
	
	bool operator<(Node _node)
	{
		return f < _node.f;
	}
	bool operator==(Node _node)
	{
		return (node == _node.node) && (pointer == _node.pointer) && (f == _node.f);
	}

	void operator=(Node _node)
	{
		node = _node.node;
		pointer = _node.pointer;
		h = _node.h;
		f = _node.f;
		g = _node.g;
	}
};