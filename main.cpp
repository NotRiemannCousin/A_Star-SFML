#include "functions.hpp"
#include <unistd.h>
#include "/storage/emulated/0/my-project/src/button.h"

//~~~~~~~~~~~~~	std	~~~~~~~~~~~~~//

int screensizex = 1080, screensizey = 2340;

bool pf = false;

std::vector<node> openlist;
std::vector<node> closedlist;

//const std::string path = "/storage/emulated/0/download/20 by 20 orthogonal maze.png";
//const std::string path = "/storage/emulated/0/dcim/pixel studio/maze_02.png";
//const std::string path = "/storage/emulated/0/dcim/camera/maze_01.jpg";
//const std::string path = "/storage/emulated/0/pictures/maze - 03.png";
//const std::string path = "/storage/emulated/0/pictures/maze - 03 b.png";
const std::string path = "/storage/emulated/0/pictures/200x200 sigma maze - 04.png";

//~~~~~~~~~~~~~	sf	~~~~~~~~~~~~~//

vector2i end, init;

node current;

//~~~~~~~~~~~~~	functions	~~~~~~~~~~~~~//

void pathfinder(vector2i init, vector2i end, image *img, text *t);

int main()
{
	//~~~~~~~~~~~~~	std	~~~~~~~~~~~~~//

	int index = 0;

	//~~~~~~~~~~~~~	sf	~~~~~~~~~~~~~//

	font font;
	if (!font.loadfromfile("/storage/emulated/0/my-project/src/arialceb.ttf"))
	{
		fprintf(stderr, "couldn't load the font\n");
		return exit_failure;
	}

	image img;
	if (!img.loadfromfile(path))
	{
		
		fprintf(stderr, "couldn't load the image\n");
		return exit_failure;
	}

	texture tex;
	tex.loadfromimage(img);

	sprite spr;
	spr.settexture(tex);
	spr.setscale(screensizex / (float)img.getsize().x, screensizex / (float)img.getsize().x);
	spr.setposition(0, (screensizey - spr.getscale().y * img.getsize().y) / 2);

	text t((char *)"", font, 70);
	t.setfillcolor(color::white);
	t.setposition(vector2f(0, spr.getglobalbounds().top - t.getcharactersize() * 1.3));

	//~~~~~~~~~~~~~	me	~~~~~~~~~~~~~//

	button reset(font, (char *)"reset", 50, vector2f(screensizex / 2 + 300, spr.getglobalbounds().height + spr.getposition().y + 50), 4.3), pause(font, (char *)"pause", 50, vector2f(screensizex / 2 - 300, spr.getglobalbounds().height + spr.getglobalbounds().top + 50), 5);

	closedlist.reserve(img.getsize().x * img.getsize().y);
	openlist.reserve(img.getsize().x * img.getsize().y);

	//////////////////////////////////////////
	//~~~~~~~~~~~~~	loop	~~~~~~~~~~~~~//
	//////////////////////////////////////////

	renderwindow window(videomode(1080, 2340), "sfml");
	while (window.isopen())
	{
		////////////////////////////////////////////
		//~~~~~~~~~~~~~	events	~~~~~~~~~~~~~//
		////////////////////////////////////////////

		event e;
		while (window.pollevent(e))
		{
			switch (e.type)
			{
			case event::closed:
				window.close();
				break;
			case event::touchbegan:

				if (spr.getglobalbounds().contains(touch::getposition(0).x, touch::getposition(0).y))
				{
					vector2i pos = (vector2i(touch::getposition(0).x / spr.getscale().x, (touch::getposition(0).y - spr.getglobalbounds().top) / spr.getscale().y));
					if (index == 0)
					{
						if (comparecolor(img.getpixel(pos.x, pos.y), color(230, 230, 230)))
						{
							img.setpixel(pos.x, pos.y, color::blue);
							tex.update(img);
							init = pos;
							index++;
						}
					}
					else if (index == 1)
					{
						if (comparecolor(img.getpixel(pos.x, pos.y), color(230, 230, 230)))
						{
							img.setpixel(pos.x, pos.y, color(230, 230, 230));
							tex.update(img);
							end = pos;
							index++;

							pf = true;

							current(init, null, dist(init, end), 0);

							openlist.push_back(current);
						}
					}
				}

				for (int i = 0; i < 3; i++)
				{
					if (pause.box.getglobalbounds().contains(vector2f(touch::getposition(i))))
					{
						pf = !pf;
						break;
					}
					if (reset.box.getglobalbounds().contains(vector2f(touch::getposition(i))))
					{
						if (index == 0)
							window.close();

						img.loadfromfile(path);
						tex.update(img);
						pf = false;
						index = 0;
						openlist.clear();
						closedlist.clear();

						break;
					}
				}
				break;
			}
		}

		/////////////////////////////////////////////////////
		//~~~~~~~~~~~~~	pathfinder loop	~~~~~~~~~~~~~//
		/////////////////////////////////////////////////////

		if (pf)
		{
			if (current.getnode() != end)
			{
				for (int i = 0; i < 1050; i++)
				//while(current.getnode() != end)
				{
					if (current.getnode() != end)
						pathfinder(init, end, &img, &t);
					else
						break;

					//usleep(10000);

					if (openlist.empty()){
						pf = false;
				img.setpixel(init.x, init.y, color::blue);
				img.setpixel(end.x, end.y, color(150, 5, 5));
				break;
					}
				}
				t.setstring(std::to_string(current.getnode().x) + ", " + std::to_string(current.getnode().y));
			}
			else
			{
				for (int i = 0; i < openlist.size(); i++)
				{
					img.setpixel(openlist[i].getnode().x, openlist[i].getnode().y, color::white);
				}
				for (int i = 0; i < closedlist.size(); i++)
				{
					img.setpixel(closedlist[i].getnode().x, closedlist[i].getnode().y, color::white);
				}
				openlist.clear();
				int count = 0;

				node *act = &closedlist.back();
				while (act->getpointer() != null)
				{
					img.setpixel(act->getnode().x, act->getnode().y, color::red);
					act = act->getpointer();
					count++;
				}
				
				img.setpixel(init.x, init.y, color::blue);
				img.setpixel(end.x, end.y, color(150, 5, 5));
				pf = false;
				img.savetofile("/storage/emulated/0/log/maze.png");

				t.setstring(std::to_string(closedlist.size()) + " - " + std::to_string(count));
				closedlist.clear();
			}
			tex.update(img);
		}

		////////////////////////////////////////////////////
		//~~~~~~~~~~~~~	window display	~~~~~~~~~~~~~//
		////////////////////////////////////////////////////

		window.clear(color(25, 25, 25));
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

void pathfinder(vector2i init, vector2i end, image *img, text *t)
{
	if (openlist.empty() && !closedlist.empty())
		return;

	std::vector<node> li;
	li.reserve(8);
	node *act;

	current = openlist.back();
	openlist.pop_back();

	closedlist.emplace_back(current);

	///////////////////////////////////////////////////
	//~~~~~~~~~~~~~	pixel checker	~~~~~~~~~~~~~//
	///////////////////////////////////////////////////

	{
		vector2i past, _cur = current.getnode();

		//~~~~~~~~~~~~~	perpendicular	~~~~~~~~~~~~~//

		past = _cur + vector2i(0, 1);

		if (past.y < img->getsize().y)
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 10);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}

		past = _cur + vector2i(0, -1);

		if (past.y >= 0)
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 10);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}

		past = _cur + vector2i(1, 0);

		if (past.x < img->getsize().x)
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 10);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}

		past = _cur + vector2i(-1, 0);

		if (past.x > -1)
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 10);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}

		//~~~~~~~~~~~~~	diagonal	~~~~~~~~~~~~~//

		past = _cur + vector2i(1, 1);

		if ((past.x < img->getsize().x) && (past.y < img->getsize().y))
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 14);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}
		past = _cur + vector2i(-1, -1);

		if ((past.x > -1) && (past.y > -1))
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 14);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}

		past = _cur + vector2i(1, -1);

		if ((past.x < img->getsize().x) && (past.y > -1))
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 14);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}

		past = _cur + vector2i(-1, 1);

		if ((past.x > -1) && (past.y < img->getsize().y))
		{
			if (comparecolor(img->getpixel(past.x, past.y), color(100, 150, 150)))
			{
				act = search(openlist, past);
				if (act == null)
				{
					li.emplace_back(past, closedlist.back(), dist(past, end) * 10, current.getg() + 14);
				}
				else
				{
					if (act->getg() > current.getg() + 10)
					{
						act->update(closedlist.back(), current.getg() + 10);
					}
				}
			}
		}
	}

	/////////////////////////////////////////////////
	//~~~~~~~~~~~~~	paint pixel	~~~~~~~~~~~~~//
	/////////////////////////////////////////////////

	for (node &k : li)
	{
		openlist.emplace_back(k);
		img->setpixel(k.getnode().x, k.getnode().y, color(200, 255, 200));
	}
	li.clear();
	
	img->setpixel(closedlist.back().getnode().x, closedlist.back().getnode().y, color(155, 90, 90));

	std::sort(openlist.begin(), openlist.end(), [](node &a, node &b) {
		if (a.getf() != b.getf())
			return a.getf() > b.getf();
		else
			return a.getg() > b.getg();
	});

	img->setpixel(current.getnode().x, current.getnode().y, color(230, 5, 5));
}
