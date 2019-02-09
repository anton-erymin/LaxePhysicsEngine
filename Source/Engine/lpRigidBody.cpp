#include "StdAfx.h"
#include "lpRigidBody.h"


lpSurface::lpSurface(float restitution, float friction)
{
	setRestitution(restitution);
	setFriction(friction);
}


void lpSurface::setRestitution(float restitution)
{
	float rest = restitution;
	if (rest < 0.0f) rest = 0.0f;
	if (rest > 1.0f) rest = 1.0f;
	m_restitution = rest;
}


float lpSurface::getRestitution()
{
	return m_restitution;
}


void lpSurface::setFriction(float friction)
{
	float frict = friction;
	if (frict < 0.0f) frict = 0.0f;
	m_friction = frict;
}


float lpSurface::getFriction()
{
	return m_friction;
}




lpRigidBody::lpRigidBody(void)
{
	m_orientation.identity();
	m_transform.identity();

	m_geometry = new lpCollisionGeometry();

	m_surface = new lpSurface();

	m_mass = m_invMass = 0.0f;

	m_invI.identity();

	m_linearDamping = 0.8f;
	m_angularDamping = 0.8f;

	m_immovable = false;

	m_freezed = false;
	m_canFreeze = true;
	m_energy = 0.0f;
	m_freezeEps = 0.2f;
	m_freezing = false;
	m_numFramesToFreeze = 20;
}

lpRigidBody::~lpRigidBody(void)
{
}


void lpRigidBody::integrate(float dt)
{
	// ���������� ����
			
	if (m_immovable) return;
			
	// ���� ���� ����, �� �� ����������� ���
	/*if (m_freezed) 
	{
		clearAccums();
		return;
	}*/


	// ��������� ������� ������������ ���� ���� ����������
	if (m_canFreeze)
	{
		// ������� ��������������� ������� ����
		m_energy = m_linearVel * m_linearVel + m_angularVel * m_angularVel;
		if (m_energy < m_freezeEps)
		{
			// ���� ������� ������ ���������
			if (!m_freezed)
			{
				if (!m_freezing)
				{
					// ���� ��������� ��� �� ��������, �������� ��
					m_freezing = true;
					// ������� ������ � 0
					m_freezeFrames = 0;
				}
				else
				{	
					// ����� ���� ���� � �������� ��������� ����������� ������� ������ ���������
					m_freezeFrames++;
					if (m_freezeFrames > m_numFramesToFreeze)
					{
						freeze();
						m_freezing = 0;
					}
				}
			}
		}
		else
		{
			// ���� ������� �������
			if (m_freezing)
			{
				// �� ���� ���� � �������� ��������� ���������� ��
				m_freezing = false;
			}
			else if (m_freezed)
			{
				unfreeze();
			}
		}
	}

			
	// �������������� ��������� �������� ����			
	// ����� ������			
					 
	// �������������� ��������� ��������� �������� ����	
				
	// ��������� ��������� ����
	// a = F / m
	m_acceleration = m_forceAccum * m_invMass;
			
	// ����������� ��������� �� ��������
	// v += a * dt
	m_deltaVel = m_acceleration * dt;
	m_linearVel += m_deltaVel;
			
	// ��������� ��������
	m_linearVel *= pow(m_linearDamping, dt);
			
	// ����������� ��������� �� ����������
	// p += v * dt
	m_pos.addScaled(m_linearVel, dt);
			
	// ����������� ��������� ������������� ��������
	// ������� ������� ���������		
	// ��������� ��������� ������ ����
	// angAcc = torque / I
	m_torqueAccum *= m_invIWorld;
			
	// ����������� ��������� �� ������� ��������
	// w += angAcc * dt
	m_angularVel.addScaled(m_torqueAccum, dt);	
			
	// ��������� ������� ��������
	m_angularVel *= pow(m_angularDamping, dt);
			
	// ����������� ��������� �� ����������
	// R += w * dt
	lpVec3 axis(m_angularVel);
	float angle = axis.norm();
	if (fabsf(angle) > 0.001f)
	{
		axis *= 1.0f / angle;
		m_orientation.rotate(angle * dt, axis);
	}
			
	// ��������� ����
	update();
			
	// ������� ������������
	clearAccums();
		
	
}


void lpRigidBody::addGeometry(lpCollisionPrimitive *geom)
{
	geom->m_body = this;
	m_geometry->addGeometry(geom);
}


void lpRigidBody::updateAABB()
{
	lpAxisAlignedBoundingBox::transformAABB(m_localAABB, m_worldAABB, m_orientation, m_pos);
}


void lpRigidBody::update()
{
	// ���������� ������� � ����� ����
	updateAABB();
}


void lpRigidBody::rotate(float angle, const lpVec3 &axis)
{
	m_orientation.rotate(angle, axis);
}


void lpRigidBody::setSurfaceParams(float restitution, float friction)
{
	m_surface->setRestitution(restitution);
	m_surface->setFriction(friction);
}


void lpRigidBody::setGravity(const lpVec3 &gravity)
{
	m_gravity = gravity;
}


void lpRigidBody::setMass(float mass)
{
	m_mass = mass;
	m_invMass = 1.0f / mass;
}


void lpRigidBody::setInertiaTensor(const lpMat3 &inertiaTensor)
{
	m_invI = inertiaTensor;
	// ����������� ������ �������
	m_invI.invert();
}


void lpRigidBody::applyForce(const lpVec3 &force)
{
	// ���������� ���� � ������ ����
	m_forceAccum += force;
}


void lpRigidBody::applyForceAtPoint(const lpVec3 &force, const lpVec3 point)
{
	// ���������� ���� � ������������ ����� ����
	// ��������� ������ ����
	lpVec3 torque = point;
	torque -= m_pos;
	torque %= force;
	m_forceAccum += force;
	m_torqueAccum += torque;		
	unfreeze();
}


void lpRigidBody::applyTorque(const lpVec3 &torque)
{
	// ���������� ������� ����
	m_torqueAccum += torque;
	unfreeze();
}


void lpRigidBody::clearAccums()
{
	m_forceAccum.clear();
	m_torqueAccum.clear();
}


void lpRigidBody::multByInverseInertiaTensorWorld(lpVec3 &vec)
{
	// ��������� ��������� ������� ������� �� ������ � ������� ��������� ����
	vec = m_orientation.multTransposed(vec);
	vec *= m_invI;
	vec *= m_orientation;
}


void lpRigidBody::calcInverseInertiaTensorWorld()
{
	m_invIWorld = m_orientation * m_invI * ~m_orientation;
}


void lpRigidBody::freeze()
{
	m_freezed = true;
	m_linearVel.clear();
	m_angularVel.clear();
}


void lpRigidBody::unfreeze()
{
	m_freezed = false;
}


void lpRigidBody::makeImmovable()
{
	m_immovable = true;
}


void lpRigidBody::setPosition(float x, float y, float z)
{
	m_pos.setTo(x, y, z);
}


void lpRigidBody::setPosition(const lpVec3 &pos)
{
	m_pos = pos;
}


lpVec3 lpRigidBody::getPosition()
{
	return m_pos;
}


float lpRigidBody::getLinearDamping()
{
	return m_linearDamping;
}


void lpRigidBody::setLinearDamping(float linDamping)
{
	m_linearDamping = linDamping;
}


float lpRigidBody::getAngularDamping()
{
	return m_angularDamping;
}


void lpRigidBody::setAngularDamping(float angDamping)
{
	m_angularDamping = angDamping;
}