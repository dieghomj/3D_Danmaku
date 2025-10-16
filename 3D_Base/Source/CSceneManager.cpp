#include "CSceneManager.h"

CSceneManager::CSceneManager()
	: m_pCurrentScene	(nullptr)
	, m_pSceneList		()
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::AddScene(CScene* scene, const char* name)
{
	m_pSceneList[name] = scene;
}

CScene* CSceneManager::ChangeScene(const char* name)
{
	//‘O‚ÌƒV[ƒ“”j‰ó
	if (m_pCurrentScene)
	{
		//m_pCurrentScene->~CScene();	
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
	}

	m_pCurrentScene = m_pSceneList[name];

	if (m_pCurrentScene)
	{
		m_pCurrentScene->Create();
		m_pCurrentScene->LoadData();
	}

	return m_pCurrentScene;
}

void CSceneManager::Update()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Update();
	}
}

void CSceneManager::Draw()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Draw();
	}
}
