#include "pch.h"
#include "MonoBehaviour.h"

// ������Ʈ�� �����ڿ� COMPONENT_TYPE�� �ް� ���ֱ� ������ �ʱ�ȭ ����Ʈ�� ���� 
// COMPONENT_TYPE�� ����������մϴ�.
MonoBehaviour::MonoBehaviour() : Component(COMPONENT_TYPE::MONO_BEHAVIOUR)
{
}

MonoBehaviour::~MonoBehaviour()
{
}
