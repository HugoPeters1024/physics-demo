#include "deps.h"

int main() {

  std::unique_ptr<IRenderer> renderer = std::make_unique<Rasterizer>();
  Camera::Camera camera(1.4f);
  Keyboards::Keyboard keyboard(renderer->getWindowPointer());

  rp3d::Vector3 gravity(0, -9.81f, 0);
  rp3d::DynamicsWorld world(gravity);

  rp3d::Transform cubeTransform(rp3d::Vector3(0,0,0), rp3d::Quaternion::fromEulerAngles(0, 0, 0.2f));
  rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
  cubeBody->setAngularVelocity(rp3d::Vector3(0, 10, 0));
  cubeBody->setLinearVelocity(rp3d::Vector3(0.1, 0, 0));
  const rp3d::Vector3 cubeHalfExtends(0.5, 0.5, 0.5);
  rp3d::BoxShape boxShape(cubeHalfExtends);
  rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 0.4);
  renderer->addCube(cubeProxy, &boxShape);

  rp3d::Transform floorTransform(rp3d::Vector3(0,-5,0), rp3d::Quaternion().identity());
  rp3d::RigidBody* floorBody = world.createRigidBody(floorTransform);
  floorBody->setType(reactphysics3d::BodyType::KINEMATIC);
  floorBody->setAngularVelocity(rp3d::Vector3(0, 0, 0));
  rp3d::Material& floorMaterial = floorBody->getMaterial();
  floorMaterial.setBounciness(rp3d::decimal(0.0));
  floorMaterial.setFrictionCoefficient(1.0);
  const rp3d::Vector3 floorHalfExtends(10, 0.2, 10);
  rp3d::BoxShape floorShape(floorHalfExtends);
  float* heightFieldData = generateHeightMap(100, 100, 20);
  rp3d::HeightFieldShape heightField(100, 100, 0, 30, heightFieldData, rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE, 1);
  rp3d::ProxyShape* floorProxy = floorBody->addCollisionShape(&heightField, rp3d::Transform::identity(), 0);
  renderer->addHeightMap(floorProxy, &heightField, heightFieldData);

  int tick=1;
  while(!renderer->shouldClose())
  {
    if (tick++%50 == 0)
    {
      rp3d::Transform cubeTransform(rp3d::Vector3(10*sin(glfwGetTime()),5,10*cos(glfwGetTime())), rp3d::Quaternion::fromEulerAngles(0, 0, 0));
      rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
      rp3d::Material &cubeMaterial = cubeBody->getMaterial();
      cubeMaterial.setBounciness(0.0);
      cubeMaterial.setFrictionCoefficient(1);
      rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 100);
      renderer->addCube(cubeProxy, &boxShape);
    }
    world.update(1.0/60.0);
    keyboard.swapBuffers();
    camera.update(renderer->getWindowRatio(), &keyboard);
    Vector3 camPos = camera.getPosition();
    rp3d::Ray ray(rp3d::Vector3(camPos.x, 100, camPos.z), rp3d::Vector3(camPos.x, -100, camPos.z));
    RayCastCacher cacher(&camera);
    world.raycast(ray, &cacher);
    renderer->loop(&camera);
//    printf("%s\n", body->getLinearVelocity().to_string().c_str());

  }
  return 0;
}

