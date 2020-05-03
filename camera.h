namespace Camera {

using namespace Keyboards;
  
class Camera
{
private:
   float height;
   float fov;
   Matrix4 matrix;
   void calcMatrix(float ratio);

public:
   Vector3 viewDir;
   Vector3 moveDir() const { return Vector3(viewDir.x, 0, viewDir.z).normalized(); }
   Camera(float fov, float height);
   void update(float ratio, Keyboard* keyboard, Vector2 mouseDelta);
   Vector3 pos;
   Matrix4 getMatrix() const { return matrix; }
   Vector3 getPosition() const { return pos; }
   void setPosition(Vector3 v) { pos = v; }
   Ray getRay(float u, float v) const;
};


Camera::Camera(float fov, float height)
{
  viewDir = Vector3(0, 0, -1); 
  this->fov = fov;
  this->height = height;
}

void Camera::update(float ratio, Keyboard* keyboard, Vector2 mouseDelta)
{
  float speed = 0.8f;
  float rot_speed = 0.02f;

  Vector3 move_dir = moveDir();
  Vector3 unitY = Vector3(0, 1, 0);

  // Sideways move vector
  Vector3 move_tan = Vector3(-move_dir.z, move_dir.y, move_dir.x) * speed;

  // Forwards move vector
  Vector3 move_par = move_dir * speed;

  // Tangent to view vector in xz plane
  Vector3 view_tan = Vector3::cross(move_dir, unitY).normalize() * rot_speed;

  // Tangent to view vector and view tangent
  Vector3 view_bitan = Vector3::cross(viewDir, view_tan).normalize() * rot_speed;

  // Change to adding tangent or bi-tangent and then normalizing
  float ny = viewDir.y + view_bitan.y;
  if (ny < 0.99 && ny > -0.99)
    viewDir -= view_bitan * mouseDelta.y;

  viewDir += view_tan * mouseDelta.x;

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
  Matrix4 t = Matrix4::FromTranslation(-pos - Vector3(0, height,0));
  Matrix4 r = Matrix4::FromAxisRotations(phi, theta, 0);
  matrix = p * r * t;
}

Ray Camera::getRay(float u, float v) const
{
  assert(abs(viewDir.z) < 0.999);
  Vector3 tangent = Vector3::cross(viewDir, Vector3(0,0,1));
  Vector3 bitangent = Vector3::cross(viewDir, tangent);

  Vector3 center = pos + fov * viewDir;
  Vector3 point = center + u * tangent - v * bitangent;
  Vector3 dir = (point - pos).normalized();
  return {pos, dir};
}

}
