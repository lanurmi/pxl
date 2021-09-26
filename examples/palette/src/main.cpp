#include <pxl/engine.h>
#include <pxl/utils/scene.h>
#include <pxl/graphics/batch.h>
#include <pxl/utils/input_binding.h>
#include <pxl/utils/filestream.h>
#include <pxl/assets/aseprite.h>
#include <pxl/graphics/batch.h>

class TextureDrawComponent : public pxl::Component, public pxl::IDrawable
{
public:
	pxl::TextureRef texture;
	void draw(pxl::Batch& batch)
	{
		if (texture == nullptr) return;
		batch.texture(texture, entity()->position, pxl::Color::white);
	}
};

class PaletteScene : public pxl::Scene
{
public:
	PaletteScene() : pxl::Scene("Palette Scene"){}
	void begin() override
	{
		pxl::Scene::begin();

		auto entity = createEntity(pxl::Vec2(0,0));

		pxl::Aseprite p0("content/gb0.ase");
		pxl::Aseprite p1("content/gb1.ase");
		pxl::Aseprite p2("content/gb2.ase");
		palettes[0] = pxl::Texture::create(p0.frames[0].image);
		palettes[1] = pxl::Texture::create(p1.frames[0].image);
		palettes[2] = pxl::Texture::create(p2.frames[0].image);

		pxl::Aseprite img(pxl::path::combine(pxl::platform::applicationPath(), "content/image.ase"));
		image = pxl::Texture::create(img.frames[0].image);

		palette_material = pxl::Material::create(pxl::graphics::createPaletteShader());

		palette_material->setTexture("u_palette", palettes[palette_index]);

		auto component = entity->add(TextureDrawComponent());
		component->texture = image;
 		
		esc = pxl::bindings::createButton();
		esc->bind(pxl::Key::Escape);

		left = pxl::bindings::createButton();
		left->bind(pxl::Key::Left);

		right = pxl::bindings::createButton();
		right->bind(pxl::Key::Right);
	}
	void end() override
	{
		pxl::Scene::end();
	}

	void update() override
	{
		if (esc->pressed())
		{
			//escape pressed, close the app
			pxl::end();
		}

		//Switch palette
		if (right->pressed()) {
			palette_index = pxl::calc::clamp(palette_index + 1, 0, 2);
			palette_material->setTexture("u_palette", palettes[palette_index]);
		}
		else if (left->pressed()) {
			palette_index = pxl::calc::clamp(palette_index - 1, 0, 3);
			palette_material->setTexture("u_palette", palettes[palette_index]);
		}	
	}

	void draw() override {

		pxl::backbuffer->clear(pxl::Color::black);
		batch.clear();
		batch.pushMaterial(palette_material);

		auto& drawus = drawables();

		for (auto d : drawus) 	{
			d->draw(batch);
		}

		batch.draw(pxl::backbuffer);
	}
private:
	pxl::VirtualButtonRef esc;
	pxl::VirtualButtonRef left;
	pxl::VirtualButtonRef right;
	pxl::TextureRef palettes[3];
	int palette_index = 0;
	pxl::TextureRef image;
	pxl::Batch batch;
	pxl::MaterialRef palette_material;
};

int main()
{
	// set initial scene
	pxl::scenes::set<PaletteScene>();

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
