class CameraRayCaster : public rp3d::RaycastCallback
{
public:
    rp3d::RaycastInfo m_info;
    Camera::Camera* m_camera;
    CameraRayCaster(Camera::Camera* camera)
    {
      m_camera = camera;
    }

    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override {
      if (m_camera->getPosition().y < info.worldPoint.y + 5)
      {
        if (info.worldPoint.y + 5 > m_camera->pos.y) {
          m_camera->pos.y = info.worldPoint.y + 5, m_camera->pos.y;
          m_camera->velocity.y = 0;
        }
      }

      return m_info.hitFraction;
    }

};