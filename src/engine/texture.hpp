#pragma once
#include "inc.h"

class Texture {
public:
	Texture() = default;
	~Texture() = default;
	bool Load(const fs::path& path);
private:
	wrl::ComPtr<ID3D11ShaderResourceView> tex;
};