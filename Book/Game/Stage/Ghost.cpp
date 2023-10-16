#include "stdafx.h"
#include "Ghost.h"

namespace
{
	const Vector3 LENGTH_MIN = Vector3(1000000.0f, 1000000.0f, 1000000.0f);
	const float LENGTH = 100.0f;
}

Ghost::Ghost()
{

}

Ghost::~Ghost()
{

}

bool Ghost::Start()
{
	//���f����ǂݍ��݃��f���̑傫�������߂�
	m_modelRender.Init("Assets/modelData/level_test/tkm/box.tkm");
	m_modelRender.Update();

	return true;
}

Vector3& Ghost::CreateGhostBox()
{
	const auto& tkmFile = m_modelRender.GetModel().GetTkmFile();
	const auto& meshParts = tkmFile.GetMeshParts();

	Vector3		lengthMax;
	Vector3		lengthMin = LENGTH_MIN;

	//���b�V������Â��ׂĂ���
	for (const auto& mesh : meshParts)
	{
		for (const auto& vertex : mesh.vertexBuffer)
		{
			auto pos = vertex.pos;
			//���f���̉��̒��������߂�
			if (lengthMax.x < pos.x)
			{
				lengthMax.x = pos.x;
			}
			if (lengthMin.x > pos.x)
			{
				lengthMin.x = pos.x;
			}
			//���f���̍��������߂�
			if (lengthMax.y < pos.y)
			{
				lengthMax.y = pos.y;
			}
			if (lengthMin.y > pos.y)
			{
				lengthMin.y = pos.y;
			}
			//���f���̉��s�����߂�
			if (lengthMax.z < pos.z)
			{
				lengthMax.z = pos.z;
			}
			if (lengthMin.z > pos.z)
			{
				lengthMin.z = pos.z;
			}
		}
	}

	Vector3 length = lengthMax - lengthMin;
	Vector3 boxSize;

	//�T�C�Y���|����
	boxSize.x = length.x * m_scale.x;
	boxSize.y = length.y * m_scale.y * 2.0f;
	boxSize.z = length.z * m_scale.z;

	return boxSize;
}

bool Ghost::IsHit(const Vector3& playerPosition)
{
	// �X�P�[�����l����������
	Vector3 distace = RangeWithTarget(playerPosition);
	distace.x /= m_scale.x;
	distace.y /= m_scale.y;
	distace.z /= m_scale.z;

	// ���W�����ɂ����Ĉ��ȉ��̏ꍇ
	if (distace.x <= LENGTH && distace.y <= LENGTH && distace.z <= LENGTH) {
		return true;
	}

	return false;
}

Vector3 Ghost::RangeWithTarget(const Vector3& playerPosition)
{
	// �������v�Z
	Vector3 diff = m_position - playerPosition;
	// ���ۂ̋������v�Z���ĕԂ�
	diff.x = std::sqrt(diff.x * diff.x);
	diff.y = std::sqrt(diff.y * diff.y);
	diff.z = std::sqrt(diff.z * diff.z);

	return diff;
}

void Ghost::Update()
{
	m_modelRender.Update();
}