#include "deps.h"


int main() {

  Renderer renderer;
  Camera::Camera camera(1.4f);
  Keyboards::Keyboard keyboard(renderer.getWindowPointer());

  rp3d::Vector3 gravity(0, -9.81, 0);
  rp3d::DynamicsWorld world(gravity);

  rp3d::Vector3 initPosition(0,3,0);
  rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
  rp3d::Transform transform(initPosition, initOrientation);

  rp3d::RigidBody* body = world.createRigidBody(transform);


  while(!renderer.shouldClose())
  {
    world.update(1.0/60.0);
    keyboard.swapBuffers();
    camera.update(renderer.getWindowRatio(), &keyboard);
    renderer.loop(camera.getMatrix());
//    printf("%s\n", body->getLinearVelocity().to_string().c_str());

  }

  return 0;
}
