#pragma once
#include "CScene.h"
#include <map>
class CSceneManager
{
public:

	CSceneManager();
	~CSceneManager();

	void AddScene(CScene* scene, const char* name);
	CScene* ChangeScene(const char* name);

	void Update();
	void Draw();	

private:

	CScene* m_pCurrentScene;
	std::map< const char*, CScene* > m_pSceneList;
};