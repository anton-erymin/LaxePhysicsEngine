#pragma once

#include "export.h"
#include <vector>

#include "lpRigidBody.h"
#include "lpJoint.h"
#include "lpContactJoint.h"
#include "lpSolver.h"
#include "lpSequentialSolver.h"
#include "lpLCPSolver.h"
#include "lpCollisionTest.h"



class DLL_EXPORT lpWorld
{
public:
	// ������ ���������, ��������� �������� ����������� ������������
	std::vector<lpContactJoint*> m_contacts;
	// ���������� ��������� ���������
	unsigned int m_numContacts;

	// ������ �������� � ����
	std::vector<lpJoint*> m_joints;

protected:
	// ������ ���
	std::vector<lpRigidBody*> m_bodies;
	// ���������� ���������� ���
	int m_numDynamicBodies;

	// ������, ������������ ��� ������� ���������
	lpSolver *m_solver;

	// ������ ���������� ����
	lpVec3 m_gravity;
	

public:
	lpWorld(int solverType);
	virtual ~lpWorld();

	void addBody(lpRigidBody *body);
	void removeBody(const lpRigidBody *body);

	void setGravity(const lpVec3 &gravity);

	void step(float dt);

	void updateBodies();

protected:
	virtual void collide() = 0;

	void solve(float dt);
};



class DLL_EXPORT lpWorldSimple : public lpWorld
{
public:
	lpWorldSimple(int solverType);

private:
	void collide();
};