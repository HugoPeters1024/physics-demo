#include "deps.h"

int main() {
  std::unique_ptr<IRenderer> renderer = std::make_unique<Rasterizer>();
  Camera::Camera camera(1.6f);
  Keyboards::Keyboard keyboard(renderer->getWindowPointer());

  rp3d::Vector3 gravity(0, -9.81f, 0);
  rp3d::DynamicsWorld world(gravity);

  rp3d::Transform cubeTransform(rp3d::Vector3(0,10,0), rp3d::Quaternion::fromEulerAngles(0, 0, 0.2f));
  rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
  cubeBody->setAngularVelocity(rp3d::Vector3(0, 10, 0));
  cubeBody->setLinearVelocity(rp3d::Vector3(0.1, 0, 0));
  const rp3d::Vector3 cubeHalfExtends(5.5, 5.5, 0.5);
  rp3d::BoxShape boxShape(cubeHalfExtends);
  rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 0.4);
  renderer->addCube(cubeProxy, &boxShape);

  rp3d::SphereShape sphereShape(1.6f);

  rp3d::Transform floorTransform(rp3d::Vector3(0,-5,0), rp3d::Quaternion().identity());
  rp3d::RigidBody* floorBody = world.createRigidBody(floorTransform);
  floorBody->setType(reactphysics3d::BodyType::KINEMATIC);
  floorBody->setAngularVelocity(rp3d::Vector3(0, 0, 0));
  rp3d::Material& floorMaterial = floorBody->getMaterial();
  floorMaterial.setBounciness(rp3d::decimal(0.2));
  floorMaterial.setFrictionCoefficient(1.0);
  const rp3d::Vector3 floorHalfExtends(10, 0.2, 10);
  rp3d::BoxShape floorShape(floorHalfExtends);
  float* heightFieldData = generateHeightMap(200, 200, 30, 20);
  rp3d::HeightFieldShape heightField(200, 200, 0, 30, heightFieldData, rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE, 1);
  rp3d::ProxyShape* floorProxy = floorBody->addCollisionShape(&heightField, rp3d::Transform::identity(), 0);
  renderer->addHeightMap(floorProxy, &heightField, heightFieldData);


  auto sun = new Light();
  sun->position = Vector3(0,40,0);
  sun->color = Vector3(1)*13000;
  sun->quadratic = 0.5;
  renderer->addLight(sun);

  std::vector<Light*> lights;
  for(int i=0; i<10; i++) {
    float p = (float)i / 10;
    float x = 40 * cos(p * 6.28);
    float z = 40 * sin(p * 6.28);
    auto light = new Light();
    light->position = Vector3(p * 100, 3, 0),
    light->color = Vector3(p, 1-p, 0.5 + 0.5*sin(p*15)) * 50;
    //renderer->addLight(light);
    lights.push_back(light);
  }

  int tick=1;
  while(!renderer->shouldClose())
  {
    if (tick++%150 == 0) {
      rp3d::Transform cubeTransform(rp3d::Vector3(10*sin(glfwGetTime()),35,10*cos(glfwGetTime())), rp3d::Quaternion::fromEulerAngles(0, 0, 0));
      rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
      rp3d::Material &cubeMaterial = cubeBody->getMaterial();
      cubeMaterial.setBounciness(0.3);
      cubeMaterial.setFrictionCoefficient(1);
      rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 100);
      renderer->addCube(cubeProxy, &boxShape);

      /*
      rp3d::Transform sphereTransform(rp3d::Vector3(10 * sin(glfwGetTime()), 5, 10 * cos(glfwGetTime())),
                                      rp3d::Quaternion::fromEulerAngles(0, 0, 0));
      rp3d::RigidBody *sphereBody = world.createRigidBody(sphereTransform);
      rp3d::Material &sphereMaterial = sphereBody->getMaterial();
      sphereMaterial.setBounciness(0.0);
      sphereMaterial.setFrictionCoefficient(1);
      rp3d::ProxyShape *sphereProxy = sphereBody->addCollisionShape(&sphereShape, cubeTransform, 100);
      renderer->addSphere(sphereProxy, &sphereShape);
       */
    }

    world.update(1.0/60.0);
    keyboard.swapBuffers();
    camera.update(renderer->getWindowRatio(), &keyboard);
    Vector3 camPos = camera.getPosition();
    CameraRayCaster cacher(&camera);

    rp3d::Ray ray1(rp3d::Vector3(camPos.x, 100, camPos.z), rp3d::Vector3(camPos.x, -100, camPos.z+0.1));
    world.raycast(ray1, &cacher);
    rp3d::Ray ray2(rp3d::Vector3(camPos.x, 100, camPos.z), rp3d::Vector3(camPos.x+0.1, -100, camPos.z));
    world.raycast(ray2, &cacher);
    rp3d::Ray ray3(rp3d::Vector3(camPos.x, 100, camPos.z), rp3d::Vector3(camPos.x-0.1, -100, camPos.z));
    world.raycast(ray3, &cacher);
    rp3d::Ray ray4(rp3d::Vector3(camPos.x, 100, camPos.z), rp3d::Vector3(camPos.x, -100, camPos.z-0.1));
    world.raycast(ray4, &cacher);

    renderer->loop(&camera);
//    printf("%s\n", body->getLinearVelocity().to_string().c_str());

  }
  return 0;
}

