#pragma once

#include "export.h"
#include "lpJoint.h"
#include "lpMath.h"


class lpRigidBody;


class DLL_EXPORT lpContactJoint : public lpJoint
{
public:
	// ����������� ��������� �����
	float m_restitution;
		
	// ����������� ������ ��������
	float m_friction;
		
	// ���������� ����� ��������
	lpVec3 m_point;
		
	// ������� �������� � ������� �����������
	lpVec3 m_normal;
		
	// ������� ������������� � ��������
	float m_depth;
		
		
	lpVec3 m_posChange[2];
	lpVec3 m_orChange[2];
	lpVec3 m_velChange[2];
	lpVec3 m_angVelChange[2];
		
	// ���������� ������������� ������
private:	
		// ������� �������������� ��� ������� ��������� ��������� � ������ ��������
	lpMat3 m_transform;
		
	// �������������� ��� ������
	lpVec3 m_frictionDir1;
	lpVec3 m_frictionDir2;
		
			
public:
	lpVec3 m_r1;
	lpVec3 m_r2;

	lpVec3 m_relVel;
	lpVec3 m_relVelLocal;
		
	float m_desiredDeltaVelocity;


	lpContactJoint(void);
	~lpContactJoint(void);

	void prepare();
	void calcDesiredDeltaVelocity();
	void calcRelativeVelocity();
	float calcLambda(const lpVec3 &normal, float dv);
	void solveVelocity();
	void solvePenetration();
	void calcContactTransform();

	float** jacobian(float idt);
};
