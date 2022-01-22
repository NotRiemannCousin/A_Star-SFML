#include "Functions.hpp"
#include <unistd.h>
#include "Button.h"

//~~~~~~~~~~~~~	std	~~~~~~~~~~~~~//


int screenSizeX = 1080, screenSizeY = 2340, ci = 0;

bool pf = false;

std::vector<Node> openList;
std::vector<Node> closedList;

const std::string path; //here goes the image


//~~~~~~~~~~~~~	sf	~~~~~~~~~~~~~//


Vector2i end, init;

Touch touch;

Node current;

Color backgroundColor;

//~~~~~~~~~~~~~	functions	~~~~~~~~~~~~~//


void pathfinder(Vector2i init, Vector2i end, Image *img, Text *t);

int main()
{
	//~~~~~~~~~~~~~	std	~~~~~~~~~~~~~//

	int index = 0;

	//~~~~~~~~~~~~~	sf	~~~~~~~~~~~~~//

	Font font;
	if (!font.loadFromFile("arialceb.ttf"))
	{
		fprintf(stderr, "Couldn't load font\n");
		return EXIT_FAILURE;
	}

	Image img;
	if(!img.loadFromFile(path))
		return EXIT_FAILURE; 

	Texture tex;
	tex.loadFromImage(img);

	Sprite spr;
	spr.setTexture(tex);
	spr.setScale(screenSizeX / (float)img.getSize().x, screenSizeX / (float)img.getSize().x);
	spr.setPosition(0, (screenSizeY - spr.getScale().y * img.getSize().y) / 2);
	
	Text t((char*) "", font, 70);
	t.setFillColor(Color::White);
	t.setPosition(Vector2f(0, spr.getGlobalBounds().top - t.getCharacterSize() * 1.3));
	
	//~~~~~~~~~~~~~	me	~~~~~~~~~~~~~//

	Button reset(font, (char*) "reset", 50, Vector2f(screenSizeX / 2 + 300, spr.getGlobalBounds().height + spr.getPosition().y + 50), 4.3), pause(font, (char*) "pause", 50, Vector2f(screenSizeX / 2 - 300, spr.getGlobalBounds().height + spr.getGlobalBounds().top + 50), 5);

	//////////////////////////////////////////
	//~~~~~~~~~~~~~	loop	~~~~~~~~~~~~~//
	//////////////////////////////////////////

	RenderWindow window(VideoMode(1080, 2340), "sfml");
	while (window.isOpen())
	{
		////////////////////////////////////////////
		//~~~~~~~~~~~~~	events	~~~~~~~~~~~~~//
		////////////////////////////////////////////

		Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::TouchBegan:

				if (spr.getGlobalBounds().contains(Touch::getPosition(0).x, Touch::getPosition(0).y))
				{
					Vector2i pos = (Vector2i(touch.getPosition(0).x / spr.getScale().x, (touch.getPosition(0).y - spr.getGlobalBounds().top) / spr.getScale().y));
					if (index == 0)
					{
						if (compareColor(img.getPixel(pos.x, pos.y), Color(230, 230, 230)))
						{
							img.setPixel(pos.x, pos.y, Color::Blue);
							tex.update(img);
							init = pos;
							index++;
						}
					}
					else if (index == 1)
					{
						if (compareColor(img.getPixel(pos.x, pos.y), Color(230, 230, 230)))
						{
							img.setPixel(pos.x, pos.y, Color(230, 230, 230));
							tex.update(img);
							end = pos;
							index++;

							pf = true;
							current = Node(init, init, sqrt(pow(init.x - end.x, 2)) + sqrt(pow(init.y - end.y, 2)), 0);
						}
					}
				}

				for (int i = 0; i < 3; i++)
				{
					if (pause.box.getGlobalBounds().contains(Vector2f(touch.getPosition(i))))
					{
						pf = !pf;
						break;
					}
					if (reset.box.getGlobalBounds().contains(Vector2f(touch.getPosition(i))))
					{
						img.loadFromFile(path);
						tex.update(img);
						pf = false;
						index = 0;
						openList.clear();
						closedList.clear();

						break;
					}
				}

				/*pause.click(touch, []() mutable -> void{ 
					pf = !pf;
				});*/
				/*pause.click(touch, [](){
					log_f("", log_file);
				});*/
				/*reset.click(touch, [&img, &tex, path, &pf, &index, &openList, &closedList](){
					button::reset(&img, &tex, path, &pf, &index, &openList, &closedList);
				};*/

				break;
			case Event::TouchMoved:
				while (Touch::isDown(1))
				{
				}
				break;
			case Event::TouchEnded:
				break;
			}
		}

		/////////////////////////////////////////////////////
		//~~~~~~~~~~~~~	pathfinder loop	~~~~~~~~~~~~~//
		/////////////////////////////////////////////////////

		if (pf)
		{
			if (current.getNode() != end)
			{
				for (int i = 0; i < 450; i++)
				{
					if (current.getNode() != end)
					{
						pathfinder(init, end, &img, &t);

						//usleep(1000000);
					}
					else
					{
						break;
					}
				}
				t.setString(std::to_string(closedList.size()));
				
				/*for (int i = 0; i < closedList.size(); i++)
				{
					img.setPixel(closedList[i].getNode().x, closedList[i].getNode().y, Color::Red);
				}*/
				
			}
			else
			{
				for (int i = 0; i < openList.size(); i++)
				{
					img.setPixel(openList[i].getNode().x, openList[i].getNode().y, Color::White);
				}
				for (int i = 0; i < closedList.size(); i++)
				{
					img.setPixel(closedList[i].getNode().x, closedList[i].getNode().y, Color::White);
				}
				
				openList.clear();
				int count = 0;
				
				Node *act = &closedList.back();
				while (act->getPointer() != init)
				{
					img.setPixel(act->getNode().x, act->getNode().y, Color::Red);
					act = search(closedList, act->getPointer());
					count++;
				}
				
				img.setPixel(act->getNode().x, act->getNode().y, Color::Red);
				
				img.setPixel(end.x, end.y, Color::Blue);
				img.setPixel(init.x, init.y, Color(150, 5, 5));
				pf = false;
				img.saveToFile("/storage/emulated/0/log/maze.png");
				
				t.setString(std::to_string(closedList.size()) + " - " + std::to_string(count));
			}
			tex.update(img);
		}

		////////////////////////////////////////////////////
		//~~~~~~~~~~~~~	window display	~~~~~~~~~~~~~//
		////////////////////////////////////////////////////

		window.clear(backgroundColor);
		window.draw(spr);
		window.draw(t);
		reset.draw(window);
		pause.draw(window);
		window.display();
	}
}

/////////////////////////////////////////////////////////
//~~~~~~~~~~~~~	pathfinder function	~~~~~~~~~~~~~//
/////////////////////////////////////////////////////////

void pathfinder(Vector2i init, Vector2i end, Image *img, Text *t)
{
	std::vector<Node> li;
	Node *act;

	///////////////////////////////////////////////////
	//~~~~~~~~~~~~~	pixel checker	~~~~~~~~~~~~~//
	///////////////////////////////////////////////////

	{
		Vector2i past, _cur = current.getNode();
		
		//~~~~~~~~~~~~~	perpendicular	~~~~~~~~~~~~~//
		
		
		
		past = _cur + Vector2i(0, 1);
		
		//t->setString(std::is_null_pointer<Node> cur);
		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if (past.y < img->getSize().y)
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 10));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}
	
		past = _cur + Vector2i(0, -1);

		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if (past.y > 0)
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 10));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}
	
		
		past = _cur + Vector2i(1, 0);

		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if (past.x < img->getSize().x)
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 10));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}

		past = _cur + Vector2i(-1, 0);

		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if (past.x > 0)
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 10));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}
		
		
		//~~~~~~~~~~~~~	diagonal	~~~~~~~~~~~~~//

		
		
		past = _cur + Vector2i(1, 1);
		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if ((past.x < img->getSize().x) && (past.y < img->getSize().y))
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 14));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}
		past = _cur + Vector2i(-1, -1);

		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if ((past.x > 0) && (past.y > 0))
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 14));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}

		past = _cur + Vector2i(1, -1);

		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if ((past.x < img->getSize().x) && (past.y > 0))
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 14));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}

		past = _cur + Vector2i(-1, 1);

		if (compareColor(img->getPixel(past.x, past.y), Color(100, 225, 225)))
		{
			act = search(openList, past);
			if (act == NULL)
			{
				if ((past.x > 0) && (past.y < img->getSize().y))
				{
					li.push_back(Node(past, _cur, dist(past, end) * 10, current.getG() + 14));
				}
			}
			else
			{
				if (act->getG() > current.getG() + 10)
				{
					act->update(_cur, current.getG() + 10);
				}
			}
		}
	}

	/////////////////////////////////////////////////
	//~~~~~~~~~~~~~	paint pixel	~~~~~~~~~~~~~//
	/////////////////////////////////////////////////
	
	for (auto k : li)
	{
		openList.push_back(k);

		img->setPixel(k.getNode().x, k.getNode().y, Color(200, 255, 200));
	}
	li.clear();

	if (!closedList.empty())
	{
		img->setPixel(closedList.back().getNode().x, closedList.back().getNode().y, Color(155, 90, 90));
	}

	std::sort(openList.begin(), openList.end(), [end](Node a, Node b) {
		if (a.getF() != b.getF())
		{
			return a.getF() > b.getF();
		}
		else
		{
			return dist(a.getNode(), end) > dist(b.getNode(), end);
		}
	});
	
	img->setPixel(current.getNode().x, current.getNode().y, Color(230, 15, 15));

	current = openList.back();
	openList.pop_back();
	
	closedList.push_back(current);
}
