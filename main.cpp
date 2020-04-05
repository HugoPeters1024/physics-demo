#include "deps.h"

int main() {

  std::unique_ptr<IRenderer> renderer = std::make_unique<Rasterizer>();
  Camera::Camera camera(1.4f);
  Keyboards::Keyboard keyboard(renderer->getWindowPointer());

  rp3d::Vector3 gravity(0, -6.0f, 0);
  rp3d::DynamicsWorld world(gravity);

  rp3d::Transform cubeTransform(rp3d::Vector3(0,0,0), rp3d::Quaternion::fromEulerAngles(0, 0, 0.2f));
  rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
  cubeBody->setAngularVelocity(rp3d::Vector3(0, 10, 0));
  cubeBody->setLinearVelocity(rp3d::Vector3(0.1, 0, 0));
  const rp3d::Vector3 cubeHalfExtends(0.5, 0.5, 0.5);
  rp3d::BoxShape boxShape(cubeHalfExtends);
  rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 0.4);
  renderer->addCube(cubeProxy, &boxShape);

  rp3d::Transform floorTransform(rp3d::Vector3(0,-1,0), rp3d::Quaternion().identity());
  rp3d::RigidBody* floorBody = world.createRigidBody(floorTransform);
  floorBody->setType(reactphysics3d::BodyType::KINEMATIC);
  floorBody->setAngularVelocity(rp3d::Vector3(0, 0, 0));
  rp3d::Material& floorMaterial = floorBody->getMaterial();
  floorMaterial.setBounciness(rp3d::decimal(0.3));
  floorMaterial.setFrictionCoefficient(0.8);

  const rp3d::Vector3 floorHalfExtends(10, 0.2, 10);
  rp3d::BoxShape floorShape(floorHalfExtends);
  rp3d::ProxyShape* floorProxy = floorBody->addCollisionShape(&floorShape, floorTransform, 0);
  renderer->addCube(floorProxy, &floorShape);

  int tick=1;
  while(!renderer->shouldClose())
  {
    if (tick++%45 == 0)
    {
      rp3d::Transform cubeTransform(rp3d::Vector3(0,5,0), rp3d::Quaternion::fromEulerAngles(0, 0, 0));
      rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
      rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, tick/30);
      renderer->addCube(cubeProxy, &boxShape);
    }
    world.update(1.0/60.0);
    keyboard.swapBuffers();
    camera.update(renderer->getWindowRatio(), &keyboard);
    renderer->loop(camera);
//    printf("%s\n", body->getLinearVelocity().to_string().c_str());

  }
  return 0;
}

