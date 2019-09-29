#include "./Menu.hpp"

Menu::Menu() {}

Menu::~Menu() {}

void Menu::init(Renderer &renderer, std::vector<MenuItem> &items)
{
	this->_renderer = &renderer;
	this->_shader = renderer.shader();
	this->_camera = renderer.camera();

	this->menuItems = items;
}

void Menu::drawMenuText(sf::RenderWindow &window, MenuItem &item)
{
	std::string text = item.text;
	sf::Vector3i color = sf::Vector3i(10, 20, 50);
	sf::Vector2f startLocation = sf::Vector2f(-0.5f, item.offset / 4.0f);
	float scale = 0.15f + item.selected * 0.05f;

	if (item.selected)
		color = sf::Vector3i(250, 250, 250);

	this->_renderer->writeLine(window, item.text, color, startLocation, scale);
}

MenuAction Menu::handleInput(sf::RenderWindow &window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case event.KeyPressed:
		{
			sf::Keyboard::Key key = event.key.code;
			switch (key)
			{
				case sf::Keyboard::Up:
					this->menuLeft();
					break;
				case sf::Keyboard::Down:
					this->menuRight();
					break;
				case sf::Keyboard::Return:
					return this->select();
					break;
				case sf::Keyboard::Space:
					return this->select();
					break;
			}
			break;
		}
		default:
			return MenuAction::Nothing;
			break;
		}
	}

	return MenuAction::Nothing;
}

void Menu::menuLeft()
{
	if (this->menuItems[0].selected)
		return;

	for (int i = 0; i < this->menuItems.size() - 1; i++)
	{
		if (this->menuItems[i + 1].selected)
		{
			this->menuItems[i].selected = true;
			this->menuItems[i + 1].selected = false;
			break;
		}
	}
}

void Menu::menuRight()
{
	for (int i = 0; i < this->menuItems.size() - 1; i++)
	{
		if (this->menuItems[i].selected)
		{
			this->menuItems[i].selected = false;
			this->menuItems[i + 1].selected = true;
			break;
		}
	}
}

MenuAction Menu::select()
{
	for (int i = 0; i < this->menuItems.size(); i++)
	{
		if (this->menuItems[i].selected)
			return this->menuItems[i].option;
	}

	return MenuAction::Nothing;
}

MenuAction Menu::render(sf::RenderWindow &window)
{
	sf::Vector2u size = window.getSize();
	glViewport(0, 0, size.x, size.y);
	glClearColor(0.3f, 0.3f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (MenuItem item : this->menuItems)
		this->drawMenuText(window, item);

	window.display();

	return this->handleInput(window);
}
