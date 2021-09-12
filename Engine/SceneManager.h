#pragma once

class Scene;

// enum class를 사용해서 string을 바로 매핑하지 않는 이유는, 
// 엔진 클래스에서는 컨텐츠 클래스에서 레이어를 어떻게 활용할 지 모르기 때문에
// 지금 만드는 것보다는 컨텐츠 클래스에서 정의할 수 있도록 합니다.
enum 
{
	MAX_LAYER = 32
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;

	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

