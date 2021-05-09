#include <pxl/engine.h>
#include <pxl/utils/content.h>
#include <pxl/utils/scene.h>
#include <pxl/graphics/batch.h>

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
	HelloWorldScene() : pxl::Scene(), content("content") {}
	void begin() override
	{
		clear_color = pxl::Color::yellow;
		auto entity = createEntity(pxl::Vec2(100, 100));
		auto component = entity->add(HelloWorldComponent());
		component->texture = content.LoadTexture("helloworld.png");
	}
private:
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
