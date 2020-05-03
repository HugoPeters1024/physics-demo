class CameraRayCaster : public rp3d::RaycastCallback
{
private:
    rp3d::Vector3 worldPointTransform(rp3d::Vector3 input) const {
        input.y += 7;
        input.y = ceil(input.y*100)/100;
        return input;
    }
public:
    rp3d::RaycastInfo m_info;
    Camera::Camera* m_camera;
    float distToGround = 1000000;
    CameraRayCaster(Camera::Camera* camera)
    {
      m_camera = camera;
    }

    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override {
      rp3d::Vector3 worldPoint = worldPointTransform(info.worldPoint);
      if (m_camera->getPosition().y < worldPoint.y)
      {
        m_camera->pos.y = worldPoint.y;
        m_camera->velocity.y = 0;
        Vector3 toGround = Vector3(worldPoint) - m_camera->pos;
        distToGround = sqrt(Vector3::dot(toGround, toGround));
      }

      return m_info.hitFraction;
    }

};