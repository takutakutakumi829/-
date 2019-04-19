#pragma once
#include <map>
#include <vector>
#include "Obj.h"

using VEC_INT = std::vector<int>;
#define IMAGE_ID(X)	(ImageMng::GetInstance()->GetID(X))

class ImageMng
{
public:
	static void Create();
	static void Destroy();
	static ImageMng *GetInstance()
	{
		Create();
		return s_Instance;
	}

	// �ǂݍ��ݑ���
	const VEC_INT& GetID(std::string f_name);
	// �����ǂݍ���
	const VEC_INT& GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset);

private:

	ImageMng();
	~ImageMng();
	static ImageMng *s_Instance;

	// std::map<���̌^,�A�z�z��̌^> �A�z�z��;
	std::map<std::string, VEC_INT> imgMap;

};

