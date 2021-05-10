#include <pxl/engine.h>
#include <pxl/utils/content.h>
#include <pxl/utils/scene.h>
#include <pxl/graphics/batch.h>
#include <pxl/utils/input_binding.h>

class HelloWorldComponent : public pxl::Component, public pxl::IDrawable
{
public:
	pxl::TextureRef texture;
	void draw(pxl::Batch& batch)
	{
		if (texture == nullptr) return;
		batch.texture(texture, entity()->position, pxl::Color::white);
	}
};

class HelloWorldScene : public pxl::Scene
{
public:
	HelloWorldScene() : pxl::Scene("Hello World Scene"), content("content") {}
	void begin() override
	{
		pxl::Scene::begin();
		clear_color = pxl::Color::yellow;
		auto entity = createEntity(pxl::Vec2(rand()%800, 100));
		auto component = entity->add(HelloWorldComponent());
		component->texture = content.LoadTexture("helloworld.png");
		esc.bind(pxl::Key::Escape);
	}
	void end() override
	{
		pxl::Scene::end();
	}
	void update()
	{
		esc.update();
		if (esc.pressed())
		{
			pxl::sceneManager().set(HelloWorldScene());
		}
	}
private:
	pxl::InputBinding esc;
	pxl::Content content;
	pxl::TextureRef tex;
	pxl::Batch batch;
};

int main()
{
	// set initial scene
	pxl::sceneManager().set(HelloWorldScene());

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
	pxl::engine().start(config);

	return 0;
}
