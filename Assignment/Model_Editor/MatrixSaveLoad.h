#pragma once

class SaveLoad
{
public:
	friend class DrawModel;


private:
	SaveLoad() {};
	~SaveLoad() {};
	
	void SaveMateraial(wstring savePath, Transform* transform);
	void LoadMateraial(wstring loadPath, Transform* transform);

	void SaveFile(wstring savePath, Transform* transform);
	void LoadFile(wstring loadPath, Transform* transform);


	wstring file = L"";
	Vector3 pos, scale, rotation;
};
