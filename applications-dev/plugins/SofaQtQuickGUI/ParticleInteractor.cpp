#include "ParticleInteractor.h"
#include "Scene.h"
#include "Viewer.h"
#include "Manipulator.h"

#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/simulation/common/CleanupVisitor.h>
#include <sofa/simulation/common/DeleteVisitor.h>
#include <SofaBaseMechanics/MechanicalObject.h>
#include <SofaBoundaryCondition/FixedConstraint.h>
#include <SofaDeformable/StiffSpringForceField.h>
#include <SofaOpenglVisual/OglModel.h>

#include <qqml.h>
#include <QDebug>

namespace sofa
{

namespace qtquick
{

typedef sofa::simulation::Node Node;
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::Vec3dTypes> MechanicalObject3d;
typedef sofa::component::projectiveconstraintset::FixedConstraint<sofa::defaulttype::Vec3dTypes> FixedConstraint3d;
typedef sofa::component::interactionforcefield::StiffSpringForceField<sofa::defaulttype::Vec3dTypes> StiffSpringForceField3d;

ParticleInteractor::ParticleInteractor(QObject *parent) : QObject(parent),
    mySceneComponent(nullptr),
    myParticleIndex(-1),
    myStiffness(100),
    myNode(nullptr),
    myMechanicalState(nullptr),
    myForcefield(nullptr)
{

}

ParticleInteractor::~ParticleInteractor()
{
	release();
}

QVector3D ParticleInteractor::particlePosition() const
{
    QVector3D particlePosition(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
    if(!mySceneComponent)
        return particlePosition;

    MechanicalObject3d* mechanicalObject = static_cast<MechanicalObject3d*>(myMechanicalState);
    sofa::defaulttype::Vector3 position = mechanicalObject->readPositions()[0];

    return QVector3D(position.x(), position.y(), position.z());
}

QVector3D ParticleInteractor::interactorPosition() const
{
    QVector3D interactorPosition(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
	if(!myMechanicalState)
        return interactorPosition;

	MechanicalObject3d* mechanicalObject = static_cast<MechanicalObject3d*>(myMechanicalState);
	sofa::defaulttype::Vector3 position = mechanicalObject->readPositions()[0];

	return QVector3D(position.x(), position.y(), position.z());
}

bool ParticleInteractor::interacting() const
{
    return mySceneComponent && mySceneComponent->base() && -1 != myParticleIndex;
}

bool ParticleInteractor::start(SceneComponent* sceneComponent, int particleIndex)
{
    release();

    if(-1 == particleIndex)
        return false;

    if(!sceneComponent)
        return false;

    const Scene* scene = sceneComponent->scene();
    MechanicalObject3d* particleMechanicalObject = dynamic_cast<MechanicalObject3d*>(sceneComponent->base());

    if(!scene || !particleMechanicalObject)
        return false;

    mySceneComponent = new SceneComponent(*sceneComponent);
    myParticleIndex = particleIndex;

    QVector3D position = QVector3D(particleMechanicalObject->getPX(myParticleIndex),
                                   particleMechanicalObject->getPY(myParticleIndex),
                                   particleMechanicalObject->getPZ(myParticleIndex));

    MechanicalObject3d::SPtr mechanicalObject = sofa::core::objectmodel::New<MechanicalObject3d>();
    mechanicalObject->setName("Attractor");
    mechanicalObject->resize(1);
    mechanicalObject->writePositions()[0] = sofa::defaulttype::Vector3(position.x(), position.y(), position.z());
    myMechanicalState = mechanicalObject.get();

    FixedConstraint3d::SPtr fixedConstraint = sofa::core::objectmodel::New<FixedConstraint3d>();

    StiffSpringForceField3d::SPtr stiffSpringForcefield = sofa::core::objectmodel::New<StiffSpringForceField3d>(mechanicalObject.get(), particleMechanicalObject);
    stiffSpringForcefield->setName("Spring");
    stiffSpringForcefield->addSpring(0, myParticleIndex, myStiffness, 0.1, 0.0);
    myForcefield = stiffSpringForcefield.get();

    Node::SPtr node = scene->sofaSimulation()->GetRoot()->createChild("Interactor");
    node->addObject(mechanicalObject);
    node->addObject(fixedConstraint);
    node->addObject(stiffSpringForcefield);
    node->init(sofa::core::ExecParams::defaultInstance());
    myNode = node.get();

    Node* particleNode = dynamic_cast<Node*>(stiffSpringForcefield->getMState2()->getContext());
    particleNode->moveObject(stiffSpringForcefield);

    interactorPositionChanged(position);
    interactingChanged(true);

    return true;
}

bool ParticleInteractor::update(const QVector3D& position)
{
    if(!myMechanicalState)
        return false;

    MechanicalObject3d* mechanicalObject = static_cast<MechanicalObject3d*>(myMechanicalState);
    mechanicalObject->writePositions()[0] = sofa::defaulttype::Vector3(position.x(), position.y(), position.z());
    interactorPositionChanged(position);

    return true;
}

void ParticleInteractor::release()
{
    if(myNode)
    {
        StiffSpringForceField3d::SPtr stiffSpringForcefield = static_cast<StiffSpringForceField3d*>(myForcefield);
        myNode->moveObject(stiffSpringForcefield);

        Node::SPtr node = static_cast<Node*>(myNode);
        node->detachFromGraph();
        node->execute<sofa::simulation::CleanupVisitor>(sofa::core::ExecParams::defaultInstance());
        node->execute<sofa::simulation::DeleteVisitor>(sofa::core::ExecParams::defaultInstance());
    }

    myNode = nullptr;
    myMechanicalState = nullptr;
    myForcefield = nullptr;

    myParticleIndex = -1;

    delete mySceneComponent;
    mySceneComponent = nullptr;

    interactingChanged(false);
}

}

}