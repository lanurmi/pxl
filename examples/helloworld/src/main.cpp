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
		auto mousePosition = pxl::mouse::localPosition();
		auto windowSize = pxl::platform::drawSize();
		if (mousePosition.x > 0 &&
			mousePosition.y > 0 &&
			mousePosition.x < windowSize.x &&
			mousePosition.y < windowSize.y) {
			entity()->transform.position = mousePosition - pxl::Vec2(texture->width() / 2.0f, texture->height() / 2.0f);
		}
	}
	void draw(pxl::Batch& batch) override
	{
		if (texture == nullptr) return;
		batch.texture(texture, entity()->transform.position, pxl::Color::white);
	}
};

COMPONENT(HelloWorldComponent)


pxl::VirtualButtonRef esc;
pxl::TextureRef tex;
pxl::Batch batch;
pxl::World world;

void awake()
{
	world.awake();

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

	auto& hellocomponents = world.all<HelloWorldComponent>();
	for (auto hellcomponent : hellocomponents)
	{
		hellcomponent->update();
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

	auto& hellocomponents = world.all<HelloWorldComponent>();
	for (auto hellcomponent : hellocomponents)
	{
		hellcomponent->draw(batch);
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
	config.awake += std::bind(awake);
	config.update += std::bind(update);
	config.draw += std::bind(draw);

	// start
	pxl::begin(config);

	return 0;
}
