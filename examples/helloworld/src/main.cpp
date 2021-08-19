#include <pxl/engine.h>
#include <pxl/utils/scene.h>
#include <pxl/graphics/batch.h>
#include <pxl/utils/input_binding.h>
#include <pxl/utils/filestream.h>

class HelloWorldComponent : public pxl::Component, public pxl::IDrawable, public pxl::IUpdateable
{
public:
	pxl::TextureRef texture;
	void update() override
	{
		auto mousePosition = pxl::mouse::drawPosition();
		auto windowPosition = pxl::platform::position();
		auto windowSize = pxl::platform::drawSize();
		if (mousePosition.x > windowPosition.x &&
			mousePosition.y > windowPosition.y &&
			mousePosition.x < windowPosition.x + windowSize.x &&
			mousePosition.y < windowPosition.y + windowSize.y) {
			entity()->position = mousePosition - windowPosition - pxl::Vec2(texture->width() / 2.0f, texture->height() / 2.0f);
		}
	}
	void draw(pxl::Batch& batch) override
	{
		if (texture == nullptr) return;
		batch.texture(texture, entity()->position, pxl::Color::white);
	}
};

class HelloWorldScene : public pxl::Scene
{
public:
	HelloWorldScene() : pxl::Scene("Hello World Scene"){}
	void begin() override
	{
		pxl::Scene::begin();


		// entity holds components
		auto entity = createEntity(pxl::Vec2(0, 0));

		// component does things (in this case draws texture)
		auto component = entity->add(HelloWorldComponent());
		component->texture = pxl::Texture::create(pxl::path::combine(pxl::platform::applicationPath(), "content/helloworld.png"));
		
		//escape key does things
		esc = pxl::bindings::createButton();
		esc->bind(pxl::Key::Escape);
	}
	void end() override
	{
		pxl::Scene::end();
	}
	void update()
	{
		if (esc->pressed())
		{
			//escape pressed, close the app
			pxl::end();
		}
	}
	void draw()
	{
		auto &drawables = this->drawables();
		batch.begin(nullptr, pxl::Color::black);
		for (auto it : drawables) 	{
			it->draw(batch);
		}
		batch.end();
	}
private:
	pxl::VirtualButtonRef esc;
	pxl::TextureRef tex;
	pxl::Batch batch;
};

int main()
{
	// set initial scene
	pxl::scenes::set<HelloWorldScene>();

	// engine configuration
	pxl::Config config;
	config.name = "Pxl Hello world";
	config.title = "Pxl Hello world";
	config.fixed_update = true;
	config.vertical_sync = false;
	config.target_framerate = 60;
	config.width = 1280;
	config.height = 720;

	// start
	pxl::begin(config);

	return 0;
}
