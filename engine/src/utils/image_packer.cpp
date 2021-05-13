#include <pxl/utils/image_packer.h>
#include <pxl/math/calc.h>
#include <stdio.h>
#include <cstring>
#include <algorithm>

using namespace pxl;

ImagePacker::ImagePacker(int width, int height) : _width(width), _height(height)
{

}

ImagePacker::~ImagePacker()
{

}

void ImagePacker::add(u32 id, const Image& image)
{
	add(id, image.width(), image.height(), image.pixels());
}

struct Node
{
	Node()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		right = nullptr;
		bottom = nullptr;
		reserved = false;
	}
	int x;
	int y;
	int width;
	int height;
	Node* right;
	Node* bottom;
	bool reserved;
};

void ImagePacker::add(u32 id, int width, int height, const Color* pixels)
{
	Entry e;
	e.id = id;
	e.width = width;
	e.height = height;
	_entries.emplace_back(e);

	Buffer buf;
	buf.id = id;
	buf.pixels = pixels;
	_pixels.emplace_back(buf);
}

static Node* findNode(Node* root, int w, int h)
{
	if (root->reserved)
	{
		auto next = findNode(root->right, w, h);
		if (next == nullptr)
		{
			next = findNode(root->bottom, w, h);
		}
		return next;
	}
	else if (root->width >= w && root->height >= h)
	{
		return root;
	}
	else
	{
		return nullptr;
	}
}

static Node* splitNode(Node* node, int w, int h, std::vector<Node*> &nodeBuffer)
{
	node->reserved = true;

	node->right = new Node();
	node->right->x = node->x + w;
	node->right->y = node->y;
	node->right->width = node->width - w;
	node->right->height = h;

	node->bottom = new Node();
	node->bottom->x = node->x;
	node->bottom->y = node->y + h;
	node->bottom->width = node->width;
	node->bottom->height = node->height - h;

	nodeBuffer.push_back(node->right);
	nodeBuffer.push_back(node->bottom);

	return node;
}

int ImagePacker::doPack(int fromEntry)
{
	vector<Node*> nodes;

	auto rootNode = new Node();
	rootNode->width = _width;
	rootNode->height = _height;

	nodes.push_back(rootNode);
	int packed = fromEntry;
	for (int p = fromEntry; p < _entries.size(); p++)
	{
		auto& entry = _entries[p];
		auto node = findNode(rootNode, entry.width, entry.height);
		if (node != nullptr)
		{
			auto dstNode = splitNode(node, entry.width, entry.height, nodes);
			entry.x = dstNode->x;
			entry.y = dstNode->y;
			packed++;
		}
		else
		{
			break;
		}
	}
	for (auto it : nodes)
	{
		delete it;
	}

	Image image(_width, _height);

	for (int p = fromEntry; p < packed; p++)
	{
		auto& e = _entries[p];
		for (auto& buf : _pixels)
		{
			if (buf.id == e.id)
			{
				image.setPixels(Rect(e.x, e.y, e.width, e.height), buf.pixels);
			}
		}
	}

	_result.emplace_back(Result());

	for (int p = 0; p < _entries.size(); p++)
	{
		_result.back().entries.push_back(_entries[p]);
	}
	_result.back().image = std::move(image);

	return packed;
}

const std::vector<ImagePacker::Result>& ImagePacker::pack()
{
	_result.clear();
	std::sort(_entries.begin(), _entries.end(), [](const Entry& e0, const Entry& e1)
		{
			return e0.width * e0.height > e1.width * e1.height;
		});
	
	int packed = 0;
	while (packed < _entries.size())
	{
		packed = doPack(packed);
	}

	_pixels.clear();
	_entries.clear();
	return _result;
}