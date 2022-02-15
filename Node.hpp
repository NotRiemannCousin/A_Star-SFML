#include <SFML/Graphics.hpp>

using namespace sf;

struct Node
{
  private:
	Vector2i node;
	Node *pointer;
	int h = 0, g = 0, f = 0;

  public:
	Node(Vector2i n, Node &p, int _h, int _g)
	{
		pointer = (&p);
		node = n;
		h = _h;
		g = _g;
		f = g + h;
	};
	Node(Vector2i n, Node *p, int _h, int _g)
	{
		node = n;
		pointer = (p);
		h = _h;
		g = _g;
		f = g + h;
	};
	Node(){};
	~Node(){};

	Vector2i getNode() { return node; };
	Node *getPointer() { return pointer; };
	int getF() { return f; };
	int getG() { return g; };
	int getH() { return h; };

	void update(Node &p, int _g)
	{
		pointer = &p;
		g = _g;
		f = g + h;
	}
	void operator()(Vector2i n, Node *p, int _h, int _g)
	{
		pointer = (p);
		node = n;
		h = _h;
		g = _g;
		f = g + h;
	};
};
