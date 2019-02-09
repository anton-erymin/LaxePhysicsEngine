#pragma once

#include "export.h"

#include "lpMath.h"
#include "lpBroadPhase.h"
#include "lpNarrowPhase.h"


class DLL_EXPORT lpSurface
{
public:
	// ��������� ����������� ����
		
	// ����������� �������
	float m_restitution;
	// ����������� ������
	float m_friction;


	lpSurface(float restitution = 0.8f, float friction = 0.6f);
	void	setRestitution(float restitution);
	float	getRestitution();
	void	setFriction(float friction);
	float	getFriction();
};


class DLL_EXPORT lpRigidBody
{
public:
	// ������� � ������� �������
	lpVec3 m_pos; //0 4 8
	// ������� ���������� � ������� �������. �������� ������ ��������
	lpMat3 m_orientation; //12 16 20 24 28 32 36 40 44
		
	// ������� ������������� �� ��������� ������� � �������. ���������� ������� � ����������
	lpMat3 m_transform; //48 - 80
		
	// ��������� ����
	lpAxisAlignedBoundingBox m_localAABB; //84 88 92 96 100 104
	// ��������������� ����
	lpAxisAlignedBoundingBox m_worldAABB; //108 112 116 120 124 128
		
	// �������� ��������
	lpVec3 m_linearVel; //132 136 140
	// ������� ��������
	lpVec3 m_angularVel; //144 148 152

	// ������� ����
	float m_energy;	//156
	// ����, ������������ ������� �� ���� � ������ ������
	bool m_freezed; // 160


	// �������� ���������
	lpVec3 m_acceleration;
	// ������� ���������
	lpVec3 m_angularAcceleration; 

	// ���� ������������ ��� ����
	bool m_immovable;
		
	// ��������� ����, ��������������� ������������ ��� ����������� �������� � ����� ����
	lpCollisionGeometry *m_geometry; 
		
	// ����� ���������������� ������, �������� ��� ������������ ������ ��� ����������� ���� �� ������
	void *m_userData; 
		
	// ��������� ����������� ����
	lpSurface *m_surface;
		
	int m_id;
		
		
	// ��������� ��������� ��������
	lpVec3 m_deltaVel; 
	
	// ����� ����
	float m_mass; 
	// �������� ����� ����
	float m_invMass;
	// ��������������� ������ ������� � ��������� �����������
	lpMat3 m_invI;
	// ��������������� ������ ������� � ������� �����������
	lpMat3 m_invIWorld;	
		

	
	// ���� ������������, ����� �� ���� "�������"
	bool m_canFreeze;
private:
	// ����� ������� ����������� ����
	float m_freezeEps;
	// ���� �������� ��������� ����
	bool m_freezing;
	short m_freezeFrames;
	short m_numFramesToFreeze;

public:
	// ������ ����������
	lpVec3 m_gravity;

	// ����������� ���
	lpVec3 m_forceAccum;
	// ����������� �������� ���
	lpVec3 m_torqueAccum;

	lpVec3 m_pseudoForce;
	lpVec3 m_pseudoTorque;

private:
	// ������������ ��������� �������� � ������� ���������
	float m_linearDamping;
	float m_angularDamping;


public:
	lpRigidBody(void);
	~lpRigidBody(void);

	void integrate(float dt);
	void addGeometry(lpCollisionPrimitive *geom);
	void updateAABB();
	void update();

	void rotate(float angle, const lpVec3 &axis);

	void setSurfaceParams(float restitution, float friction);

	void setGravity(const lpVec3 &gravity);

	void setMass(float mass);
	void setInertiaTensor(const lpMat3 &inertiaTensor);

	void applyForce(const lpVec3 &force);
	void applyForceAtPoint(const lpVec3 &force, const lpVec3 point);
	void applyTorque(const lpVec3 &torque);

	void clearAccums();

	void multByInverseInertiaTensorWorld(lpVec3 &vec);
	void calcInverseInertiaTensorWorld();

	void freeze();
	void unfreeze();

	void makeImmovable();

	float	getLinearDamping();
	void	setLinearDamping(float linDamping);
	float	getAngularDamping();
	void	setAngularDamping(float angDamping);

	void		setPosition(float x, float y, float z);
	void		setPosition(const lpVec3 &pos);
	lpVec3		getPosition();
};