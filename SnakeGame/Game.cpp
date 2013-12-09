#include "Game.hpp"

Game::Game() : 
	avgDt(0.0f),
	step(0.001f), 
	acc(0), 
	t(0),
	targetFps(120.0f)
{
	this->rw = new sf::RenderWindow(sf::VideoMode(1600, 900), "SnakeGame", sf::Style::Close, sf::ContextSettings(0, 0, 8));
	rw->setMouseCursorVisible(false);

	sf::Image icon;
	icon.loadFromFile("assets/icon.png");
	rw->setIcon(128, 128, icon.getPixelsPtr());

	if (sf::Shader::isAvailable())
	{
		gaussianShader = new sf::Shader();
		gaussianShader->loadFromFile("assets/gaussian.frag", sf::Shader::Fragment);
	}

	textRenderer = new ShadowTextRenderer(gaussianShader);

	gotoMainMenu();
	//initGame();

}

Game::~Game()
{
	delete rw;
}

void Game::gotoMainMenu()
{
	GameState = GameStates::MainMenu;

	if (!font)
	{
		font = new sf::Font();
		font->loadFromFile("assets/DejaVuSans.ttf");
	}

	if (!titleText)
	{
		titleText = new sf::Text("SnakeGame", *font, 160);
		titleText->setColor(sf::Color(255, 200, 0, 255));
		//titleText->setString("SnakeGame");
		//titleText->setPosition(sf::Vector2f(0.0f, 200.0f));
		titleText->setPosition(sf::Vector2f(rw->getSize().x / 2.0f - titleText->getGlobalBounds().width / 2.0f, 50.0f));
	}

	if (!mainGrid)
	{
		mainGrid = new TextGrid(sf::Vector2i(1, 5), sf::Vector2f(300.0f, 32.0f), gaussianShader);
		mainGrid->GameState = GameStates::MainMenu;
		mainGrid->SetPosition(sf::Vector2f(100.0f, 400.0f));

		mainGrid->SetText(sf::Vector2i(0, 0), "quick game");
		mainGrid->SetText(sf::Vector2i(0, 1), "new game");
		mainGrid->SetText(sf::Vector2i(0, 4), "quit");

		mainGrid->SetAction(sf::Vector2i(0, 0), [this]() 
		{ 
			this->gotoGame();
		});

		mainGrid->SetAction(sf::Vector2i(0, 1), [this]() 
		{ 
			this->gotoGameSetup();
		});

		mainGrid->SetAction(sf::Vector2i(0, 4), [this]() 
		{ 
			rw->close();
		});

		AddObject(mainGrid);
	}

	if (map)
	{
		delete map;
		map = 0;
	}
}

void Game::gotoGameSetup()
{
	GameState = GameStates::GameSetup;

	if (!backGrid)
	{
		backGrid = new TextGrid(sf::Vector2i(1, 1), sf::Vector2f(200.0f, 32.0f), gaussianShader);
		backGrid->GameState = GameStates::GameSetup;
		backGrid->SetPosition(sf::Vector2f(100.0f, 400.0f));

		backGrid->SetText(sf::Vector2i(0, 0), "go back");

		backGrid->SetAction(sf::Vector2i(0, 0), [this]() 
		{ 
			gotoMainMenu();
		});

		AddObject(backGrid);

		levelGrid = new TextGrid(sf::Vector2i(1, 3), sf::Vector2f(200.0f, 32.0f), gaussianShader);
		levelGrid->GameState = GameStates::GameSetup;
		levelGrid->SetPosition(sf::Vector2f(100.0f, 500.0f));

		levelGrid->SetText(sf::Vector2i(0, 0), "Level 1");
		levelGrid->SetText(sf::Vector2i(0, 1), "Level 2");
		levelGrid->SetText(sf::Vector2i(0, 2), "Level 3");

		levelGrid->SetAction(sf::Vector2i(0, 0), [this]() 
		{ 
			this->nextLevel = 0;
		});

		levelGrid->SetAction(sf::Vector2i(0, 1), [this]() 
		{ 
			this->nextLevel = 1;
		});

		levelGrid->SetAction(sf::Vector2i(0, 2), [this]() 
		{ 
			this->nextLevel = 2;
		});

		AddObject(levelGrid);
	}

	if (!colorGrid)
	{
		colorGrid = new ColorGrid(sf::Vector2i(20, 8), sf::Vector2f(32.0f, 32.0f));
		colorGrid->GameState = GameStates::GameSetup;
		colorGrid->SetPosition(sf::Vector2f(308.0f, 500.0f));
		AddObject(colorGrid);
	}

	curSetupGrid = levelGrid;
	curSetupGrid->DrawSelection = true;
	backGrid->DrawSelection = false;
	colorGrid->DrawSelection = false;
}

void Game::gotoPauseMenu()
{
	GameState = GameState | GameStates::PauseMenu;

	if (!pauseGrid)
	{
		pauseGrid = new TextGrid(sf::Vector2i(1, 5), sf::Vector2f(300.0f, 32.0f), gaussianShader);
		pauseGrid->GameState = GameStates::PauseMenu;
		pauseGrid->SetPosition(sf::Vector2f(100.0f, 350.0f));

		pauseGrid->SetText(sf::Vector2i(0, 0), "resume game");
		pauseGrid->SetText(sf::Vector2i(0, 3), "main menu");
		pauseGrid->SetText(sf::Vector2i(0, 4), "quit");

		pauseGrid->SetAction(sf::Vector2i(0, 0), [this]() 
		{ 
			GameState = GameStates::Game;
		});

		pauseGrid->SetAction(sf::Vector2i(0, 3), [this]() 
		{ 
			gotoMainMenu();
		});

		pauseGrid->SetAction(sf::Vector2i(0, 4), [this]() 
		{ 
			rw->close();
		});
	}
	
	//AddObject(pauseGrid);
}

void Game::gotoGame(int level)
{
	GameState = GameStates::Game;

	sf::View view = rw->getView();
	view.setCenter(sf::Vector2f());
	rw->setView(view);

	replacePlayer();

	srand(time(0));

	sf::Vector2f size = sf::Vector2f(1600.0f, 900.0f);

	Map* newMap = 0;

	if (map)
	{
		newMap = map->CloneSettingsOnly();
		delete map;
		map = 0;
	}

	if (newMap)
		map = newMap;
	else
	{
		map = new Map(this, level);
		sf::Color color = ColorGrid::GeneratePrettyColor();
		player->Color = color;
		setMapColors(color);
	}
	//map = new Map(sf::FloatRect(0.0f, 0.0f, 0.0f, 0.0f), this);

	comboRectWidth = 200.0f;

	if (!scoreText)
	{
		scoreText = new sf::Text("", *font, 24);
		scoreText->setPosition(55.0f, 50.0f);
	}

	if (!comboText)
	{
		comboText = new sf::Text("", *font, 24);
		comboText->setPosition(55.0f, 50.0f + 32.0f);
	}

	if (!comboRect)
	{
		comboRect = new sf::RectangleShape(sf::Vector2f(0.0f, 32.0f));
		comboRect->setFillColor(sf::Color(0, 0, 0, 170));
		comboRect->setPosition(50.0f, 50.0f + 32.0f);
	}

	if (!pauseBg)
	{
		pauseBg = new sf::RectangleShape(sf::Vector2f(rw->getSize().x, rw->getSize().y));
		pauseBg->setFillColor(sf::Color(0, 0, 0, 160));
	}
}

void Game::gotoDeath()
{
	GameState = GameState | GameStates::Death;

	if (!retryText)
	{
		retryText = new sf::Text("Press ENTER to restart \t\t Press ESCAPE for main menu", *font, 48);
		retryText->setPosition(sf::Vector2f(rw->getSize().x / 2.0f - retryText->getGlobalBounds().width / 2.0f, 600.0f));
	}
}

void Game::replacePlayer()
{
	sf::Color color = sf::Color::Magenta;
	if (player != 0)
	{
		color = player->Color;
		RemoveObject(static_cast<GameObject*>(player));
	}

	player = new PlayerWorm();
	AddObject(player);

	player->Color = color;
	player->MaxSpeed = 0.3f;
}

void Game::handleKeyDown(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Num1)
		Settings::Quality = GraphicsQuality::High;
	else if (key == sf::Keyboard::Num2)
		Settings::Quality = GraphicsQuality::Medium;
	else if (key == sf::Keyboard::Num3)
		Settings::Quality = GraphicsQuality::Low;

	if ((GameState & GameStates::GameSetup) == GameStates::GameSetup)
	{
		ElementGrid* grid = curSetupGrid;

		if (key == sf::Keyboard::Down)
		{
			bool success = grid->MoveSelection(sf::Vector2i(0, 1));
			if (!success && grid == backGrid)
			{
				backGrid->DrawSelection = false;
				curSetupGrid = levelGrid;
			}
			else if (!success && grid == levelGrid)
			{
				levelGrid->Selection = sf::Vector2i(0, 0);
			}
		}
		else if (key == sf::Keyboard::Up)
		{
			bool success = grid->MoveSelection(sf::Vector2i(0, -1));
			if (!success && grid == colorGrid)
			{
				colorGrid->DrawSelection = false;
				curSetupGrid = backGrid;
				backGrid->DrawSelection = true;
			}
			else if (!success && grid == levelGrid)
			{
				curSetupGrid = backGrid;
				backGrid->DrawSelection = true;
			}
		}
		else if (key == sf::Keyboard::Right)
		{
			bool success = grid->MoveSelection(sf::Vector2i(1, 0));
			if (!success && grid == levelGrid)
			{
				curSetupGrid = colorGrid;
				colorGrid->DrawSelection = true;
			}
		}
		else if (key == sf::Keyboard::Left && curSetupGrid == colorGrid)
		{
			bool success = grid->MoveSelection(sf::Vector2i(-1, 0));
			if (!success && grid == colorGrid)
			{
				curSetupGrid = levelGrid;
				colorGrid->DrawSelection = false;
			}
		}

		levelGrid->RunSelection();

		if (key == sf::Keyboard::Return)
		{
			if (curSetupGrid == backGrid)
			{
				grid->RunSelection();
			}
			else if (curSetupGrid == levelGrid)
			{
				curSetupGrid = colorGrid;
				colorGrid->DrawSelection = true;
			}
			else if (curSetupGrid == colorGrid)
			{
				gotoGame(nextLevel);
				player->Color = colorGrid->GetSelectionColor();
				setMapColors(player->Color);
			}
		}
	}
	else if ((GameState & GameStates::MainMenu) == GameStates::MainMenu || (GameState & GameStates::PauseMenu) == GameStates::PauseMenu)
	{
		TextGrid* grid;
		if ((GameState & GameStates::MainMenu) == GameStates::MainMenu)
			grid = mainGrid;
		else
			grid = pauseGrid;

		if (key == sf::Keyboard::Down)
			grid->MoveSelection(sf::Vector2i(0, 1));

		if (key == sf::Keyboard::Up)
			grid->MoveSelection(sf::Vector2i(0, -1));

		if (key == sf::Keyboard::Return)
			grid->RunSelection();
	}

	if ((GameState & GameStates::Game) == GameStates::Game && (GameState & GameStates::Death) != GameStates::Death)
	{
		if (key == sf::Keyboard::Escape)
		{
			GameState = GameState ^ GameStates::PauseMenu;

			if ((GameState & GameStates::PauseMenu) == GameStates::PauseMenu)
			{
				gotoPauseMenu();
			}
		}
	}

	if ((GameState & GameStates::Death) == GameStates::Death)
	{
		if (key == sf::Keyboard::Escape)
			gotoMainMenu();

		if (key == sf::Keyboard::Return)
			gotoGame();
	}
}

void Game::handleKeyUp(sf::Keyboard::Key key)
{
	
}

void Game::setMapColors(sf::Color base)
{
	float h = ColorGrid::RGBToHSV(base);

	map->SetObstacleColor(ColorGrid::HSVToRGB(h + 180.0f, 0.6f, 1.0f));
	map->SetBackgroundColor(ColorGrid::HSVToRGB(h + 180.0f, 0.6f, 0.2f));

	map->SetFoodColor(ColorGrid::HSVToRGB(h + 90.0f, 1.0f, 1.0f));
	map->SetFoodLayerColor(ColorGrid::HSVToRGB(h + 155.0f, 1.0f, 1.0f));
}

void Game::Run()
{
	bool draw = false;

	while (rw->isOpen())
	{
		sf::Event event;
		while (rw->pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					rw->close();
					break;
				}
				case sf::Event::KeyPressed:
				{
					handleKeyDown(event.key.code);
					break;
				}
				case sf::Event::KeyReleased:
				{
					handleKeyUp(event.key.code);
					break;
				}
			}
		}

		float dt = clock.getElapsedTime().asSeconds();
		//float dt = 0.0001f;
		clock.restart();

		avgDt = avgDt * 0.999f + dt * 0.001f;

		acc += dt;

		draw = false;

		while (acc >= step)
		{
			acc -= step;
			t += step;
			frame++;

			Tick(step, frame);

			draw = true;
		}

		Update(dt);

		if (draw)
			Draw(*rw, sf::RenderStates(sf::RenderStates::Default));

		if (dt * 2.0f < 1.0f / targetFps)
			sf::sleep(sf::seconds(1.0f / targetFps - dt * 2.0f));
	}
}

void Game::Update(float dt)
{
	/*
	for (auto& vec : objects)
	{
		for (auto& obj : vec.second)
		{
			if ((obj->GameState & GameState) == GameState)
				obj->Update(dt);
		}
	}
	*/
};

void Game::Tick(float step, int frame)
{
	for (auto& vec : objects)
	{
		for (auto& obj : vec.second)
		{
			if (GameState == obj->GameState)
				obj->Tick(step, frame);
		}
	}

	if ((GameState & GameStates::Game) != GameStates::Game) return;

	const float collisionStep = 0.5f;

	auto flat = GetFlat();
	for (int i = 0; i < flat.size(); i++)
	{
		Worm* worm = dynamic_cast<Worm*>(flat[i]);

		if (worm != 0)
		{
			GameObject* hit = 0;
			CollisionInfo collides = map->Collides(*worm);
			hit = map;

			if (!collides.Collides)
			{
				std::vector<sf::Vector2f>& points = worm->GetCollisionPoints();

				sf::Vector2f* last = 0;

				for (int k = 0; k < points.size(); k++)
				{
					sf::Vector2f* cur = &points[k];

					if (last != 0)
					{
						//if (thor::length(*cur - curHead) < worm->Thickness) continue;
						//if (thor::length(*last - curHead) < worm->Thickness) continue;

						sf::Vector2f test = *last;

						sf::Vector2f sub = *cur - *last;
						if (sub == sf::Vector2f()) continue;

						sf::Vector2f dir = thor::unitVector(*cur - *last);
						sf::Vector2f whileDir;

						do
						{
							collides = worm->Collides(CircleObstacle(test, worm->Thickness / 2.0f));

							if (collides.Collides) break;

							const float step = collisionStep;
							test += dir * step;

							sf::Vector2f sub = *cur - test;
							if (sub == sf::Vector2f()) break;

							whileDir = thor::unitVector(sub);
						} while (thor::dotProduct(dir, whileDir) >= 0.99f);

						if (collides.Collides)
						{
							hit = worm;
							break;
						}
					}

					last = cur;
				}
			}

			if (collides.Collides)
			{
				bool die = player->OnCollision(collides, hit);

				if (die)
				{
					gotoDeath();
					//replacePlayer();
				}
			}
		}
	}
	

	int deleted = 0;
	for (auto it = map->Foods.begin(); it != map->Foods.end();)
	{
		Food* obj = *it;
		CollisionInfo info = player->Collides(*obj);

		if (info.Collides)
		{
			if ((*it)->RemoveLayer())
			{
				player->OnCollision(info, obj);
			}
			else
			{
				player->OnEat((*it)->Points);

				map->MaxLayers = player->Score > 0 ? (float)player->Score / 1000.0f : 0;

				RemoveObject(*it);

				it = map->Foods.erase(it);

				deleted++;
			}
		}
		else
		{
			it++;
		}
		
	}

	for (int i = 0; i < deleted; i++)
	{
		map->OnEat();
	}


	std::stringstream ss;
	ss << "Combo   " << player->Combo << "x";
	comboText->setString(ss.str());

	comboRect->setSize(sf::Vector2f(comboRectWidth * (player->ComboLeft / player->ComboTime), comboRect->getSize().y));

	ss.str("");
	ss << "Score     " << player->Score;
	scoreText->setString(ss.str());

	/*
	float camX = 0.0f;
	float camY = 0.0f;

	if (rw->getSize().x < map->Area.width)
	{
		camX = std::max(map->Area.left + rw->getSize().x / 2.0f, player->GetHeadPos().x);
		camX = std::min(map->Area.left + map->Area.width - rw->getSize().x / 2.0f, camX);
	}

	if (rw->getSize().y < map->Area.height)
	{
		camY = std::max(map->Area.top + rw->getSize().y / 2.0f, player->GetHeadPos().y);
		camY = std::min(map->Area.top + map->Area.height - rw->getSize().y / 2.0f, camY);
	}

	sf::View view = rw->getView();
	view.setCenter(sf::Vector2f(camX, camY));
	rw->setView(view);
	*/

	sf::View view = rw->getView();
	view.setCenter(player->GetHeadPos());
	rw->setView(view);
};

void Game::Draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	sf::Color clear = sf::Color(0, 100, 255, 255);

	if ((GameState & GameStates::Game) == GameStates::Game)
	{
		if (map->Inverse)
			clear = map->ObstacleColor;
		else
			clear = map->BackgroundColor;
	}

	rw->clear(clear);

	sf::Vector2f topLeft = target.mapPixelToCoords(sf::Vector2i(), target.getView());
	sf::Vector2f botRight = target.mapPixelToCoords(sf::Vector2i(target.getSize().x, target.getSize().y), target.getView());
	sf::FloatRect visibleRect = sf::FloatRect(topLeft.x, topLeft.y, (botRight - topLeft).x, (botRight - topLeft).y);

	for (auto& vec : objects)
	{
		for (auto& obj : vec.second)
		{
			if ((obj->GameState & GameState) == obj->GameState)
			{
				Circle* circle = dynamic_cast<Circle*>(obj);
			
				if (circle && !visibleRect.intersects(circle->GetRect())) continue;

				obj->Draw(target, states);
			}
		}
	}

	sf::View oldView = rw->getView();
	rw->setView(rw->getDefaultView());

	if ((GameState & GameStates::Game) == GameStates::Game)
	{
		// UI drawing

		//bool high = avgDt < 1.0f / targetFps;
		bool high = Settings::Quality == GraphicsQuality::High;

		textRenderer->DrawDropShadowText(target, *scoreText, high);
		//target.draw(*scoreText);

		target.draw(*comboRect);
		textRenderer->DrawDropShadowText(target, *comboText, high);
		//target.draw(*comboText);
	}

	if ((GameState & GameStates::PauseMenu) == GameStates::PauseMenu || (GameState & GameStates::Death) == GameStates::Death)
	{
		target.draw(*pauseBg, states);
	}

	if ((GameState & GameStates::PauseMenu) == GameStates::PauseMenu)
	{
		pauseGrid->Draw(target, states);
	}

	if ((GameState & GameStates::Death) == GameStates::Death)
	{
		textRenderer->DrawDropShadowText(target, *retryText);
	}

	if ((GameState & GameStates::MainMenu) == GameStates::MainMenu || (GameState & GameStates::GameSetup) == GameStates::GameSetup)
	{
		bool high = Settings::Quality <= GraphicsQuality::Medium;
		textRenderer->DrawDropShadowText(target, *titleText, high);
	}

	rw->setView(oldView);
	
	rw->display();
}


