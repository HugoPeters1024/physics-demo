#include "deps.h"

int main() {
  std::unique_ptr<IRenderer> renderer = std::make_unique<Rasterizer>();
  Camera::Camera camera(1.2f, 1);
  Keyboards::Keyboard keyboard(renderer->getWindowPointer());

  rp3d::Vector3 gravity(0, -9.81f, 0);
  rp3d::DynamicsWorld world(gravity);

  rp3d::Transform cubeTransform(rp3d::Vector3(0,10,0), rp3d::Quaternion::fromEulerAngles(0, 0, 0.2f));
  rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
  cubeBody->setAngularVelocity(rp3d::Vector3(0, 10, 0));
  cubeBody->setLinearVelocity(rp3d::Vector3(0.1, 0, 0));
  const rp3d::Vector3 cubeHalfExtends(1, 0.2, 1);
  rp3d::BoxShape boxShape(cubeHalfExtends);
  rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 0.4);
  renderer->addCube(cubeProxy, &boxShape);

  rp3d::SphereShape sphereShape(0.6f);
  rp3d::Transform sphereTransform(rp3d::Vector3(1.6, 10, 0),
                                  rp3d::Quaternion::fromEulerAngles(0, 0, 0));
  rp3d::RigidBody *sphereBody = world.createRigidBody(sphereTransform);
  sphereBody->setAngularDamping(0.3);
  sphereBody->setLinearDamping(0.5);
  rp3d::Material &sphereMaterial = sphereBody->getMaterial();
  sphereMaterial.setBounciness(0.3);
  sphereMaterial.setRollingResistance(1);
  rp3d::ProxyShape *sphereProxy = sphereBody->addCollisionShape(&sphereShape, sphereTransform, 1);
  //renderer->addSphere(sphereProxy, &sphereShape);

  rp3d::Transform floorTransform(rp3d::Vector3(0,-5,0), rp3d::Quaternion().identity());
  rp3d::RigidBody* floorBody = world.createRigidBody(floorTransform);
  floorBody->setType(reactphysics3d::BodyType::STATIC);
  floorBody->setAngularVelocity(rp3d::Vector3(0, 0, 0));
  rp3d::Material& floorMaterial = floorBody->getMaterial();
  floorMaterial.setBounciness(rp3d::decimal(0.0));
  floorMaterial.setFrictionCoefficient(1.0);
  const rp3d::Vector3 floorHalfExtends(10, 0.2, 10);
  rp3d::BoxShape floorShape(floorHalfExtends);
  float* heightFieldData = generateHeightMap(200, 200, 5, 5);
  rp3d::HeightFieldShape heightField(200, 200, 0, 5, heightFieldData, rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE, 1);
  rp3d::ProxyShape* floorProxy = floorBody->addCollisionShape(&heightField, rp3d::Transform::identity(), 0);
  renderer->addHeightMap(floorProxy, &heightField, heightFieldData);

  rp3d::Transform lanternTransform(rp3d::Vector3(0, 5, 0), rp3d::Quaternion().identity());
  rp3d::RigidBody* lanternBody = world.createRigidBody(lanternTransform);
  //lanternBody->setType(reactphysics3d::BodyType::STATIC);
  auto boundingBox = renderer->getResourceRepo()->getLampMesh()->getBoundingBox();
  auto extends = (boundingBox.max - boundingBox.min) / 2;
  const rp3d::Vector3 lanternHalfExtends(extends.x, extends.y, extends.z);
  rp3d::BoxShape lanternShape(lanternHalfExtends);
  rp3d::ProxyShape* lanternProxy = lanternBody->addCollisionShape(&lanternShape, lanternTransform, 1);
  renderer->addLantern(lanternProxy, &lanternShape);



  auto sun = new Light();
  sun->position = Vector3(-80,80,0);
  sun->color = Vector3(1)*10;
  sun->quadratic = 0.2;
  renderer->addLight(sun);

  for(int i=0; i<60; i++) {
    float f = (float)i / 60.f;
    rp3d::Transform cubeTransform(rp3d::Vector3(9*sin(f*6), 0.2*i-2, 9 * cos(f*6)),
                                  rp3d::Quaternion::fromEulerAngles(0,0,0));
    rp3d::RigidBody *cubeBody = world.createRigidBody(cubeTransform);
    cubeBody->setType(reactphysics3d::BodyType::STATIC);
    rp3d::Material &cubeMaterial = cubeBody->getMaterial();
    cubeMaterial.setBounciness(0.3);
    cubeMaterial.setFrictionCoefficient(1);
    rp3d::ProxyShape *cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 0.2);
    renderer->addCube(cubeProxy, &boxShape);

    rp3d::Transform lanternTransform(rp3d::Vector3(9 * sin(f*6), 0.2*i+0.3-2, 9 * cos(f*6)),
                                     rp3d::Quaternion().identity());
    rp3d::RigidBody *lanternBody = world.createRigidBody(lanternTransform);
    auto lanternMaterial = lanternBody->getMaterial();
    lanternMaterial.setBounciness(0);
    //lanternBody->setType(reactphysics3d::BodyType::STATIC);
    rp3d::ProxyShape *lanternProxy = lanternBody->addCollisionShape(&lanternShape, lanternTransform, 0.01);
    auto lantern = renderer->addLantern(lanternProxy, &lanternShape);
    lantern->getLight()->color = Vector3(1, 0.3, 0.2) * 10;
  }

  int tick=1;
  float avgCamSpeed = 0;
  while(!renderer->shouldClose())
  {
    float lightness = std::fmax(1 - glfwGetTime() / 40.0f, 0.3f);
    sun->color = Vector3(1) * 3000 * lightness;
    renderer->setLightness(lightness);
    if (tick++%40 == 0) {
      /*
      rp3d::Transform cubeTransform(rp3d::Vector3(10*sin(glfwGetTime()),35,10*cos(glfwGetTime())), rp3d::Quaternion::fromEulerAngles(0, 0, 0));
      rp3d::RigidBody* cubeBody = world.createRigidBody(cubeTransform);
      rp3d::Material &cubeMaterial = cubeBody->getMaterial();
      cubeMaterial.setBounciness(0.3);
      cubeMaterial.setFrictionCoefficient(1);
      rp3d::ProxyShape* cubeProxy = cubeBody->addCollisionShape(&boxShape, cubeTransform, 100);
      renderer->addCube(cubeProxy, &boxShape);
       */

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
      rp3d::Transform lanternTransform(rp3d::Vector3(20 * sin(glfwGetTime()), 5, 20 * cos(glfwGetTime())),
                                       rp3d::Quaternion().identity());
      rp3d::RigidBody *lanternBody = world.createRigidBody(lanternTransform);
      //lanternBody->setType(reactphysics3d::BodyType::STATIC);
      rp3d::ProxyShape *lanternProxy = lanternBody->addCollisionShape(&lanternShape, lanternTransform, 0.1);
      auto lantern = renderer->addLantern(lanternProxy, &lanternShape);
      lantern->getLight()->color = Vector3(1, 0.3, 0.2) * 10;
      lantern->getLight()->quadratic = 3;
    }

    world.update(1.0/60.0);
    keyboard.swapBuffers();

    double mouseX, mouseY;
    int windowW, windowH;
    glfwGetWindowSize(renderer->getWindowPointer(), &windowW, &windowH);
    glfwGetCursorPos(renderer->getWindowPointer(), &mouseX, &mouseY);
    Vector2 offset = Vector2(mouseX / windowW, mouseY / windowH) - Vector2(0.5);
    offset = offset * 100;
    glfwSetCursorPos(renderer->getWindowPointer(), windowW / 2, windowH / 2);

    camera.pos = sphereProxy->getLocalToWorldTransform().getPosition();
    rp3d::Vector3 camVelocity = sphereBody->getLinearVelocity();
    float camSpeed = camVelocity.y;
    avgCamSpeed = avgCamSpeed*0.9f + camSpeed * 0.1f;
    float latteralSpeed = Vector2(camVelocity.x, camVelocity.z).length();
    if (keyboard.isDown(Keyboards::JUMP) && abs(camSpeed) < 1) { sphereBody->applyForceToCenterOfMass(rp3d::Vector3(0,1,0)*300); }
    if (keyboard.isDown(Keyboards::MOVE_FORWARD) && latteralSpeed < 5) { sphereBody->applyForceToCenterOfMass(camera.moveDir().toRP3D() * 20); }
    if (keyboard.isDown(Keyboards::MOVE_BACKWARD) && latteralSpeed < 5) { sphereBody->applyForceToCenterOfMass(camera.moveDir().toRP3D() * -20); }
    camera.update(renderer->getWindowRatio(), &keyboard, offset);
    renderer->loop(&camera, avgCamSpeed * std::fmax(Vector3::dot(-Vector3(camVelocity).normalized(), camera.viewDir), 0));
//    printf("%s\n", body->getLinearVelocity().to_string().c_str());

  }
  return 0;
}

