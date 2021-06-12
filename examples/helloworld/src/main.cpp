#include <pxl/engine.h>
#include <pxl/utils/scene.h>
#include <pxl/graphics/batch.h>
#include <pxl/utils/input_binding.h>
#include <pxl/utils/filestream.h>

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
	HelloWorldScene() : pxl::Scene("Hello World Scene"){}
	void begin() override
	{
		pxl::Scene::begin();


		// entity holds components
		auto entity = createEntity(pxl::Vec2(rand()%800, 100));

		// component does things (in this case draws texture)
		auto component = entity->add(HelloWorldComponent());
		component->texture = pxl::Texture::create(pxl::path::combine(pxl::platform().applicationPath(), "content/helloworld.png"));
		
		//escape key does things
		esc = pxl::bindings().CreateButton();
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
			pxl::engine().end();
		}
	}
private:
	pxl::VirtualButtonRef esc;
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
	pxl::engine().begin(config);

	return 0;
}
