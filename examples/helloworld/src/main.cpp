#include <pxl/engine.h>
#include <pxl/ec/world.h>
#include <pxl/ec/component.h>
#include <pxl/graphics/batch.h>
#include <pxl/utils/input_binding.h>
#include <pxl/utils/filestream.h>
#include <pxl/backbuffer.h>
#include <pxl/time.h>

class HelloWorldComponent : public pxl::Component<HelloWorldComponent>, public pxl::IDrawable, public pxl::IUpdateable
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
			entity()->transform.position = mousePosition - windowPosition - pxl::Vec2(texture->width() / 2.0f, texture->height() / 2.0f);
		}
	}
	void draw(pxl::Batch& batch) override
	{
		if (texture == nullptr) return;
		batch.texture(texture, entity()->transform.position, pxl::Color::white);
	}
};


pxl::VirtualButtonRef esc;
pxl::TextureRef tex;
pxl::Batch batch;
pxl::World world;

void awake()
{
	auto e = world.entity(pxl::Vec2::zero);
	auto helloComponent = e->add(HelloWorldComponent());
	tex = pxl::Texture::create(pxl::Image("content/helloworld.png"));
	helloComponent->texture = tex;

	esc = pxl::bindings::createButton();
	esc->bind(pxl::Key::Escape);
}

void update()
{
	world.update();

	auto hellocomponent = world.first<HelloWorldComponent>();
	while (hellocomponent != nullptr)
	{
		hellocomponent->update();
		hellocomponent = hellocomponent->next();
	}

	if (esc->pressed())
	{
		//escape pressed, close the app
		pxl::end();
	}
}

void draw() 
{
	pxl::backbuffer->clear(pxl::Color::black);
	batch.clear();

	auto hello = world.first<HelloWorldComponent>();
	while (hello != nullptr)
	{
		hello->draw(batch);
		hello = hello->next();
	}

	batch.draw(pxl::backbuffer);
}

int main()
{

	// engine configuration
	pxl::Config config;
	config.name = "Pxl Hello world";
	config.title = "Pxl Hello world";
	config.fixed_update = true;
	config.vertical_sync = false;
	config.target_framerate = 60;
	config.width = 1280;
	config.height = 720;
	config.awake = std::bind(awake);
	config.update = std::bind(update);
	config.draw = std::bind(draw);

	// start
	pxl::begin(config);

	return 0;
}
