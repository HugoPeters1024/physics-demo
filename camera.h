namespace Camera {

using namespace Keyboards;
  
class Camera
{
private:
   float fov;
   float gravity;
   Vector3 pos, viewDir;
   Vector3 velocity;
   Matrix4 matrix;
   void calcMatrix(float ratio);

public:
   Camera(float fov);
   void update(float ratio, Keyboard* keyboard);
   Matrix4 getMatrix() const { return matrix; }
   Vector3 getPosition() const { return pos; }
   void setPosition(Vector3 v) { pos = v; }
};


Camera::Camera(float fov)
{
  viewDir = Vector3(0, 0, -1); 
  this->fov = fov;
  gravity = 0.05f;
}

void Camera::update(float ratio, Keyboard* keyboard)
{
  if (pos.y + velocity.y > 17) {
    pos += velocity;
    velocity.y -= gravity;
  } else {
    velocity.y = 0;
  }

  if (keyboard->isPressed(JUMP)) {
      velocity.y += 1;
  }

  float speed = 0.1f;
  float rot_speed = 0.02f;

  Vector3 move_dir = Vector3(viewDir.x, 0, viewDir.z).normalize();
  Vector3 unitY = Vector3(0, 1, 0);

  // Sideways move vector
  Vector3 move_tan = Vector3(-move_dir.z, move_dir.y, move_dir.x) * speed;

  // Forwards move vector
  Vector3 move_par = move_dir * speed;

  // Up/down move vector
  Vector3 move_vert = unitY * speed;

  // Tangent to view vector in xz plane
  Vector3 view_tan = Vector3::cross(move_dir, unitY).normalize() * rot_speed;

  // Tangent to view vector and view tangent
  Vector3 view_bitan = Vector3::cross(viewDir, view_tan).normalize() * rot_speed;

  if (keyboard->isDown(MOVE_FORWARD))   pos += move_par; 
  if (keyboard->isDown(MOVE_BACKWARD))  pos -= move_par;
  if (keyboard->isDown(MOVE_LEFT))      pos -= move_tan;
  if (keyboard->isDown(MOVE_RIGHT))     pos += move_tan;
  if (keyboard->isDown(MOVE_UP))        pos += move_vert;
  if (keyboard->isDown(MOVE_DOWN))      pos -= move_vert;

  // Change to adding tangent or bi-tangent and then normalizing
  if (keyboard->isDown(LOOK_UP)) {
    float ny = viewDir.y + view_bitan.y;
    if (ny < 0.99 && ny > -0.99) 
      viewDir += view_bitan;
  }

  if (keyboard->isDown(LOOK_DOWN)) {
    float ny = viewDir.y - view_bitan.y;
    if (ny < 0.99 && ny > -0.99)
      viewDir -= view_bitan;
  }

  if (keyboard->isDown(LOOK_LEFT))      viewDir -= view_tan;
  if (keyboard->isDown(LOOK_RIGHT))     viewDir += view_tan;
  viewDir.normalize();
  calcMatrix(ratio);
}

void Camera::calcMatrix(float screenRatio)
{
  float theta, phi;
  Vector3 xz = Vector3(viewDir.x, 0, viewDir.z).normalize();

  theta = -atan2(xz.x, -xz.z);
  if (theta < 0)
    theta = theta + 2 * 3.1415926f;

  phi = atan2(viewDir.y, sqrt(viewDir.x * viewDir.x + viewDir.z * viewDir.z));

  Matrix4 p = Matrix4::FromPerspective(fov, screenRatio, 0.1f, 1000.0f);
  Matrix4 t = Matrix4::FromTranslation(-pos);
  Matrix4 r = Matrix4::FromAxisRotations(phi, theta, 0);
  matrix = p * r * t;
}
}
