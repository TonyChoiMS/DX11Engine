#include "pch.h"
#include "MonoBehaviour.h"

// 컴포넌트의 생성자에 COMPONENT_TYPE을 받게 되있기 때문에 초기화 리스트를 통해 
// COMPONENT_TYPE를 정의해줘야합니다.
MonoBehaviour::MonoBehaviour() : Component(COMPONENT_TYPE::MONO_BEHAVIOUR)
{
}

MonoBehaviour::~MonoBehaviour()
{
}
