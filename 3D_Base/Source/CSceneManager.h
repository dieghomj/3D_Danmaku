#pragma once
#include <map>

class CScene;

class CSceneManager
{
public:

	CSceneManager();
	~CSceneManager();

	//Stacked State Machine�ǉ�
	//<----------
	
	//�V�[�����X�g�ɃV�[����ǉ�
	void AddScene(CScene* scene, const char* name);
	//�V�[���ύX
	CScene* ChangeScene(const char* name);

	//���݂̃V�[���X�V
	void Update();
	//���݂̃V�[���`��
	void Draw();	

private:
	
	CScene* m_pCurrentScene;
	std::map< const char*, CScene* > m_pSceneList;
};